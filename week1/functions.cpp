#include <iostream>
#include <string>
#include <cmath>  // For mathematical functions

// Function declaration (prototype)
void greet();
void greet_person(std::string name);
int add(int a, int b);
double add(double a, double b);  // Function overloading
int subtract(int a, int b);
int multiply(int a, int b);
double divide(double a, double b);
int power(int base, int exponent = 2);  // Default argument
void swap_by_value(int a, int b);
void swap_by_reference(int &a, int &b);
void print_array(int arr[], int size);
bool is_prime(int number);
int factorial(int n);  // Recursive function

int main() {
    std::cout << "===== Functions in C++ =====" << std::endl;
    
    // Calling a function with no parameters and no return value
    std::cout << "\n--- Simple function with no parameters ---" << std::endl;
    greet();
    
    // Calling a function with parameters
    std::cout << "\n--- Function with parameters ---" << std::endl;
    std::string user_name;
    std::cout << "Enter your name: ";
    std::getline(std::cin, user_name);
    greet_person(user_name);
    
    // Calling a function with return value
    std::cout << "\n--- Function with return value ---" << std::endl;
    int num1, num2;
    std::cout << "Enter two integers: ";
    std::cin >> num1 >> num2;
    
    int sum = add(num1, num2);
    std::cout << num1 << " + " << num2 << " = " << sum << std::endl;
    
    int difference = subtract(num1, num2);
    std::cout << num1 << " - " << num2 << " = " << difference << std::endl;
    
    int product = multiply(num1, num2);
    std::cout << num1 << " * " << num2 << " = " << product << std::endl;
    
    double quotient = divide(static_cast<double>(num1), static_cast<double>(num2));
    std::cout << num1 << " / " << num2 << " = " << quotient << std::endl;
    
    // Function overloading
    std::cout << "\n--- Function overloading ---" << std::endl;
    double double1 = 3.5, double2 = 2.7;
    std::cout << "Integer addition: " << add(5, 3) << std::endl;
    std::cout << "Double addition: " << add(double1, double2) << std::endl;
    
    // Default arguments
    std::cout << "\n--- Default arguments ---" << std::endl;
    std::cout << "5^2 = " << power(5) << " (using default exponent)" << std::endl;
    std::cout << "5^3 = " << power(5, 3) << " (specifying exponent)" << std::endl;
    
    // Pass by value vs. pass by reference
    std::cout << "\n--- Pass by value vs. Pass by reference ---" << std::endl;
    int x = 10, y = 20;
    std::cout << "Before swap_by_value: x = " << x << ", y = " << y << std::endl;
    swap_by_value(x, y);
    std::cout << "After swap_by_value: x = " << x << ", y = " << y << " (no change)" << std::endl;
    
    std::cout << "Before swap_by_reference: x = " << x << ", y = " << y << std::endl;
    swap_by_reference(x, y);
    std::cout << "After swap_by_reference: x = " << x << ", y = " << y << " (swapped)" << std::endl;
    
    // Array as function parameter
    std::cout << "\n--- Array as function parameter ---" << std::endl;
    int numbers[] = {10, 20, 30, 40, 50};
    int size = sizeof(numbers) / sizeof(numbers[0]);
    std::cout << sizeof(numbers) << " " << sizeof(numbers[0]) << std::endl;
    std::cout << "Array elements: ";
    print_array(numbers, size);
    
    // Function to check if a number is prime
    std::cout << "\n--- Function to check if a number is prime ---" << std::endl;
    int num_to_check;
    std::cout << "Enter a number to check if it's prime: ";
    std::cin >> num_to_check;
    
    if (is_prime(num_to_check)) {
        std::cout << num_to_check << " is a prime number." << std::endl;
    } else {
        std::cout << num_to_check << " is not a prime number." << std::endl;
    }
    
    // Recursive function
    std::cout << "\n--- Recursive function ---" << std::endl;
    int n;
    std::cout << "Enter a number to calculate its factorial: ";
    std::cin >> n;
    
    if (n < 0) {
        std::cout << "Factorial is not defined for negative numbers." << std::endl;
    } else {
        std::cout << n << "! = " << factorial(n) << std::endl;
    }
    
    return 0;
}

// Function definitions

// Simple function with no parameters and no return value
void greet() {
    std::cout << "Hello, welcome to C++ functions!" << std::endl;
}

// Function with parameters
void greet_person(std::string name) {
    std::cout << "Hello, " << name << "! Welcome to C++ functions!" << std::endl;
}

// Function with return value
int add(int a, int b) {
    return a + b;
}

// Function overloading - same name but different parameters
double add(double a, double b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

double divide(double a, double b) {
    if (b == 0) {
        std::cout << "Error: Division by zero!" << std::endl;
        return 0;
    }
    return a / b;
}

// Function with default argument
int power(int base, int exponent) {
    int result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

// Pass by value - changes to parameters don't affect original variables
void swap_by_value(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
    std::cout << "Inside swap_by_value: a = " << a << ", b = " << b << std::endl;
}

// Pass by reference - changes to parameters affect original variables
void swap_by_reference(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
    std::cout << "Inside swap_by_reference: a = " << a << ", b = " << b << std::endl;
}

// Array as function parameter
void print_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

// Function to check if a number is prime
bool is_prime(int number) {
    if (number <= 1) {
        return false;
    }
    
    // Check from 2 to square root of number
    for (int i = 2; i <= sqrt(number); i++) {
        if (number % i == 0) {
            return false;
        }
    }
    
    return true;
}

// Recursive function to calculate factorial
int factorial(int n) {
    // Base case
    if (n == 0 || n == 1) {
        return 1;
    }
    
    // Recursive case
    return n * factorial(n - 1);
}
