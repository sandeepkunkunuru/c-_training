/*
 * Memory-Efficient Container Usage
 * 
 * This program demonstrates techniques for optimizing memory usage with STL containers.
 * Understanding memory efficiency is crucial for high-performance applications and
 * systems with limited memory resources.
 * 
 * Key Learning Points:
 * - Container memory overhead and layout
 * - Reserve vs resize strategies
 * - Memory pool techniques
 * - Small object optimization
 * - Cache-friendly data structures
 */

#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <array>
#include <string>
#include <memory>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <numeric>

// Helper class to track memory allocations
class MemoryTracker {
private:
    static size_t totalAllocated;
    static size_t totalDeallocated;
    static size_t currentUsage;
    
public:
    static void* allocate(size_t size) {
        totalAllocated += size;
        currentUsage += size;
        return std::malloc(size);
    }
    
    static void deallocate(void* ptr, size_t size) {
        totalDeallocated += size;
        currentUsage -= size;
        std::free(ptr);
    }
    
    static void printStats() {
        std::cout << "Memory Stats:" << std::endl;
        std::cout << "  Total Allocated: " << totalAllocated << " bytes" << std::endl;
        std::cout << "  Total Deallocated: " << totalDeallocated << " bytes" << std::endl;
        std::cout << "  Current Usage: " << currentUsage << " bytes" << std::endl;
        std::cout << "  Peak Usage: " << totalAllocated - totalDeallocated + currentUsage << " bytes" << std::endl;
    }
    
    static void reset() {
        totalAllocated = 0;
        totalDeallocated = 0;
        currentUsage = 0;
    }
};

size_t MemoryTracker::totalAllocated = 0;
size_t MemoryTracker::totalDeallocated = 0;
size_t MemoryTracker::currentUsage = 0;

// Custom allocator that tracks memory usage
template<typename T>
class TrackingAllocator {
public:
    using value_type = T;
    
    TrackingAllocator() = default;
    
    template<typename U>
    TrackingAllocator(const TrackingAllocator<U>&) {}
    
    T* allocate(size_t n) {
        size_t size = n * sizeof(T);
        return static_cast<T*>(MemoryTracker::allocate(size));
    }
    
    void deallocate(T* ptr, size_t n) {
        size_t size = n * sizeof(T);
        MemoryTracker::deallocate(ptr, size);
    }
    
    template<typename U>
    bool operator==(const TrackingAllocator<U>&) const { return true; }
    
    template<typename U>
    bool operator!=(const TrackingAllocator<U>&) const { return false; }
};

void demonstrateContainerOverhead() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Container Memory Overhead Analysis" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    const size_t numElements = 1000;
    
    std::cout << "Analyzing memory overhead for " << numElements << " integers:" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    
    // Vector
    {
        std::vector<int> vec;
        vec.reserve(numElements);
        for (size_t i = 0; i < numElements; ++i) {
            vec.push_back(static_cast<int>(i));
        }
        
        size_t dataSize = numElements * sizeof(int);
        size_t containerSize = vec.capacity() * sizeof(int);
        size_t overhead = containerSize - dataSize;
        
        std::cout << "Vector:" << std::endl;
        std::cout << "  Data size: " << dataSize << " bytes" << std::endl;
        std::cout << "  Container size: " << containerSize << " bytes" << std::endl;
        std::cout << "  Overhead: " << overhead << " bytes (" 
                  << std::fixed << std::setprecision(1) 
                  << (100.0 * overhead / dataSize) << "%)" << std::endl;
        std::cout << "  Elements: " << vec.size() << ", Capacity: " << vec.capacity() << std::endl;
    }
    
    // List
    {
        std::list<int> lst;
        for (size_t i = 0; i < numElements; ++i) {
            lst.push_back(static_cast<int>(i));
        }
        
        size_t dataSize = numElements * sizeof(int);
        // Each list node typically contains: data + prev pointer + next pointer
        size_t nodeSize = sizeof(int) + 2 * sizeof(void*);
        size_t containerSize = numElements * nodeSize;
        size_t overhead = containerSize - dataSize;
        
        std::cout << "\nList:" << std::endl;
        std::cout << "  Data size: " << dataSize << " bytes" << std::endl;
        std::cout << "  Container size (estimated): " << containerSize << " bytes" << std::endl;
        std::cout << "  Overhead: " << overhead << " bytes (" 
                  << std::fixed << std::setprecision(1) 
                  << (100.0 * overhead / dataSize) << "%)" << std::endl;
        std::cout << "  Node size: " << nodeSize << " bytes per element" << std::endl;
    }
    
    // Array (stack-allocated)
    {
        // Can't create variable-sized array, so we'll simulate
        size_t dataSize = numElements * sizeof(int);
        size_t containerSize = dataSize; // No overhead for std::array
        
        std::cout << "\nArray (std::array):" << std::endl;
        std::cout << "  Data size: " << dataSize << " bytes" << std::endl;
        std::cout << "  Container size: " << containerSize << " bytes" << std::endl;
        std::cout << "  Overhead: 0 bytes (0%)" << std::endl;
        std::cout << "  Note: Stack-allocated, compile-time size" << std::endl;
    }
    
    // String vs vector<char>
    {
        std::string str(numElements, 'A');
        std::vector<char> charVec(numElements, 'A');
        
        std::cout << "\nString vs Vector<char>:" << std::endl;
        std::cout << "  String size: " << str.size() << ", capacity: " << str.capacity() << std::endl;
        std::cout << "  Vector size: " << charVec.size() << ", capacity: " << charVec.capacity() << std::endl;
        std::cout << "  String may use Small String Optimization (SSO)" << std::endl;
    }
}

