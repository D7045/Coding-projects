#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<ctime>
#include<iomanip>

using namespace std;

struct Task{
    int id;
    string description;
    string status;
    time_t created_at;
    time_t updated_at;
};

time_t getCurrentTime() {
    return time(0);
}

string formattime(time_t now){
    tm* lm=localtime(&now);
    stringstream ss;
    ss<<lm->tm_year+1900<<"-" << setw(2) << setfill('0')<<lm->tm_mon+1<<"-"<< setw(2) << setfill('0')<<lm->tm_mday<<" "<< setw(2) << setfill('0')<<lm->tm_hour<<":"<<lm->tm_min<<":"<<lm->tm_sec;
    return ss.str();
}

int extractInt(const string& line) {
    size_t pos = line.find(":");
    if (pos == string::npos) return -1;  // Invalid format
    try {
        return stoi(line.substr(pos + 1));
    } catch (const invalid_argument&) {
        return -1;  // Return -1 if stoi fails
    }
}

// Function to safely extract string values from a line
string extractString(const string& line) {
    size_t pos = line.find(":");
    if (pos == string::npos) return "";
    string value = line.substr(pos + 2);
    if (!value.empty() && value.front() == '\"') value.erase(0, 1);
    if (!value.empty() && value.back() == '\"') value.pop_back();
    return value;
}

vector<Task> loadtasks() {
    vector<Task> tasks;
    ifstream file("tasks.json");

    if (!file) {
        ofstream create_file("tasks.json");
        create_file << "{\n  \"tasks\": []\n}";
        create_file.close();
        return tasks;
    }

    string json, line;
    while (getline(file, line)) {
        json += line;  // Read entire JSON content
    }
    file.close();

    // Find the "tasks" array
    size_t start = json.find("[");
    size_t end = json.find("]");
    if (start == string::npos || end == string::npos) {
        cerr << "Error: Invalid JSON format!" << endl;
        return tasks;
    }

    string taskArray = json.substr(start + 1, end - start - 1);  // Extract task array

    // Parse each task
    size_t pos = 0;
    while ((pos = taskArray.find("{")) != string::npos) {
        size_t closePos = taskArray.find("}", pos);
        if (closePos == string::npos) break;

        string taskJson = taskArray.substr(pos, closePos - pos + 1);
        taskArray.erase(0, closePos + 1);

        Task task;
        try {
            task.id = stoi(taskJson.substr(taskJson.find("\"id\":") + 5));
            task.description = taskJson.substr(taskJson.find("\"description\":") + 15);
            task.description = task.description.substr(0, task.description.find("\""));
            task.status = taskJson.substr(taskJson.find("\"status\":") + 10);
            task.status = task.status.substr(0, task.status.find("\""));
            task.created_at = stoi(taskJson.substr(taskJson.find("\"created_at\":") + 14));
            task.updated_at = stoi(taskJson.substr(taskJson.find("\"updated_at\":") + 14));
            
            tasks.push_back(task);
        } catch (const invalid_argument &e) {
            cerr << "Error parsing JSON: " << e.what() << endl;
        }
    }

    return tasks;
}

void savetasks(const vector<Task>& tasks) {
    ofstream file("tasks.json");
    
    file << "{\n  \"tasks\": [\n";
    for (size_t i = 0; i < tasks.size(); i++) {
        file << "    {\n";
        file << "      \"id\": " << tasks[i].id << ",\n";
        file << "      \"description\": \"" << tasks[i].description << "\",\n";
        file << "      \"status\": \"" << tasks[i].status << "\",\n";
        file << "      \"created_at\": " << tasks[i].created_at << ",\n";
        file << "      \"updated_at\": " << tasks[i].updated_at << "\n";
        file << "    }";
        if (i != tasks.size() - 1) file << ",";
        file << "\n";
    }
    file << "  ]\n}";
    
    file.close();
}

void addtask(string description){
    vector<Task> tasks=loadtasks();
    Task task;
    task.id=tasks.empty()?1 : tasks.back().id+1;
    task.description=description;
    task.status="todo";
    task.created_at=task.updated_at=getCurrentTime();
    tasks.push_back(task);
    savetasks(tasks);
    cout<<"Task added successfully!"<<endl;
}

void listtasks() {
    vector<Task> tasks = loadtasks();
    if (tasks.empty()) {
        cout << "No tasks available.\n";
        return;
    }
    for (const Task &task : tasks) {
        cout << "ID: " << task.id << " | " << task.description << " [" << task.status << "]\n";
        cout << "Created: " << formattime(task.created_at) << " | Updated: " << formattime(task.updated_at) << endl;
    }
}

void updatetask(int id, string description){
    vector<Task> tasks=loadtasks();
    for(size_t i=0; i<tasks.size(); i++){
        if(tasks[i].id==id){
            tasks[i].description=description;
            tasks[i].updated_at=getCurrentTime();
            savetasks(tasks);
            cout<<"Task updated successfully!"<<endl;
            return;
        }
    }
    cout<<"Task not found!"<<endl;
}

void deletetask(int id){
    vector<Task> tasks=loadtasks();
    for(size_t i=0; i<tasks.size(); i++){
        if(tasks[i].id==id){
            tasks.erase(tasks.begin()+i);
            savetasks(tasks);
            cout<<"Task deleted successfully!"<<endl;
            return;
        }
    }  
    cout<<"Task not found!"<<endl;
}

int main(int argc, char* argv[]){
    if(argc<2 || string(argv[1])=="help"){
        "Usage: task_tracker <command> [options]\n";
        "Commands:\n";
        "  add <description> --> Add a new task\n";
        "  update <id> <description> --> Update an existing task\n";
        "  delete <id> --> Delete a task\n";
        "  list --> List all tasks\n";
        "  help --> Show this help message\n";
        return 1;
    }
    string command=argv[1];
    if(command=="add" && argc==3){
        string description=argv[2];
        addtask(description);
    }
    else if(command=="update" && argc==4)
        updatetask(stoi(argv[2]), argv[3]);
    else if(command=="delete" && argc==3)
        deletetask(stoi(argv[2]));
    else if(command=="list")
        listtasks();
    else
        cout<<"Invalid command! Use 'help' to see the list of commands."<<endl;    
    return 0;   
}