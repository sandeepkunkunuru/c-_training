# Week 5: Build Systems and Development Tools

Welcome to Phase 2 of your C++ training! This week focuses on professional C++ development tools and practices essential for contributing to large projects like Google's OR-Tools.

## Learning Objectives
- Master CMake build system fundamentals and advanced features
- Set up comprehensive testing frameworks and practices
- Learn debugging techniques and tools
- Understand static analysis and code quality tools
- Practice continuous integration and development workflows
- Prepare for professional C++ development environments

## Phase 2 Overview
**Week 5**: Build Systems and Tools (CMake, Testing, Debugging)
**Week 6**: Performance Optimization and Profiling
**Week 7**: Design Patterns in C++
**Week 8**: Advanced Multithreading and Concurrency

## Daily Topics

### Day 1: CMake Fundamentals
- CMake basics and project structure
- Variables, functions, and macros
- Target-based build system
- Finding and linking libraries
- Cross-platform considerations

**Exercises:**
- [cmake_basics/](cmake_basics/) - Simple CMake project setup
- [cmake_libraries/](cmake_libraries/) - Working with external libraries
- [cmake_advanced/](cmake_advanced/) - Advanced CMake features

### Day 2: Testing Frameworks
- Google Test (gtest) integration
- Unit testing best practices
- Test-driven development (TDD)
- Mocking and test doubles
- Code coverage analysis

**Exercises:**
- [testing_framework/](testing_framework/) - Complete testing setup
- [tdd_example/](tdd_example/) - Test-driven development example
- [mocking_example/](mocking_example/) - Using Google Mock

### Day 3: Debugging and Analysis Tools
- GDB debugging techniques
- Valgrind memory analysis
- AddressSanitizer and other sanitizers
- Static analysis with clang-tidy
- Code formatting and linting

**Exercises:**
- [debugging_example/](debugging_example/) - Debugging techniques
- [memory_analysis/](memory_analysis/) - Memory leak detection
- [static_analysis/](static_analysis/) - Code quality tools

### Day 4: Package Management and Dependencies
- Conan package manager
- vcpkg integration
- Git submodules
- Dependency management strategies
- Version control best practices

**Exercises:**
- [conan_example/](conan_example/) - Using Conan for dependencies
- [vcpkg_example/](vcpkg_example/) - vcpkg integration
- [dependency_management/](dependency_management/) - Best practices

### Day 5: Continuous Integration and Automation
- GitHub Actions for C++
- Automated testing and deployment
- Cross-platform builds
- Documentation generation
- Release management

**Exercises:**
- [ci_cd_example/](ci_cd_example/) - Complete CI/CD pipeline
- [cross_platform/](cross_platform/) - Multi-platform builds
- [documentation/](documentation/) - Automated documentation

## Tools You'll Learn

### Build Systems
- **CMake**: Modern C++ build system
- **Ninja**: Fast build tool
- **Make**: Traditional build automation

### Testing
- **Google Test**: Unit testing framework
- **Google Mock**: Mocking framework
- **Catch2**: Alternative testing framework
- **CTest**: CMake testing integration

### Debugging & Analysis
- **GDB**: GNU Debugger
- **LLDB**: LLVM Debugger
- **Valgrind**: Memory analysis
- **AddressSanitizer**: Memory error detection
- **ThreadSanitizer**: Race condition detection

### Static Analysis
- **clang-tidy**: Static analysis tool
- **cppcheck**: Static analysis
- **clang-format**: Code formatting
- **include-what-you-use**: Header analysis

### Package Management
- **Conan**: C++ package manager
- **vcpkg**: Microsoft's package manager
- **Hunter**: CMake-based package manager

### CI/CD
- **GitHub Actions**: Continuous integration
- **Travis CI**: Build automation
- **AppVeyor**: Windows builds
- **Docker**: Containerization

## Prerequisites
Before starting Week 5, ensure you have:
- Completed Weeks 1-4 (C++ fundamentals through advanced STL)
- Basic understanding of command line tools
- Git version control knowledge
- Text editor or IDE setup

## Installation Requirements

### macOS (using Homebrew)
```bash
# Install CMake
brew install cmake

# Install testing tools
brew install googletest

# Install debugging tools
brew install gdb lldb

# Install static analysis tools
brew install llvm clang-format

# Install package managers
brew install conan
```

### Ubuntu/Debian
```bash
# Install CMake
sudo apt-get install cmake

# Install testing tools
sudo apt-get install libgtest-dev libgmock-dev

# Install debugging tools
sudo apt-get install gdb valgrind

# Install static analysis tools
sudo apt-get install clang-tidy cppcheck

# Install package managers
pip install conan
```

### Windows
- Install Visual Studio 2019/2022 with C++ tools
- Install CMake from cmake.org
- Install vcpkg for package management
- Use WSL2 for Linux-like development environment

## Project Structure
```
week5/
├── README.md                 # This file
├── cmake_basics/            # Day 1: CMake fundamentals
├── cmake_libraries/         # Day 1: Library integration
├── cmake_advanced/          # Day 1: Advanced CMake
├── testing_framework/       # Day 2: Testing setup
├── tdd_example/            # Day 2: Test-driven development
├── mocking_example/        # Day 2: Mocking techniques
├── debugging_example/      # Day 3: Debugging tools
├── memory_analysis/        # Day 3: Memory tools
├── static_analysis/        # Day 3: Code quality
├── conan_example/          # Day 4: Conan package manager
├── vcpkg_example/          # Day 4: vcpkg integration
├── dependency_management/  # Day 4: Best practices
├── ci_cd_example/          # Day 5: CI/CD pipeline
├── cross_platform/         # Day 5: Multi-platform
└── documentation/          # Day 5: Documentation
```

## Learning Resources

### CMake
- [CMake Official Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
- [Modern CMake](https://cliutils.gitlab.io/modern-cmake/)
- [CMake Cookbook](https://github.com/dev-cafe/cmake-cookbook)

### Testing
- [Google Test Primer](https://github.com/google/googletest/blob/master/docs/primer.md)
- [Test-Driven Development in C++](https://www.amazon.com/Test-Driven-Development-Kent-Beck/dp/0321146530)

### Debugging
- [GDB Tutorial](https://www.gnu.org/software/gdb/documentation/)
- [Valgrind User Manual](https://valgrind.org/docs/manual/manual.html)

### Package Management
- [Conan Documentation](https://docs.conan.io/)
- [vcpkg Documentation](https://vcpkg.io/en/getting-started.html)

## Success Criteria
By the end of Week 5, you should be able to:

1. **CMake Proficiency**
   - Create complex CMake projects with multiple targets
   - Integrate external libraries and dependencies
   - Write portable, cross-platform build scripts

2. **Testing Expertise**
   - Set up comprehensive test suites
   - Practice test-driven development
   - Use mocking for complex dependencies
   - Measure and improve code coverage

3. **Debugging Skills**
   - Use debuggers effectively for problem solving
   - Detect and fix memory leaks
   - Apply static analysis for code quality

4. **Professional Workflow**
   - Manage dependencies with package managers
   - Set up automated CI/CD pipelines
   - Follow industry best practices

## Connection to OR-Tools
This week's skills directly apply to OR-Tools contribution:

- **CMake**: OR-Tools uses CMake as its primary build system
- **Testing**: Google Test is used throughout OR-Tools
- **Code Quality**: OR-Tools follows strict coding standards
- **CI/CD**: Understanding automated testing and deployment
- **Dependencies**: Managing complex library dependencies

Let's begin with CMake fundamentals!