void demonstrateReserveVsResize() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Reserve vs Resize Strategies" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    const size_t targetSize = 100000;
    
    auto measureTime = [](auto func) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count() / 1000.0; // Return milliseconds
    };
    
    // Strategy 1: No reserve (default growth)
    double noReserveTime = measureTime([&]() {
        std::vector<int> vec;
        for (size_t i = 0; i < targetSize; ++i) {
            vec.push_back(static_cast<int>(i));
        }
    });
    
    // Strategy 2: Reserve exact size
    double reserveTime = measureTime([&]() {
        std::vector<int> vec;
        vec.reserve(targetSize);
        for (size_t i = 0; i < targetSize; ++i) {
            vec.push_back(static_cast<int>(i));
        }
    });
    
    // Strategy 3: Resize and fill
    double resizeTime = measureTime([&]() {
        std::vector<int> vec(targetSize);
        for (size_t i = 0; i < targetSize; ++i) {
            vec[i] = static_cast<int>(i);
        }
    });
    
    // Strategy 4: Resize with value
    double resizeValueTime = measureTime([&]() {
        std::vector<int> vec(targetSize, 0);
        std::iota(vec.begin(), vec.end(), 0);
    });
    
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Performance comparison for " << targetSize << " elements:" << std::endl;
    std::cout << "  No reserve:     " << std::setw(8) << noReserveTime << " ms" << std::endl;
    std::cout << "  With reserve:   " << std::setw(8) << reserveTime << " ms" << std::endl;
    std::cout << "  Resize + fill:  " << std::setw(8) << resizeTime << " ms" << std::endl;
    std::cout << "  Resize + iota:  " << std::setw(8) << resizeValueTime << " ms" << std::endl;
    
    // Demonstrate capacity growth
    std::cout << "\nCapacity growth pattern (no reserve):" << std::endl;
    std::vector<int> growthVec;
    size_t lastCapacity = 0;
    for (size_t i = 0; i < 100; ++i) {
        growthVec.push_back(static_cast<int>(i));
        if (growthVec.capacity() != lastCapacity) {
            std::cout << "Size: " << std::setw(3) << growthVec.size() 
                      << ", Capacity: " << std::setw(3) << growthVec.capacity();
            if (lastCapacity > 0) {
                double growthFactor = static_cast<double>(growthVec.capacity()) / lastCapacity;
                std::cout << " (growth factor: " << std::setprecision(2) << growthFactor << ")";
            }
            std::cout << std::endl;
            lastCapacity = growthVec.capacity();
        }
    }
}

// Compact data structure example
struct CompactPerson {
    char name[16];      // Fixed-size name
    uint16_t age;       // 16-bit age (0-65535)
    uint32_t salary;    // 32-bit salary
    uint8_t department; // Department ID (0-255)
    
    CompactPerson(const std::string& n, uint16_t a, uint32_t s, uint8_t d)
        : age(a), salary(s), department(d) {
        std::strncpy(name, n.c_str(), sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
    }
};

struct RegularPerson {
    std::string name;
    int age;
    double salary;
    std::string department;
    
    RegularPerson(const std::string& n, int a, double s, const std::string& d)
        : name(n), age(a), salary(s), department(d) {}
};

void demonstrateCompactDataStructures() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Compact Data Structures" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    const size_t numPeople = 10000;
    
    std::cout << "Size comparison:" << std::endl;
    std::cout << "  CompactPerson: " << sizeof(CompactPerson) << " bytes" << std::endl;
    std::cout << "  RegularPerson: " << sizeof(RegularPerson) << " bytes" << std::endl;
    std::cout << "  Memory ratio: " << std::fixed << std::setprecision(2) 
              << static_cast<double>(sizeof(RegularPerson)) / sizeof(CompactPerson) << "x" << std::endl;
    
