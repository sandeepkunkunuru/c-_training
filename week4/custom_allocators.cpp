/*
 * Custom Allocators Implementation
 * 
 * This program demonstrates how to implement custom allocators for STL containers.
 * Custom allocators are useful for memory optimization, debugging, and specialized
 * memory management scenarios.
 * 
 * Key Learning Points:
 * - Allocator requirements and interface
 * - Memory pool allocators
 * - Stack allocators
 * - Debugging allocators
 * - Performance implications
 */

#include <iostream>
#include <vector>
#include <list>
#include <memory>
#include <chrono>
#include <iomanip>
#include <cassert>
#include <cstdlib>
#include <new>

// 1. Logging Allocator - tracks all allocations
template<typename T>
class LoggingAllocator {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    
private:
    static size_type allocation_count_;
    static size_type deallocation_count_;
    static size_type bytes_allocated_;
    static size_type bytes_deallocated_;
    
public:
    LoggingAllocator() = default;
    
    template<typename U>
    LoggingAllocator(const LoggingAllocator<U>&) noexcept {}
    
    T* allocate(size_type n) {
        size_type bytes = n * sizeof(T);
        T* ptr = static_cast<T*>(std::malloc(bytes));
        
        if (!ptr) {
            throw std::bad_alloc();
        }
        
        ++allocation_count_;
        bytes_allocated_ += bytes;
        
        std::cout << "[ALLOC] " << n << " objects of " << sizeof(T) 
                  << " bytes each = " << bytes << " bytes at " << ptr << std::endl;
        
        return ptr;
    }
    
    void deallocate(T* ptr, size_type n) {
        size_type bytes = n * sizeof(T);
        
        ++deallocation_count_;
        bytes_deallocated_ += bytes;
        
        std::cout << "[DEALLOC] " << n << " objects = " << bytes 
                  << " bytes at " << ptr << std::endl;
        
        std::free(ptr);
    }
    
    template<typename U>
    bool operator==(const LoggingAllocator<U>&) const noexcept { return true; }
    
    template<typename U>
    bool operator!=(const LoggingAllocator<U>&) const noexcept { return false; }
    
    static void print_stats() {
        std::cout << "\n=== Allocation Statistics ===" << std::endl;
        std::cout << "Allocations: " << allocation_count_ << std::endl;
        std::cout << "Deallocations: " << deallocation_count_ << std::endl;
        std::cout << "Bytes allocated: " << bytes_allocated_ << std::endl;
        std::cout << "Bytes deallocated: " << bytes_deallocated_ << std::endl;
        std::cout << "Net bytes: " << (bytes_allocated_ - bytes_deallocated_) << std::endl;
        std::cout << "=============================" << std::endl;
    }
    
    static void reset_stats() {
        allocation_count_ = 0;
        deallocation_count_ = 0;
        bytes_allocated_ = 0;
        bytes_deallocated_ = 0;
    }
};

// Static member definitions
template<typename T>
typename LoggingAllocator<T>::size_type LoggingAllocator<T>::allocation_count_ = 0;

template<typename T>
typename LoggingAllocator<T>::size_type LoggingAllocator<T>::deallocation_count_ = 0;

template<typename T>
typename LoggingAllocator<T>::size_type LoggingAllocator<T>::bytes_allocated_ = 0;

template<typename T>
typename LoggingAllocator<T>::size_type LoggingAllocator<T>::bytes_deallocated_ = 0;

// 2. Memory Pool Allocator
template<typename T, size_t BlockSize = 1024>
class PoolAllocator {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_swap = std::false_type;
    
private:
    struct Block {
        alignas(T) char data[BlockSize * sizeof(T)];
        Block* next;
        size_type used;
        
        Block() : next(nullptr), used(0) {}
        
        T* allocate() {
            if (used >= BlockSize) {
                return nullptr;
            }
            T* ptr = reinterpret_cast<T*>(data + used * sizeof(T));
            ++used;
            return ptr;
        }
        
        bool contains(T* ptr) const {
            return ptr >= reinterpret_cast<const T*>(data) && 
                   ptr < reinterpret_cast<const T*>(data + BlockSize * sizeof(T));
        }
    };
    
    mutable Block* current_block_;
    mutable std::vector<std::unique_ptr<Block>> blocks_;
    
