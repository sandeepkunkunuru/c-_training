#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <future>
#include <atomic>
#include <random>
#include <iomanip>

// Helper function to print a line separator
void printSeparator(const std::string& title) {
    std::cout << "\n--- " << title << " ---" << std::endl;
}

// Helper function to measure execution time
template<typename Func>
long long measureExecutionTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

// Function to generate random data
std::vector<int> generateRandomData(size_t size) {
    std::vector<int> data(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 1000000);
    
    std::generate(data.begin(), data.end(), [&]() { return distrib(gen); });
    return data;
}

// Function to demonstrate algorithm performance
void demonstrateAlgorithms() {
    printSeparator("Algorithm Performance");
    
    // Generate random data
    const size_t dataSize = 1000000; // Reduced size for faster execution
    std::cout << "Generating " << dataSize << " random integers..." << std::endl;
    auto data = generateRandomData(dataSize);
    auto data2 = data;  // Copy for comparison
    
    // Standard sort
    long long sortTime = measureExecutionTime([&]() {
        std::sort(data.begin(), data.end());
    });
    std::cout << "Standard sort time: " << sortTime << " microseconds" << std::endl;
    
    // Test other algorithms
    std::cout << "\nTesting other algorithms..." << std::endl;
    
    // std::for_each
    long long forEachTime = measureExecutionTime([&]() {
        std::for_each(data.begin(), data.end(), [](int& x) { x = x * 2; });
    });
    std::cout << "for_each time: " << forEachTime << " microseconds" << std::endl;
    
    // std::transform
    std::vector<int> result(dataSize);
    
    long long transformTime = measureExecutionTime([&]() {
        std::transform(data.begin(), data.end(), result.begin(), 
                      [](int x) { return x * x; });
    });
    std::cout << "transform time: " << transformTime << " microseconds" << std::endl;
    
    // std::accumulate
    long long accumulateTime = measureExecutionTime([&]() {
        auto sum = std::accumulate(data.begin(), data.end(), 0);
        std::cout << "Accumulate result: " << sum << std::endl;
    });
    std::cout << "accumulate time: " << accumulateTime << " microseconds" << std::endl;
}

// Thread-safe counter using mutex
class MutexCounter {
private:
    std::mutex mutex;
    int value = 0;

public:
    void increment() {
        std::lock_guard<std::mutex> lock(mutex);
        ++value;
    }
    
    int get() const {
        std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(mutex));
        return value;
    }
};

// Thread-safe counter using atomic
class AtomicCounter {
private:
    std::atomic<int> value{0};

public:
    void increment() {
        ++value;
    }
    
    int get() const {
        return value.load();
    }
};

