/*
 * Container Performance Analysis
 * 
 * This program demonstrates the performance characteristics of different STL containers
 * and helps understand when to use each container type based on specific use cases.
 * 
 * Key Learning Points:
 * - Vector: Fast random access, slow insertion/deletion in middle
 * - List: Fast insertion/deletion anywhere, no random access
 * - Deque: Fast insertion/deletion at ends, slower random access than vector
 * - Set/Map: Logarithmic operations, ordered data
 * - Unordered_set/map: Average constant time operations, unordered data
 */

#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>

class PerformanceTester {
private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;
    
public:
    PerformanceTester() : gen(rd()), dis(1, 100000) {}
    
    template<typename Func>
    double measureTime(Func&& func) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count() / 1000.0; // Return milliseconds
    }
    
    void printHeader(const std::string& title) {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "  " << title << std::endl;
        std::cout << std::string(60, '=') << std::endl;
    }
    
    void testSequentialContainers() {
        printHeader("Sequential Container Performance Comparison");
        
        const size_t size = 100000;
        const size_t iterations = 1000;
        
        std::cout << std::fixed << std::setprecision(3);
        std::cout << std::setw(20) << "Operation" 
                  << std::setw(12) << "Vector" 
                  << std::setw(12) << "List" 
                  << std::setw(12) << "Deque" << std::endl;
        std::cout << std::string(56, '-') << std::endl;
        
        // Test 1: Back insertion
        std::vector<int> vec;
        std::list<int> lst;
        std::deque<int> deq;
        
        double vecTime = measureTime([&]() {
            vec.clear();
            vec.reserve(size); // Pre-allocate for fair comparison
            for (size_t i = 0; i < size; ++i) {
                vec.push_back(dis(gen));
            }
        });
        
        double lstTime = measureTime([&]() {
            lst.clear();
            for (size_t i = 0; i < size; ++i) {
                lst.push_back(dis(gen));
            }
        });
        
        double deqTime = measureTime([&]() {
            deq.clear();
            for (size_t i = 0; i < size; ++i) {
                deq.push_back(dis(gen));
            }
        });
        
        std::cout << std::setw(20) << "Back Insertion" 
                  << std::setw(12) << vecTime 
                  << std::setw(12) << lstTime 
                  << std::setw(12) << deqTime << " ms" << std::endl;
        
        // Test 2: Random access (only vector and deque)
        vecTime = measureTime([&]() {
            volatile int sum = 0;
            for (size_t i = 0; i < iterations; ++i) {
                size_t idx = dis(gen) % vec.size();
                sum += vec[idx];
            }
        });
        
        deqTime = measureTime([&]() {
            volatile int sum = 0;
            for (size_t i = 0; i < iterations; ++i) {
                size_t idx = dis(gen) % deq.size();
                sum += deq[idx];
            }
        });
        
        std::cout << std::setw(20) << "Random Access" 
                  << std::setw(12) << vecTime 
                  << std::setw(12) << "N/A" 
                  << std::setw(12) << deqTime << " ms" << std::endl;
        
        // Test 3: Middle insertion
        vecTime = measureTime([&]() {
            for (size_t i = 0; i < iterations / 10; ++i) {
                auto it = vec.begin() + vec.size() / 2;
                vec.insert(it, dis(gen));
            }
        });
        
        lstTime = measureTime([&]() {
            for (size_t i = 0; i < iterations / 10; ++i) {
                auto it = lst.begin();
                std::advance(it, lst.size() / 2);
                lst.insert(it, dis(gen));
            }
        });
        
        deqTime = measureTime([&]() {
            for (size_t i = 0; i < iterations / 10; ++i) {
                auto it = deq.begin() + deq.size() / 2;
                deq.insert(it, dis(gen));
            }
        });
        
        std::cout << std::setw(20) << "Middle Insertion" 
                  << std::setw(12) << vecTime 
                  << std::setw(12) << lstTime 
                  << std::setw(12) << deqTime << " ms" << std::endl;
        
        // Test 4: Sequential iteration
        vecTime = measureTime([&]() {
            volatile long long sum = 0;
            for (const auto& val : vec) {
                sum += val;
            }
        });
        
        lstTime = measureTime([&]() {
            volatile long long sum = 0;
            for (const auto& val : lst) {
                sum += val;
            }
        });
        
        deqTime = measureTime([&]() {
            volatile long long sum = 0;
            for (const auto& val : deq) {
                sum += val;
            }
        });
        
        std::cout << std::setw(20) << "Sequential Iteration" 
                  << std::setw(12) << vecTime 
                  << std::setw(12) << lstTime 
                  << std::setw(12) << deqTime << " ms" << std::endl;
    }
    
    void testAssociativeContainers() {
        printHeader("Associative Container Performance Comparison");
        
        const size_t size = 50000;
        const size_t lookups = 10000;
        
        std::cout << std::fixed << std::setprecision(3);
        std::cout << std::setw(20) << "Operation" 
                  << std::setw(15) << "std::set" 
                  << std::setw(15) << "std::unordered_set" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        
        std::set<int> orderedSet;
        std::unordered_set<int> unorderedSet;
        
        // Test 1: Insertion
        double setTime = measureTime([&]() {
            orderedSet.clear();
            for (size_t i = 0; i < size; ++i) {
                orderedSet.insert(dis(gen));
            }
        });
        
        double unorderedSetTime = measureTime([&]() {
            unorderedSet.clear();
            unorderedSet.reserve(size);
            for (size_t i = 0; i < size; ++i) {
                unorderedSet.insert(dis(gen));
            }
        });
        
        std::cout << std::setw(20) << "Insertion" 
                  << std::setw(15) << setTime 
                  << std::setw(15) << unorderedSetTime << " ms" << std::endl;
        
        // Test 2: Lookup
        std::vector<int> lookupValues;
        for (size_t i = 0; i < lookups; ++i) {
            lookupValues.push_back(dis(gen));
        }
        
        setTime = measureTime([&]() {
            volatile size_t found = 0;
            for (int val : lookupValues) {
                if (orderedSet.find(val) != orderedSet.end()) {
                    ++found;
                }
            }
        });
        
        unorderedSetTime = measureTime([&]() {
            volatile size_t found = 0;
            for (int val : lookupValues) {
                if (unorderedSet.find(val) != unorderedSet.end()) {
                    ++found;
                }
            }
        });
        
        std::cout << std::setw(20) << "Lookup" 
                  << std::setw(15) << setTime 
                  << std::setw(15) << unorderedSetTime << " ms" << std::endl;
        
        // Test 3: Iteration (ordered vs unordered)
        setTime = measureTime([&]() {
            volatile long long sum = 0;
            for (const auto& val : orderedSet) {
                sum += val;
            }
        });
        
        unorderedSetTime = measureTime([&]() {
            volatile long long sum = 0;
            for (const auto& val : unorderedSet) {
                sum += val;
            }
        });
        
        std::cout << std::setw(20) << "Iteration" 
                  << std::setw(15) << setTime 
                  << std::setw(15) << unorderedSetTime << " ms" << std::endl;
    }
    
    void testMapPerformance() {
        printHeader("Map Performance Comparison");
        
        const size_t size = 50000;
        const size_t operations = 10000;
        
        std::cout << std::fixed << std::setprecision(3);
        std::cout << std::setw(20) << "Operation" 
                  << std::setw(15) << "std::map" 
                  << std::setw(15) << "std::unordered_map" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        
        std::map<int, std::string> orderedMap;
        std::unordered_map<int, std::string> unorderedMap;
        
        // Generate test data
        std::vector<std::pair<int, std::string>> testData;
        for (size_t i = 0; i < size; ++i) {
            testData.emplace_back(dis(gen), "value_" + std::to_string(i));
        }
        
        // Test 1: Insertion
        double mapTime = measureTime([&]() {
            orderedMap.clear();
            for (const auto& pair : testData) {
                orderedMap[pair.first] = pair.second;
            }
        });
        
        double unorderedMapTime = measureTime([&]() {
            unorderedMap.clear();
            unorderedMap.reserve(size);
            for (const auto& pair : testData) {
                unorderedMap[pair.first] = pair.second;
            }
        });
        
        std::cout << std::setw(20) << "Insertion" 
                  << std::setw(15) << mapTime 
                  << std::setw(15) << unorderedMapTime << " ms" << std::endl;
        
        // Test 2: Lookup
        std::vector<int> keys;
        for (size_t i = 0; i < operations; ++i) {
            keys.push_back(dis(gen));
        }
        
        mapTime = measureTime([&]() {
            volatile size_t found = 0;
            for (int key : keys) {
                if (orderedMap.find(key) != orderedMap.end()) {
                    ++found;
                }
            }
        });
        
        unorderedMapTime = measureTime([&]() {
            volatile size_t found = 0;
            for (int key : keys) {
                if (unorderedMap.find(key) != unorderedMap.end()) {
                    ++found;
                }
            }
        });
        
        std::cout << std::setw(20) << "Lookup" 
                  << std::setw(15) << mapTime 
                  << std::setw(15) << unorderedMapTime << " ms" << std::endl;
        
        // Test 3: Range iteration
        mapTime = measureTime([&]() {
            volatile size_t count = 0;
            for (const auto& pair : orderedMap) {
                if (pair.first > 50000) {
                    ++count;
                }
            }
        });
        
        unorderedMapTime = measureTime([&]() {
            volatile size_t count = 0;
            for (const auto& pair : unorderedMap) {
                if (pair.first > 50000) {
                    ++count;
                }
            }
        });
        
        std::cout << std::setw(20) << "Range Iteration" 
                  << std::setw(15) << mapTime 
                  << std::setw(15) << unorderedMapTime << " ms" << std::endl;
    }
    
    void demonstrateMemoryUsage() {
        printHeader("Memory Usage Considerations");
        
        const size_t size = 100000;
        
        // Vector memory usage
        std::vector<int> vec;
        std::cout << "Vector capacity growth pattern:" << std::endl;
        for (size_t i = 0; i < 20; ++i) {
            vec.push_back(i);
            if (i < 10 || i % 5 == 0) {
                std::cout << "Size: " << std::setw(2) << vec.size() 
                          << ", Capacity: " << std::setw(2) << vec.capacity() 
                          << ", Memory: " << vec.capacity() * sizeof(int) << " bytes" << std::endl;
            }
        }
        
        std::cout << "\nMemory efficiency comparison for " << size << " integers:" << std::endl;
        
        // Vector
        std::vector<int> testVec;
        testVec.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            testVec.push_back(i);
        }
        std::cout << "Vector: " << testVec.capacity() * sizeof(int) << " bytes" << std::endl;
        
        // List (approximate)
        std::list<int> testList;
        for (size_t i = 0; i < size; ++i) {
            testList.push_back(i);
        }
        // Each list node typically has: data + 2 pointers
        size_t listMemory = size * (sizeof(int) + 2 * sizeof(void*));
        std::cout << "List (estimated): " << listMemory << " bytes" << std::endl;
        
        // Deque (approximate)
        std::deque<int> testDeque;
        for (size_t i = 0; i < size; ++i) {
            testDeque.push_back(i);
        }
        std::cout << "Deque: Similar to vector but with some overhead for block structure" << std::endl;
        
        std::cout << "\nMemory overhead ratio (List/Vector): " 
                  << static_cast<double>(listMemory) / (testVec.capacity() * sizeof(int)) 
                  << "x" << std::endl;
    }
};

