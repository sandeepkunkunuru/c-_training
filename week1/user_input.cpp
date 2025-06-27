#include <iostream>
#include <string>

int main() {
    // Getting integer input
    int age;
    std::cout << "Enter your age: ";
    std::cin >> age;
    
    // Getting floating-point input
    double height;
    std::cout << "Enter your height in meters: ";
    std::cin >> height;
    
    // Consuming the newline character left in the input buffer
    std::cin.ignore();
    
    // Getting string input with spaces
    std::string full_name;
    std::cout << "Enter your full name: ";
    std::getline(std::cin, full_name);
    
    // Getting a single character
    char grade;
    std::cout << "Enter your grade (A, B, C, D, or F): ";
    std::cin >> grade;
    
    // Display the information back to the user
    std::cout << "\n=== User Information ===" << std::endl;
    std::cout << "Name: " << full_name << std::endl;
    std::cout << "Age: " << age << " years old" << std::endl;
    std::cout << "Height: " << height << " meters" << std::endl;
    std::cout << "Grade: " << grade << std::endl;
    
    // Simple calculation with user input
    int birth_year = 2025 - age;  // Using current year
    std::cout << "You were born around: " << birth_year << std::endl;
    
    // Input validation example
    if (grade == 'A' || grade == 'B') {
        std::cout << "Great job on your grade!" << std::endl;
    } else if (grade == 'C') {
        std::cout << "You're doing okay, but could improve." << std::endl;
    } else if (grade == 'D' || grade == 'F') {
        std::cout << "You might need some additional study time." << std::endl;
    } else {
        std::cout << "Invalid grade entered." << std::endl;
    }
    
    return 0;
}