    Block* allocate_new_block() const {
        blocks_.push_back(std::make_unique<Block>());
        current_block_ = blocks_.back().get();
        return current_block_;
    }
    
public:
    PoolAllocator() : current_block_(nullptr) {}
    
    template<typename U>
    PoolAllocator(const PoolAllocator<U, BlockSize>& other) noexcept 
        : current_block_(nullptr) {}
    
    T* allocate(size_type n) {
        if (n != 1) {
            // For simplicity, only support single object allocation
            throw std::bad_alloc();
        }
        
        if (!current_block_ || current_block_->used >= BlockSize) {
            allocate_new_block();
        }
        
        T* ptr = current_block_->allocate();
        if (!ptr) {
            allocate_new_block();
            ptr = current_block_->allocate();
        }
        
        if (!ptr) {
            throw std::bad_alloc();
        }
        
        return ptr;
    }
    
    void deallocate(T* ptr, size_type n) {
        // Pool allocator doesn't deallocate individual objects
        // Memory is freed when the allocator is destroyed
    }
    
    template<typename U>
    bool operator==(const PoolAllocator<U, BlockSize>&) const noexcept { return false; }
    
    template<typename U>
    bool operator!=(const PoolAllocator<U, BlockSize>&) const noexcept { return true; }
    
    size_type get_block_count() const { return blocks_.size(); }
    size_type get_total_capacity() const { return blocks_.size() * BlockSize; }
    
    void print_stats() const {
        std::cout << "Pool Allocator Stats:" << std::endl;
        std::cout << "  Blocks: " << blocks_.size() << std::endl;
        std::cout << "  Total capacity: " << get_total_capacity() << " objects" << std::endl;
        std::cout << "  Memory usage: " << blocks_.size() * sizeof(Block) << " bytes" << std::endl;
    }
};

// 3. Stack Allocator (uses pre-allocated buffer)
template<typename T, size_t N>
class StackAllocator {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_swap = std::false_type;
    
private:
    alignas(T) mutable char buffer_[N * sizeof(T)];
    mutable size_type used_;
    
public:
    StackAllocator() : used_(0) {}
    
    template<typename U>
    StackAllocator(const StackAllocator<U, N>&) noexcept : used_(0) {}
    
    T* allocate(size_type n) {
        if (used_ + n > N) {
            throw std::bad_alloc();
        }
        
        T* ptr = reinterpret_cast<T*>(buffer_ + used_ * sizeof(T));
        used_ += n;
        return ptr;
    }
    
    void deallocate(T* ptr, size_type n) {
        // Stack allocator doesn't support individual deallocation
        // Only supports LIFO deallocation pattern
        if (ptr + n == reinterpret_cast<T*>(buffer_ + used_ * sizeof(T))) {
            used_ -= n;
        }
    }
    
    template<typename U>
    bool operator==(const StackAllocator<U, N>&) const noexcept { return false; }
    
    template<typename U>
    bool operator!=(const StackAllocator<U, N>&) const noexcept { return true; }
    
    size_type capacity() const { return N; }
    size_type size() const { return used_; }
    size_type available() const { return N - used_; }
    
    void reset() { used_ = 0; }
    
    void print_stats() const {
        std::cout << "Stack Allocator Stats:" << std::endl;
        std::cout << "  Capacity: " << N << " objects" << std::endl;
        std::cout << "  Used: " << used_ << " objects" << std::endl;
        std::cout << "  Available: " << available() << " objects" << std::endl;
        std::cout << "  Usage: " << std::fixed << std::setprecision(1) 
                  << (100.0 * used_ / N) << "%" << std::endl;
    }
};

// 4. Aligned Allocator (for SIMD operations)
template<typename T, size_t Alignment = 32>
class AlignedAllocator {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    
    static_assert(Alignment >= alignof(T), "Alignment must be at least alignof(T)");
    static_assert((Alignment & (Alignment - 1)) == 0, "Alignment must be power of 2");
    
public:
    AlignedAllocator() = default;
    
    template<typename U>
    AlignedAllocator(const AlignedAllocator<U, Alignment>&) noexcept {}
    
    T* allocate(size_type n) {
        size_type bytes = n * sizeof(T);
        
        // Use aligned_alloc (C++17) or fallback to malloc + manual alignment
        void* ptr = std::aligned_alloc(Alignment, bytes);
        
        if (!ptr) {
            throw std::bad_alloc();
        }
        
        return static_cast<T*>(ptr);
    }
    
