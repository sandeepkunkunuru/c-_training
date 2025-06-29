#include <iostream>
#include <string>
#include <tuple>
#include <array>
#include <vector>
#include <map>
#include <type_traits>
#include <utility>

// Helper function to print a line separator
void printSeparator(const std::string& title) {
    std::cout << "\n--- " << title << " ---" << std::endl;
}

// Fold expressions (C++17)
// Sum of all arguments
template<typename... Args>
constexpr auto sum(Args... args) {
    return (... + args); // Unary left fold
}

// Product of all arguments
template<typename... Args>
auto product(Args... args) {
    return (... * args); // Unary left fold
}

// Check if all arguments are true
template<typename... Args>
bool all(Args... args) {
    return (... && args); // Unary left fold
}

// Check if any argument is true
template<typename... Args>
bool any(Args... args) {
    return (... || args); // Unary left fold
}

// Print all arguments with separator
template<typename... Args>
void print(Args... args) {
    ((std::cout << args << ", "), ...); // Unary right fold with comma operator
    std::cout << std::endl;
}

// Print all arguments with custom separator
template<typename Sep, typename... Args>
void print_with_separator(Sep separator, Args... args) {
    auto print_one = [&](const auto& arg) {
        std::cout << arg << separator;
    };
    
    (..., print_one(args)); // Unary right fold
    std::cout << std::endl;
}

// Push all arguments to a vector
template<typename T, typename... Args>
auto make_vector(Args... args) {
    std::vector<T> result;
    (result.push_back(args), ...); // Unary right fold
    return result;
}

// if constexpr examples
// Function that behaves differently based on type
template<typename T>
auto process(const T& value) {
    if constexpr (std::is_integral_v<T>) {
        return value * 2; // Double integers
    } else if constexpr (std::is_floating_point_v<T>) {
        return value * 3.14; // Multiply floating points by pi
    } else if constexpr (std::is_same_v<T, std::string>) {
        return value + value; // Duplicate strings
    } else {
        return value; // Return as is for other types
    }
}

// Recursive function with base case using if constexpr
template<typename T, typename... Rest>
void processAll(T&& first, Rest&&... rest) {
    std::cout << "Processing: " << process(first) << std::endl;
    
    if constexpr (sizeof...(rest) > 0) {
        processAll(std::forward<Rest>(rest)...);
    }
}

// Compile-time factorial using if constexpr
template<int N>
constexpr int factorial() {
    if constexpr (N <= 1) {
        return 1;
    } else {
        return N * factorial<N-1>();
    }
}

// Structured bindings with custom types
struct Point3D {
    double x;
    double y;
    double z;
};

class Rectangle {
private:
    int width;
    int height;
    
public:
    Rectangle(int w, int h) : width(w), height(h) {}
    
    // These methods enable structured binding
    template<std::size_t N>
    friend auto get(const Rectangle& r) {
        if constexpr (N == 0) return r.width;
        else if constexpr (N == 1) return r.height;
    }
};

// Specializations needed for structured binding to work with Rectangle
namespace std {
    template<>
    struct tuple_size<Rectangle> : std::integral_constant<std::size_t, 2> {};
    
    template<std::size_t N>
    struct tuple_element<N, Rectangle> {
        using type = int;
    };
}

int main() {
    std::cout << "===== Modern C++ Part 3: Fold Expressions, if constexpr, Structured Bindings =====" << std::endl;

    // Fold expressions
    printSeparator("Fold Expressions (C++17)");
    
    std::cout << "sum(1, 2, 3, 4, 5) = " << sum(1, 2, 3, 4, 5) << std::endl;
    std::cout << "product(1, 2, 3, 4, 5) = " << product(1, 2, 3, 4, 5) << std::endl;
    
    std::cout << "all(true, true, true) = " << all(true, true, true) << std::endl;
    std::cout << "all(true, false, true) = " << all(true, false, true) << std::endl;
    
    std::cout << "any(false, false, true) = " << any(false, false, true) << std::endl;
    std::cout << "any(false, false, false) = " << any(false, false, false) << std::endl;
    
    std::cout << "print(1, 2.5, \"hello\"): ";
    print(1, 2.5, "hello");
    
    std::cout << "print_with_separator(\" | \", 1, 2.5, \"hello\"): ";
    print_with_separator(" | ", 1, 2.5, "hello");
    
    auto vec = make_vector<int>(1, 2, 3, 4, 5);
    std::cout << "make_vector<int>(1, 2, 3, 4, 5): ";
    for (const auto& item : vec) {
        std::cout << item << " ";
    }
    std::cout << std::endl;

    // if constexpr
    printSeparator("if constexpr (C++17)");
    
    std::cout << "process(42) = " << process(42) << std::endl;
    std::cout << "process(3.14) = " << process(3.14) << std::endl;
    std::cout << "process(\"hello\") = " << process(std::string("hello")) << std::endl;
    
    std::cout << "processAll(10, 3.14, std::string(\"world\")):" << std::endl;
    processAll(10, 3.14, std::string("world"));
    
    constexpr int fact5 = factorial<5>();
    std::cout << "factorial<5>() = " << fact5 << std::endl;
    
    // Compile-time evaluation with constexpr
    constexpr auto compileTimeSum = sum(1, 2, 3, 4, 5);
    std::cout << "Compile-time sum: " << compileTimeSum << std::endl;

    // Structured bindings
    printSeparator("Structured Bindings (C++17)");
    
    // With tuple
    std::tuple<int, std::string, double> person{42, "John", 3.14};
    auto [id, name, value] = person;
    std::cout << "Tuple: id=" << id << ", name=" << name << ", value=" << value << std::endl;
    
    // With pair
    std::pair<std::string, int> keyValue{"key", 100};
    auto [key, val] = keyValue;
    std::cout << "Pair: " << key << "=" << val << std::endl;
    
    // With array
    std::array<int, 3> coords{10, 20, 30};
    auto [x, y, z] = coords;
    std::cout << "Array: (" << x << ", " << y << ", " << z << ")" << std::endl;
    
    // With struct
    Point3D point{1.0, 2.0, 3.0};
    auto [px, py, pz] = point;
    std::cout << "Struct: (" << px << ", " << py << ", " << pz << ")" << std::endl;
    
    // With custom class
    Rectangle rect{100, 200};
    auto [width, height] = rect;
    std::cout << "Custom class: " << width << "x" << height << std::endl;
    
    // With map iteration
    std::map<std::string, int> ages{{"Alice", 30}, {"Bob", 25}, {"Charlie", 35}};
    std::cout << "Map iteration:" << std::endl;
    for (const auto& [person, age] : ages) {
        std::cout << person << " is " << age << " years old" << std::endl;
    }
    
    // Structured binding with references
    auto& [rx, ry, rz] = point;
    rx = 10.0;  // Modifies the original point
    std::cout << "After modifying through reference, point.x = " << point.x << std::endl;

    return 0;
}
