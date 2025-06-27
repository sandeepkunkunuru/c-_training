#include <iostream>
#include <cmath>
#include <string>

// Function prototypes
double add(double a, double b);
double subtract(double a, double b);
double multiply(double a, double b);
double divide(double a, double b);
double power(double base, double exponent);
double square_root(double number);
int modulus(int a, int b);
double calculate(double a, double b, char operation);
void display_menu();
bool is_valid_operation(char operation);

int main() {
    // Variables
    double num1, num2, result;
    char operation;
    bool continue_calculation = true;
    std::string input;
    
    std::cout << "===== Enhanced Calculator with Functions =====" << std::endl;
    
    while (continue_calculation) {
        // Display menu
        display_menu();
        
        // Get first number
        std::cout << "Enter first number: ";
        std::cin >> num1;
        
        // Get operation
        std::cout << "Enter operation: ";
        std::cin >> operation;
        
        // Check if operation is valid
        if (!is_valid_operation(operation)) {
            std::cout << "Invalid operation! Please try again." << std::endl;
            continue;
        }
        
        // For square root, we don't need a second number
        if (operation == 'r') {
            result = square_root(num1);
            std::cout << "√" << num1 << " = " << result << std::endl;
        } else {
            // Get second number
            std::cout << "Enter second number: ";
            std::cin >> num2;
            
            // Perform calculation
            result = calculate(num1, num2, operation);
            
            // Display result
            switch (operation) {
                case '+':
                    std::cout << num1 << " + " << num2 << " = " << result << std::endl;
                    break;
                case '-':
                    std::cout << num1 << " - " << num2 << " = " << result << std::endl;
                    break;
                case '*':
                    std::cout << num1 << " * " << num2 << " = " << result << std::endl;
                    break;
                case '/':
                    std::cout << num1 << " / " << num2 << " = " << result << std::endl;
                    break;
                case '^':
                    std::cout << num1 << " ^ " << num2 << " = " << result << std::endl;
                    break;
                case '%':
                    std::cout << static_cast<int>(num1) << " % " << static_cast<int>(num2) << " = " << result << std::endl;
                    break;
            }
        }
        
        // Ask if user wants to continue
        std::cout << "\nDo you want to perform another calculation? (y/n): ";
        std::cin >> input;
        
        if (input != "y" && input != "Y") {
            continue_calculation = false;
        }
    }
    
    std::cout << "Thank you for using the calculator!" << std::endl;
    
    return 0;
}

// Function to display menu
void display_menu() {
    std::cout << "\nAvailable operations:" << std::endl;
    std::cout << "+ : Addition" << std::endl;
    std::cout << "- : Subtraction" << std::endl;
    std::cout << "* : Multiplication" << std::endl;
    std::cout << "/ : Division" << std::endl;
    std::cout << "^ : Power" << std::endl;
    std::cout << "r : Square Root (of first number only)" << std::endl;
    std::cout << "% : Modulus (remainder after division)" << std::endl;
}

// Function to check if operation is valid
bool is_valid_operation(char operation) {
    return (operation == '+' || operation == '-' || operation == '*' || 
            operation == '/' || operation == '^' || operation == 'r' || 
            operation == '%');
}

// Function to perform calculation based on operation
double calculate(double a, double b, char operation) {
    switch (operation) {
        case '+':
            return add(a, b);
        case '-':
            return subtract(a, b);
        case '*':
            return multiply(a, b);
        case '/':
            return divide(a, b);
        case '^':
            return power(a, b);
        case '%':
            return modulus(static_cast<int>(a), static_cast<int>(b));
        default:
            std::cout << "Error: Invalid operation!" << std::endl;
            return 0;
    }
}

// Function to add two numbers
double add(double a, double b) {
    return a + b;
}

// Function to subtract two numbers
double subtract(double a, double b) {
    return a - b;
}

// Function to multiply two numbers
double multiply(double a, double b) {
    return a * b;
}

// Function to divide two numbers
double divide(double a, double b) {
    if (b == 0) {
        std::cout << "Error: Division by zero!" << std::endl;
        return 0;
    }
    return a / b;
}

// Function to calculate power
double power(double base, double exponent) {
    return pow(base, exponent);
}

// Function to calculate square root
double square_root(double number) {
    if (number < 0) {
        std::cout << "Error: Cannot calculate square root of a negative number!" << std::endl;
        return 0;
    }
    return sqrt(number);
}

// Function to calculate modulus (remainder)
int modulus(int a, int b) {
    if (b == 0) {
        std::cout << "Error: Modulus by zero!" << std::endl;
        return 0;
    }
    return a % b;
}
