#include <iostream>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <string>
#include <vector>

// Function prototypes
void display_welcome_message();
void display_rules();
int get_difficulty_level();
int generate_random_number(int min, int max);
bool is_valid_guess(int guess, int min, int max);
std::string get_hint(int guess, int target);
void display_game_history(const std::vector<int>& guesses, int total_games, int wins);
void play_game(int difficulty, int& total_games, int& wins);
bool play_again();

int main() {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr))); // how does this work? - https://www.geeksforgeeks.org/cpp/how-to-seed-a-random-number-generator-in-cpp/
    
    // Variables to track game statistics
    int total_games = 0;
    int wins = 0;
    
    // Display welcome message and rules
    display_welcome_message();
    display_rules();
    
    // Main game loop
    do {
        // Get difficulty level
        int difficulty = get_difficulty_level();
        
        // Play the game
        play_game(difficulty, total_games, wins);
        
    } while (play_again());
    
    // Display final statistics
    std::cout << "\n===== Game Over =====" << std::endl;
    std::cout << "Total games played: " << total_games << std::endl;
    std::cout << "Games won: " << wins << std::endl;
    std::cout << "Win rate: " << (total_games > 0 ? (static_cast<double>(wins) / total_games) * 100 : 0) << "%" << std::endl;
    std::cout << "Thank you for playing!" << std::endl;
    
    return 0;
}

// Function to display welcome message
void display_welcome_message() {
    std::cout << "====================================" << std::endl;
    std::cout << "  WELCOME TO THE NUMBER GUESSING GAME" << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "Try to guess the secret number!" << std::endl;
}

// Function to display game rules
void display_rules() {
    std::cout << "\n===== Game Rules =====" << std::endl;
    std::cout << "1. The computer will generate a random number based on the difficulty level." << std::endl;
    std::cout << "2. You need to guess the number within the allowed attempts." << std::endl;
    std::cout << "3. After each guess, you'll get a hint if your guess is too high or too low." << std::endl;
    std::cout << "4. You win if you guess the number within the allowed attempts." << std::endl;
}

// Function to get difficulty level from user
int get_difficulty_level() {
    int difficulty;
    
    std::cout << "\n===== Select Difficulty Level =====" << std::endl;
    std::cout << "1. Easy (1-50, 10 attempts)" << std::endl;
    std::cout << "2. Medium (1-100, 7 attempts)" << std::endl;
    std::cout << "3. Hard (1-200, 5 attempts)" << std::endl;
    
    do {
        std::cout << "Enter your choice (1-3): ";
        std::cin >> difficulty;
        
        if (difficulty < 1 || difficulty > 3) {
            std::cout << "Invalid choice! Please enter 1, 2, or 3." << std::endl;
        }
    } while (difficulty < 1 || difficulty > 3);
    
    return difficulty;
}

// Function to generate a random number within a range
int generate_random_number(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Function to check if the guess is valid
bool is_valid_guess(int guess, int min, int max) {
    return (guess >= min && guess <= max);
}

// Function to provide a hint based on the guess
std::string get_hint(int guess, int target) {
    if (guess < target) {
        return "Too low! Try a higher number.";
    } else if (guess > target) {
        return "Too high! Try a lower number.";
    } else {
        return "Correct! You guessed the number!";
    }
}

// Function to display game history
void display_game_history(const std::vector<int>& guesses, int total_games, int wins) {
    std::cout << "\n===== Game History =====" << std::endl;
    std::cout << "Your guesses this game: ";
    
    for (size_t i = 0; i < guesses.size(); i++) {
        std::cout << guesses[i];
        if (i < guesses.size() - 1) {
            std::cout << ", ";
        }
    }
    
    std::cout << std::endl;
    std::cout << "Total attempts this game: " << guesses.size() << std::endl;
    std::cout << "Total games played so far: " << total_games << std::endl;
    std::cout << "Games won so far: " << wins << std::endl;
}

// Function to play the game
void play_game(int difficulty, int& total_games, int& wins) {
    int min, max, max_attempts;
    
    // Set game parameters based on difficulty
    switch (difficulty) {
        case 1:  // Easy
            min = 1;
            max = 50;
            max_attempts = 10;
            std::cout << "\nEasy mode: Guess a number between 1 and 50. You have 10 attempts." << std::endl;
            break;
        case 2:  // Medium
            min = 1;
            max = 100;
            max_attempts = 7;
            std::cout << "\nMedium mode: Guess a number between 1 and 100. You have 7 attempts." << std::endl;
            break;
        case 3:  // Hard
            min = 1;
            max = 200;
            max_attempts = 5;
            std::cout << "\nHard mode: Guess a number between 1 and 200. You have 5 attempts." << std::endl;
            break;
    }
    
    // Generate the target number
    int target = generate_random_number(min, max);
    int attempts = 0;
    int guess;
    bool won = false;
    std::vector<int> guesses;
    
    // Game loop
    while (attempts < max_attempts) {
        std::cout << "\nAttempt " << (attempts + 1) << "/" << max_attempts << std::endl;
        std::cout << "Enter your guess (" << min << "-" << max << "): ";
        std::cin >> guess;
        
        // Check if the guess is valid
        if (!is_valid_guess(guess, min, max)) {
            std::cout << "Invalid guess! Please enter a number between " << min << " and " << max << "." << std::endl;
            continue;
        }
        
        // Record the guess
        guesses.push_back(guess);
        attempts++;
        
        // Get hint
        std::string hint = get_hint(guess, target);
        std::cout << hint << std::endl;
        
        // Check if the player won
        if (guess == target) {
            won = true;
            break;
        }
    }
    
    // Game result
    if (won) {
        std::cout << "\nCongratulations! You guessed the number " << target << " in " << attempts << " attempts!" << std::endl;
        wins++;
    } else {
        std::cout << "\nGame over! You've used all your attempts. The number was " << target << "." << std::endl;
    }
    
    // Update game statistics
    total_games++;
    
    // Display game history
    display_game_history(guesses, total_games, wins);
}

// Function to ask if the player wants to play again
bool play_again() {
    char choice;
    std::cout << "\nDo you want to play again? (y/n): ";
    std::cin >> choice;
    
    return (choice == 'y' || choice == 'Y');
}
