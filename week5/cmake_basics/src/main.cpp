/*
 * Calculator Main Program
 * 
 * This program demonstrates CMake executable creation and library linking.
 * It provides an interactive calculator using the MathUtils library.
 */

#include "math_utils.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace MathUtils;

void print_banner() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Advanced Calculator - CMake Basics Demo" << std::endl;
    std::cout << "  Version: " << VERSION << std::endl;
    std::cout << "  Built: " << BUILD_DATE << std::endl;
    std::cout << "  Compiler: " << COMPILER_INFO << std::endl;
    std::cout << std::string(60, '=') << std::endl;
}

void print_menu() {
    std::cout << "\nAvailable operations:" << std::endl;
    std::cout << "1. Basic arithmetic (+, -, *, /, ^, sqrt)" << std::endl;
    std::cout << "2. Statistical analysis" << std::endl;
    std::cout << "3. Geometry calculations" << std::endl;
    std::cout << "4. Number theory" << std::endl;
    std::cout << "5. Run tests" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "\nEnter your choice: ";
}

void basic_arithmetic() {
    Calculator calc;
    
    std::cout << "\n--- Basic Arithmetic ---" << std::endl;
    std::cout << "Enter two numbers: ";
    double a, b;
    std::cin >> a >> b;
    
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Results:" << std::endl;
    std::cout << "  " << a << " + " << b << " = " << calc.add(a, b) << std::endl;
    std::cout << "  " << a << " - " << b << " = " << calc.subtract(a, b) << std::endl;
    std::cout << "  " << a << " * " << b << " = " << calc.multiply(a, b) << std::endl;
    
    try {
        std::cout << "  " << a << " / " << b << " = " << calc.divide(a, b) << std::endl;
    } catch (const std::exception& e) {
        std::cout << "  Division error: " << e.what() << std::endl;
    }
    
    std::cout << "  " << a << " ^ " << b << " = " << calc.power(a, b) << std::endl;
    
    try {
        std::cout << "  sqrt(" << a << ") = " << calc.sqrt(a) << std::endl;
        std::cout << "  sqrt(" << b << ") = " << calc.sqrt(b) << std::endl;
    } catch (const std::exception& e) {
        std::cout << "  Square root error: " << e.what() << std::endl;
    }
    
    // Factorial for integers
    int ia = static_cast<int>(a);
    int ib = static_cast<int>(b);
    if (ia >= 0 && ia <= 20) {
        std::cout << "  " << ia << "! = " << calc.factorial(ia) << std::endl;
    }
    if (ib >= 0 && ib <= 20) {
        std::cout << "  " << ib << "! = " << calc.factorial(ib) << std::endl;
    }
    
    // GCD and LCM
    if (ia > 0 && ib > 0) {
        std::cout << "  gcd(" << ia << ", " << ib << ") = " << calc.gcd(ia, ib) << std::endl;
        std::cout << "  lcm(" << ia << ", " << ib << ") = " << calc.lcm(ia, ib) << std::endl;
    }
}

void statistical_analysis() {
    std::cout << "\n--- Statistical Analysis ---" << std::endl;
    std::cout << "Enter numbers separated by spaces (end with 'done'): ";
    
    std::vector<double> values;
    std::string input;
    std::cin.ignore(); // Clear the input buffer
    
    while (std::getline(std::cin, input) && input != "done") {
        std::istringstream iss(input);
        double value;
        while (iss >> value) {
            values.push_back(value);
        }
        if (input.find("done") != std::string::npos) break;
        std::cout << "Continue entering numbers (or 'done' to finish): ";
    }
    
    if (values.empty()) {
        std::cout << "No values entered." << std::endl;
        return;
    }
    
    std::cout << "\nEntered " << values.size() << " values: ";
    for (size_t i = 0; i < std::min(values.size(), size_t(10)); ++i) {
        std::cout << values[i] << " ";
    }
    if (values.size() > 10) {
        std::cout << "... (showing first 10)";
    }
    std::cout << std::endl;
    
    // Calculate statistics
    auto stats = Statistics::describe(values);
    std::cout << "\nStatistical Summary:" << std::endl;
    std::cout << stats.to_string() << std::endl;
    
    // Additional statistics
    try {
        std::cout << "\nAdditional Statistics:" << std::endl;
        std::cout << "  25th percentile: " << Statistics::percentile(values, 25.0) << std::endl;
        std::cout << "  75th percentile: " << Statistics::percentile(values, 75.0) << std::endl;
        std::cout << "  90th percentile: " << Statistics::percentile(values, 90.0) << std::endl;
    } catch (const std::exception& e) {
        std::cout << "  Error calculating percentiles: " << e.what() << std::endl;
    }
}

