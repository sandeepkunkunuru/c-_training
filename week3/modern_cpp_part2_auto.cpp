#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <tuple>
#include <typeinfo>
#include <memory>
#include <type_traits>
#include <utility>
#include <functional>

// Helper function to get type name (implementation-dependent)
template <typename T>
std::string typeName() {
    #ifdef __GNUG__
    // GCC/Clang implementation
    const char* mangledName = typeid(T).name();
    return mangledName;
    #else
    // Generic fallback
    return "unknown";
    #endif
}

// Function to demonstrate return type deduction
auto getValueA() {
    return 42;  // Returns int
}

auto getValueB() -> double {
    return 42.0;  // Returns double with trailing return type
}

// Function template with decltype return type
template<typename T, typename U>
auto add(T t, U u) -> decltype(t + u) {
    return t + u;
}

// Function that returns a lambda
auto createMultiplier(int factor) {
    return [factor](int value) { return factor * value; };
}

// Class with auto return types
class AutoMethods {
private:
    int value;
    
public:
    AutoMethods(int v) : value(v) {}
    
    auto getValue() const {
        return value;
    }
    
    auto getValueSquared() const {
        return value * value;
    }
};

// Tuple for structured binding examples
std::tuple<std::string, int, double> getPersonInfo() {
    return {"Alice", 30, 75000.50};
}

// Custom struct for structured binding
struct Point {
    int x;
    int y;
    int z;
};

// Class with custom structured binding support
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
    std::cout << "===== Modern C++ Part 2: Auto Type Deduction =====" << std::endl;

    // Basic auto usage
    std::cout << "\n--- Basic Auto Usage ---" << std::endl;
    
    auto i = 42;                   // int
    auto d = 42.0;                 // double
    auto s = "Hello, World!";      // const char*
    auto str = std::string("Hi");  // std::string
    
    std::cout << "i: " << i << " (type: " << typeName<decltype(i)>() << ")" << std::endl;
    std::cout << "d: " << d << " (type: " << typeName<decltype(d)>() << ")" << std::endl;
    std::cout << "s: " << s << " (type: " << typeName<decltype(s)>() << ")" << std::endl;
    std::cout << "str: " << str << " (type: " << typeName<decltype(str)>() << ")" << std::endl;

    // Auto with references and const
    std::cout << "\n--- Auto with References and Const ---" << std::endl;
    
    int x = 10;
    const int cx = 20;
    
    auto a1 = x;           // int (by value)
    auto& a2 = x;          // int& (reference)
    const auto& a3 = x;    // const int& (const reference)
    auto* a4 = &x;         // int* (pointer)
    const auto* a5 = &x;   // const int* (pointer to const)
    
    auto a6 = cx;          // int (by value, drops const)
    auto& a7 = cx;         // const int& (reference to const)
    
    std::cout << "a1: " << a1 << " (type: " << typeName<decltype(a1)>() << ")" << std::endl;
    std::cout << "a2: " << a2 << " (type: " << typeName<decltype(a2)>() << ")" << std::endl;
    std::cout << "a3: " << a3 << " (type: " << typeName<decltype(a3)>() << ")" << std::endl;
    std::cout << "a4: " << *a4 << " (type: " << typeName<decltype(a4)>() << ")" << std::endl;
    std::cout << "a5: " << *a5 << " (type: " << typeName<decltype(a5)>() << ")" << std::endl;
    std::cout << "a6: " << a6 << " (type: " << typeName<decltype(a6)>() << ")" << std::endl;
    std::cout << "a7: " << a7 << " (type: " << typeName<decltype(a7)>() << ")" << std::endl;
    
    // Modify through reference
    a2 = 100;
    std::cout << "After modifying a2, x = " << x << std::endl;

    // Auto with containers and iterators
    std::cout << "\n--- Auto with Containers and Iterators ---" << std::endl;
    
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::map<std::string, int> map = {{"one", 1}, {"two", 2}, {"three", 3}};
    
    // Without auto (verbose)
    for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // With auto (cleaner)
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Range-based for loop with auto
    for (const auto& item : vec) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
    
    // Map iteration with structured binding (C++17)
    for (const auto& [key, value] : map) {
        std::cout << key << ": " << value << ", ";
    }
    std::cout << std::endl;

    // decltype usage
    std::cout << "\n--- decltype Usage ---" << std::endl;
    
    int a = 10;
    decltype(a) b = 20;  // Same type as a (int)
    
    decltype(a + b) c = a + b;  // Type of the expression (int)
    
    decltype(std::string("hello")) str2 = "world";  // std::string
    
    std::cout << "b: " << b << " (type: " << typeName<decltype(b)>() << ")" << std::endl;
    std::cout << "c: " << c << " (type: " << typeName<decltype(c)>() << ")" << std::endl;
    std::cout << "str2: " << str2 << " (type: " << typeName<decltype(str2)>() << ")" << std::endl;
    
    // decltype with functions
    auto result1 = add(10, 20);       // int
    auto result2 = add(10.5, 20.7);   // double
    auto result3 = add(10, 20.7);     // double
    
    std::cout << "result1: " << result1 << " (type: " << typeName<decltype(result1)>() << ")" << std::endl;
    std::cout << "result2: " << result2 << " (type: " << typeName<decltype(result2)>() << ")" << std::endl;
    std::cout << "result3: " << result3 << " (type: " << typeName<decltype(result3)>() << ")" << std::endl;

    // Auto return types
    std::cout << "\n--- Auto Return Types ---" << std::endl;
    
    auto val1 = getValueA();  // int
    auto val2 = getValueB();  // double
    
    std::cout << "val1: " << val1 << " (type: " << typeName<decltype(val1)>() << ")" << std::endl;
    std::cout << "val2: " << val2 << " (type: " << typeName<decltype(val2)>() << ")" << std::endl;
    
    // Auto with lambdas
    auto multiplier = createMultiplier(10);
    std::cout << "5 * 10 = " << multiplier(5) << std::endl;
    
    // Auto methods
    AutoMethods am(5);
    auto amValue = am.getValue();
    auto amSquared = am.getValueSquared();
    
    std::cout << "am.getValue(): " << amValue << " (type: " << typeName<decltype(amValue)>() << ")" << std::endl;
    std::cout << "am.getValueSquared(): " << amSquared << " (type: " << typeName<decltype(amSquared)>() << ")" << std::endl;

    // Structured bindings (C++17)
    std::cout << "\n--- Structured Bindings (C++17) ---" << std::endl;
    
    // With tuple
    auto [name, age, salary] = getPersonInfo();
    std::cout << "Person: " << name << ", " << age << " years old, $" << salary << std::endl;
    
    // With pair
    std::pair<int, std::string> pair = {1, "one"};
    auto [id, value] = pair;
    std::cout << "Pair: " << id << " - " << value << std::endl;
    
    // With array
    int arr[3] = {1, 2, 3};
    auto [a_x, a_y, a_z] = arr;
    std::cout << "Array: " << a_x << ", " << a_y << ", " << a_z << std::endl;
    
    // With struct
    Point point = {10, 20, 30};
    auto [px, py, pz] = point;
    std::cout << "Point: (" << px << ", " << py << ", " << pz << ")" << std::endl;
    
    // With custom class
    Rectangle rect(100, 200);
    auto [width, height] = rect;
    std::cout << "Rectangle: " << width << " x " << height << std::endl;
    
    // Structured binding with references
    auto& [rx, ry, rz] = point;
    rx = 100;  // Modifies the original point
    std::cout << "After modifying through reference, point.x = " << point.x << std::endl;

    return 0;
}
