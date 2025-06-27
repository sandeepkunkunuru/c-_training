#include <iostream>
#include <string>
#include <vector>
#include <type_traits>

// Basic function template
template<typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

// Function template with multiple template parameters
template<typename T, typename U>
auto add(T a, U b) -> decltype(a + b) {
    return a + b;
}

// Function template with default template parameter
template<typename T = int>
T getDefaultValue() {
    return T{};
}

// Template specialization
template<>
std::string maximum<std::string>(std::string a, std::string b) {
    return (a.length() > b.length()) ? a : b;
}

// Function template with non-type template parameter
template<typename T, int Size>
class Array {
private:
    T data[Size];
public:
    Array() {
        for (int i = 0; i < Size; ++i) {
            data[i] = T{};
        }
    }
    
    T& operator[](int index) {
        if (index < 0 || index >= Size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }
    
    int size() const {
        return Size;
    }
    
    void print() const {
        std::cout << "Array contents: ";
        for (int i = 0; i < Size; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }
};

// SFINAE example: enable_if to create different implementations based on type properties
template<typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
isEven(T value) {
    return value % 2 == 0;
}

template<typename T>
typename std::enable_if<!std::is_integral<T>::value, bool>::type
isEven(T) {
    std::cout << "isEven is only defined for integral types." << std::endl;
    return false;
}

// Variadic template example
template<typename T>
T sum(T value) {
    return value;
}

template<typename T, typename... Args>
T sum(T first, Args... args) {
    return first + sum(args...);
}

// Template with concepts-like constraints (C++17 version using SFINAE)
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
square(T value) {
    return value * value;
}

int main() {
    std::cout << "===== Function Templates =====" << std::endl;
    
    // Basic function template
    std::cout << "Maximum of 10 and 20: " << maximum(10, 20) << std::endl;
    std::cout << "Maximum of 3.14 and 2.71: " << maximum(3.14, 2.71) << std::endl;
    
    // Template with multiple parameters
    std::cout << "Add int and double: " << add(5, 3.14) << std::endl;
    
    // Default template parameter
    std::cout << "Default int value: " << getDefaultValue() << std::endl;
    std::cout << "Default string value: " << getDefaultValue<std::string>() << std::endl;
    
    // Template specialization
    std::cout << "Maximum string by length - 'apple' vs 'banana': " 
              << maximum(std::string("apple"), std::string("banana")) << std::endl;
    
    std::cout << "\n===== Non-type Template Parameters =====" << std::endl;
    
    // Non-type template parameter
    Array<int, 5> intArray;
    for (int i = 0; i < intArray.size(); ++i) {
        intArray[i] = i * 10;
    }
    intArray.print();
    
    Array<double, 3> doubleArray;
    doubleArray[0] = 1.1;
    doubleArray[1] = 2.2;
    doubleArray[2] = 3.3;
    doubleArray.print();
    
    std::cout << "\n===== SFINAE Examples =====" << std::endl;
    
    // SFINAE examples
    std::cout << "Is 42 even? " << (isEven(42) ? "Yes" : "No") << std::endl;
    std::cout << "Is 43 even? " << (isEven(43) ? "Yes" : "No") << std::endl;
    
    // This will use the non-integral version
    isEven(3.14);
    
    std::cout << "\n===== Variadic Templates =====" << std::endl;
    
    // Variadic templates
    std::cout << "Sum of 1: " << sum(1) << std::endl;
    std::cout << "Sum of 1, 2, 3: " << sum(1, 2, 3) << std::endl;
    std::cout << "Sum of 1.1, 2.2, 3.3, 4.4: " << sum(1.1, 2.2, 3.3, 4.4) << std::endl;
    
    std::cout << "\n===== Concept-like Constraints =====" << std::endl;
    
    // Concepts-like constraints
    std::cout << "Square of 5: " << square(5) << std::endl;
    std::cout << "Square of 2.5: " << square(2.5) << std::endl;
    
    // This would cause a compilation error:
    // square(std::string("can't square a string"));
    
    return 0;
}
