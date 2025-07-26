/*
 * Demo Program
 * 
 * This program demonstrates various features of the MathUtils library
 * and showcases CMake's ability to create multiple executables.
 */

#include "math_utils.hpp"
#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

using namespace MathUtils;

void demonstrate_calculator() {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "  Calculator Demonstration" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    Calculator calc;
    std::cout << "Calculator: " << calc.to_string() << std::endl;
    
    // Basic operations
    std::cout << "\nBasic Operations:" << std::endl;
    std::cout << "  15 + 7 = " << calc.add(15, 7) << std::endl;
    std::cout << "  15 - 7 = " << calc.subtract(15, 7) << std::endl;
    std::cout << "  15 * 7 = " << calc.multiply(15, 7) << std::endl;
    std::cout << "  15 / 7 = " << std::fixed << std::setprecision(3) << calc.divide(15, 7) << std::endl;
    std::cout << "  2^10 = " << calc.power(2, 10) << std::endl;
    std::cout << "  sqrt(144) = " << calc.sqrt(144) << std::endl;
    std::cout << "  5! = " << calc.factorial(5) << std::endl;
    
    // Number theory
    std::cout << "\nNumber Theory:" << std::endl;
    std::cout << "  Is 17 prime? " << (calc.is_prime(17) ? "Yes" : "No") << std::endl;
    std::cout << "  Is 15 prime? " << (calc.is_prime(15) ? "Yes" : "No") << std::endl;
    std::cout << "  gcd(48, 18) = " << calc.gcd(48, 18) << std::endl;
    std::cout << "  lcm(12, 15) = " << calc.lcm(12, 15) << std::endl;
}

void demonstrate_statistics() {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "  Statistics Demonstration" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    // Generate random data
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(50.0, 15.0);
    
    std::vector<double> data;
    data.reserve(100);
    for (int i = 0; i < 100; ++i) {
        data.push_back(dist(gen));
    }
    
    std::cout << "Generated 100 random numbers from normal distribution (μ=50, σ=15)" << std::endl;
    
    // Calculate statistics
    auto stats = Statistics::describe(data);
    std::cout << "\nDescriptive Statistics:" << std::endl;
    std::cout << stats.to_string() << std::endl;
    
    // Additional statistics
    std::cout << "\nPercentiles:" << std::endl;
    std::cout << "  10th percentile: " << std::fixed << std::setprecision(2) 
              << Statistics::percentile(data, 10) << std::endl;
    std::cout << "  25th percentile: " << Statistics::percentile(data, 25) << std::endl;
    std::cout << "  50th percentile: " << Statistics::percentile(data, 50) << std::endl;
    std::cout << "  75th percentile: " << Statistics::percentile(data, 75) << std::endl;
    std::cout << "  90th percentile: " << Statistics::percentile(data, 90) << std::endl;
    
    // Show first few values
    std::cout << "\nFirst 10 values: ";
    for (int i = 0; i < 10; ++i) {
        std::cout << std::fixed << std::setprecision(1) << data[i] << " ";
    }
    std::cout << std::endl;
}

