#include <iostream>

int main() {
    // Variables to store the numbers and operation
    double num1, num2, result;
    char operation;
    
    // Display welcome message
    std::cout << "===== Simple Calculator =====" << std::endl;
    std::cout << "Operations available: +, -, *, /, %" << std::endl;
    
    // Get first number
    std::cout << "Enter first number: ";
    std::cin >> num1;
    
    // Get operation
    std::cout << "Enter operation (+, -, *, /, %): ";
    std::cin >> operation;
    
    // Get second number
    std::cout << "Enter second number: ";
    std::cin >> num2;
    
    // Perform calculation based on the operation
    switch (operation) {
        case '+':
            result = num1 + num2;
            std::cout << num1 << " + " << num2 << " = " << result << std::endl;
            break;
            
        case '-':
            result = num1 - num2;
            std::cout << num1 << " - " << num2 << " = " << result << std::endl;
            break;
            
        case '*':
            result = num1 * num2;
            std::cout << num1 << " * " << num2 << " = " << result << std::endl;
            break;
            
        case '/':
            // Check for division by zero
            if (num2 != 0) {
                result = num1 / num2;
                std::cout << num1 << " / " << num2 << " = " << result << std::endl;
            } else {
                std::cout << "Error: Division by zero is not allowed." << std::endl;
            }
            break;
            
        case '%':
            // Modulus operator works only with integers
            if (num2 != 0) {
                // Cast to int for modulus operation
                result = static_cast<int>(num1) % static_cast<int>(num2);
                std::cout << static_cast<int>(num1) << " % " << static_cast<int>(num2) << " = " << result << std::endl;
                std::cout << "(Note: Modulus operation converted numbers to integers)" << std::endl;
            } else {
                std::cout << "Error: Division by zero is not allowed." << std::endl;
            }
            break;
            
        default:
            std::cout << "Error: Invalid operation. Please use +, -, *, /, or %" << std::endl;
    }
    
    return 0;
}