    void deallocate(T* ptr, size_type n) {
        std::free(ptr);
    }
    
    template<typename U>
    bool operator==(const AlignedAllocator<U, Alignment>&) const noexcept { return true; }
    
    template<typename U>
    bool operator!=(const AlignedAllocator<U, Alignment>&) const noexcept { return false; }
    
    static constexpr size_type alignment() { return Alignment; }
};

void demonstrateLoggingAllocator() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Logging Allocator Demonstration" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    LoggingAllocator<int>::reset_stats();
    
    {
        std::vector<int, LoggingAllocator<int>> vec;
        
        std::cout << "Creating vector and adding elements:" << std::endl;
        for (int i = 0; i < 10; ++i) {
            vec.push_back(i);
        }
        
        std::cout << "\nResizing vector to 20 elements:" << std::endl;
        vec.resize(20, 42);
        
        std::cout << "\nVector contents: ";
        for (int val : vec) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
        
        std::cout << "\nVector going out of scope..." << std::endl;
    }
    
    LoggingAllocator<int>::print_stats();
}

void demonstratePoolAllocator() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Pool Allocator Demonstration" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    const size_t num_elements = 2500; // More than one block
    
    {
        std::list<int, PoolAllocator<int, 1000>> pool_list;
        
        std::cout << "Adding " << num_elements << " elements to list with pool allocator:" << std::endl;
        
        for (size_t i = 0; i < num_elements; ++i) {
            pool_list.push_back(static_cast<int>(i));
        }
        
        std::cout << "List size: " << pool_list.size() << std::endl;
        
        // Access the allocator to print stats
        auto alloc = pool_list.get_allocator();
        alloc.print_stats();
        
        std::cout << "\nFirst 10 elements: ";
        auto it = pool_list.begin();
        for (int i = 0; i < 10 && it != pool_list.end(); ++i, ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
    
    std::cout << "Pool allocator destroyed (all memory freed)" << std::endl;
}

void demonstrateStackAllocator() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Stack Allocator Demonstration" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    {
        std::vector<int, StackAllocator<int, 100>> stack_vec;
        
        std::cout << "Creating vector with stack allocator (capacity: 100):" << std::endl;
        
        // Add elements
        for (int i = 0; i < 50; ++i) {
            stack_vec.push_back(i * 2);
        }
        
        std::cout << "Added 50 elements" << std::endl;
        stack_vec.get_allocator().print_stats();
        
        std::cout << "\nVector contents (first 10): ";
        for (size_t i = 0; i < 10 && i < stack_vec.size(); ++i) {
            std::cout << stack_vec[i] << " ";
        }
        std::cout << std::endl;
        
        // Try to exceed capacity
        std::cout << "\nTrying to add 60 more elements (will exceed capacity):" << std::endl;
        try {
            for (int i = 0; i < 60; ++i) {
                stack_vec.push_back(i);
            }
        } catch (const std::bad_alloc& e) {
            std::cout << "Caught std::bad_alloc: " << e.what() << std::endl;
            stack_vec.get_allocator().print_stats();
        }
    }
}

void demonstrateAlignedAllocator() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Aligned Allocator Demonstration" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    {
        std::vector<float, AlignedAllocator<float, 32>> aligned_vec;
        
        // Add some elements
        for (int i = 0; i < 16; ++i) {
            aligned_vec.push_back(static_cast<float>(i) * 1.5f);
        }
        
        std::cout << "Created vector with 32-byte aligned allocator" << std::endl;
        std::cout << "Vector size: " << aligned_vec.size() << std::endl;
        std::cout << "Data pointer: " << aligned_vec.data() << std::endl;
        std::cout << "Alignment: " << AlignedAllocator<float, 32>::alignment() << " bytes" << std::endl;
        
        // Check alignment
        uintptr_t addr = reinterpret_cast<uintptr_t>(aligned_vec.data());
        bool is_aligned = (addr % 32) == 0;
        std::cout << "Is properly aligned: " << (is_aligned ? "Yes" : "No") << std::endl;
        
        std::cout << "Vector contents: ";
        for (float val : aligned_vec) {
            std::cout << std::fixed << std::setprecision(1) << val << " ";
        }
        std::cout << std::endl;
    }
}

