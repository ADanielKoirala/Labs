// templateDice.cpp
// Daniel Koirala CS3

#include <iostream>
#include <string>
#include <random>
#include <cctype>
using namespace std;

// ======= Template Method Pattern: Game framework =======
class Game {
public:
    virtual ~Game() = default;

    //template, don't modify
    void play() {
        movesCount_ = 0;
        playerWon_  = -1;
        initializeGame();

        while (true) {
            // Each player makes a move
            for (int p = 0; p < playersCount_; ++p) {
                makeMove(p);
            }
            ++movesCount_;
            if (endOfGame()) break;
        }
        printWinner();
    }

protected:
    // Methods to be implemented by subclasses
    virtual void initializeGame() = 0;
    virtual void makeMove(int player) = 0;
    virtual bool endOfGame() = 0;
    virtual void printWinner() = 0;

    // Game state
    int playersCount_ = 0;
    int movesCount_   = 0;   // rounds played
    int playerWon_    = -1;  // index of the winner (0 or 1)
};

// ======= Concrete Game: Dice =======
class Dice : public Game {
public:
    Dice()
        : rng_(static_cast<unsigned>(time(nullptr))),
          d6_(1, 6),
          coin_(0, 1) {}

protected:
    void initializeGame() override {
        playersCount_ = kNumPlayers;

        players_[0] = "Computer";
        players_[1] = "You";

        // Initially both players will roll
        reRoll_[0] = true;
        reRoll_[1] = true;

        // Initialize high scores
        highRoll_[0] = 0;
        highRoll_[1] = 0;

        cout << "Welcome to Template Dice!\n";
        cout << "Each of you rolls 5 dice up to " << kMaxRounds
             << " rounds. Highest total wins (computer wins ties).\n\n";
    }

    void makeMove(int player) override {
        // If player chose to pass, show passed message
        if (!reRoll_[player]) {
            cout << players_[player] << " rolled: passed, "
                 << nameHigh(player) << " = " << highRoll_[player] << "\n";
            return;
        }

        // Roll kNumDice dice
        int sum = 0;
        cout << players_[player] << " rolled: ";
        for (int i = 0; i < kNumDice; ++i) {
            int r = d6_(rng_);
            sum += r;
            cout << r << (i + 1 < kNumDice ? ' ' : ' ');
        }

        // Update high score so far
        if (sum > highRoll_[player]) {
            highRoll_[player] = sum;
        }

        cout << "= " << sum << ", " << nameHigh(player)
             << " = " << highRoll_[player] << "\n";

        // Decide whether to re-roll next round
        if (player == 0) {
            // Computer: random yes/no
            reRoll_[0] = (coin_(rng_) == 1);
        } else {
            // Human: ask
            reRoll_[1] = askYesNo("Roll again? [y/n] ");
        }
    }

    bool endOfGame() override {
        // Check if both players passed or max rounds reached
        bool bothPassNext = (!reRoll_[0] && !reRoll_[1]);
        bool reachedMax   = (movesCount_ >= kMaxRounds);

        if (!bothPassNext && !reachedMax) {
            return false;
        }

        // Decide winner once
        if (playerWon_ == -1) {
            // Human wins only if strictly greater; computer wins ties
            playerWon_ = (highRoll_[1] > highRoll_[0]) ? 1 : 0;
        }
        return true;
    }

    void printWinner() override {
        cout << "\nFinal scores:\n";
        cout << "Computer: " << highRoll_[0] << "\n";
        cout << "You:      " << highRoll_[1] << "\n";

        if (playerWon_ == 1) {
            cout << "you win\n";
        } else {
            cout << "you lose\n";
        }
    }

private:
    static constexpr int kNumPlayers = 2;
    static constexpr int kNumDice    = 5;
    static constexpr int kMaxRounds  = 3;

    string nameHigh(int p) const {
        return (p == 0) ? "computer's highest score" : "your highest score";
    }

    static bool askYesNo(const string& prompt) {
        while (true) {
            cout << prompt;
            string s;
            if (!(cin >> s)) {
                // If input fails, clear and treat as "no"
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return false;
            }
            if (!s.empty()) {
                char c = static_cast<char>(tolower(s[0]));
                if (c == 'y') return true;
                if (c == 'n') return false;
            }
            cout << "Please enter 'y' or 'n'.\n";
        }
    }

    // Game state
    int    highRoll_[kNumPlayers]{};
    bool   reRoll_[kNumPlayers]{};
    string players_[kNumPlayers];

    // Random number generation
    std::mt19937 rng_;
    std::uniform_int_distribution<int> d6_;
    std::uniform_int_distribution<int> coin_;
};

// ======= Main DEMO =======
int main() {
    Dice game;
    game.play();
    return 0;
}
