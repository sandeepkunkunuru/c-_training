#include <iostream>

int main() {
    // Variables for demonstration
    int a = 10;
    int b = 3;
    
    // Arithmetic Operators
    std::cout << "=== Arithmetic Operators ===" << std::endl;
    std::cout << "a = " << a << ", b = " << b << std::endl;
    std::cout << "a + b = " << (a + b) << std::endl;  // Addition
    std::cout << "a - b = " << (a - b) << std::endl;  // Subtraction
    std::cout << "a * b = " << (a * b) << std::endl;  // Multiplication
    std::cout << "a / b = " << (a / b) << std::endl;  // Division (integer division)
    std::cout << "a % b = " << (a % b) << std::endl;  // Modulus (remainder)
    
    // Note on floating-point division
    double c = 10.0;
    double d = 3.0;
    std::cout << "c = " << c << ", d = " << d << std::endl;
    std::cout << "c / d = " << (c / d) << " (floating-point division)" << std::endl;
    
    // Increment and Decrement Operators
    std::cout << "\n=== Increment and Decrement Operators ===" << std::endl;
    int x = 5;
    std::cout << "Initial x = " << x << std::endl;
    
    // Pre-increment
    std::cout << "++x = " << (++x) << " (pre-increment)" << std::endl;
    std::cout << "x is now " << x << std::endl;
    
    // Post-increment
    std::cout << "x++ = " << (x++) << " (post-increment)" << std::endl;
    std::cout << "x is now " << x << std::endl;
    
    // Pre-decrement
    std::cout << "--x = " << (--x) << " (pre-decrement)" << std::endl;
    std::cout << "x is now " << x << std::endl;
    
    // Post-decrement
    std::cout << "x-- = " << (x--) << " (post-decrement)" << std::endl;
    std::cout << "x is now " << x << std::endl;
    
    // Assignment Operators
    std::cout << "\n=== Assignment Operators ===" << std::endl;
    int y = 10;
    std::cout << "Initial y = " << y << std::endl;
    
    y += 5;  // Equivalent to: y = y + 5
    std::cout << "After y += 5, y = " << y << std::endl;
    
    y -= 3;  // Equivalent to: y = y - 3
    std::cout << "After y -= 3, y = " << y << std::endl;
    
    y *= 2;  // Equivalent to: y = y * 2
    std::cout << "After y *= 2, y = " << y << std::endl;
    
    y /= 4;  // Equivalent to: y = y / 4
    std::cout << "After y /= 4, y = " << y << std::endl;
    
    y %= 3;  // Equivalent to: y = y % 3
    std::cout << "After y %= 3, y = " << y << std::endl;
    
    // Comparison Operators
    std::cout << "\n=== Comparison Operators ===" << std::endl;
    int m = 10;
    int n = 20;
    std::cout << "m = " << m << ", n = " << n << std::endl;
    
    std::cout << "m == n: " << (m == n) << " (equal to)" << std::endl;
    std::cout << "m != n: " << (m != n) << " (not equal to)" << std::endl;
    std::cout << "m > n: " << (m > n) << " (greater than)" << std::endl;
    std::cout << "m < n: " << (m < n) << " (less than)" << std::endl;
    std::cout << "m >= n: " << (m >= n) << " (greater than or equal to)" << std::endl;
    std::cout << "m <= n: " << (m <= n) << " (less than or equal to)" << std::endl;
    
    // Logical Operators
    std::cout << "\n=== Logical Operators ===" << std::endl;
    bool p = true;
    bool q = false;
    std::cout << "p = " << p << ", q = " << q << std::endl;
    
    std::cout << "p && q: " << (p && q) << " (logical AND)" << std::endl;
    std::cout << "p || q: " << (p || q) << " (logical OR)" << std::endl;
    std::cout << "!p: " << (!p) << " (logical NOT)" << std::endl;
    std::cout << "!q: " << (!q) << " (logical NOT)" << std::endl;
    
    // Bitwise Operators (for advanced understanding)
    std::cout << "\n=== Bitwise Operators ===" << std::endl;
    unsigned int r = 5;  // 0101 in binary
    unsigned int s = 3;  // 0011 in binary
    std::cout << "r = " << r << " (0101 in binary), s = " << s << " (0011 in binary)" << std::endl;
    
    std::cout << "r & s: " << (r & s) << " (bitwise AND)" << std::endl;
    std::cout << "r | s: " << (r | s) << " (bitwise OR)" << std::endl;
    std::cout << "r ^ s: " << (r ^ s) << " (bitwise XOR)" << std::endl;
    std::cout << "~r: " << (~r) << " (bitwise NOT)" << std::endl;
    std::cout << "r << 1: " << (r << 1) << " (left shift)" << std::endl;
    std::cout << "r >> 1: " << (r >> 1) << " (right shift)" << std::endl;
    
    return 0;
}
