#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <variant>
#include <any>
#include <functional>

// Helper function to print optional values
template<typename T>
void printOptional(const std::string& name, const std::optional<T>& opt) {
    std::cout << name << ": ";
    if (opt.has_value()) {
        std::cout << *opt << std::endl;
    } else {
        std::cout << "nullopt" << std::endl;
    }
}

// Function that may or may not return a value
std::optional<std::string> findUserById(int id) {
    // Simulated database lookup
    if (id == 1) return "Alice";
    if (id == 2) return "Bob";
    if (id == 3) return "Charlie";
    return std::nullopt;  // User not found
}

// Function that returns an optional with error message
std::optional<double> safeDivide(double a, double b) {
    if (b == 0.0) {
        return std::nullopt;  // Division by zero
    }
    return a / b;
}

// Visitor for std::variant
struct PrintVisitor {
    void operator()(int i) const {
        std::cout << "int: " << i << std::endl;
    }
    
    void operator()(double d) const {
        std::cout << "double: " << d << std::endl;
    }
    
    void operator()(const std::string& s) const {
        std::cout << "string: " << s << std::endl;
    }
};

// Function that returns different types based on input
std::variant<int, double, std::string> processValue(const std::string& input) {
    // Try to parse as int
    try {
        size_t pos = 0;
        int i = std::stoi(input, &pos);
        if (pos == input.size()) {
            return i;  // Successfully parsed as int
        }
    } catch (...) {}
    
    // Try to parse as double
    try {
        size_t pos = 0;
        double d = std::stod(input, &pos);
        if (pos == input.size()) {
            return d;  // Successfully parsed as double
        }
    } catch (...) {}
    
    // Default to string
    return input;
}

// Class to demonstrate std::any
class Registry {
private:
    std::unordered_map<std::string, std::any> values;

public:
    template<typename T>
    void set(const std::string& key, T value) {
        values[key] = value;
    }
    
    template<typename T>
    std::optional<T> get(const std::string& key) {
        auto it = values.find(key);
        if (it != values.end()) {
            try {
                return std::any_cast<T>(it->second);
            } catch (const std::bad_any_cast&) {
                return std::nullopt;
            }
        }
        return std::nullopt;
    }
    
    bool has(const std::string& key) const {
        return values.find(key) != values.end();
    }
};

