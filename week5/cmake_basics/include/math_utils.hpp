/*
 * Math Utils Library Header
 * 
 * This header demonstrates CMake library creation and linking.
 * It provides basic mathematical operations and utilities.
 */

#pragma once

#include <vector>
#include <string>

namespace MathUtils {

/**
 * @brief Basic arithmetic operations
 */
class Calculator {
public:
    Calculator() = default;
    ~Calculator() = default;
    
    // Basic operations
    double add(double a, double b) const;
    double subtract(double a, double b) const;
    double multiply(double a, double b) const;
    double divide(double a, double b) const;
    
    // Advanced operations
    double power(double base, double exponent) const;
    double sqrt(double value) const;
    double factorial(int n) const;
    
    // Vector operations
    double sum(const std::vector<double>& values) const;
    double mean(const std::vector<double>& values) const;
    double variance(const std::vector<double>& values) const;
    double standard_deviation(const std::vector<double>& values) const;
    
    // Utility functions
    bool is_prime(int n) const;
    int gcd(int a, int b) const;
    int lcm(int a, int b) const;
    
    // String representation
    std::string to_string() const;
};

/**
 * @brief Statistical functions
 */
namespace Statistics {
    double min(const std::vector<double>& values);
    double max(const std::vector<double>& values);
    double median(const std::vector<double>& values);
    double percentile(const std::vector<double>& values, double p);
    
    struct DescriptiveStats {
        double mean;
        double median;
        double std_dev;
        double min_val;
        double max_val;
        size_t count;
        
        std::string to_string() const;
    };
    
    DescriptiveStats describe(const std::vector<double>& values);
}

/**
 * @brief Geometry utilities
 */
namespace Geometry {
    struct Point2D {
        double x, y;
        Point2D(double x = 0, double y = 0) : x(x), y(y) {}
        
        double distance_to(const Point2D& other) const;
        std::string to_string() const;
    };
    
    struct Circle {
        Point2D center;
        double radius;
        
        Circle(Point2D center, double radius) : center(center), radius(radius) {}
        
        double area() const;
        double circumference() const;
        bool contains(const Point2D& point) const;
        std::string to_string() const;
    };
    
    struct Rectangle {
        Point2D top_left;
        double width, height;
        
        Rectangle(Point2D top_left, double width, double height) 
            : top_left(top_left), width(width), height(height) {}
        
        double area() const;
        double perimeter() const;
        bool contains(const Point2D& point) const;
        std::string to_string() const;
    };
}

/**
 * @brief Number theory utilities
 */
namespace NumberTheory {
    std::vector<int> prime_factors(int n);
    std::vector<int> divisors(int n);
    bool is_perfect_number(int n);
    bool is_palindrome(int n);
    int reverse_digits(int n);
    int digit_sum(int n);
}

// Version information
extern const char* VERSION;
extern const char* BUILD_DATE;
extern const char* COMPILER_INFO;

} // namespace MathUtils
