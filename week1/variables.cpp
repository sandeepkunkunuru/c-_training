#include <iostream>
#include <string>

int main() {
    // Integer types
    int integer_number = 42;
    short short_number = 123;
    long long_number = 123456789L;
    long long very_long_number = 123456789012345LL;
    
    // Floating-point types
    float float_number = 3.14159f;
    double double_number = 2.71828182845904;
    
    // Character types
    char single_character = 'A';
    
    // Boolean type
    bool is_true = true;
    bool is_false = false;
    
    // String type (from the Standard Template Library)
    std::string text = "Hello, C++!";
    
    // Constants
    const int DAYS_IN_WEEK = 7;
    
    // Display all variables
    std::cout << "=== Integer Types ===" << std::endl;
    std::cout << "int: " << integer_number << std::endl;
    std::cout << "short: " << short_number << std::endl;
    std::cout << "long: " << long_number << std::endl;
    std::cout << "long long: " << very_long_number << std::endl;
    
    std::cout << "\n=== Floating-Point Types ===" << std::endl;
    std::cout << "float: " << float_number << std::endl;
    std::cout << "double: " << double_number << std::endl;
    
    std::cout << "\n=== Character Types ===" << std::endl;
    std::cout << "char: " << single_character << " (ASCII value: " 
              << static_cast<int>(single_character) << ")" << std::endl;
    
    std::cout << "\n=== Boolean Types ===" << std::endl;
    std::cout << "true: " << is_true << std::endl;
    std::cout << "false: " << is_false << std::endl;
    
    std::cout << "\n=== String Type ===" << std::endl;
    std::cout << "string: " << text << std::endl;
    
    std::cout << "\n=== Constants ===" << std::endl;
    std::cout << "DAYS_IN_WEEK: " << DAYS_IN_WEEK << std::endl;
    
    // Type sizes
    std::cout << "\n=== Size of Types (in bytes) ===" << std::endl;
    std::cout << "Size of int: " << sizeof(int) << std::endl;
    std::cout << "Size of short: " << sizeof(short) << std::endl;
    std::cout << "Size of long: " << sizeof(long) << std::endl;
    std::cout << "Size of long long: " << sizeof(long long) << std::endl;
    std::cout << "Size of float: " << sizeof(float) << std::endl;
    std::cout << "Size of double: " << sizeof(double) << std::endl;
    std::cout << "Size of char: " << sizeof(char) << std::endl;
    std::cout << "Size of bool: " << sizeof(bool) << std::endl;
    
    return 0;
}
