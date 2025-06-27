#include <iostream>
#include <vector>

int main() {
    std::cout << "===== Loops in C++ =====" << std::endl;
    
    // For Loop
    std::cout << "\n--- Basic for loop ---" << std::endl;
    std::cout << "Counting from 1 to 10:" << std::endl;
    for (int i = 1; i <= 10; i++) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    
    // For loop with different step
    std::cout << "\n--- For loop with step of 2 ---" << std::endl;
    std::cout << "Even numbers from 2 to 20:" << std::endl;
    for (int i = 2; i <= 20; i += 2) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    
    // For loop with decreasing counter
    std::cout << "\n--- For loop with decreasing counter ---" << std::endl;
    std::cout << "Countdown from 10 to 1:" << std::endl;
    for (int i = 10; i >= 1; i--) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    
    // For loop with break statement
    std::cout << "\n--- For loop with break statement ---" << std::endl;
    std::cout << "Counting from 1 until we hit 5:" << std::endl;
    for (int i = 1; i <= 10; i++) {
        std::cout << i << " ";
        if (i == 5) {
            std::cout << "(breaking here)";
            break;  // Exit the loop when i equals 5
        }
    }
    std::cout << std::endl;
    
    // For loop with continue statement
    std::cout << "\n--- For loop with continue statement ---" << std::endl;
    std::cout << "Numbers from 1 to 10, skipping 5:" << std::endl;
    for (int i = 1; i <= 10; i++) {
        if (i == 5) {
            std::cout << "(skipped) ";
            continue;  // Skip the rest of the loop body for this iteration
        }
        std::cout << i << " ";
    }
    std::cout << std::endl;
    
    // Nested for loops
    std::cout << "\n--- Nested for loops ---" << std::endl;
    std::cout << "Multiplication table (1-5):" << std::endl;
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 5; j++) {
            std::cout << i << " x " << j << " = " << (i * j) << "\t";
        }
        std::cout << std::endl;
    }
    
    // While loop
    std::cout << "\n--- While loop ---" << std::endl;
    std::cout << "Counting from 1 to 5:" << std::endl;
    int counter = 1;
    while (counter <= 5) {
        std::cout << counter << " ";
        counter++;
    }
    std::cout << std::endl;
    
    // While loop with user input
    std::cout << "\n--- While loop with user input ---" << std::endl;
    int sum = 0;
    int input;
    std::cout << "Enter positive numbers to add (enter 0 or negative to stop):" << std::endl;
    
    // Read first input
    std::cin >> input;
    
    while (input > 0) {
        sum += input;
        std::cout << "Current sum: " << sum << std::endl;
        std::cout << "Enter another number (0 or negative to stop): ";
        std::cin >> input;
    }
    
    std::cout << "Final sum: " << sum << std::endl;
    
    // Do-while loop
    std::cout << "\n--- Do-while loop ---" << std::endl;
    int number;
    
    do {
        std::cout << "Enter a number between 1 and 10: ";
        std::cin >> number;
        
        if (number < 1 || number > 10) {
            std::cout << "Invalid input! Please try again." << std::endl;
        }
    } while (number < 1 || number > 10);
    
    std::cout << "You entered a valid number: " << number << std::endl;
    
    // Range-based for loop (C++11 and later)
    std::cout << "\n--- Range-based for loop ---" << std::endl;
    std::vector<int> numbers = {10, 20, 30, 40, 50};
    std::cout << "Elements in the vector:" << std::endl;
    
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    // Range-based for loop with auto keyword
    std::cout << "\n--- Range-based for loop with auto ---" << std::endl;
    std::cout << "Elements in the vector (using auto):" << std::endl;
    
    for (auto num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
