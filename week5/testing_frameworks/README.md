# Testing Frameworks - Week 5 Day 2

This section covers testing frameworks in C++, focusing on Google Test (gtest), mocking, and Test-Driven Development (TDD).

## Learning Objectives

By the end of this section, you will understand:

1. **Google Test Framework**
   - Setting up Google Test with CMake
   - Writing basic unit tests
   - Test fixtures and parameterized tests
   - Assertions and expectations

2. **Mocking with Google Mock**
   - Creating mock objects
   - Setting expectations
   - Behavior verification
   - Integration with Google Test

3. **Test-Driven Development (TDD)**
   - Red-Green-Refactor cycle
   - Writing tests first
   - Incremental development
   - Refactoring with confidence

## Directory Structure

```
testing_frameworks/
├── README.md              # This file
├── gtest_basics/          # Basic Google Test examples
│   ├── CMakeLists.txt     # CMake configuration for tests
│   ├── src/               # Source code to test
│   │   ├── calculator.hpp
│   │   └── calculator.cpp
│   └── tests/             # Test files
│       ├── test_calculator.cpp
│       ├── test_fixtures.cpp
│       └── test_parameterized.cpp
├── mocking/               # Google Mock examples
│   ├── CMakeLists.txt
│   ├── src/
│   │   ├── database.hpp   # Interface to mock
│   │   ├── user_service.hpp
│   │   └── user_service.cpp
│   └── tests/
│       ├── test_mocking.cpp
│       └── test_user_service.cpp
└── tdd_example/           # TDD demonstration
    ├── CMakeLists.txt
    ├── src/
    │   ├── string_utils.hpp
    │   └── string_utils.cpp
    └── tests/
        └── test_string_utils.cpp
```

## Key Concepts

### Google Test Assertions

- **EXPECT_*** vs **ASSERT_***
  - `EXPECT_*` continues execution after failure
  - `ASSERT_*` stops execution after failure

- **Common Assertions**
  - `EXPECT_EQ(expected, actual)` - Equality
  - `EXPECT_NE(val1, val2)` - Inequality
  - `EXPECT_LT(val1, val2)` - Less than
  - `EXPECT_LE(val1, val2)` - Less than or equal
  - `EXPECT_GT(val1, val2)` - Greater than
  - `EXPECT_GE(val1, val2)` - Greater than or equal
  - `EXPECT_TRUE(condition)` - Boolean true
  - `EXPECT_FALSE(condition)` - Boolean false
  - `EXPECT_STREQ(str1, str2)` - String equality
  - `EXPECT_THROW(statement, exception)` - Exception throwing
  - `EXPECT_NO_THROW(statement)` - No exception throwing

### Test Fixtures

Test fixtures allow you to:
- Set up common test data
- Share setup/teardown code
- Group related tests

```cpp
class CalculatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        calc = std::make_unique<Calculator>();
    }
    
    void TearDown() override {
        calc.reset();
    }
    
    std::unique_ptr<Calculator> calc;
};
```

### Parameterized Tests

Test the same logic with different inputs:

```cpp
class CalculatorParamTest : public ::testing::TestWithParam<std::tuple<int, int, int>> {};

TEST_P(CalculatorParamTest, Addition) {
    auto [a, b, expected] = GetParam();
    EXPECT_EQ(expected, calculator.add(a, b));
}

INSTANTIATE_TEST_SUITE_P(
    AdditionTests,
    CalculatorParamTest,
    ::testing::Values(
        std::make_tuple(1, 2, 3),
        std::make_tuple(-1, 1, 0),
        std::make_tuple(0, 0, 0)
    )
);
```

### Google Mock

Mock objects simulate dependencies:

```cpp
class MockDatabase : public DatabaseInterface {
public:
    MOCK_METHOD(bool, save, (const User& user), (override));
    MOCK_METHOD(std::optional<User>, find, (int id), (override));
    MOCK_METHOD(void, delete_user, (int id), (override));
};
```

### TDD Cycle

1. **Red**: Write a failing test
2. **Green**: Write minimal code to make it pass
3. **Refactor**: Improve code while keeping tests green

## Building and Running

Each subdirectory has its own CMakeLists.txt. To build and run tests:

```bash
# For each example directory
cd gtest_basics  # or mocking, tdd_example
mkdir build && cd build
cmake ..
make
ctest  # or make test
```

## Prerequisites

- Google Test and Google Mock libraries
- CMake 3.16+
- C++17 compiler

Install Google Test:

```bash
# macOS
brew install googletest

# Ubuntu/Debian
sudo apt-get install libgtest-dev libgmock-dev

# Build from source (if needed)
git clone https://github.com/google/googletest.git
cd googletest
mkdir build && cd build
cmake ..
make
sudo make install
```

## Best Practices

1. **Test Organization**
   - One test file per source file
   - Group related tests in test suites
   - Use descriptive test names

2. **Test Quality**
   - Test one thing at a time
   - Make tests independent
   - Use meaningful assertions
   - Test edge cases and error conditions

3. **Test Maintenance**
   - Keep tests simple and readable
   - Refactor tests along with code
   - Remove obsolete tests
   - Use test fixtures for common setup

4. **TDD Benefits**
   - Better design through testability
   - Comprehensive test coverage
   - Confidence in refactoring
   - Documentation through tests

## Next Steps

After completing these examples:
1. Practice writing tests for existing code
2. Try TDD for a new feature
3. Explore advanced Google Test features
4. Learn about integration testing
5. Study continuous testing practices
