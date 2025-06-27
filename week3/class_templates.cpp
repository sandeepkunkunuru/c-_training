#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <type_traits>

// Basic class template
template<typename T>
class Box {
private:
    T value;
    
public:
    Box() : value(T{}) {}
    explicit Box(const T& val) : value(val) {}
    
    T getValue() const { return value; }
    void setValue(const T& val) { value = val; }
    
    // Template method inside a class template
    template<typename U>
    Box<U> cast() const {
        return Box<U>(static_cast<U>(value));
    }
};

// Class template with multiple template parameters
template<typename T, typename U>
class Pair {
private:
    T first;
    U second;
    
public:
    Pair(const T& f, const U& s) : first(f), second(s) {}
    
    T getFirst() const { return first; }
    U getSecond() const { return second; }
    
    void setFirst(const T& f) { first = f; }
    void setSecond(const U& s) { second = s; }
    
    void print() const {
        std::cout << "(" << first << ", " << second << ")" << std::endl;
    }
};

// Class template with default template parameters
template<typename T, typename Container = std::vector<T>>
class Stack {
private:
    Container container;
    
public:
    void push(const T& item) {
        container.push_back(item);
    }
    
    T pop() {
        if (container.empty()) {
            throw std::out_of_range("Stack is empty");
        }
        
        T item = container.back();
        container.pop_back();
        return item;
    }
    
    bool isEmpty() const {
        return container.empty();
    }
    
    size_t size() const {
        return container.size();
    }
    
    void print() const {
        std::cout << "Stack contents (top to bottom): ";
        for (auto it = container.rbegin(); it != container.rend(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
};

// Class template specialization
template<>
class Box<std::string> {
private:
    std::string value;
    
public:
    Box() : value("") {}
    explicit Box(const std::string& val) : value(val) {}
    
    std::string getValue() const { return value; }
    void setValue(const std::string& val) { value = val; }
    
    // String-specific methods
    int length() const { return value.length(); }
    bool isEmpty() const { return value.empty(); }
    std::string toUpperCase() const {
        std::string result = value;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }
};

// Template with non-type parameters
template<typename T, size_t Size>
class FixedArray {
private:
    T data[Size];
    
public:
    FixedArray() {
        for (size_t i = 0; i < Size; ++i) {
            data[i] = T{};
        }
    }
    
    T& operator[](size_t index) {
        if (index >= Size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }
    
    const T& operator[](size_t index) const {
        if (index >= Size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }
    
    size_t size() const { return Size; }
};

// Template template parameters
template<typename T, template<typename, typename> class Container = std::vector>
class GenericContainer {
private:
    Container<T, std::allocator<T>> data;
    
public:
    void add(const T& item) {
        data.push_back(item);
    }
    
    void remove(const T& item) {
        data.erase(std::remove(data.begin(), data.end(), item), data.end());
    }
    
    bool contains(const T& item) const {
        return std::find(data.begin(), data.end(), item) != data.end();
    }
    
    size_t size() const {
        return data.size();
    }
    
    void print() const {
        std::cout << "Container contents: ";
        for (const auto& item : data) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
};

// Curiously Recurring Template Pattern (CRTP)
template<typename Derived>
class Base {
public:
    void interface() {
        // Cast to the derived type
        static_cast<Derived*>(this)->implementation();
    }
    
    void implementation() {
        std::cout << "Base implementation" << std::endl;
    }
};

class Derived1 : public Base<Derived1> {
public:
    void implementation() {
        std::cout << "Derived1 implementation" << std::endl;
    }
};

class Derived2 : public Base<Derived2> {
    // Uses base implementation
};

// Mixins via templates
template<typename Base>
class LoggingMixin : public Base {
public:
    template<typename... Args>
    LoggingMixin(Args&&... args) : Base(std::forward<Args>(args)...) {}
    
    void log(const std::string& message) const {
        std::cout << "Log: " << message << std::endl;
    }
};

class SimpleClass {
public:
    void doSomething() {
        std::cout << "Doing something..." << std::endl;
    }
};

int main() {
    std::cout << "===== Basic Class Templates =====" << std::endl;
    
    // Basic class template
    Box<int> intBox(42);
    std::cout << "Int box value: " << intBox.getValue() << std::endl;
    
    Box<double> doubleBox(3.14);
    std::cout << "Double box value: " << doubleBox.getValue() << std::endl;
    
    // Template method
    Box<double> convertedBox = intBox.cast<double>();
    std::cout << "Converted box value: " << convertedBox.getValue() << std::endl;
    
    std::cout << "\n===== Multiple Template Parameters =====" << std::endl;
    
    // Multiple template parameters
    Pair<int, std::string> pair1(1, "one");
    pair1.print();
    
    Pair<double, bool> pair2(3.14, true);
    pair2.print();
    
    std::cout << "\n===== Default Template Parameters =====" << std::endl;
    
    // Default template parameters
    Stack<int> intStack;
    intStack.push(10);
    intStack.push(20);
    intStack.push(30);
    intStack.print();
    
    std::cout << "Popped: " << intStack.pop() << std::endl;
    intStack.print();
    
    std::cout << "\n===== Class Template Specialization =====" << std::endl;
    
    // Class template specialization
    Box<std::string> stringBox("Hello, templates!");
    std::cout << "String box value: " << stringBox.getValue() << std::endl;
    std::cout << "String length: " << stringBox.length() << std::endl;
    std::cout << "Uppercase: " << stringBox.toUpperCase() << std::endl;
    
    std::cout << "\n===== Non-type Template Parameters =====" << std::endl;
    
    // Non-type template parameters
    FixedArray<int, 5> fixedArray;
    for (size_t i = 0; i < fixedArray.size(); ++i) {
        fixedArray[i] = i * 100;
    }
    
    for (size_t i = 0; i < fixedArray.size(); ++i) {
        std::cout << "fixedArray[" << i << "] = " << fixedArray[i] << std::endl;
    }
    
    std::cout << "\n===== Template Template Parameters =====" << std::endl;
    
    // Template template parameters
    GenericContainer<int> container;
    container.add(10);
    container.add(20);
    container.add(30);
    container.add(20);  // Duplicate
    container.print();
    
    container.remove(20);
    container.print();
    
    std::cout << "Contains 30? " << (container.contains(30) ? "Yes" : "No") << std::endl;
    std::cout << "Contains 20? " << (container.contains(20) ? "Yes" : "No") << std::endl;
    
    std::cout << "\n===== CRTP (Curiously Recurring Template Pattern) =====" << std::endl;
    
    // CRTP
    Derived1 d1;
    d1.interface();  // Uses Derived1's implementation
    
    Derived2 d2;
    d2.interface();  // Uses Base's implementation
    
    std::cout << "\n===== Mixins via Templates =====" << std::endl;
    
    // Mixins
    LoggingMixin<SimpleClass> loggingClass;
    loggingClass.doSomething();
    loggingClass.log("Operation completed");
    
    return 0;
}