// Function to demonstrate concurrency features
void demonstrateConcurrency() {
    printSeparator("Modern Concurrency Features");
    
    // Compare mutex vs atomic performance
    const int numIncrements = 1000000;
    const int numThreads = 10;
    
    MutexCounter mutexCounter;
    AtomicCounter atomicCounter;
    
    std::cout << "Comparing mutex vs atomic for " << numIncrements 
              << " increments across " << numThreads << " threads..." << std::endl;
    
    // Test mutex counter
    long long mutexTime = measureExecutionTime([&]() {
        std::vector<std::thread> threads;
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([&mutexCounter, numIncrements]() {
                for (int j = 0; j < numIncrements / 10; ++j) {
                    mutexCounter.increment();
                }
            });
        }
        
        for (auto& t : threads) {
            t.join();
        }
    });
    
    // Test atomic counter
    long long atomicTime = measureExecutionTime([&]() {
        std::vector<std::thread> threads;
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([&atomicCounter, numIncrements]() {
                for (int j = 0; j < numIncrements / 10; ++j) {
                    atomicCounter.increment();
                }
            });
        }
        
        for (auto& t : threads) {
            t.join();
        }
    });
    
    std::cout << "Mutex counter result: " << mutexCounter.get() << std::endl;
    std::cout << "Atomic counter result: " << atomicCounter.get() << std::endl;
    std::cout << "Mutex time: " << mutexTime << " microseconds" << std::endl;
    std::cout << "Atomic time: " << atomicTime << " microseconds" << std::endl;
    std::cout << "Speedup (atomic vs mutex): " << static_cast<double>(mutexTime) / atomicTime << "x" << std::endl;
    
    // Demonstrate std::future and std::async
    printSeparator("std::future and std::async");
    
    auto futureResult = std::async(std::launch::async, []() {
        std::cout << "Async task running in thread: " 
                  << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return 42;
    });
    
    std::cout << "Main thread: " << std::this_thread::get_id() 
              << " waiting for future..." << std::endl;
    
    // Do some work while waiting
    std::cout << "Doing some work in main thread..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Get the result (will wait if not ready)
    int result = futureResult.get();
    std::cout << "Future result: " << result << std::endl;
    
    // Demonstrate shared_mutex (reader-writer lock)
    printSeparator("std::shared_mutex (C++17)");
    
    std::shared_mutex sharedMutex;
    std::vector<int> sharedData(10, 0);
    
    // Writer thread
    std::thread writerThread([&]() {
        for (int i = 0; i < 5; ++i) {
            std::unique_lock<std::shared_mutex> lock(sharedMutex);
            std::cout << "Writer thread " << std::this_thread::get_id() 
                      << " acquired exclusive lock" << std::endl;
            
            // Simulate write operation
            for (size_t j = 0; j < sharedData.size(); ++j) {
                sharedData[j] = i + 1;
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            
            std::cout << "Writer updated data to: ";
            for (int val : sharedData) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
            
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    });
    
    // Reader threads
    std::vector<std::thread> readerThreads;
    for (int i = 0; i < 3; ++i) {
        readerThreads.emplace_back([&, i]() {
            for (int j = 0; j < 10; ++j) {
                std::shared_lock<std::shared_mutex> lock(sharedMutex);
                std::cout << "Reader " << i << " (thread " << std::this_thread::get_id() 
                          << ") acquired shared lock" << std::endl;
                
                // Simulate read operation
                int sum = 0;
                for (int val : sharedData) {
                    sum += val;
                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
                }
                
                std::cout << "Reader " << i << " calculated sum: " << sum << std::endl;
                
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
    }
    
    writerThread.join();
    for (auto& t : readerThreads) {
        t.join();
    }
    
    // Demonstrate scoped_lock (C++17)
    printSeparator("std::scoped_lock (C++17)");
    
    std::mutex mutex1, mutex2, mutex3;
    
    auto deadlockProne = [&](int id) {
        if (id == 1) {
            std::lock_guard<std::mutex> lock1(mutex1);
            std::cout << "Thread " << id << " acquired mutex1" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::lock_guard<std::mutex> lock2(mutex2);
            std::cout << "Thread " << id << " acquired mutex2" << std::endl;
        } else {
            std::lock_guard<std::mutex> lock2(mutex2);
            std::cout << "Thread " << id << " acquired mutex2" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::lock_guard<std::mutex> lock1(mutex1);
            std::cout << "Thread " << id << " acquired mutex1" << std::endl;
        }
    };
    
    auto deadlockFree = [&](int id) {
        std::scoped_lock lock(mutex1, mutex2, mutex3);  // Locks all mutexes at once
        std::cout << "Thread " << id << " acquired all mutexes at once" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    };
    
    std::cout << "Demonstrating std::scoped_lock to prevent deadlock..." << std::endl;
    
    std::vector<std::thread> scopedLockThreads;
    for (int i = 0; i < 5; ++i) {
        scopedLockThreads.emplace_back(deadlockFree, i);
    }
    
    for (auto& t : scopedLockThreads) {
        t.join();
    }
    
    std::cout << "All threads completed without deadlock!" << std::endl;
}

int main() {
    std::cout << "===== Modern C++ Part 5: Concurrency and Parallel Algorithms =====" << std::endl;
    
    demonstrateAlgorithms();
    demonstrateConcurrency();
    
    return 0;
}