int main() {
    std::cout << "===== Modern C++ Features: Optional, Variant, Any =====" << std::endl;

    // std::optional
    std::cout << "\n--- std::optional (C++17) ---" << std::endl;
    
    // Creating optional values
    std::optional<int> opt1;                  // Empty optional
    std::optional<int> opt2 = 42;             // Optional with value
    std::optional<int> opt3 = std::nullopt;   // Explicitly empty
    std::optional<std::string> opt4 = "hello";// Optional with string
    
    // Checking if value exists
    std::cout << "opt1.has_value(): " << opt1.has_value() << std::endl;
    std::cout << "opt2.has_value(): " << opt2.has_value() << std::endl;
    std::cout << "bool(opt1): " << static_cast<bool>(opt1) << std::endl;
    std::cout << "bool(opt2): " << static_cast<bool>(opt2) << std::endl;
    
    // Accessing values
    if (opt2) {
        std::cout << "opt2 value: " << *opt2 << std::endl;
    }
    
    std::cout << "opt2.value(): " << opt2.value() << std::endl;
    
    try {
        std::cout << "opt1.value(): " << opt1.value() << std::endl;
    } catch (const std::bad_optional_access& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    
    // Default values
    std::cout << "opt1.value_or(0): " << opt1.value_or(0) << std::endl;
    std::cout << "opt2.value_or(0): " << opt2.value_or(0) << std::endl;
    
    // Using optional in functions
    printOptional("User 1", findUserById(1));
    printOptional("User 5", findUserById(5));
    
    auto result1 = safeDivide(10.0, 2.0);
    auto result2 = safeDivide(10.0, 0.0);
    
    printOptional("10.0 / 2.0", result1);
    printOptional("10.0 / 0.0", result2);

    // std::variant
    std::cout << "\n--- std::variant (C++17) ---" << std::endl;
    
    // Creating variants
    std::variant<int, double, std::string> var1 = 42;
    std::variant<int, double, std::string> var2 = 3.14;
    std::variant<int, double, std::string> var3 = "hello";
    
    // Checking variant type
    std::cout << "var1 index: " << var1.index() << std::endl;
    std::cout << "var2 index: " << var2.index() << std::endl;
    std::cout << "var3 index: " << var3.index() << std::endl;
    
    // Accessing variant values
    std::cout << "var1 as int: " << std::get<int>(var1) << std::endl;
    std::cout << "var2 as double: " << std::get<double>(var2) << std::endl;
    std::cout << "var3 as string: " << std::get<std::string>(var3) << std::endl;
    
    // Accessing with get_if (safe access)
    if (auto pval = std::get_if<int>(&var1)) {
        std::cout << "var1 contains int: " << *pval << std::endl;
    }
    
    if (auto pval = std::get_if<std::string>(&var1)) {
        std::cout << "var1 contains string: " << *pval << std::endl;
    } else {
        std::cout << "var1 does not contain string" << std::endl;
    }
    
    // Using visitor pattern
    std::cout << "Using visitor pattern:" << std::endl;
    std::visit(PrintVisitor{}, var1);
    std::visit(PrintVisitor{}, var2);
    std::visit(PrintVisitor{}, var3);
    
    // Using lambda as visitor
    auto printVisitor = [](const auto& value) {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, int>) {
            std::cout << "int: " << value << std::endl;
        } else if constexpr (std::is_same_v<T, double>) {
            std::cout << "double: " << value << std::endl;
        } else if constexpr (std::is_same_v<T, std::string>) {
            std::cout << "string: " << value << std::endl;
        }
    };
    
    std::cout << "Using lambda visitor:" << std::endl;
    std::visit(printVisitor, var1);
    std::visit(printVisitor, var2);
    std::visit(printVisitor, var3);
    
    // Practical example with processValue
    auto result_a = processValue("42");
    auto result_b = processValue("3.14");
    auto result_c = processValue("hello");
    
    std::cout << "processValue(\"42\") -> ";
    std::visit(printVisitor, result_a);
    
    std::cout << "processValue(\"3.14\") -> ";
    std::visit(printVisitor, result_b);
    
    std::cout << "processValue(\"hello\") -> ";
    std::visit(printVisitor, result_c);

    // std::any
    std::cout << "\n--- std::any (C++17) ---" << std::endl;
    
    // Creating any objects
    std::any a1;                      // Empty
    std::any a2 = 42;                 // Containing int
    std::any a3 = 3.14;               // Containing double
    std::any a4 = std::string("hi");  // Containing string
    
    // Checking if any has a value
    std::cout << "a1.has_value(): " << a1.has_value() << std::endl;
    std::cout << "a2.has_value(): " << a2.has_value() << std::endl;
    
    // Getting the type
    std::cout << "a2 type: " << a2.type().name() << std::endl;
    std::cout << "a3 type: " << a3.type().name() << std::endl;
    std::cout << "a4 type: " << a4.type().name() << std::endl;
    
    // Accessing values
    std::cout << "a2 as int: " << std::any_cast<int>(a2) << std::endl;
    std::cout << "a3 as double: " << std::any_cast<double>(a3) << std::endl;
    std::cout << "a4 as string: " << std::any_cast<std::string>(a4) << std::endl;
    
    // Safe access with pointers
    if (int* i = std::any_cast<int>(&a2)) {
        std::cout << "a2 contains int: " << *i << std::endl;
    }
    
    if (std::string* s = std::any_cast<std::string>(&a2)) {
        std::cout << "a2 contains string: " << *s << std::endl;
    } else {
        std::cout << "a2 does not contain string" << std::endl;
    }
    
    // Exception handling
    try {
        std::cout << std::any_cast<float>(a3) << std::endl;
    } catch (const std::bad_any_cast& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    
    // Using Registry with std::any
    Registry registry;
    registry.set("int_value", 42);
    registry.set("double_value", 3.14);
    registry.set("string_value", std::string("hello"));
    
    if (auto value = registry.get<int>("int_value")) {
        std::cout << "int_value: " << *value << std::endl;
    }
    
    if (auto value = registry.get<std::string>("string_value")) {
        std::cout << "string_value: " << *value << std::endl;
    }
    
    if (auto value = registry.get<double>("non_existent")) {
        std::cout << "non_existent: " << *value << std::endl;
    } else {
        std::cout << "non_existent key not found" << std::endl;
    }
    
    if (auto value = registry.get<int>("string_value")) {
        std::cout << "string_value as int: " << *value << std::endl;
    } else {
        std::cout << "string_value is not an int" << std::endl;
    }

    return 0;
}
