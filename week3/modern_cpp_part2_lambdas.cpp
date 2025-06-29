#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <map>
#include <memory>
#include <chrono>
#include <iomanip>

// Helper function to print a vector
template<typename T>
void printVector(const std::string& name, const std::vector<T>& vec) {
    std::cout << name << ": ";
    for (const auto& item : vec) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

// Helper function to measure execution time
template<typename Func>
long long measureExecutionTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

// Class to demonstrate capturing this in lambdas
class LambdaCapture {
private:
    int value;
    std::vector<std::function<void()>> callbacks;

public:
    LambdaCapture(int val) : value(val) {}

    void addCallback() {
        // Capture this by value (C++17)
        callbacks.push_back([*this]() {
            std::cout << "Captured value (by value): " << value << std::endl;
        });
    }

    void addCallbackRef() {
        // Capture this by reference
        callbacks.push_back([this]() {
            std::cout << "Captured value (by reference): " << value << std::endl;
        });
    }

    void setValue(int val) {
        value = val;
    }

    void executeCallbacks() {
        for (const auto& callback : callbacks) {
            callback();
        }
    }
};

int main() {
    std::cout << "===== Modern C++ Part 2: Lambda Expressions =====" << std::endl;

    // Basic lambda syntax
    std::cout << "\n--- Basic Lambda Syntax ---" << std::endl;
    
    // Lambda with no captures
    auto greet = []() { std::cout << "Hello, World!" << std::endl; };
    greet();
    
    // Lambda with parameters
    auto add = [](int a, int b) { return a + b; };
    std::cout << "5 + 3 = " << add(5, 3) << std::endl;
    
    // Lambda with return type
    auto divide = [](double a, double b) -> double {
        if (b == 0) return 0;
        return a / b;
    };
    std::cout << "10 / 2 = " << divide(10, 2) << std::endl;
    std::cout << "10 / 0 = " << divide(10, 0) << std::endl;

    // Lambda captures
    std::cout << "\n--- Lambda Captures ---" << std::endl;
    
    int x = 10;
    int y = 20;
    
    // Capture by value
    auto captureByValue = [x, y]() {
        std::cout << "Captured by value: x = " << x << ", y = " << y << std::endl;
    };
    
    // Capture by reference
    auto captureByRef = [&x, &y]() {
        std::cout << "Captured by reference: x = " << x << ", y = " << y << std::endl;
        x += 5; // Modifies the original x
    };
    
    // Capture all by value
    auto captureAllByValue = [=]() {
        std::cout << "Captured all by value: x = " << x << ", y = " << y << std::endl;
    };
    
    // Capture all by reference
    auto captureAllByRef = [&]() {
        std::cout << "Captured all by reference: x = " << x << ", y = " << y << std::endl;
        y += 5; // Modifies the original y
    };
    
    // Mixed capture
    auto mixedCapture = [=, &y]() {
        std::cout << "Mixed capture: x = " << x << " (by value), y = " << y << " (by reference)" << std::endl;
        y += 10; // Modifies the original y
    };
    
    captureByValue();
    captureByRef();
    std::cout << "After captureByRef: x = " << x << std::endl;
    
    captureAllByValue();
    captureAllByRef();
    std::cout << "After captureAllByRef: y = " << y << std::endl;
    
    mixedCapture();
    std::cout << "After mixedCapture: y = " << y << std::endl;

    // Mutable lambdas
    std::cout << "\n--- Mutable Lambdas ---" << std::endl;
    
    int counter = 0;
    
    // Without mutable - can't modify captured variables
    auto immutableCounter = [counter]() {
        // counter++; // Error: can't modify captured variable
        return counter;
    };
    
    // With mutable - can modify captured variables (but only the copy)
    auto mutableCounter = [counter]() mutable {
        counter++; // Modifies the captured copy, not the original
        return counter;
    };
    
    std::cout << "Original counter: " << counter << std::endl;
    std::cout << "First call to mutableCounter: " << mutableCounter() << std::endl;
    std::cout << "Second call to mutableCounter: " << mutableCounter() << std::endl;
    std::cout << "Original counter after calls: " << counter << std::endl;

    // Generic lambdas (C++14)
    std::cout << "\n--- Generic Lambdas (C++14) ---" << std::endl;
    
    auto genericAdd = [](auto a, auto b) {
        return a + b;
    };
    
    std::cout << "genericAdd(5, 3) = " << genericAdd(5, 3) << std::endl;
    std::cout << "genericAdd(5.5, 3.2) = " << genericAdd(5.5, 3.2) << std::endl;
    std::cout << "genericAdd(\"Hello, \", \"World!\") = " << genericAdd(std::string("Hello, "), std::string("World!")) << std::endl;

    // Lambda init captures (C++14)
    std::cout << "\n--- Lambda Init Captures (C++14) ---" << std::endl;
    
    auto ptr = std::make_unique<int>(42);
    std::cout << "ptr value before move: " << *ptr << std::endl;
    
    // Move the unique_ptr into the lambda
    auto captureMove = [ptr = std::move(ptr)]() {
        std::cout << "Captured unique_ptr value: " << *ptr << std::endl;
    };
    
    if (ptr) {
        std::cout << "Original ptr still valid (shouldn't happen)" << std::endl;
    } else {
        std::cout << "Original ptr moved into lambda" << std::endl;
    }
    
    captureMove();

    // Capturing this in lambdas
    std::cout << "\n--- Capturing 'this' in Lambdas ---" << std::endl;
    
    LambdaCapture lc(100);
    lc.addCallback();      // Capture by value
    lc.addCallbackRef();   // Capture by reference
    
    std::cout << "Before changing value:" << std::endl;
    lc.executeCallbacks();
    
    lc.setValue(200);
    std::cout << "After changing value:" << std::endl;
    lc.executeCallbacks();

    // Using lambdas with STL algorithms
    std::cout << "\n--- Lambdas with STL Algorithms ---" << std::endl;
    
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    printVector("Original numbers", numbers);
    
    // Find first number greater than 5
    auto it = std::find_if(numbers.begin(), numbers.end(), [](int n) { return n > 5; });
    if (it != numbers.end()) {
        std::cout << "First number > 5: " << *it << std::endl;
    }
    
    // Count even numbers
    int evenCount = std::count_if(numbers.begin(), numbers.end(), [](int n) { return n % 2 == 0; });
    std::cout << "Number of even values: " << evenCount << std::endl;
    
    // Transform numbers (multiply by 2)
    std::vector<int> doubled(numbers.size());
    std::transform(numbers.begin(), numbers.end(), doubled.begin(), [](int n) { return n * 2; });
    printVector("Doubled numbers", doubled);
    
    // Sort in descending order
    std::sort(numbers.begin(), numbers.end(), [](int a, int b) { return a > b; });
    printVector("Sorted in descending order", numbers);
    
    // Remove numbers less than 5
    auto newEnd = std::remove_if(numbers.begin(), numbers.end(), [](int n) { return n < 5; });
    numbers.erase(newEnd, numbers.end());
    printVector("After removing numbers < 5", numbers);

    // Recursive lambdas (C++14)
    std::cout << "\n--- Recursive Lambdas (C++14) ---" << std::endl;
    
    // Factorial using recursive lambda
    std::function<int(int)> factorial = [&factorial](int n) {
        return n <= 1 ? 1 : n * factorial(n - 1);
    };
    
    std::cout << "Factorial of 5: " << factorial(5) << std::endl;
    std::cout << "Factorial of 10: " << factorial(10) << std::endl;

    // Performance comparison: function pointer vs lambda
    std::cout << "\n--- Performance: Function Pointer vs Lambda ---" << std::endl;
    
    // Function pointer
    int (*funcPtr)(int, int) = [](int a, int b) { return a + b; };
    
    // Lambda
    auto lambdaFunc = [](int a, int b) { return a + b; };
    
    const int iterations = 10000000;
    
    long long funcPtrTime = measureExecutionTime([&]() {
        int sum = 0;
        for (int i = 0; i < iterations; ++i) {
            sum += funcPtr(i, i+1);
        }
    });
    
    long long lambdaTime = measureExecutionTime([&]() {
        int sum = 0;
        for (int i = 0; i < iterations; ++i) {
            sum += lambdaFunc(i, i+1);
        }
    });
    
    std::cout << "Function pointer time: " << funcPtrTime << " microseconds" << std::endl;
    std::cout << "Lambda time: " << lambdaTime << " microseconds" << std::endl;
    std::cout << "Ratio (FuncPtr/Lambda): " << static_cast<double>(funcPtrTime) / lambdaTime << std::endl;

    return 0;
}
