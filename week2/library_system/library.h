#ifndef LIBRARY_H
#define LIBRARY_H

#include "library_item.h"
#include "book.h"
#include "dvd.h"
#include "journal.h"
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>

class Library {
private:
    std::string name;
    std::string address;
    std::vector<std::shared_ptr<LibraryItem>> items;
    
public:
    // Constructor
    Library(const std::string& libraryName, const std::string& libraryAddress)
        : name(libraryName), address(libraryAddress) {}
    
    // Add an item to the library
    void addItem(std::shared_ptr<LibraryItem> item) {
        items.push_back(item);
        std::cout << item->getItemType() << " \"" << item->getTitle() 
                  << "\" added to the library." << std::endl;
    }
    
    // Remove an item from the library
    bool removeItem(const std::string& itemId) {
        auto it = std::find_if(items.begin(), items.end(),
                              [&itemId](const std::shared_ptr<LibraryItem>& item) {
                                  return item->getItemId() == itemId;
                              });
        
        if (it != items.end()) {
            std::string title = (*it)->getTitle();
            std::string type = (*it)->getItemType();
            items.erase(it);
            std::cout << type << " \"" << title << "\" removed from the library." << std::endl;
            return true;
        }
        
        std::cout << "Error: Item with ID " << itemId << " not found." << std::endl;
        return false;
    }
    
    // Find an item by ID
    std::shared_ptr<LibraryItem> findItem(const std::string& itemId) const {
        auto it = std::find_if(items.begin(), items.end(),
                              [&itemId](const std::shared_ptr<LibraryItem>& item) {
                                  return item->getItemId() == itemId;
                              });
        
        if (it != items.end()) {
            return *it;
        }
        
        return nullptr;
    }
    
    // Search items by title (partial match)
    std::vector<std::shared_ptr<LibraryItem>> searchByTitle(const std::string& titleQuery) const {
        std::vector<std::shared_ptr<LibraryItem>> results;
        
        for (const auto& item : items) {
            // Case-insensitive search
            std::string title = item->getTitle();
            std::string query = titleQuery;
            
            // Convert both to lowercase for comparison
            std::transform(title.begin(), title.end(), title.begin(), ::tolower);
            std::transform(query.begin(), query.end(), query.begin(), ::tolower);
            
            if (title.find(query) != std::string::npos) {
                results.push_back(item);
            }
        }
        
        return results;
    }
    
    // Check out an item
    bool checkOutItem(const std::string& itemId, const std::string& borrower, const std::string& dueDate) {
        auto item = findItem(itemId);
        if (item) {
            return item->checkOut(borrower, dueDate);
        }
        
        std::cout << "Error: Item with ID " << itemId << " not found." << std::endl;
        return false;
    }
    
    // Return an item
    bool returnItem(const std::string& itemId) {
        auto item = findItem(itemId);
        if (item) {
            return item->returnItem();
        }
        
        std::cout << "Error: Item with ID " << itemId << " not found." << std::endl;
        return false;
    }
    
    // Display all items in the library
    void displayAllItems() const {
        std::cout << "\n===== Library Inventory =====" << std::endl;
        std::cout << "Library: " << name << std::endl;
        std::cout << "Address: " << address << std::endl;
        std::cout << "Total Items: " << items.size() << std::endl;
        
        if (items.empty()) {
            std::cout << "No items in the library." << std::endl;
            return;
        }
        
        std::cout << "\nItems:" << std::endl;
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << (i + 1) << ". " << items[i]->getItemType() << ": " 
                      << items[i]->getTitle() << " (ID: " << items[i]->getItemId() << ") - "
                      << (items[i]->isCheckedOut() ? "Checked Out" : "Available") << std::endl;
        }
    }
    
    // Display checked out items
    void displayCheckedOutItems() const {
        std::cout << "\n===== Checked Out Items =====" << std::endl;
        
        int count = 0;
        for (const auto& item : items) {
            if (item->isCheckedOut()) {
                std::cout << "- " << item->getItemType() << ": " << item->getTitle() 
                          << " (ID: " << item->getItemId() << ")" << std::endl;
                std::cout << "  Borrower: " << item->getBorrowerName() 
                          << ", Due Date: " << item->getDueDate() << std::endl;
                count++;
            }
        }
        
        if (count == 0) {
            std::cout << "No items are currently checked out." << std::endl;
        } else {
            std::cout << "Total checked out items: " << count << std::endl;
        }
    }
    
    // Get library statistics
    void displayStatistics() const {
        int totalBooks = 0;
        int totalDVDs = 0;
        int totalJournals = 0;
        int checkedOutItems = 0;
        
        for (const auto& item : items) {
            if (item->getItemType() == "Book") totalBooks++;
            else if (item->getItemType() == "DVD") totalDVDs++;
            else if (item->getItemType() == "Journal") totalJournals++;
            
            if (item->isCheckedOut()) checkedOutItems++;
        }
        
        std::cout << "\n===== Library Statistics =====" << std::endl;
        std::cout << "Library: " << name << std::endl;
        std::cout << "Total Items: " << items.size() << std::endl;
        std::cout << "Books: " << totalBooks << std::endl;
        std::cout << "DVDs: " << totalDVDs << std::endl;
        std::cout << "Journals: " << totalJournals << std::endl;
        std::cout << "Checked Out Items: " << checkedOutItems << std::endl;
        std::cout << "Available Items: " << (items.size() - checkedOutItems) << std::endl;
    }
    
    // Getter methods
    std::string getName() const { return name; }
    std::string getAddress() const { return address; }
    size_t getItemCount() const { return items.size(); }
};

#endif // LIBRARY_H