    // Memory usage comparison
    std::vector<CompactPerson> compactPeople;
    std::vector<RegularPerson> regularPeople;
    
    compactPeople.reserve(numPeople);
    regularPeople.reserve(numPeople);
    
    for (size_t i = 0; i < numPeople; ++i) {
        std::string name = "Person" + std::to_string(i);
        compactPeople.emplace_back(name, static_cast<uint16_t>(20 + i % 50), 
                                  static_cast<uint32_t>(50000 + i % 50000), 
                                  static_cast<uint8_t>(i % 4));
        regularPeople.emplace_back(name, static_cast<int>(20 + i % 50), 
                                  static_cast<double>(50000 + i % 50000), 
                                  "Dept" + std::to_string(i % 4));
    }
    
    size_t compactMemory = compactPeople.capacity() * sizeof(CompactPerson);
    size_t regularMemory = regularPeople.capacity() * sizeof(RegularPerson);
    
    std::cout << "\nMemory usage for " << numPeople << " people:" << std::endl;
    std::cout << "  Compact: " << compactMemory << " bytes" << std::endl;
    std::cout << "  Regular: " << regularMemory << " bytes (+ string heap allocations)" << std::endl;
    std::cout << "  Savings: " << regularMemory - compactMemory << " bytes (" 
              << std::fixed << std::setprecision(1)
              << (100.0 * (regularMemory - compactMemory) / regularMemory) << "%)" << std::endl;
    
    // Performance comparison
    auto measureTime = [](auto func) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count() / 1000.0;
    };
    
    double compactTime = measureTime([&]() {
        volatile uint64_t sum = 0;
        for (const auto& person : compactPeople) {
            sum += person.salary;
        }
    });
    
    double regularTime = measureTime([&]() {
        volatile double sum = 0;
        for (const auto& person : regularPeople) {
            sum += person.salary;
        }
    });
    
    std::cout << "\nIteration performance:" << std::endl;
    std::cout << "  Compact: " << std::setprecision(3) << compactTime << " ms" << std::endl;
    std::cout << "  Regular: " << std::setprecision(3) << regularTime << " ms" << std::endl;
    std::cout << "  Speedup: " << std::setprecision(2) << regularTime / compactTime << "x" << std::endl;
}

// Memory pool implementation
template<typename T, size_t BlockSize = 1024>
class MemoryPool {
private:
    struct Block {
        alignas(T) char data[BlockSize * sizeof(T)];
        Block* next;
        size_t used;
        
        Block() : next(nullptr), used(0) {}
    };
    
    Block* currentBlock;
    std::vector<std::unique_ptr<Block>> blocks;
    
public:
    MemoryPool() : currentBlock(nullptr) {
        allocateNewBlock();
    }
    
    ~MemoryPool() = default;
    
    T* allocate() {
        if (!currentBlock || currentBlock->used >= BlockSize) {
            allocateNewBlock();
        }
        
        T* ptr = reinterpret_cast<T*>(currentBlock->data + currentBlock->used * sizeof(T));
        ++currentBlock->used;
        return ptr;
    }
    
    void deallocate(T* ptr) {
        // Simple pool - no individual deallocation
        // All memory is freed when pool is destroyed
    }
    
    size_t getTotalAllocatedBlocks() const {
        return blocks.size();
    }
    
    size_t getTotalMemoryUsage() const {
        return blocks.size() * sizeof(Block);
    }
    
private:
    void allocateNewBlock() {
        blocks.push_back(std::make_unique<Block>());
        currentBlock = blocks.back().get();
    }
};

void demonstrateMemoryPool() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Memory Pool Demonstration" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    const size_t numAllocations = 100000;
    
    auto measureTime = [](auto func) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count() / 1000.0;
    };
    
    // Standard allocation
    double standardTime = measureTime([&]() {
        std::vector<int*> ptrs;
        ptrs.reserve(numAllocations);
        
        for (size_t i = 0; i < numAllocations; ++i) {
            ptrs.push_back(new int(static_cast<int>(i)));
        }
        
        for (int* ptr : ptrs) {
            delete ptr;
        }
    });
    
    // Memory pool allocation
    double poolTime = measureTime([&]() {
        MemoryPool<int> pool;
        std::vector<int*> ptrs;
        ptrs.reserve(numAllocations);
        
        for (size_t i = 0; i < numAllocations; ++i) {
            int* ptr = pool.allocate();
            *ptr = static_cast<int>(i);
            ptrs.push_back(ptr);
        }
        
        // No individual deallocation needed - pool handles it
    });
    
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Allocation performance for " << numAllocations << " integers:" << std::endl;
    std::cout << "  Standard new/delete: " << standardTime << " ms" << std::endl;
    std::cout << "  Memory pool:         " << poolTime << " ms" << std::endl;
    std::cout << "  Speedup:             " << std::setprecision(2) << standardTime / poolTime << "x" << std::endl;
    
    // Demonstrate pool memory usage
    MemoryPool<int, 100> smallPool; // 100 ints per block
    
    std::cout << "\nMemory pool block allocation:" << std::endl;
    for (size_t i = 0; i < 250; ++i) {
        smallPool.allocate();
        if (i % 50 == 49) {
            std::cout << "After " << (i + 1) << " allocations: " 
                      << smallPool.getTotalAllocatedBlocks() << " blocks, "
                      << smallPool.getTotalMemoryUsage() << " bytes" << std::endl;
        }
    }
}

