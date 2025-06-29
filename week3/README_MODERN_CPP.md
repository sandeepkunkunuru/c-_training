# Modern C++ Features

This directory contains comprehensive examples demonstrating modern C++ features introduced in C++11, C++14, and C++17. These examples are designed to help you understand and apply these features in real-world scenarios, particularly in preparation for working with complex C++ projects like Google's OR-Tools.

## Files Overview

### 1. Move Semantics and Smart Pointers (`modern_cpp_part1.cpp`)

Demonstrates:
- Move semantics (move constructors, move assignment operators)
- Performance comparison between copy and move operations
- Return Value Optimization (RVO)
- Perfect forwarding with `std::forward`
- Smart pointers:
  - `std::unique_ptr` with custom deleters
  - `std::shared_ptr` with reference counting
  - `std::weak_ptr` for breaking circular references

### 2. Lambda Expressions (`modern_cpp_part2_lambdas.cpp`)

Demonstrates:
- Basic lambda syntax and usage
- Capture clauses (by value, by reference, mixed captures)
- Mutable lambdas
- Generic lambdas (C++14)
- Lambda init captures (C++14)
- Capturing `this` pointer
- Using lambdas with STL algorithms
- Recursive lambdas with `std::function`
- Performance comparison between function pointers and lambdas

### 3. Auto Type Deduction and Structured Bindings (`modern_cpp_part2_auto.cpp`)

Demonstrates:
- `auto` type deduction with variables, references, pointers
- `decltype` for type deduction from expressions
- Functions with auto return types and trailing return types
- Structured bindings with:
  - Tuples and pairs
  - Arrays
  - Structs and custom classes
  - Map iteration

### 4. Optional, Variant, Any (`modern_cpp_optional.cpp`)

Demonstrates:
- `std::optional` for representing optional values
- `std::variant` for type-safe unions
- `std::any` for type-erased storage
- Visitor pattern with variants
- `if constexpr` for compile-time conditional code

### 5. Fold Expressions and if constexpr (`modern_cpp_fold_expressions.cpp`)

Demonstrates:
- Fold expressions for parameter pack operations (C++17)
- `if constexpr` for compile-time conditional code
- Compile-time evaluation with `constexpr`
- Structured bindings with custom types
- Template metaprogramming techniques

### 6. Filesystem and String View (`modern_cpp_filesystem.cpp`)

Demonstrates:
- `std::filesystem` for file and directory operations (C++17)
- Path manipulation and decomposition
- Directory iteration (regular and recursive)
- File operations (create, copy, rename)
- `std::string_view` for non-owning string references
- Performance comparison between `std::string` and `std::string_view`

### 7. Concurrency Features (`modern_cpp_concurrency.cpp`)

Demonstrates:
- Algorithm performance measurements
- Thread-safe counters with mutex and atomic
- Performance comparison between mutex and atomic operations
- `std::future` and `std::async` for asynchronous tasks
- `std::shared_mutex` for reader-writer locks (C++17)
- `std::scoped_lock` for deadlock prevention (C++17)

## Compilation and Execution

Compile each example with C++17 support:

```bash
g++ -std=c++17 modern_cpp_part1.cpp -o modern_cpp_part1
g++ -std=c++17 modern_cpp_part2_lambdas.cpp -o modern_cpp_part2_lambdas
g++ -std=c++17 modern_cpp_part2_auto.cpp -o modern_cpp_part2_auto
g++ -std=c++17 modern_cpp_optional.cpp -o modern_cpp_optional
g++ -std=c++17 modern_cpp_fold_expressions.cpp -o modern_cpp_fold_expressions
g++ -std=c++17 modern_cpp_filesystem.cpp -o modern_cpp_filesystem
g++ -std=c++17 modern_cpp_concurrency.cpp -o modern_cpp_concurrency -pthread
```

Run each example:

```bash
./modern_cpp_part1
./modern_cpp_part2_lambdas
./modern_cpp_part2_auto
./modern_cpp_optional
./modern_cpp_fold_expressions
./modern_cpp_filesystem
./modern_cpp_concurrency
```

## Key Takeaways

1. **Resource Management**: Modern C++ provides tools like move semantics and smart pointers that make resource management safer and more efficient.

2. **Code Expressiveness**: Features like lambdas, auto, and structured bindings make code more concise and readable.

3. **Type Safety**: `std::optional`, `std::variant`, and `std::any` provide type-safe alternatives to error-prone patterns.

4. **Compile-Time Programming**: `constexpr`, `if constexpr`, and fold expressions enable more work to be done at compile time.

5. **Standard Library Enhancements**: New components like `std::filesystem` and `std::string_view` provide standardized solutions for common tasks.

6. **Concurrency Support**: Modern C++ offers improved tools for writing correct and efficient concurrent code.

These examples serve as a reference for applying modern C++ features in your own code, particularly when working with complex C++ projects that leverage these features extensively.