void demonstratePerformanceComparison() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Performance Comparison" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    const size_t num_operations = 100000;
    
    auto measureTime = [](auto func) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count() / 1000.0; // Return milliseconds
    };
    
    // Test 1: Vector with frequent push_back
    double std_time = measureTime([&]() {
        std::vector<int> vec;
        for (size_t i = 0; i < num_operations; ++i) {
            vec.push_back(static_cast<int>(i));
        }
    });
    
    double pool_time = measureTime([&]() {
        std::vector<int, PoolAllocator<int, 1000>> vec;
        for (size_t i = 0; i < num_operations; ++i) {
            vec.push_back(static_cast<int>(i));
        }
    });
    
    // Test 2: List with frequent insertions
    double std_list_time = measureTime([&]() {
        std::list<int> lst;
        for (size_t i = 0; i < num_operations / 10; ++i) { // Fewer operations for list
            lst.push_back(static_cast<int>(i));
        }
    });
    
    double pool_list_time = measureTime([&]() {
        std::list<int, PoolAllocator<int, 1000>> lst;
        for (size_t i = 0; i < num_operations / 10; ++i) {
            lst.push_back(static_cast<int>(i));
        }
    });
    
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Performance results:" << std::endl;
    std::cout << "\nVector (" << num_operations << " push_back operations):" << std::endl;
    std::cout << "  Standard allocator: " << std_time << " ms" << std::endl;
    std::cout << "  Pool allocator:     " << pool_time << " ms" << std::endl;
    std::cout << "  Speedup:            " << std::setprecision(2) << std_time / pool_time << "x" << std::endl;
    
    std::cout << "\nList (" << (num_operations / 10) << " push_back operations):" << std::endl;
    std::cout << "  Standard allocator: " << std::setprecision(3) << std_list_time << " ms" << std::endl;
    std::cout << "  Pool allocator:     " << pool_list_time << " ms" << std::endl;
    std::cout << "  Speedup:            " << std::setprecision(2) << std_list_time / pool_list_time << "x" << std::endl;
    
    std::cout << "\nNote: Pool allocator performance depends on allocation patterns." << std::endl;
    std::cout << "It's most beneficial for containers with many small allocations." << std::endl;
}

void demonstrateAllocatorRebinding() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Allocator Rebinding" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << "Allocator rebinding allows containers to allocate different types" << std::endl;
    std::cout << "using the same allocator. This is important for containers like" << std::endl;
    std::cout << "std::list which need to allocate nodes, not just the value type." << std::endl;
    
    // Demonstrate with a simple example
    LoggingAllocator<int>::reset_stats();
    
    {
        std::cout << "\nCreating std::list<int> with LoggingAllocator<int>:" << std::endl;
        std::list<int, LoggingAllocator<int>> lst;
        
        for (int i = 0; i < 5; ++i) {
            lst.push_back(i);
        }
        
        std::cout << "List contents: ";
        for (int val : lst) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
    
    std::cout << "\nNote: The allocator was rebound to allocate list nodes," << std::endl;
    std::cout << "not just int values. Check the allocation sizes:" << std::endl;
    LoggingAllocator<int>::print_stats();
}

int main() {
    std::cout << "===== Custom Allocators Implementation =====" << std::endl;
    std::cout << "This program demonstrates various custom allocator implementations" << std::endl;
    std::cout << "and their use cases in STL containers." << std::endl;
    
    demonstrateLoggingAllocator();
    demonstratePoolAllocator();
    demonstrateStackAllocator();
    demonstrateAlignedAllocator();
    demonstratePerformanceComparison();
    demonstrateAllocatorRebinding();
    
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "Custom Allocator Use Cases:" << std::endl;
    std::cout << "1. Logging Allocator: Debug memory usage and detect leaks" << std::endl;
    std::cout << "2. Pool Allocator: Optimize frequent small allocations" << std::endl;
    std::cout << "3. Stack Allocator: Use stack memory for temporary containers" << std::endl;
    std::cout << "4. Aligned Allocator: Ensure proper alignment for SIMD operations" << std::endl;
    std::cout << "5. Custom allocators can significantly improve performance" << std::endl;
    std::cout << "6. Always measure performance impact in your specific use case" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    return 0;
}
