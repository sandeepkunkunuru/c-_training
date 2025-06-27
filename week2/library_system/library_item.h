#ifndef LIBRARY_ITEM_H
#define LIBRARY_ITEM_H

#include <string>
#include <iostream>

// Abstract base class for all library items
class LibraryItem {
protected:
    std::string title;
    std::string itemId;
    bool checkedOut;
    std::string borrowerName;
    std::string dueDate;
    double dailyFine;
    int maxLoanDays;

public:
    // Constructor
    LibraryItem(const std::string& itemTitle, const std::string& id, double fine, int loanDays)
        : title(itemTitle), itemId(id), checkedOut(false), 
          borrowerName(""), dueDate(""), dailyFine(fine), maxLoanDays(loanDays) {}
    
    // Virtual destructor
    virtual ~LibraryItem() {}
    
    // Pure virtual method to get item type
    virtual std::string getItemType() const = 0;
    
    // Pure virtual method to display details
    virtual void displayDetails() const = 0;
    
    // Check out item
    bool checkOut(const std::string& borrower, const std::string& date) {
        if (checkedOut) {
            std::cout << "Error: Item is already checked out." << std::endl;
            return false;
        }
        
        checkedOut = true;
        borrowerName = borrower;
        dueDate = date;
        std::cout << getItemType() << " \"" << title << "\" checked out to " 
                  << borrower << " until " << date << std::endl;
        return true;
    }
    
    // Return item
    bool returnItem() {
        if (!checkedOut) {
            std::cout << "Error: Item is not checked out." << std::endl;
            return false;
        }
        
        checkedOut = false;
        std::cout << getItemType() << " \"" << title << "\" returned by " 
                  << borrowerName << std::endl;
        borrowerName = "";
        dueDate = "";
        return true;
    }
    
    // Calculate fine (assuming current date is provided)
    double calculateFine(const std::string& currentDate) const {
        if (!checkedOut) {
            return 0.0;
        }
        
        // In a real system, we would calculate days overdue based on actual dates
        // For simplicity, we'll just use a placeholder value
        int daysOverdue = 0;  // Placeholder
        
        if (daysOverdue > 0) {
            return daysOverdue * dailyFine;
        }
        return 0.0;
    }
    
    // Getter methods
    std::string getTitle() const { return title; }
    std::string getItemId() const { return itemId; }
    bool isCheckedOut() const { return checkedOut; }
    std::string getBorrowerName() const { return borrowerName; }
    std::string getDueDate() const { return dueDate; }
    double getDailyFine() const { return dailyFine; }
    int getMaxLoanDays() const { return maxLoanDays; }
    
    // Setter methods
    void setTitle(const std::string& newTitle) { title = newTitle; }
    void setDailyFine(double newFine) { dailyFine = newFine; }
};

#endif // LIBRARY_ITEM_H
