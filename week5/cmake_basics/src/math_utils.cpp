/*
 * Math Utils Library Implementation
 * 
 * This file implements the mathematical utilities defined in math_utils.hpp.
 * It demonstrates CMake library compilation and organization.
 */

#include "math_utils.hpp"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <sstream>
#include <iomanip>

namespace MathUtils {

// Version information
const char* VERSION = "1.0.0";
const char* BUILD_DATE = __DATE__ " " __TIME__;
const char* COMPILER_INFO = 
#ifdef __GNUC__
    "GCC " __VERSION__;
#elif defined(__clang__)
    "Clang " __clang_version__;
#elif defined(_MSC_VER)
    "MSVC";
#else
    "Unknown compiler";
#endif

// Calculator implementation
double Calculator::add(double a, double b) const {
    return a + b;
}

double Calculator::subtract(double a, double b) const {
    return a - b;
}

double Calculator::multiply(double a, double b) const {
    return a * b;
}

double Calculator::divide(double a, double b) const {
    if (std::abs(b) < 1e-10) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
}

double Calculator::power(double base, double exponent) const {
    return std::pow(base, exponent);
}

double Calculator::sqrt(double value) const {
    if (value < 0) {
        throw std::invalid_argument("Square root of negative number");
    }
    return std::sqrt(value);
}

double Calculator::factorial(int n) const {
    if (n < 0) {
        throw std::invalid_argument("Factorial of negative number");
    }
    if (n == 0 || n == 1) return 1.0;
    
    double result = 1.0;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

double Calculator::sum(const std::vector<double>& values) const {
    return std::accumulate(values.begin(), values.end(), 0.0);
}

double Calculator::mean(const std::vector<double>& values) const {
    if (values.empty()) {
        throw std::invalid_argument("Cannot calculate mean of empty vector");
    }
    return sum(values) / values.size();
}

double Calculator::variance(const std::vector<double>& values) const {
    if (values.empty()) {
        throw std::invalid_argument("Cannot calculate variance of empty vector");
    }
    
    double m = mean(values);
    double sum_sq_diff = 0.0;
    
    for (double value : values) {
        double diff = value - m;
        sum_sq_diff += diff * diff;
    }
    
    return sum_sq_diff / values.size();
}

double Calculator::standard_deviation(const std::vector<double>& values) const {
    return std::sqrt(variance(values));
}

bool Calculator::is_prime(int n) const {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

int Calculator::gcd(int a, int b) const {
    a = std::abs(a);
    b = std::abs(b);
    
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int Calculator::lcm(int a, int b) const {
    if (a == 0 || b == 0) return 0;
    return std::abs(a * b) / gcd(a, b);
}

std::string Calculator::to_string() const {
    return "MathUtils::Calculator v" + std::string(VERSION);
}

// Statistics implementation
namespace Statistics {
    double min(const std::vector<double>& values) {
        if (values.empty()) {
            throw std::invalid_argument("Cannot find min of empty vector");
        }
        return *std::min_element(values.begin(), values.end());
    }
    
    double max(const std::vector<double>& values) {
        if (values.empty()) {
            throw std::invalid_argument("Cannot find max of empty vector");
        }
        return *std::max_element(values.begin(), values.end());
    }
    
    double median(const std::vector<double>& values) {
        if (values.empty()) {
            throw std::invalid_argument("Cannot find median of empty vector");
        }
        
        std::vector<double> sorted_values = values;
        std::sort(sorted_values.begin(), sorted_values.end());
        
        size_t n = sorted_values.size();
        if (n % 2 == 0) {
            return (sorted_values[n/2 - 1] + sorted_values[n/2]) / 2.0;
        } else {
            return sorted_values[n/2];
        }
    }
    
    double percentile(const std::vector<double>& values, double p) {
        if (values.empty()) {
            throw std::invalid_argument("Cannot find percentile of empty vector");
        }
        if (p < 0.0 || p > 100.0) {
            throw std::invalid_argument("Percentile must be between 0 and 100");
        }
        
        std::vector<double> sorted_values = values;
        std::sort(sorted_values.begin(), sorted_values.end());
        
        double index = (p / 100.0) * (sorted_values.size() - 1);
        size_t lower = static_cast<size_t>(std::floor(index));
        size_t upper = static_cast<size_t>(std::ceil(index));
        
        if (lower == upper) {
            return sorted_values[lower];
        } else {
            double weight = index - lower;
            return sorted_values[lower] * (1 - weight) + sorted_values[upper] * weight;
        }
    }
    
    std::string DescriptiveStats::to_string() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << "DescriptiveStats{count=" << count 
            << ", mean=" << mean 
            << ", median=" << median
            << ", std_dev=" << std_dev 
            << ", min=" << min_val 
            << ", max=" << max_val << "}";
        return oss.str();
    }
    
    DescriptiveStats describe(const std::vector<double>& values) {
        if (values.empty()) {
            return {0.0, 0.0, 0.0, 0.0, 0.0, 0};
        }
        
        Calculator calc;
        DescriptiveStats stats;
        
        stats.count = values.size();
        stats.mean = calc.mean(values);
        stats.median = median(values);
        stats.std_dev = calc.standard_deviation(values);
        stats.min_val = min(values);
        stats.max_val = max(values);
        
        return stats;
    }
}

// Geometry implementation
namespace Geometry {
    double Point2D::distance_to(const Point2D& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
    
    std::string Point2D::to_string() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << "Point2D(" << x << ", " << y << ")";
        return oss.str();
    }
    
    double Circle::area() const {
        return M_PI * radius * radius;
    }
    
    double Circle::circumference() const {
        return 2 * M_PI * radius;
    }
    
    bool Circle::contains(const Point2D& point) const {
        return center.distance_to(point) <= radius;
    }
    
    std::string Circle::to_string() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << "Circle{center=" << center.to_string() << ", radius=" << radius << "}";
        return oss.str();
    }
    
    double Rectangle::area() const {
        return width * height;
    }
    
    double Rectangle::perimeter() const {
        return 2 * (width + height);
    }
    
    bool Rectangle::contains(const Point2D& point) const {
        return point.x >= top_left.x && 
               point.x <= top_left.x + width &&
               point.y >= top_left.y && 
               point.y <= top_left.y + height;
    }
    
    std::string Rectangle::to_string() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << "Rectangle{top_left=" << top_left.to_string() 
            << ", width=" << width << ", height=" << height << "}";
        return oss.str();
    }
}

// Number theory implementation
namespace NumberTheory {
    std::vector<int> prime_factors(int n) {
        std::vector<int> factors;
        
        // Handle 2 separately
        while (n % 2 == 0) {
            factors.push_back(2);
            n /= 2;
        }
        
        // Check odd factors
        for (int i = 3; i * i <= n; i += 2) {
            while (n % i == 0) {
                factors.push_back(i);
                n /= i;
            }
        }
        
        // If n is still greater than 2, it's a prime
        if (n > 2) {
            factors.push_back(n);
        }
        
        return factors;
    }
    
    std::vector<int> divisors(int n) {
        std::vector<int> divs;
        
        for (int i = 1; i * i <= n; ++i) {
            if (n % i == 0) {
                divs.push_back(i);
                if (i != n / i) {
                    divs.push_back(n / i);
                }
            }
        }
        
        std::sort(divs.begin(), divs.end());
        return divs;
    }
    
    bool is_perfect_number(int n) {
        if (n <= 1) return false;
        
        int sum = 1; // 1 is always a divisor
        for (int i = 2; i * i <= n; ++i) {
            if (n % i == 0) {
                sum += i;
                if (i != n / i) {
                    sum += n / i;
                }
            }
        }
        
        return sum == n;
    }
    
    bool is_palindrome(int n) {
        return n == reverse_digits(n);
    }
    
    int reverse_digits(int n) {
        int reversed = 0;
        int original = std::abs(n);
        
        while (original > 0) {
            reversed = reversed * 10 + original % 10;
            original /= 10;
        }
        
        return n < 0 ? -reversed : reversed;
    }
    
    int digit_sum(int n) {
        int sum = 0;
        n = std::abs(n);
        
        while (n > 0) {
            sum += n % 10;
            n /= 10;
        }
        
        return sum;
    }
}

} // namespace MathUtils
