#include <iostream>
#include <string>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

// Helper function to print a line separator
void printSeparator(const std::string& title) {
    std::cout << "\n--- " << title << " ---" << std::endl;
}

// Helper function to print file information
void printFileInfo(const fs::directory_entry& entry) {
    try {
        auto lastWriteTime = fs::last_write_time(entry);
        auto timePoint = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            lastWriteTime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
        
        std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
        
        std::cout << std::left << std::setw(40) << entry.path().filename().string();
        
        if (fs::is_regular_file(entry)) {
            std::cout << std::setw(10) << fs::file_size(entry) << " bytes";
        } else {
            std::cout << std::setw(10) << "<DIR>";
        }
        
        std::cout << "  " << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        
        if (fs::is_symlink(entry)) {
            std::cout << " -> " << fs::read_symlink(entry);
        }
        
        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error getting file info: " << e.what() << std::endl;
    }
}

// Function to demonstrate string_view
void demonstrateStringView() {
    printSeparator("std::string_view (C++17)");
    
    // Create a string
    std::string str = "Hello, World!";
    
    // Create string_view from string
    std::string_view sv1 = str;
    std::cout << "sv1: " << sv1 << std::endl;
    
    // Create string_view from C-string
    std::string_view sv2 = "Hello, C++17!";
    std::cout << "sv2: " << sv2 << std::endl;
    
    // Substring without copying
    std::string_view sv3 = sv1.substr(0, 5);  // "Hello"
    std::cout << "sv3 (substring of sv1): " << sv3 << std::endl;
    
    // Modify the original string
    str[1] = 'a';
    std::cout << "After modifying str, sv1: " << sv1 << std::endl;
    std::cout << "sv3 is still: " << sv3 << std::endl;
    
    // Performance comparison
    const int iterations = 1000000;
    
    // Using string
    auto startString = std::chrono::high_resolution_clock::now();
    std::string s = "This is a test string for performance comparison";
    for (int i = 0; i < iterations; ++i) {
        std::string sub = s.substr(10, 4);  // "test"
        if (sub != "test") {
            std::cout << "Unexpected result" << std::endl;
        }
    }
    auto endString = std::chrono::high_resolution_clock::now();
    
    // Using string_view
    auto startStringView = std::chrono::high_resolution_clock::now();
    std::string_view sv = "This is a test string for performance comparison";
    for (int i = 0; i < iterations; ++i) {
        std::string_view subView = sv.substr(10, 4);  // "test"
        if (subView != "test") {
            std::cout << "Unexpected result" << std::endl;
        }
    }
    auto endStringView = std::chrono::high_resolution_clock::now();
    
    auto stringTime = std::chrono::duration_cast<std::chrono::microseconds>(endString - startString).count();
    auto stringViewTime = std::chrono::duration_cast<std::chrono::microseconds>(endStringView - startStringView).count();
    
    std::cout << "Time for " << iterations << " substring operations:" << std::endl;
    std::cout << "  std::string:      " << stringTime << " microseconds" << std::endl;
    std::cout << "  std::string_view: " << stringViewTime << " microseconds" << std::endl;
    std::cout << "  Ratio (string/string_view): " << static_cast<double>(stringTime) / stringViewTime << std::endl;
}

// Function to demonstrate filesystem
void demonstrateFilesystem() {
    printSeparator("std::filesystem (C++17)");
    
    // Current path
    fs::path currentPath = fs::current_path();
    std::cout << "Current path: " << currentPath << std::endl;
    
    // Create a directory
    fs::path tempDir = currentPath / "temp_dir";
    try {
        if (fs::exists(tempDir)) {
            fs::remove_all(tempDir);
        }
        
        fs::create_directory(tempDir);
        std::cout << "Created directory: " << tempDir << std::endl;
        
        // Create some files
        std::ofstream file1(tempDir / "file1.txt");
        file1 << "This is file 1" << std::endl;
        file1.close();
        
        std::ofstream file2(tempDir / "file2.txt");
        file2 << "This is file 2" << std::endl;
        file2.close();
        
        // Create a subdirectory
        fs::path subDir = tempDir / "subdir";
        fs::create_directory(subDir);
        
        std::ofstream file3(subDir / "file3.txt");
        file3 << "This is file 3 in subdirectory" << std::endl;
        file3.close();
        
        // List directory contents
        std::cout << "\nContents of " << tempDir << ":" << std::endl;
        std::cout << std::left << std::setw(40) << "Name" << std::setw(10) << "Size" << "  Last Modified" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        for (const auto& entry : fs::directory_iterator(tempDir)) {
            printFileInfo(entry);
        }
        
        // Recursive directory listing
        std::cout << "\nRecursive listing of " << tempDir << ":" << std::endl;
        std::cout << std::left << std::setw(40) << "Name" << std::setw(10) << "Size" << "  Last Modified" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        for (const auto& entry : fs::recursive_directory_iterator(tempDir)) {
            printFileInfo(entry);
        }
        
        // File operations
        fs::path file1Path = tempDir / "file1.txt";
        fs::path file1CopyPath = tempDir / "file1_copy.txt";
        
        // Copy a file
        fs::copy_file(file1Path, file1CopyPath);
        std::cout << "\nCopied " << file1Path << " to " << file1CopyPath << std::endl;
        
        // Get file size
        std::cout << "Size of " << file1Path << ": " << fs::file_size(file1Path) << " bytes" << std::endl;
        
        // Rename a file
        fs::path file2Path = tempDir / "file2.txt";
        fs::path file2RenamedPath = tempDir / "file2_renamed.txt";
        fs::rename(file2Path, file2RenamedPath);
        std::cout << "Renamed " << file2Path << " to " << file2RenamedPath << std::endl;
        
        // Check if path exists
        std::cout << "Does " << file2Path << " exist? " << (fs::exists(file2Path) ? "Yes" : "No") << std::endl;
        std::cout << "Does " << file2RenamedPath << " exist? " << (fs::exists(file2RenamedPath) ? "Yes" : "No") << std::endl;
        
        // Path decomposition
        fs::path samplePath = tempDir / "subdir" / "file3.txt";
        std::cout << "\nPath decomposition for: " << samplePath << std::endl;
        std::cout << "  Root name: " << samplePath.root_name() << std::endl;
        std::cout << "  Root directory: " << samplePath.root_directory() << std::endl;
        std::cout << "  Root path: " << samplePath.root_path() << std::endl;
        std::cout << "  Relative path: " << samplePath.relative_path() << std::endl;
        std::cout << "  Parent path: " << samplePath.parent_path() << std::endl;
        std::cout << "  Filename: " << samplePath.filename() << std::endl;
        std::cout << "  Stem: " << samplePath.stem() << std::endl;
        std::cout << "  Extension: " << samplePath.extension() << std::endl;
        
        // Space information
        fs::space_info space = fs::space(currentPath);
        std::cout << "\nDisk space information for " << currentPath << ":" << std::endl;
        std::cout << "  Capacity: " << space.capacity / (1024 * 1024) << " MB" << std::endl;
        std::cout << "  Free: " << space.free / (1024 * 1024) << " MB" << std::endl;
        std::cout << "  Available: " << space.available / (1024 * 1024) << " MB" << std::endl;
        
        // Clean up
        fs::remove_all(tempDir);
        std::cout << "\nRemoved directory: " << tempDir << std::endl;
        
    } catch (const fs::filesystem_error& e) {
        std::cout << "Filesystem error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "===== Modern C++ Part 4: string_view and filesystem =====" << std::endl;
    
    demonstrateStringView();
    demonstrateFilesystem();
    
    return 0;
}
