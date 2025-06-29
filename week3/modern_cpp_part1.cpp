#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <utility>
#include <algorithm>
#include <functional>

// Helper function to measure execution time
template<typename Func>
long long measureExecutionTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

// A simple class to demonstrate move semantics
class Resource {
private:
    std::string name;
    int* data;
    size_t size;

public:
    // Constructor
    Resource(const std::string& n, size_t s) 
        : name(n), size(s) {
        std::cout << "Constructor: " << name << " with size " << size << std::endl;
        data = new int[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = static_cast<int>(i);
        }
    }

    // Copy constructor
    Resource(const Resource& other) 
        : name(other.name), size(other.size) {
        std::cout << "Copy constructor: " << name << std::endl;
        data = new int[size];
        std::copy(other.data, other.data + size, data);
    }

    // Move constructor
    Resource(Resource&& other) noexcept 
        : name(std::move(other.name)), data(other.data), size(other.size) {
        std::cout << "Move constructor: " << name << std::endl;
        other.data = nullptr;
        other.size = 0;
    }

    // Copy assignment operator
    Resource& operator=(const Resource& other) {
        std::cout << "Copy assignment: " << other.name << std::endl;
        if (this != &other) {
            delete[] data;
            name = other.name;
            size = other.size;
            data = new int[size];
            std::copy(other.data, other.data + size, data);
        }
        return *this;
    }

