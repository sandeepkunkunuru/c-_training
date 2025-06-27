# Week 3: Advanced C++ Features and Standard Library

## Overview
Week 3 focuses on advanced C++ features and standard library components that are essential for modern C++ programming and will be valuable for contributing to Google's OR-Tools project.

## Learning Objectives
- Master advanced C++ features like templates, exceptions, and RAII
- Understand and use key components of the C++ Standard Library
- Learn modern C++ features (C++11/14/17)
- Apply advanced concepts in practical examples
- Prepare for OR-Tools specific development

## Daily Topics

### Day 1: Templates and Generic Programming
- Function templates
- Class templates
- Template specialization
- Variadic templates
- SFINAE (Substitution Failure Is Not An Error)
- Concepts (C++20 preview)
- Practical examples: Creating generic containers and algorithms

### Day 2: Exception Handling and RAII
- Exception handling basics
- Try-catch blocks
- Custom exceptions
- Exception safety guarantees
- RAII (Resource Acquisition Is Initialization)
- Smart pointers in-depth
- Practical examples: Resource management patterns

### Day 3: Standard Library Containers and Algorithms
- STL containers in-depth (vector, list, map, set, unordered_map, etc.)
- Container adaptors (stack, queue, priority_queue)
- Iterators and iterator categories
- Algorithm library overview
- Common algorithms (sort, find, transform, etc.)
- Lambda expressions with algorithms
- Practical examples: Solving problems with STL

### Day 4: Modern C++ Features
- Move semantics and rvalue references
- Perfect forwarding
- Lambda expressions in-depth
- auto and decltype
- Range-based for loops
- Structured bindings
- std::optional, std::variant, std::any
- Practical examples: Modernizing legacy code

### Day 5: Concurrency and Parallelism
- Thread basics
- Mutexes and locks
- Condition variables
- Futures and promises
- Atomic operations
- Thread pools
- Parallel algorithms (C++17)
- Practical examples: Building concurrent data structures

## Mini-Project: Advanced Task Scheduler
For this week's mini-project, you'll build an advanced task scheduling system that incorporates many of the concepts learned during the week. The system will:

1. Use templates for generic task types
2. Implement proper exception handling and RAII
3. Utilize STL containers and algorithms for task management
4. Incorporate modern C++ features for clean, efficient code
5. Include concurrent execution of tasks

## Compilation Instructions
All code examples should be compiled with C++17 support:
```bash
g++ -std=c++17 filename.cpp -o output_name -pthread
```

## Resources

### Books
- "Effective Modern C++" by Scott Meyers
- "C++ Templates: The Complete Guide" by David Vandevoorde and Nicolai M. Josuttis
- "C++ Concurrency in Action" by Anthony Williams

### Online Resources
- [C++ Reference](https://en.cppreference.com/)
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
- [Compiler Explorer](https://godbolt.org/)

### Videos
- CppCon talks on templates, STL, and modern C++ features
- Jason Turner's "C++ Weekly" episodes

## Exercises
Each topic includes exercises that progressively build your understanding. Complete all exercises to ensure a solid grasp of the concepts before moving to the next topic.

## Assessment
At the end of the week, you should be able to:
1. Write generic code using templates
2. Implement proper resource management with RAII
3. Effectively use STL containers and algorithms
4. Apply modern C++ features to write clean, efficient code
5. Implement basic concurrent programming patterns
6. Understand how these advanced features are used in OR-Tools
