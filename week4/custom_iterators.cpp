/*
 * Custom Iterators Implementation
 * 
 * This program demonstrates how to implement custom iterators for user-defined containers.
 * Understanding iterators is crucial for creating STL-compatible containers and algorithms.
 * 
 * Key Learning Points:
 * - Iterator categories and their requirements
 * - Implementing iterator traits
 * - Creating STL-compatible iterators
 * - Iterator adapters and utilities
 * - Performance considerations
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <memory>
#include <cassert>

// Forward declarations
template<typename T> class SimpleVector;
template<typename T> class SimpleVectorIterator;
template<typename T> class RangeIterator;

// 1. Simple Vector with Custom Iterator
template<typename T>
class SimpleVector {
private:
    std::unique_ptr<T[]> data_;
    size_t size_;
    size_t capacity_;
    
public:
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = SimpleVectorIterator<T>;
    using const_iterator = SimpleVectorIterator<const T>;
    
    SimpleVector() : data_(nullptr), size_(0), capacity_(0) {}
    
    explicit SimpleVector(size_t capacity) 
        : data_(std::make_unique<T[]>(capacity)), size_(0), capacity_(capacity) {}
    
    ~SimpleVector() = default;
    
    // Move constructor and assignment
    SimpleVector(SimpleVector&& other) noexcept
        : data_(std::move(other.data_)), size_(other.size_), capacity_(other.capacity_) {
        other.size_ = 0;
        other.capacity_ = 0;
    }
    
    SimpleVector& operator=(SimpleVector&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }
    
    // Disable copy for simplicity
    SimpleVector(const SimpleVector&) = delete;
    SimpleVector& operator=(const SimpleVector&) = delete;
    
    void push_back(const T& value) {
        if (size_ >= capacity_) {
            resize(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = value;
    }
    
    void push_back(T&& value) {
        if (size_ >= capacity_) {
            resize(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = std::move(value);
    }
    
    T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }
    
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }
    
    // Iterator interface
    iterator begin() { return iterator(data_.get()); }
    iterator end() { return iterator(data_.get() + size_); }
    const_iterator begin() const { return const_iterator(data_.get()); }
    const_iterator end() const { return const_iterator(data_.get() + size_); }
    const_iterator cbegin() const { return const_iterator(data_.get()); }
    const_iterator cend() const { return const_iterator(data_.get() + size_); }
    
private:
    void resize(size_t new_capacity) {
        auto new_data = std::make_unique<T[]>(new_capacity);
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
        }
        data_ = std::move(new_data);
        capacity_ = new_capacity;
    }
};

// 2. Random Access Iterator Implementation
template<typename T>
class SimpleVectorIterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = std::remove_cv_t<T>;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    
private:
    pointer ptr_;
    
public:
    explicit SimpleVectorIterator(pointer ptr) : ptr_(ptr) {}
    
    // Copy constructor and assignment
    SimpleVectorIterator(const SimpleVectorIterator&) = default;
    SimpleVectorIterator& operator=(const SimpleVectorIterator&) = default;
    
    // Conversion from non-const to const iterator
    template<typename U, typename = std::enable_if_t<std::is_same_v<U, std::remove_const_t<T>>>>
    SimpleVectorIterator(const SimpleVectorIterator<U>& other) : ptr_(other.ptr_) {}
    
    // Dereference operators
    reference operator*() const { return *ptr_; }
    pointer operator->() const { return ptr_; }
    reference operator[](difference_type n) const { return ptr_[n]; }
    
    // Increment/decrement operators
    SimpleVectorIterator& operator++() { ++ptr_; return *this; }
    SimpleVectorIterator operator++(int) { auto temp = *this; ++ptr_; return temp; }
    SimpleVectorIterator& operator--() { --ptr_; return *this; }
    SimpleVectorIterator operator--(int) { auto temp = *this; --ptr_; return temp; }
    
    // Arithmetic operators
    SimpleVectorIterator& operator+=(difference_type n) { ptr_ += n; return *this; }
    SimpleVectorIterator& operator-=(difference_type n) { ptr_ -= n; return *this; }
    SimpleVectorIterator operator+(difference_type n) const { return SimpleVectorIterator(ptr_ + n); }
    SimpleVectorIterator operator-(difference_type n) const { return SimpleVectorIterator(ptr_ - n); }
    difference_type operator-(const SimpleVectorIterator& other) const { return ptr_ - other.ptr_; }
    
    // Comparison operators
    bool operator==(const SimpleVectorIterator& other) const { return ptr_ == other.ptr_; }
    bool operator!=(const SimpleVectorIterator& other) const { return ptr_ != other.ptr_; }
    bool operator<(const SimpleVectorIterator& other) const { return ptr_ < other.ptr_; }
    bool operator<=(const SimpleVectorIterator& other) const { return ptr_ <= other.ptr_; }
    bool operator>(const SimpleVectorIterator& other) const { return ptr_ > other.ptr_; }
    bool operator>=(const SimpleVectorIterator& other) const { return ptr_ >= other.ptr_; }
    
    // Allow access to pointer for conversion constructor
    template<typename U> friend class SimpleVectorIterator;
};

// Non-member arithmetic operators
template<typename T>
SimpleVectorIterator<T> operator+(typename SimpleVectorIterator<T>::difference_type n, 
                                  const SimpleVectorIterator<T>& it) {
    return it + n;
}

// 3. Range Iterator (generates values on-the-fly)
template<typename T>
class RangeIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;
    
private:
    T current_;
    T step_;
    
public:
    RangeIterator(T start, T step = T(1)) : current_(start), step_(step) {}
    
    reference operator*() const { return current_; }
    pointer operator->() const { return &current_; }
    
    RangeIterator& operator++() { current_ += step_; return *this; }
    RangeIterator operator++(int) { auto temp = *this; current_ += step_; return temp; }
    
    bool operator==(const RangeIterator& other) const { return current_ == other.current_; }
    bool operator!=(const RangeIterator& other) const { return current_ != other.current_; }
};

// 4. Filter Iterator (skips elements that don't match predicate)
template<typename Iterator, typename Predicate>
class FilterIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    using difference_type = typename std::iterator_traits<Iterator>::difference_type;
    using pointer = typename std::iterator_traits<Iterator>::pointer;
    using reference = typename std::iterator_traits<Iterator>::reference;
    
private:
    Iterator current_;
    Iterator end_;
    Predicate predicate_;
    
    void advance_to_valid() {
        while (current_ != end_ && !predicate_(*current_)) {
            ++current_;
        }
    }
    
public:
    FilterIterator(Iterator current, Iterator end, Predicate pred)
        : current_(current), end_(end), predicate_(pred) {
        advance_to_valid();
    }
    
    reference operator*() const { return *current_; }
    pointer operator->() const { return current_.operator->(); }
    
    FilterIterator& operator++() {
        ++current_;
        advance_to_valid();
        return *this;
    }
    
    FilterIterator operator++(int) {
        auto temp = *this;
        ++(*this);
        return temp;
    }
    
    bool operator==(const FilterIterator& other) const { return current_ == other.current_; }
    bool operator!=(const FilterIterator& other) const { return current_ != other.current_; }
};

// Helper function to create filter iterators
template<typename Iterator, typename Predicate>
FilterIterator<Iterator, Predicate> make_filter_iterator(Iterator current, Iterator end, Predicate pred) {
    return FilterIterator<Iterator, Predicate>(current, end, pred);
}

// 5. Transform Iterator (applies function to elements on-the-fly)
template<typename Iterator, typename Function>
class TransformIterator {
public:
    using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
    using value_type = std::invoke_result_t<Function, typename std::iterator_traits<Iterator>::reference>;
    using difference_type = typename std::iterator_traits<Iterator>::difference_type;
    using pointer = void; // Not meaningful for transform iterator
    using reference = value_type;
    
private:
    Iterator current_;
    Function function_;
    
public:
    TransformIterator(Iterator current, Function func) : current_(current), function_(func) {}
    
    reference operator*() const { return function_(*current_); }
    
    TransformIterator& operator++() { ++current_; return *this; }
    TransformIterator operator++(int) { auto temp = *this; ++current_; return temp; }
    
    bool operator==(const TransformIterator& other) const { return current_ == other.current_; }
    bool operator!=(const TransformIterator& other) const { return current_ != other.current_; }
};

// Helper function to create transform iterators
template<typename Iterator, typename Function>
TransformIterator<Iterator, Function> make_transform_iterator(Iterator current, Function func) {
    return TransformIterator<Iterator, Function>(current, func);
}

void demonstrateSimpleVector() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Simple Vector with Custom Iterator" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    SimpleVector<int> vec(10);
    
    // Add some elements
    for (int i = 1; i <= 10; ++i) {
        vec.push_back(i * i);
    }
    
    std::cout << "Vector contents using custom iterator:" << std::endl;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Range-based for loop (uses begin/end)
    std::cout << "Using range-based for loop:" << std::endl;
    for (const auto& value : vec) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    
    // STL algorithms work with our custom iterator
    std::cout << "Using STL algorithms:" << std::endl;
    
    // Find
    auto it = std::find(vec.begin(), vec.end(), 25);
    if (it != vec.end()) {
        std::cout << "Found 25 at position: " << std::distance(vec.begin(), it) << std::endl;
    }
    
    // Transform
    SimpleVector<int> doubled(vec.size());
    std::transform(vec.begin(), vec.end(), doubled.begin(), 
                   [](int x) { return x * 2; });
    
    std::cout << "Doubled values: ";
    for (const auto& value : doubled) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    
    // Sort
    std::sort(vec.begin(), vec.end(), std::greater<int>());
    std::cout << "Sorted in descending order: ";
    for (const auto& value : vec) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

void demonstrateRangeIterator() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Range Iterator (Generator)" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    // Create a range from 0 to 20 with step 2
    auto range_begin = RangeIterator<int>(0, 2);
    auto range_end = RangeIterator<int>(20, 2);
    
    std::cout << "Range [0, 20) with step 2:" << std::endl;
    for (auto it = range_begin; it != range_end; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Use with STL algorithms
    std::vector<int> collected;
    auto range_begin2 = RangeIterator<int>(1, 3);
    auto range_end2 = RangeIterator<int>(25, 3);
    
    std::copy(range_begin2, range_end2, std::back_inserter(collected));
    
    std::cout << "Collected range [1, 25) with step 3:" << std::endl;
    for (int value : collected) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    
    // Sum using accumulate
    auto sum = std::accumulate(range_begin2, range_end2, 0);
    std::cout << "Sum of range: " << sum << std::endl;
}

void demonstrateFilterIterator() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Filter Iterator" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    
    std::cout << "Original numbers: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // Filter even numbers
    auto even_predicate = [](int x) { return x % 2 == 0; };
    auto even_begin = make_filter_iterator(numbers.begin(), numbers.end(), even_predicate);
    auto even_end = make_filter_iterator(numbers.end(), numbers.end(), even_predicate);
    
    std::cout << "Even numbers: ";
    for (auto it = even_begin; it != even_end; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Filter numbers greater than 10
    auto gt10_predicate = [](int x) { return x > 10; };
    auto gt10_begin = make_filter_iterator(numbers.begin(), numbers.end(), gt10_predicate);
    auto gt10_end = make_filter_iterator(numbers.end(), numbers.end(), gt10_predicate);
    
    std::cout << "Numbers > 10: ";
    for (auto it = gt10_begin; it != gt10_end; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Use with STL algorithms
    std::vector<int> filtered_even;
    std::copy(even_begin, even_end, std::back_inserter(filtered_even));
    
    std::cout << "Filtered even numbers (copied to vector): ";
    for (int n : filtered_even) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
}

void demonstrateTransformIterator() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Transform Iterator" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::cout << "Original numbers: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // Transform to squares
    auto square_func = [](int x) { return x * x; };
    auto square_begin = make_transform_iterator(numbers.begin(), square_func);
    auto square_end = make_transform_iterator(numbers.end(), square_func);
    
    std::cout << "Squares: ";
    for (auto it = square_begin; it != square_end; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Transform to strings
    auto to_string_func = [](int x) { return "num_" + std::to_string(x); };
    auto string_begin = make_transform_iterator(numbers.begin(), to_string_func);
    auto string_end = make_transform_iterator(numbers.end(), to_string_func);
    
    std::cout << "As strings: ";
    for (auto it = string_begin; it != string_end; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Use with STL algorithms
    auto sum_of_squares = std::accumulate(square_begin, square_end, 0);
    std::cout << "Sum of squares: " << sum_of_squares << std::endl;
    
    // Combine transform and filter
    auto even_squares_begin = make_filter_iterator(square_begin, square_end, 
                                                   [](int x) { return x % 2 == 0; });
    auto even_squares_end = make_filter_iterator(square_end, square_end, 
                                                 [](int x) { return x % 2 == 0; });
    
    std::cout << "Even squares: ";
    for (auto it = even_squares_begin; it != even_squares_end; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void demonstrateIteratorTraits() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Iterator Traits and Categories" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    using VecIter = SimpleVectorIterator<int>;
    using RangeIter = RangeIterator<int>;
    
    // Check iterator categories
    std::cout << "Iterator category information:" << std::endl;
    
    std::cout << "SimpleVectorIterator category: ";
    if constexpr (std::is_same_v<typename std::iterator_traits<VecIter>::iterator_category, 
                                std::random_access_iterator_tag>) {
        std::cout << "Random Access" << std::endl;
    }
    
    std::cout << "RangeIterator category: ";
    if constexpr (std::is_same_v<typename std::iterator_traits<RangeIter>::iterator_category, 
                                std::forward_iterator_tag>) {
        std::cout << "Forward" << std::endl;
    }
    
    // Demonstrate iterator category detection
    auto test_iterator_category = [](auto iter) {
        using Category = typename std::iterator_traits<decltype(iter)>::iterator_category;
        
        if constexpr (std::is_same_v<Category, std::random_access_iterator_tag>) {
            std::cout << "  Supports random access (O(1) advance)" << std::endl;
        } else if constexpr (std::is_same_v<Category, std::bidirectional_iterator_tag>) {
            std::cout << "  Supports bidirectional traversal" << std::endl;
        } else if constexpr (std::is_same_v<Category, std::forward_iterator_tag>) {
            std::cout << "  Supports forward traversal only" << std::endl;
        } else if constexpr (std::is_same_v<Category, std::input_iterator_tag>) {
            std::cout << "  Input iterator (single-pass)" << std::endl;
        } else if constexpr (std::is_same_v<Category, std::output_iterator_tag>) {
            std::cout << "  Output iterator (write-only)" << std::endl;
        }
    };
    
    std::cout << "\nTesting SimpleVectorIterator:" << std::endl;
    SimpleVector<int> vec(5);
    test_iterator_category(vec.begin());
    
    std::cout << "\nTesting RangeIterator:" << std::endl;
    test_iterator_category(RangeIterator<int>(0));
    
    std::cout << "\nTesting std::vector iterator:" << std::endl;
    std::vector<int> std_vec;
    test_iterator_category(std_vec.begin());
}

void demonstratePerformanceComparison() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Performance Comparison" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    const size_t size = 1000000;
    
    auto measureTime = [](auto func) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count() / 1000.0; // Return milliseconds
    };
    
    // Prepare data
    SimpleVector<int> custom_vec(size);
    std::vector<int> std_vec;
    std_vec.reserve(size);
    
    for (size_t i = 0; i < size; ++i) {
        int value = static_cast<int>(i);
        custom_vec.push_back(value);
        std_vec.push_back(value);
    }
    
    // Test iteration performance
    double custom_time = measureTime([&]() {
        volatile long long sum = 0;
        for (auto it = custom_vec.begin(); it != custom_vec.end(); ++it) {
            sum += *it;
        }
    });
    
    double std_time = measureTime([&]() {
        volatile long long sum = 0;
        for (auto it = std_vec.begin(); it != std_vec.end(); ++it) {
            sum += *it;
        }
    });
    
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Iteration performance for " << size << " elements:" << std::endl;
    std::cout << "  Custom iterator: " << custom_time << " ms" << std::endl;
    std::cout << "  std::vector:     " << std_time << " ms" << std::endl;
    std::cout << "  Ratio:           " << custom_time / std_time << "x" << std::endl;
    
    // Test random access performance
    std::vector<size_t> indices;
    for (size_t i = 0; i < 10000; ++i) {
        indices.push_back(i * (size / 10000));
    }
    
    double custom_random = measureTime([&]() {
        volatile long long sum = 0;
        for (size_t idx : indices) {
            sum += custom_vec[idx];
        }
    });
    
    double std_random = measureTime([&]() {
        volatile long long sum = 0;
        for (size_t idx : indices) {
            sum += std_vec[idx];
        }
    });
    
    std::cout << "\nRandom access performance (10,000 accesses):" << std::endl;
    std::cout << "  Custom iterator: " << custom_random << " ms" << std::endl;
    std::cout << "  std::vector:     " << std_random << " ms" << std::endl;
    std::cout << "  Ratio:           " << custom_random / std_random << "x" << std::endl;
}

int main() {
    std::cout << "===== Custom Iterators Implementation =====" << std::endl;
    std::cout << "This program demonstrates how to implement custom iterators" << std::endl;
    std::cout << "that are compatible with STL algorithms and containers." << std::endl;
    
    demonstrateSimpleVector();
    demonstrateRangeIterator();
    demonstrateFilterIterator();
    demonstrateTransformIterator();
    demonstrateIteratorTraits();
    demonstratePerformanceComparison();
    
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "Key Iterator Implementation Guidelines:" << std::endl;
    std::cout << "1. Define proper iterator_traits (category, value_type, etc.)" << std::endl;
    std::cout << "2. Implement all required operators for the iterator category" << std::endl;
    std::cout << "3. Ensure const-correctness and proper conversions" << std::endl;
    std::cout << "4. Test with STL algorithms to verify compatibility" << std::endl;
    std::cout << "5. Consider performance implications of your design" << std::endl;
    std::cout << "6. Use iterator adapters for common patterns (filter, transform)" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    return 0;
}