    // Move assignment operator
    Resource& operator=(Resource&& other) noexcept {
        std::cout << "Move assignment: " << other.name << std::endl;
        if (this != &other) {
            delete[] data;
            name = std::move(other.name);
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }

    // Destructor
    ~Resource() {
        std::cout << "Destructor: " << name << std::endl;
        delete[] data;
    }

    // Utility methods
    void display() const {
        std::cout << "Resource " << name << " with size " << size << std::endl;
        if (data) {
            std::cout << "First few elements: ";
            for (size_t i = 0; i < std::min(size, size_t(5)); ++i) {
                std::cout << data[i] << " ";
            }
            std::cout << std::endl;
        } else {
            std::cout << "Data is null (moved from)" << std::endl;
        }
    }

    size_t getSize() const { return size; }
    std::string getName() const { return name; }
};

// Function that returns by value (potential for RVO/NRVO)
Resource createResource(const std::string& name, size_t size) {
    Resource r(name, size);
    return r;  // NRVO candidate
}

// Function that takes an rvalue reference
void processResource(Resource&& res) {
    std::cout << "Processing rvalue resource: " << res.getName() << std::endl;
    // We need to move the rvalue reference to actually move it
    Resource newRes = std::move(res);
    newRes.display();
}

// Demonstrate smart pointers
void demonstrateSmartPointers() {
    std::cout << "\n===== Smart Pointers =====" << std::endl;

    // unique_ptr - exclusive ownership
    std::cout << "\n--- std::unique_ptr ---" << std::endl;
    {
        std::cout << "Creating unique_ptr" << std::endl;
        std::unique_ptr<Resource> uniqueRes = std::make_unique<Resource>("UniqueResource", 1000);
        uniqueRes->display();

        // Transfer ownership
        std::cout << "Transferring ownership" << std::endl;
        std::unique_ptr<Resource> newOwner = std::move(uniqueRes);
        
        if (uniqueRes) {
            std::cout << "Original still owns the resource (shouldn't happen)" << std::endl;
        } else {
            std::cout << "Original no longer owns the resource" << std::endl;
        }
        
        newOwner->display();
        
        // Custom deleter
        std::cout << "Using custom deleter" << std::endl;
        auto customDeleter = [](Resource* r) {
            std::cout << "Custom deleter called for " << r->getName() << std::endl;
            delete r;
        };
        
        std::unique_ptr<Resource, decltype(customDeleter)> customRes(
            new Resource("CustomDeletedResource", 500), customDeleter);
        customRes->display();
        
        // unique_ptr will automatically delete the resource when it goes out of scope
        std::cout << "Exiting unique_ptr scope" << std::endl;
    }
    std::cout << "After unique_ptr scope" << std::endl;

    // shared_ptr - shared ownership
    std::cout << "\n--- std::shared_ptr ---" << std::endl;
    {
        std::cout << "Creating shared_ptr" << std::endl;
        std::shared_ptr<Resource> sharedRes1 = std::make_shared<Resource>("SharedResource", 1000);
        std::cout << "Use count: " << sharedRes1.use_count() << std::endl;
        
        {
            std::cout << "Creating second shared_ptr" << std::endl;
            std::shared_ptr<Resource> sharedRes2 = sharedRes1;
            std::cout << "Use count: " << sharedRes1.use_count() << std::endl;
            
            std::cout << "Creating third shared_ptr" << std::endl;
            std::shared_ptr<Resource> sharedRes3 = sharedRes1;
            std::cout << "Use count: " << sharedRes1.use_count() << std::endl;
            
            sharedRes3.reset();  // Explicitly release ownership
            std::cout << "After reset - Use count: " << sharedRes1.use_count() << std::endl;
            
            std::cout << "Exiting inner scope" << std::endl;
        }
        
        std::cout << "After inner scope - Use count: " << sharedRes1.use_count() << std::endl;
        sharedRes1->display();
        
        // Custom deleter with shared_ptr
        std::cout << "Using shared_ptr with custom deleter" << std::endl;
        std::shared_ptr<Resource> customSharedRes(new Resource("CustomSharedResource", 500), 
            [](Resource* r) {
                std::cout << "Custom shared_ptr deleter for " << r->getName() << std::endl;
                delete r;
            });
        customSharedRes->display();
        
        std::cout << "Exiting shared_ptr scope" << std::endl;
    }
    std::cout << "After shared_ptr scope" << std::endl;

    // weak_ptr - non-owning reference
    std::cout << "\n--- std::weak_ptr ---" << std::endl;
    {
        std::shared_ptr<Resource> sharedRes = std::make_shared<Resource>("WeakResource", 1000);
        std::weak_ptr<Resource> weakRes = sharedRes;
        
        std::cout << "shared_ptr use count: " << sharedRes.use_count() << std::endl;
        std::cout << "weak_ptr expired? " << weakRes.expired() << std::endl;
        
        // Use the weak_ptr by converting to shared_ptr
        if (auto tempShared = weakRes.lock()) {
            std::cout << "Successfully locked weak_ptr" << std::endl;
            tempShared->display();
            std::cout << "shared_ptr use count during lock: " << sharedRes.use_count() << std::endl;
        }
        
        // Reset the original shared_ptr
        sharedRes.reset();
        std::cout << "After resetting shared_ptr" << std::endl;
        std::cout << "weak_ptr expired? " << weakRes.expired() << std::endl;
        
        // Try to use the weak_ptr again
        if (auto tempShared = weakRes.lock()) {
            std::cout << "Successfully locked weak_ptr (shouldn't happen)" << std::endl;
        } else {
            std::cout << "Failed to lock weak_ptr - resource is gone" << std::endl;
        }
    }
}

// Demonstrate move semantics
void demonstrateMoveSemantics() {
    std::cout << "\n===== Move Semantics =====" << std::endl;

    // Basic move construction
    std::cout << "\n--- Basic Move Construction ---" << std::endl;
    Resource original("Original", 1000);
    original.display();
    
    std::cout << "Moving resource..." << std::endl;
    Resource moved = std::move(original);
    
    std::cout << "After move:" << std::endl;
    original.display();  // Should be empty
    moved.display();     // Should have the data

    // Copy vs Move performance
    std::cout << "\n--- Copy vs Move Performance ---" << std::endl;
    
    // Prepare a large resource
    Resource largeResource("LargeResource", 1000000);
    
    // Measure copy time
    long long copyTime = measureExecutionTime([&largeResource]() {
        Resource copy = largeResource;  // Copy construction
    });
    
    // Measure move time
    long long moveTime = measureExecutionTime([&largeResource]() {
        Resource moved = std::move(largeResource);  // Move construction
        // Restore largeResource for fair comparison
        largeResource = std::move(moved);
    });
    
    std::cout << "Copy time: " << copyTime << " microseconds" << std::endl;
    std::cout << "Move time: " << moveTime << " microseconds" << std::endl;
    std::cout << "Ratio (Copy/Move): " << static_cast<double>(copyTime) / moveTime << std::endl;

    // Return Value Optimization (RVO)
    std::cout << "\n--- Return Value Optimization ---" << std::endl;
    std::cout << "Creating resource via function return:" << std::endl;
    Resource fromFunction = createResource("FunctionResource", 1000);
    fromFunction.display();

    // Perfect forwarding
    std::cout << "\n--- Perfect Forwarding ---" << std::endl;
    Resource perfectForward("PerfectForward", 500);
    std::cout << "Calling function with rvalue:" << std::endl;
    processResource(std::move(perfectForward));
    std::cout << "After processing, original resource:" << std::endl;
    perfectForward.display();  // Should be empty

    // std::move with STL containers
    std::cout << "\n--- std::move with STL Containers ---" << std::endl;
    std::vector<Resource> resources;
    std::cout << "Creating resource for vector:" << std::endl;
    Resource forVector("VectorResource", 500);
    
    std::cout << "Pushing resource to vector with std::move:" << std::endl;
    resources.push_back(std::move(forVector));
    
    std::cout << "Original resource after move to vector:" << std::endl;
    forVector.display();  // Should be empty
    
    std::cout << "Resource in vector:" << std::endl;
    resources[0].display();
}

int main() {
    std::cout << "===== Modern C++ Part 1: Move Semantics and Smart Pointers =====" << std::endl;
    
    demonstrateMoveSemantics();
    demonstrateSmartPointers();
    
    return 0;
}
