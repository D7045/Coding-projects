#include <iostream>
#include <iomanip>  // for setw()
#include <cstdlib>  // for srand()
#include <ctime>    // for time()

using namespace std;

// Class to represent a set of cards
class Card {
private:
    int n1, n2, eCard, total;

public:
    // Constructor to initialize card values and calculate total
    Card() {
        n1 = rand() % 10 + 1;
        n2 = rand() % 10 + 1;
        total = (n1 + n2) % 10;
        eCard = 0;
    }

   // Function to draw extra card
    void drawExtra() {
        if (total != 8 && total != 9) {  // Only draw extra card if no natural win
            eCard = rand() % 10 + 1;
            total = (total + eCard) % 10;
        }
    }

    
    int getTotal() const { return total; }
    int getN1() const { return n1; } 
    int getN2() const { return n2; }
    int getECard() const { return eCard; }

    // Print card details
    void printCard(bool isNat = false) const {
        cout << "========\n";
        cout << "|      |\n";
        cout << "|  " <<setw(2)<< n1 <<"  |\n";
        cout << "|      |\n";
        cout << "========\n";
        cout << "First Card: " << n1 << "\n";
	cout << "========\n";
        cout << "|      |\n";
        cout << "|  " <<setw(2)<< n2 <<"  |\n";
        cout << "|      |\n";
        cout << "========\n";
        cout << "Second Card: " << n2 << "\n";
	cout << "========\n";
        cout << "|      |\n";
        cout << "|  " <<setw(2)<< eCard <<"  |\n";
        cout << "|      |\n";
        cout << "========\n";
	cout<<"Total: "<< total <<"  \n";
        if (isNat) {
            cout << "This is a natural win 8/9\n";
        } else if (eCard > 0) {
            cout << "Extra Card: " << eCard << "\n";
        }
    }
};

// Class to manage the game
class Game {
private:
    int balance;

    int getBetType() const {
        int type;
        cout << "1-BANKER \n2-PLAYER \n";
        cout << "Choose your bet: ";
        while (!(cin >> type) || (type < 1 || type > 2)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please choose 1 (BANKER) or 2 (PLAYER): ";
        }
        return type;
    }

    int getBetAmount() const {
        int amt;
        do {
            cout << "Place your bet, Betting amount: ";
            cin >> amt;
            if (amt > balance) 
                cout << "Bet amount exceeds your current balance of " << balance << ". Try again.\n";
            else if (amt <= 0) 
                cout << "Bet amount must be greater than 0. Try again.\n";
            
    	} while (amt > balance || amt <= 0);
	      return amt;
    }


    int determineWinner(int bSum, int pSum) const {
        cout << "\n==================================\n";
        if ((bSum == 8 || bSum == 9 ) && (pSum == 8 || pSum == 9)) {
            cout << "DRAW\n";
            return 3;
        } else if (bSum < pSum) {
            cout << "PLAYER WINS\n";
            return 2;
        } else {
            cout << "BANKER WINS\n";
            return 1;
        }
    }

    int calcPrize(int winner, int betType, int bet) const {
        if (winner == betType) {
            cout << "YOU WIN!\n";
            return bet;  // won bet
        } else if (winner == 3) {
            cout << "Tie Game!\n";
            return 0;  // no money won or lost
        } else {
            cout << "YOU LOSE!\n";
            return -bet;  // lost the bet
        }
    }


public:
    // Constructor to initialize game balance
    Game() {
        balance=10000;
}

    void welcome() const {
        cout << "==============================\n";
        cout << "WELCOME TO VIRTUAL BACCARAT\n";
        cout << "==============================\n";
    }

    void play() {
        char again;
        bool cont = true;

        do {
            cout << "Current balance: " << balance << endl;

            // Betting phase
            int betType = getBetType();
            int bet = getBetAmount();

            // Cards drawn for banker and player
            cout << "\n==============================\n";
            cout << "      = BANKER'S CARDS =\n";
            cout << "  ==============================\n";
            Card banker;
            banker.drawExtra();
            banker.printCard(banker.getTotal() == 8 || banker.getTotal() == 9);

            cout << "\n==============================\n";
            cout << "      = PLAYER'S CARDS =\n";
            cout << "  ==============================\n";
            Card player;
            player.drawExtra();
            player.printCard(player.getTotal() == 8 || player.getTotal() == 9);

            // Determine winner and calculate prize won 
            int win = determineWinner(banker.getTotal(), player.getTotal());
            int prize = calcPrize(win, betType, bet);
            balance += prize;

            cout << "Current Money: " << balance << "\n\n";

            //To check if the player wants to continue
            if (balance <= 0) {
                cout << "Insufficient funds!\n";
                cout << "THANK YOU FOR PLAYING!\n";
                break;
            }
            cout << "THANK YOU FOR PLAYING!\n";
            cout << "PLAY AGAIN? Y/N: ";
            cin >> again;
            cont = (again == 'Y' || again == 'y') && balance > 0;

        } while (cont);
    }
};

int main() {
    // Seed random number generator
    srand(time(0));

    Game baccarat;
    baccarat.welcome();
    baccarat.play();

    return 0;
}