void demonstrateCacheFriendlyAccess() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Cache-Friendly Data Access Patterns" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    const size_t size = 10000;
    const size_t iterations = 1000;
    
    // Array of structures (AoS)
    struct AoS {
        int x, y, z;
        double value;
    };
    
    std::vector<AoS> aos(size);
    for (size_t i = 0; i < size; ++i) {
        aos[i] = {static_cast<int>(i), static_cast<int>(i * 2), static_cast<int>(i * 3), static_cast<double>(i)};
    }
    
    // Structure of arrays (SoA)
    struct SoA {
        std::vector<int> x, y, z;
        std::vector<double> value;
        
        SoA(size_t n) : x(n), y(n), z(n), value(n) {}
    };
    
    SoA soa(size);
    for (size_t i = 0; i < size; ++i) {
        soa.x[i] = static_cast<int>(i);
        soa.y[i] = static_cast<int>(i * 2);
        soa.z[i] = static_cast<int>(i * 3);
        soa.value[i] = static_cast<double>(i);
    }
    
    auto measureTime = [](auto func) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count() / 1000.0;
    };
    
    // Test: Sum only the 'value' field (cache-friendly for SoA)
    double aosTime = measureTime([&]() {
        for (size_t iter = 0; iter < iterations; ++iter) {
            volatile double sum = 0;
            for (const auto& item : aos) {
                sum += item.value;
            }
        }
    });
    
    double soaTime = measureTime([&]() {
        for (size_t iter = 0; iter < iterations; ++iter) {
            volatile double sum = 0;
            for (double val : soa.value) {
                sum += val;
            }
        }
    });
    
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Summing 'value' field (" << iterations << " iterations):" << std::endl;
    std::cout << "  Array of Structures (AoS): " << aosTime << " ms" << std::endl;
    std::cout << "  Structure of Arrays (SoA): " << soaTime << " ms" << std::endl;
    std::cout << "  SoA speedup: " << std::setprecision(2) << aosTime / soaTime << "x" << std::endl;
    
    // Test: Access all fields (cache-friendly for AoS)
    double aosAllTime = measureTime([&]() {
        for (size_t iter = 0; iter < iterations; ++iter) {
            volatile double sum = 0;
            for (const auto& item : aos) {
                sum += item.x + item.y + item.z + item.value;
            }
        }
    });
    
    double soaAllTime = measureTime([&]() {
        for (size_t iter = 0; iter < iterations; ++iter) {
            volatile double sum = 0;
            for (size_t i = 0; i < size; ++i) {
                sum += soa.x[i] + soa.y[i] + soa.z[i] + soa.value[i];
            }
        }
    });
    
    std::cout << "\nAccessing all fields (" << iterations << " iterations):" << std::endl;
    std::cout << "  Array of Structures (AoS): " << aosAllTime << " ms" << std::endl;
    std::cout << "  Structure of Arrays (SoA): " << soaAllTime << " ms" << std::endl;
    std::cout << "  AoS speedup: " << std::setprecision(2) << soaAllTime / aosAllTime << "x" << std::endl;
}

int main() {
    std::cout << "===== Memory-Efficient Container Usage =====" << std::endl;
    std::cout << "This program demonstrates techniques for optimizing memory usage" << std::endl;
    std::cout << "with STL containers and custom data structures." << std::endl;
    
    demonstrateContainerOverhead();
    demonstrateReserveVsResize();
    demonstrateCompactDataStructures();
    demonstrateMemoryPool();
    demonstrateCacheFriendlyAccess();
    
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "Memory Optimization Guidelines:" << std::endl;
    std::cout << "1. Use reserve() when you know the final size" << std::endl;
    std::cout << "2. Choose containers based on access patterns" << std::endl;
    std::cout << "3. Consider compact data structures for large datasets" << std::endl;
    std::cout << "4. Use memory pools for frequent small allocations" << std::endl;
    std::cout << "5. Structure data for cache-friendly access patterns" << std::endl;
    std::cout << "6. Measure actual memory usage and performance" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    return 0;
}
