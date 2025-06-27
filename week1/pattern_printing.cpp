#include <iostream>
#include <string>

int main() {
    std::cout << "===== Pattern Printing with Loops =====" << std::endl;
    
    int height;
    std::cout << "Enter the height for patterns (5-10 recommended): ";
    std::cin >> height;
    
    // Validate input
    if (height <= 0) {
        std::cout << "Height must be positive. Using default height of 5." << std::endl;
        height = 5;
    } else if (height > 20) {
        std::cout << "Height too large. Using maximum height of 20." << std::endl;
        height = 20;
    }
    
    // Pattern 1: Right-angled triangle with asterisks
    std::cout << "\n--- Pattern 1: Right-angled triangle ---" << std::endl;
    for (int i = 1; i <= height; i++) {
        for (int j = 1; j <= i; j++) {
            std::cout << "* ";
        }
        std::cout << std::endl;
    }
    
    // Pattern 2: Inverted right-angled triangle
    std::cout << "\n--- Pattern 2: Inverted right-angled triangle ---" << std::endl;
    for (int i = height; i >= 1; i--) {
        for (int j = 1; j <= i; j++) {
            std::cout << "* ";
        }
        std::cout << std::endl;
    }
    
    // Pattern 3: Pyramid
    std::cout << "\n--- Pattern 3: Pyramid ---" << std::endl;
    for (int i = 1; i <= height; i++) {
        // Print spaces
        for (int j = 1; j <= height - i; j++) {
            std::cout << " ";
        }
        
        // Print stars
        for (int k = 1; k <= 2 * i - 1; k++) {
            std::cout << "*";
        }
        
        std::cout << std::endl;
    }
    
    // Pattern 4: Number triangle
    std::cout << "\n--- Pattern 4: Number triangle ---" << std::endl;
    for (int i = 1; i <= height; i++) {
        for (int j = 1; j <= i; j++) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
    
    // Pattern 5: Number pyramid with same numbers in rows
    std::cout << "\n--- Pattern 5: Number pyramid with same numbers in rows ---" << std::endl;
    for (int i = 1; i <= height; i++) {
        // Print spaces
        for (int j = 1; j <= height - i; j++) {
            std::cout << " ";
        }
        
        // Print numbers
        for (int k = 1; k <= i; k++) {
            std::cout << i << " ";
        }
        
        std::cout << std::endl;
    }
    
    // Pattern 6: Character pattern
    std::cout << "\n--- Pattern 6: Character pattern ---" << std::endl;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j <= i; j++) {
            // Print characters A, B, C, etc.
            char ch = 'A' + j;
            std::cout << ch << " ";
        }
        std::cout << std::endl;
    }
    
    // Pattern 7: Hollow square
    std::cout << "\n--- Pattern 7: Hollow square ---" << std::endl;
    for (int i = 1; i <= height; i++) {
        for (int j = 1; j <= height; j++) {
            // Print stars only for the border
            if (i == 1 || i == height || j == 1 || j == height) {
                std::cout << "* ";
            } else {
                std::cout << "  ";  // Two spaces to maintain alignment
            }
        }
        std::cout << std::endl;
    }
    
    // Pattern 8: Diamond pattern
    std::cout << "\n--- Pattern 8: Diamond pattern ---" << std::endl;
    int n = height / 2 + 1;
    
    // Upper half of the diamond
    for (int i = 1; i <= n; i++) {
        // Print spaces
        for (int j = 1; j <= n - i; j++) {
            std::cout << " ";
        }
        
        // Print stars
        for (int k = 1; k <= 2 * i - 1; k++) {
            std::cout << "*";
        }
        
        std::cout << std::endl;
    }
    
    // Lower half of the diamond
    for (int i = n - 1; i >= 1; i--) {
        // Print spaces
        for (int j = 1; j <= n - i; j++) {
            std::cout << " ";
        }
        
        // Print stars
        for (int k = 1; k <= 2 * i - 1; k++) {
            std::cout << "*";
        }
        
        std::cout << std::endl;
    }
    
    return 0;
}
