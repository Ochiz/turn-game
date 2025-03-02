#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <mutex>
#include <coroutine>
//For this week's homework, I created a mini turn-based game for two players. Each player takes turns choosing actions like attacking, defending, or healing. 
//The game uses coroutines to manage turns smoothly and delegates to handle different actions. I also included a multicast delegate to trigger multiple actions at once. 
//Lambda expressions simplify the code, and templates make it flexible. To ensure smooth gameplay, I used multithreading with mutexes to handle turns and avoid conflicts. 
//This project demonstrates various programming techniques in a fun and interactive way.
// Basic player class
class Player {
public:
    std::string name;
    int health;
    bool isDefending;

    // Constructor initializes player's name, health, and defense status
    Player(std::string n, int h) : name(n), health(h), isDefending(false) {}

    // Method to reduce player's health by damage amount
    void takeDamage(int dmg) {
        if (isDefending) {
            int reducedDamage = dmg - 5;  // Reduce damage by 5 if defending
            if (reducedDamage < 0) reducedDamage = 0;  // Ensure damage doesn't go negative
            std::cout << name << " takes " << reducedDamage << " damage (reduced by 5).\n";
            health -= reducedDamage;
            isDefending = false;  // Reset defending status after taking damage
        }
        else {
            std::cout << name << " takes " << dmg << " damage.\n";
            health -= dmg;
        }
        if (health < 0) health = 0;
    }

    // Method to increase player's health by a specified amount
    void heal(int amount) {
        health += amount;
    }

    // Method to set defending status
    void defend() {
        isDefending = true;
    }

    // Method to check if the player is still alive
    bool isAlive() const {
        return health > 0;
    }
};

// Coroutine structure for turn-taking
// A coroutine is a function that can suspend execution to be resumed later. Here, I define a coroutine for player turns.
struct Turn {
    struct promise_type {
        Turn get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; } // Coroutine starts immediately.
        std::suspend_always final_suspend() noexcept { return {}; } // Coroutine suspends at the end.
        void return_void() {} // No return value for the coroutine.
        void unhandled_exception() { std::terminate(); } // Handle exceptions by terminating the program.
    };
};

// Coroutine for a player's turn
Turn playerTurn(Player& player, Player& opponent, std::mutex& mtx) {
    {
        std::lock_guard<std::mutex> lock(mtx); // Lock the mutex to ensure synchronized access to the console.
        std::cout << player.name << "'s turn. Current health: " << player.health << "\n";
        std::cout << "Choose an action:\n";
        std::cout << "1. Attack\n2. Defend\n3. Heal\n";
    }

    int choice;
    {
        std::lock_guard<std::mutex> lock(mtx); // Lock the mutex again for synchronized input.
        std::cin >> choice;
    }

    {
        std::lock_guard<std::mutex> lock(mtx); // Lock the mutex again for synchronized output.
        switch (choice) {
        case 1:
            std::cout << player.name << " attacks " << opponent.name << "!\n";
            opponent.takeDamage(10);
            break;
        case 2:
            std::cout << player.name << " defends!\n";
            player.defend();  // Set defending status
            break;
        case 3:
            std::cout << player.name << " heals!\n";
            player.heal(10);  // Heal for a fixed amount for simplicity
            std::cout << player.name << " heals for 10 health.\n";
            break;
        default:
            std::cout << "Invalid choice. Skipping turn.\n";
            break;
        }
    }
    co_await std::suspend_always{}; // Suspend the coroutine.
}

// Delegate type for player actions
// A delegate is a type that represents references to methods with a particular parameter list and return type.
using ActionDelegate = std::function<void(Player&, Player&)>;

// Multicast delegate class
// A multicast delegate is a delegate that holds references to more than one method and invokes them all when called.
class MulticastDelegate {
public:
    void add(ActionDelegate action) {
        actions.push_back(action);
    }

    void invoke(Player& player, Player& opponent) {
        for (auto& action : actions) {
            action(player, opponent); // Invoke all actions added to the multicast delegate.
        }
    }

private:
    std::vector<ActionDelegate> actions; // Store multiple actions.
};

class Game {
public:
    std::vector<Player> players;
    std::mutex mtx;  // Mutex for synchronized output

    // Method to add a player to the game
    void addPlayer(const Player& player) {
        players.push_back(player);
    }

    // Method to perform all actions on all players
    void performTurn(Player& player, Player& opponent) {
        playerTurn(player, opponent, mtx); // Call the coroutine to perform the player's turn.
    }
};

// Function template to apply actions using templates
// Templates allow us to write generic functions that work with any data type.
template<typename T>
void applyAction(Player& player, Player& opponent, T action) {
    action(player, opponent); // Apply the action to the player and opponent.
}

int main() {
    Game game;
    game.addPlayer({ "Player 1", 100 });
    game.addPlayer({ "Player 2", 100 });

    // Starting game
    std::cout << "Starting the game. Press Enter to begin...\n";
    std::cin.ignore();  // Wait for Enter key press

    // Multicast delegate for player actions
    MulticastDelegate multicastDelegate;
    // Lambda expression: an anonymous function that can be used as an argument to algorithms or stored in variables.
    multicastDelegate.add([](Player& player, Player& opponent) {
        std::cout << player.name << " prepares to act...\n";
        });

    while (true) {
        for (size_t i = 0; i < game.players.size(); ++i) {
            Player& player = game.players[i];
            Player& opponent = game.players[(i + 1) % game.players.size()];

            if (player.isAlive() && opponent.isAlive()) {
                std::thread turnThread([&game, &player, &opponent]() {
                    game.performTurn(player, opponent); // Use multithreading to handle player turns.
                    });
                turnThread.join();  // Ensure turn completes before moving to the next
            }
        }

        // Check for game end condition
        int alivePlayers = 0;
        for (auto& player : game.players) {
            if (player.isAlive()) {
                alivePlayers++;
            }
        }
        if (alivePlayers < 2) {
            break;  // End the game if fewer than 2 players are alive
        }
    }

    // Announce the winner
    for (auto& player : game.players) {
        if (player.isAlive()) {
            std::cout << player.name << " wins the game!\n";
        }
    }

    std::cout << "Game over.\n";
    return 0;
}
/*
Coroutines:
The struct Turn and playerTurn function are used as coroutines. Coroutines allow functions to suspend and resume execution, which is useful for asynchronous operations.
Delegates:
ActionDelegate is a delegate type using std::function<void(Player&, Player&)>, which allows us to store and call functions that match this signature.
Multicasting of a delegate:
MulticastDelegate class allows multiple ActionDelegate actions to be added and invoked. This enables us to call multiple methods in response to an event.
Lambda expressions:
Lambda expressions are used to create anonymous functions that can be passed as arguments or stored. For example, in main(), a lambda is added to multicastDelegate to print a message when a player prepares to act.
Templates:
applyAction function template demonstrates the use of templates to create generic functions that can operate on different types.
Multithreading:
Player turns are handled in separate threads using std::thread, and std::lock_guard<std::mutex> ensures synchronized access to shared resources.
*/