void geometry_calculations() {
    std::cout << "\n--- Geometry Calculations ---" << std::endl;
    std::cout << "Choose geometry type:" << std::endl;
    std::cout << "1. Circle" << std::endl;
    std::cout << "2. Rectangle" << std::endl;
    std::cout << "3. Point distance" << std::endl;
    std::cout << "Enter choice: ";
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1: {
            std::cout << "Enter circle center (x y) and radius: ";
            double x, y, radius;
            std::cin >> x >> y >> radius;
            
            Geometry::Circle circle({x, y}, radius);
            std::cout << "\nCircle: " << circle.to_string() << std::endl;
            std::cout << "Area: " << circle.area() << std::endl;
            std::cout << "Circumference: " << circle.circumference() << std::endl;
            
            std::cout << "Enter a point (x y) to test containment: ";
            std::cin >> x >> y;
            Geometry::Point2D test_point(x, y);
            std::cout << "Point " << test_point.to_string() 
                      << (circle.contains(test_point) ? " is inside" : " is outside") 
                      << " the circle." << std::endl;
            break;
        }
        case 2: {
            std::cout << "Enter rectangle top-left (x y), width, and height: ";
            double x, y, width, height;
            std::cin >> x >> y >> width >> height;
            
            Geometry::Rectangle rect({x, y}, width, height);
            std::cout << "\nRectangle: " << rect.to_string() << std::endl;
            std::cout << "Area: " << rect.area() << std::endl;
            std::cout << "Perimeter: " << rect.perimeter() << std::endl;
            
            std::cout << "Enter a point (x y) to test containment: ";
            std::cin >> x >> y;
            Geometry::Point2D test_point(x, y);
            std::cout << "Point " << test_point.to_string() 
                      << (rect.contains(test_point) ? " is inside" : " is outside") 
                      << " the rectangle." << std::endl;
            break;
        }
        case 3: {
            std::cout << "Enter first point (x y): ";
            double x1, y1;
            std::cin >> x1 >> y1;
            
            std::cout << "Enter second point (x y): ";
            double x2, y2;
            std::cin >> x2 >> y2;
            
            Geometry::Point2D p1(x1, y1);
            Geometry::Point2D p2(x2, y2);
            
            std::cout << "\nDistance between " << p1.to_string() 
                      << " and " << p2.to_string() 
                      << " is " << p1.distance_to(p2) << std::endl;
            break;
        }
        default:
            std::cout << "Invalid choice." << std::endl;
    }
}

void number_theory() {
    std::cout << "\n--- Number Theory ---" << std::endl;
    std::cout << "Enter a positive integer: ";
    int n;
    std::cin >> n;
    
    if (n <= 0) {
        std::cout << "Please enter a positive integer." << std::endl;
        return;
    }
    
    Calculator calc;
    
    std::cout << "\nAnalysis of " << n << ":" << std::endl;
    std::cout << "  Is prime: " << (calc.is_prime(n) ? "Yes" : "No") << std::endl;
    std::cout << "  Is perfect: " << (NumberTheory::is_perfect_number(n) ? "Yes" : "No") << std::endl;
    std::cout << "  Is palindrome: " << (NumberTheory::is_palindrome(n) ? "Yes" : "No") << std::endl;
    std::cout << "  Reversed: " << NumberTheory::reverse_digits(n) << std::endl;
    std::cout << "  Digit sum: " << NumberTheory::digit_sum(n) << std::endl;
    
    auto factors = NumberTheory::prime_factors(n);
    std::cout << "  Prime factors: ";
    for (size_t i = 0; i < factors.size(); ++i) {
        if (i > 0) std::cout << " × ";
        std::cout << factors[i];
    }
    std::cout << std::endl;
    
    auto divisors = NumberTheory::divisors(n);
    std::cout << "  Divisors: ";
    for (size_t i = 0; i < divisors.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << divisors[i];
    }
    std::cout << std::endl;
}

void run_tests() {
    std::cout << "\n--- Running Tests ---" << std::endl;
    
    Calculator calc;
    bool all_passed = true;
    
    // Test basic arithmetic
    std::cout << "Testing basic arithmetic..." << std::endl;
    if (calc.add(2, 3) != 5) {
        std::cout << "  FAIL: Addition test" << std::endl;
        all_passed = false;
    }
    if (calc.multiply(4, 5) != 20) {
        std::cout << "  FAIL: Multiplication test" << std::endl;
        all_passed = false;
    }
    
    // Test statistics
    std::cout << "Testing statistics..." << std::endl;
    std::vector<double> test_data = {1, 2, 3, 4, 5};
    if (std::abs(calc.mean(test_data) - 3.0) > 1e-6) {
        std::cout << "  FAIL: Mean calculation" << std::endl;
        all_passed = false;
    }
    
    // Test geometry
    std::cout << "Testing geometry..." << std::endl;
    Geometry::Circle circle({0, 0}, 5);
    if (std::abs(circle.area() - (M_PI * 25)) > 1e-6) {
        std::cout << "  FAIL: Circle area calculation" << std::endl;
        all_passed = false;
    }
    
    // Test number theory
    std::cout << "Testing number theory..." << std::endl;
    if (!calc.is_prime(17)) {
        std::cout << "  FAIL: Prime number test" << std::endl;
        all_passed = false;
    }
    
    if (all_passed) {
        std::cout << "All tests PASSED!" << std::endl;
    } else {
        std::cout << "Some tests FAILED!" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // Check for test mode
    if (argc > 1 && std::string(argv[1]) == "--test") {
        run_tests();
        return 0;
    }
    
    print_banner();
    
    int choice;
    do {
        print_menu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                basic_arithmetic();
                break;
            case 2:
                statistical_analysis();
                break;
            case 3:
                geometry_calculations();
                break;
            case 4:
                number_theory();
                break;
            case 5:
                run_tests();
                break;
            case 0:
                std::cout << "Goodbye!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
        
        if (choice != 0) {
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore();
            std::cin.get();
        }
        
    } while (choice != 0);
    
    return 0;
}