void demonstrateContainerChoice() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Container Selection Guidelines" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << "\n1. Use std::vector when:" << std::endl;
    std::cout << "   - You need random access to elements" << std::endl;
    std::cout << "   - You primarily add/remove elements at the end" << std::endl;
    std::cout << "   - Memory efficiency is important" << std::endl;
    std::cout << "   - Cache performance matters" << std::endl;
    
    std::cout << "\n2. Use std::list when:" << std::endl;
    std::cout << "   - You frequently insert/delete in the middle" << std::endl;
    std::cout << "   - You don't need random access" << std::endl;
    std::cout << "   - Element stability is required (iterators don't invalidate)" << std::endl;
    
    std::cout << "\n3. Use std::deque when:" << std::endl;
    std::cout << "   - You need to insert/remove at both ends efficiently" << std::endl;
    std::cout << "   - You need random access but also front insertion" << std::endl;
    
    std::cout << "\n4. Use std::set/map when:" << std::endl;
    std::cout << "   - You need sorted data" << std::endl;
    std::cout << "   - You need guaranteed logarithmic operations" << std::endl;
    std::cout << "   - You need range queries" << std::endl;
    
    std::cout << "\n5. Use std::unordered_set/map when:" << std::endl;
    std::cout << "   - You need fast average-case lookups" << std::endl;
    std::cout << "   - Order doesn't matter" << std::endl;
    std::cout << "   - You have a good hash function" << std::endl;
}

int main() {
    std::cout << "===== STL Container Performance Analysis =====" << std::endl;
    std::cout << "This program analyzes the performance characteristics of different STL containers." << std::endl;
    std::cout << "Understanding these differences is crucial for writing efficient C++ code." << std::endl;
    
    PerformanceTester tester;
    
    tester.testSequentialContainers();
    tester.testAssociativeContainers();
    tester.testMapPerformance();
    tester.demonstrateMemoryUsage();
    
    demonstrateContainerChoice();
    
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "Performance testing complete!" << std::endl;
    std::cout << "Note: Results may vary based on compiler optimizations and hardware." << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    return 0;
}