void demonstrate_geometry() {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "  Geometry Demonstration" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    // Points
    Geometry::Point2D p1(0, 0);
    Geometry::Point2D p2(3, 4);
    Geometry::Point2D p3(-2, 1);
    
    std::cout << "Points:" << std::endl;
    std::cout << "  P1: " << p1.to_string() << std::endl;
    std::cout << "  P2: " << p2.to_string() << std::endl;
    std::cout << "  P3: " << p3.to_string() << std::endl;
    
    std::cout << "\nDistances:" << std::endl;
    std::cout << "  P1 to P2: " << std::fixed << std::setprecision(3) << p1.distance_to(p2) << std::endl;
    std::cout << "  P1 to P3: " << p1.distance_to(p3) << std::endl;
    std::cout << "  P2 to P3: " << p2.distance_to(p3) << std::endl;
    
    // Circle
    Geometry::Circle circle(p1, 5.0);
    std::cout << "\nCircle:" << std::endl;
    std::cout << "  " << circle.to_string() << std::endl;
    std::cout << "  Area: " << circle.area() << std::endl;
    std::cout << "  Circumference: " << circle.circumference() << std::endl;
    std::cout << "  Contains P2? " << (circle.contains(p2) ? "Yes" : "No") << std::endl;
    std::cout << "  Contains P3? " << (circle.contains(p3) ? "Yes" : "No") << std::endl;
    
    // Rectangle
    Geometry::Rectangle rect(Geometry::Point2D(-1, -1), 6, 4);
    std::cout << "\nRectangle:" << std::endl;
    std::cout << "  " << rect.to_string() << std::endl;
    std::cout << "  Area: " << rect.area() << std::endl;
    std::cout << "  Perimeter: " << rect.perimeter() << std::endl;
    std::cout << "  Contains P1? " << (rect.contains(p1) ? "Yes" : "No") << std::endl;
    std::cout << "  Contains P2? " << (rect.contains(p2) ? "Yes" : "No") << std::endl;
    std::cout << "  Contains P3? " << (rect.contains(p3) ? "Yes" : "No") << std::endl;
}

void demonstrate_number_theory() {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "  Number Theory Demonstration" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    std::vector<int> test_numbers = {12, 28, 153, 1634, 9474, 54748};
    
    for (int n : test_numbers) {
        std::cout << "\nAnalyzing " << n << ":" << std::endl;
        
        // Prime factors
        auto factors = NumberTheory::prime_factors(n);
        std::cout << "  Prime factorization: ";
        for (size_t i = 0; i < factors.size(); ++i) {
            if (i > 0) std::cout << " × ";
            std::cout << factors[i];
        }
        std::cout << std::endl;
        
        // Divisors
        auto divisors = NumberTheory::divisors(n);
        std::cout << "  Divisors (" << divisors.size() << "): ";
        for (size_t i = 0; i < std::min(divisors.size(), size_t(10)); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << divisors[i];
        }
        if (divisors.size() > 10) {
            std::cout << ", ...";
        }
        std::cout << std::endl;
        
        // Properties
        std::cout << "  Perfect number: " << (NumberTheory::is_perfect_number(n) ? "Yes" : "No") << std::endl;
        std::cout << "  Palindrome: " << (NumberTheory::is_palindrome(n) ? "Yes" : "No") << std::endl;
        std::cout << "  Digit sum: " << NumberTheory::digit_sum(n) << std::endl;
        std::cout << "  Reversed: " << NumberTheory::reverse_digits(n) << std::endl;
    }
    
    // Find some perfect numbers
    std::cout << "\nSearching for perfect numbers up to 10000:" << std::endl;
    std::vector<int> perfect_numbers;
    for (int i = 1; i <= 10000; ++i) {
        if (NumberTheory::is_perfect_number(i)) {
            perfect_numbers.push_back(i);
        }
    }
    
    std::cout << "Perfect numbers found: ";
    for (size_t i = 0; i < perfect_numbers.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << perfect_numbers[i];
    }
    std::cout << std::endl;
}

void demonstrate_version_info() {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "  Version Information" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    std::cout << "MathUtils Library Information:" << std::endl;
    std::cout << "  Version: " << VERSION << std::endl;
    std::cout << "  Build Date: " << BUILD_DATE << std::endl;
    std::cout << "  Compiler: " << COMPILER_INFO << std::endl;
    
    std::cout << "\nCMake Build Information:" << std::endl;
    std::cout << "  This demo was built using CMake" << std::endl;
    std::cout << "  Demonstrates library linking and multiple executables" << std::endl;
    std::cout << "  Shows modern CMake best practices" << std::endl;
}

int main() {
    std::cout << "===== MathUtils Library Demo =====" << std::endl;
    std::cout << "This program demonstrates the capabilities of the MathUtils library" << std::endl;
    std::cout << "built with CMake." << std::endl;
    
    demonstrate_version_info();
    demonstrate_calculator();
    demonstrate_statistics();
    demonstrate_geometry();
    demonstrate_number_theory();
    
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "Demo completed successfully" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    return 0;
}
