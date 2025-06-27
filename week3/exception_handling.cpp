#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <functional>

// Custom exception classes
class FileException : public std::exception {
private:
    std::string message;
    
public:
    explicit FileException(const std::string& filename, const std::string& error)
        : message("File error: " + error + " for file '" + filename + "'") {}
    
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InvalidArgumentException : public std::exception {
private:
    std::string message;
    
public:
    explicit InvalidArgumentException(const std::string& error)
        : message("Invalid argument: " + error) {}
    
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// RAII file wrapper
class FileHandler {
private:
    std::fstream file;
    std::string filename;
    bool isOpen;
    
public:
    explicit FileHandler(const std::string& name, std::ios_base::openmode mode = std::ios_base::in)
        : filename(name), isOpen(false) {
        file.open(filename, mode);
        if (!file.is_open()) {
            throw FileException(filename, "Failed to open file");
        }
        isOpen = true;
        std::cout << "File '" << filename << "' opened successfully." << std::endl;
    }
    
    ~FileHandler() {
        if (isOpen) {
            file.close();
            std::cout << "File '" << filename << "' closed." << std::endl;
        }
    }
    
    // Prevent copying
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;
    
    // Allow moving
    FileHandler(FileHandler&& other) noexcept
        : file(std::move(other.file)), filename(std::move(other.filename)), isOpen(other.isOpen) {
        other.isOpen = false;
    }
    
    FileHandler& operator=(FileHandler&& other) noexcept {
        if (this != &other) {
            if (isOpen) {
                file.close();
            }
            file = std::move(other.file);
            filename = std::move(other.filename);
            isOpen = other.isOpen;
            other.isOpen = false;
        }
        return *this;
    }
    
    void write(const std::string& data) {
        if (!isOpen) {
            throw FileException(filename, "File is not open");
        }
        file << data;
        if (file.fail()) {
            throw FileException(filename, "Write operation failed");
        }
    }
    
    std::string read() {
        if (!isOpen) {
            throw FileException(filename, "File is not open");
        }
        
        std::string content;
        std::string line;
        while (std::getline(file, line)) {
            content += line + "\n";
        }
        
        if (file.bad()) {
            throw FileException(filename, "Read operation failed");
        }
        
        return content;
    }
};

// RAII memory management with unique_ptr
class Resource {
private:
    std::string name;
    
public:
    explicit Resource(const std::string& n) : name(n) {
        std::cout << "Resource '" << name << "' acquired." << std::endl;
    }
    
    ~Resource() {
        std::cout << "Resource '" << name << "' released." << std::endl;
    }
    
    void use() const {
        std::cout << "Using resource '" << name << "'." << std::endl;
    }
};

// Function that demonstrates exception safety with RAII
void processResources() {
    // Resources will be automatically released when function exits,
    // even if an exception is thrown
    std::unique_ptr<Resource> resource1 = std::make_unique<Resource>("Database");
    std::unique_ptr<Resource> resource2 = std::make_unique<Resource>("Network");
    
    resource1->use();
    resource2->use();
    
    // Simulate an error
    if (rand() % 2 == 0) {
        throw std::runtime_error("Random error occurred during processing");
    }
    
    std::cout << "Resources processed successfully." << std::endl;
}

// Function that demonstrates nested try-catch blocks
void nestedExceptionHandling(int value) {
    try {
        std::cout << "Outer try block: Processing value " << value << std::endl;
        
        try {
            if (value < 0) {
                throw InvalidArgumentException("Value cannot be negative");
            }
            
            if (value == 0) {
                throw std::runtime_error("Value cannot be zero");
            }
            
            std::cout << "Inner try block: Value is positive: " << value << std::endl;
        }
        catch (const InvalidArgumentException& e) {
            std::cout << "Inner catch block: " << e.what() << std::endl;
            // Rethrow to outer handler
            throw;
        }
    }
    catch (const InvalidArgumentException& e) {
        std::cout << "Outer catch block: Caught InvalidArgumentException: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Outer catch block: Caught std::exception: " << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "Outer catch block: Caught unknown exception" << std::endl;
    }
}

// Exception safety guarantees demonstration
class SafetyDemo {
public:
    // Basic guarantee: if an exception is thrown, no resources are leaked
    static void basicGuarantee(const std::string& filename) {
        std::cout << "\n--- Basic Guarantee Example ---" << std::endl;
        
        std::unique_ptr<Resource> resource = std::make_unique<Resource>("BasicGuarantee");
        
        try {
            // This might throw, but resource will be cleaned up by unique_ptr
            FileHandler file(filename);
            file.write("Testing basic guarantee");
        }
        catch (const std::exception& e) {
            std::cout << "Exception caught: " << e.what() << std::endl;
            // Resource is automatically cleaned up, even when exception occurs
        }
    }
    
    // Strong guarantee: if an exception is thrown, the state is unchanged
    static void strongGuarantee(std::vector<int>& data, int newValue) {
        std::cout << "\n--- Strong Guarantee Example ---" << std::endl;
        
        // Save the original state
        std::vector<int> backup = data;
        
        try {
            // Perform operations that might throw
            data.push_back(newValue);
            
            if (newValue == 0) {
                throw std::invalid_argument("Zero values not allowed");
            }
            
            // More operations...
            std::cout << "Value " << newValue << " added successfully." << std::endl;
        }
        catch (const std::exception& e) {
            // Restore the original state
            data = backup;
            std::cout << "Exception caught: " << e.what() << std::endl;
            std::cout << "Data restored to original state." << std::endl;
        }
    }
    
    // Nothrow guarantee: operations will not throw exceptions
    static void nothrowGuarantee(std::vector<int>& data) noexcept {
        std::cout << "\n--- Nothrow Guarantee Example ---" << std::endl;
        
        try {
            // Use operations that are guaranteed not to throw
            if (!data.empty()) {
                data.pop_back();  // pop_back() is noexcept
                std::cout << "Last element removed." << std::endl;
            } else {
                std::cout << "Vector is empty, nothing to remove." << std::endl;
            }
        }
        catch (...) {
            // This should never happen with proper nothrow operations
            std::cout << "This should never happen!" << std::endl;
        }
    }
};

// RAII with scope guard pattern
template<typename F>
class ScopeGuard {
private:
    F function;
    bool dismissed;
    
public:
    explicit ScopeGuard(F f) : function(std::move(f)), dismissed(false) {}
    
    ~ScopeGuard() {
        if (!dismissed) {
            function();
        }
    }
    
    void dismiss() {
        dismissed = true;
    }
    
    // Prevent copying
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
    
    // Allow moving
    ScopeGuard(ScopeGuard&& other) noexcept
        : function(std::move(other.function)), dismissed(other.dismissed) {
        other.dismissed = true;
    }
};

// Helper function to create a scope guard
template<typename F>
ScopeGuard<F> makeScopeGuard(F f) {
    return ScopeGuard<F>(std::move(f));
}

int main() {
    std::cout << "===== Exception Handling Basics =====" << std::endl;
    
    // Basic try-catch
    try {
        std::cout << "Attempting to divide by zero..." << std::endl;
        int result = 10 / 0;
        std::cout << "Result: " << result << std::endl;  // This won't execute
    }
    catch (const std::exception& e) {
        std::cout << "Standard exception caught: " << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "Unknown exception caught" << std::endl;
    }
    
    std::cout << "\n===== Custom Exceptions =====" << std::endl;
    
    try {
        throw InvalidArgumentException("Value out of range");
    }
    catch (const InvalidArgumentException& e) {
        std::cout << "Caught custom exception: " << e.what() << std::endl;
    }
    
    std::cout << "\n===== RAII File Handling =====" << std::endl;
    
    try {
        // Create a temporary file for testing
        {
            std::ofstream tempFile("temp.txt");
            tempFile << "This is a test file for RAII demonstration." << std::endl;
        }
        
        // RAII in action - file will be closed automatically
        FileHandler file("temp.txt");
        std::string content = file.read();
        std::cout << "File content: " << content;
        
        // Try to open a non-existent file
        try {
            FileHandler nonExistentFile("non_existent_file.txt");
        }
        catch (const FileException& e) {
            std::cout << "Caught file exception: " << e.what() << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    
    std::cout << "\n===== RAII Resource Management =====" << std::endl;
    
    try {
        processResources();
    }
    catch (const std::exception& e) {
        std::cout << "Exception caught in processResources: " << e.what() << std::endl;
    }
    
    std::cout << "\n===== Nested Exception Handling =====" << std::endl;
    
    nestedExceptionHandling(5);   // Positive value
    nestedExceptionHandling(-5);  // Negative value
    nestedExceptionHandling(0);   // Zero value
    
    std::cout << "\n===== Exception Safety Guarantees =====" << std::endl;
    
    std::vector<int> data = {1, 2, 3};
    
    // Basic guarantee
    SafetyDemo::basicGuarantee("temp.txt");
    SafetyDemo::basicGuarantee("non_existent_file.txt");
    
    // Strong guarantee
    std::cout << "\nBefore strong guarantee test, data size: " << data.size() << std::endl;
    SafetyDemo::strongGuarantee(data, 4);  // Should succeed
    std::cout << "After successful operation, data size: " << data.size() << std::endl;
    
    SafetyDemo::strongGuarantee(data, 0);  // Should fail and restore state
    std::cout << "After failed operation, data size: " << data.size() << std::endl;
    
    // Nothrow guarantee
    SafetyDemo::nothrowGuarantee(data);
    
    std::cout << "\n===== Scope Guard Pattern =====" << std::endl;
    
    {
        bool resourceAcquired = false;
        
        // Create a scope guard that will execute when we leave this scope
        auto guard = makeScopeGuard([&resourceAcquired]() {
            std::cout << "Scope guard executed, cleaning up resources." << std::endl;
            if (resourceAcquired) {
                std::cout << "Resource released by scope guard." << std::endl;
            }
        });
        
        std::cout << "Acquiring resource..." << std::endl;
        resourceAcquired = true;
        
        // If we decide we don't need the cleanup, we can dismiss the guard
        if (false) {  // Change to true to see the difference
            std::cout << "Dismissing scope guard." << std::endl;
            guard.dismiss();
        }
        
        std::cout << "Leaving scope..." << std::endl;
        // Scope guard will execute here unless dismissed
    }
    
    // Clean up the temporary file
    std::remove("temp.txt");
    
    return 0;
}
