#include <iostream>
#include <string>

int main() {
    std::cout << "===== Conditional Statements in C++ =====" << std::endl;
    
    // Simple if statement
    std::cout << "\n--- Simple if statement ---" << std::endl;
    int number;
    std::cout << "Enter a number: ";
    std::cin >> number;
    
    if (number > 0) {
        std::cout << number << " is positive." << std::endl;
    }
    
    // if-else statement
    std::cout << "\n--- if-else statement ---" << std::endl;
    if (number % 2 == 0) {
        std::cout << number << " is even." << std::endl;
    } else {
        std::cout << number << " is odd." << std::endl;
    }
    
    // if-else if-else statement
    std::cout << "\n--- if-else if-else statement ---" << std::endl;
    if (number > 0) {
        std::cout << number << " is positive." << std::endl;
    } else if (number < 0) {
        std::cout << number << " is negative." << std::endl;
    } else {
        std::cout << number << " is zero." << std::endl;
    }
    
    // Nested if statements
    std::cout << "\n--- Nested if statements ---" << std::endl;
    if (number >= 0) {
        std::cout << number << " is non-negative." << std::endl;
        
        if (number % 2 == 0) {
            std::cout << "It is also an even number." << std::endl;
        } else {
            std::cout << "It is also an odd number." << std::endl;
        }
    } else {
        std::cout << number << " is negative." << std::endl;
        
        if (number % 2 == 0) {
            std::cout << "It is also an even number." << std::endl;
        } else {
            std::cout << "It is also an odd number." << std::endl;
        }
    }
    
    // Logical operators with conditionals
    std::cout << "\n--- Logical operators with conditionals ---" << std::endl;
    int age;
    std::cout << "Enter your age: ";
    std::cin >> age;
    
    if (age >= 18 && age <= 65) {
        std::cout << "You are of working age." << std::endl;
    }
    
    if (age < 18 || age > 65) {
        std::cout << "You are either too young or retired." << std::endl;
    }
    
    // Switch statement
    std::cout << "\n--- Switch statement ---" << std::endl;
    char grade;
    std::cout << "Enter your grade (A, B, C, D, or F): ";
    std::cin >> grade;
    
    switch (grade) {
        case 'A':
        case 'a':
            std::cout << "Excellent!" << std::endl;
            break;
        case 'B':
        case 'b':
            std::cout << "Good job!" << std::endl;
            break;
        case 'C':
        case 'c':
            std::cout << "Satisfactory." << std::endl;
            break;
        case 'D':
        case 'd':
            std::cout << "Needs improvement." << std::endl;
            break;
        case 'F':
        case 'f':
            std::cout << "Failed." << std::endl;
            break;
        default:
            std::cout << "Invalid grade entered." << std::endl;
    }
    
    // Conditional (ternary) operator
    std::cout << "\n--- Conditional (ternary) operator ---" << std::endl;
    int x, y;
    std::cout << "Enter two numbers: ";
    std::cin >> x >> y;
    
    // Find the maximum using ternary operator
    int max = (x > y) ? x : y;
    std::cout << "Maximum of " << x << " and " << y << " is " << max << std::endl;
    
    // Nested ternary operator (can be hard to read, use with caution)
    std::string result = (x > 0) ? "positive" : (x < 0) ? "negative" : "zero";
    std::cout << "The first number is " << result << std::endl;
    
    return 0;
}
