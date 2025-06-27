#include "library.h"
#include "book.h"
#include "dvd.h"
#include "journal.h"
#include <iostream>
#include <memory>
#include <string>
#include <limits>

// Function to display the menu
void displayMenu() {
    std::cout << "\n===== Library Management System Menu =====" << std::endl;
    std::cout << "1. Add a new item" << std::endl;
    std::cout << "2. Remove an item" << std::endl;
    std::cout << "3. Search for an item by title" << std::endl;
    std::cout << "4. Display all items" << std::endl;
    std::cout << "5. Check out an item" << std::endl;
    std::cout << "6. Return an item" << std::endl;
    std::cout << "7. Display checked out items" << std::endl;
    std::cout << "8. Display library statistics" << std::endl;
    std::cout << "9. Exit" << std::endl;
    std::cout << "Enter your choice (1-9): ";
}

// Function to add a new book
void addBook(Library& library) {
    std::string title, id, author, isbn, publisher, genre;
    int pageCount, publicationYear;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Enter book title: ";
    std::getline(std::cin, title);
    
    std::cout << "Enter book ID: ";
    std::getline(std::cin, id);
    
    std::cout << "Enter author: ";
    std::getline(std::cin, author);
    
    std::cout << "Enter ISBN: ";
    std::getline(std::cin, isbn);
    
    std::cout << "Enter page count: ";
    std::cin >> pageCount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Enter publisher: ";
    std::getline(std::cin, publisher);
    
    std::cout << "Enter publication year: ";
    std::cin >> publicationYear;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Enter genre: ";
    std::getline(std::cin, genre);
    
    auto book = std::make_shared<Book>(title, id, author, isbn, pageCount, 
                                       publisher, publicationYear, genre);
    library.addItem(book);
}

// Function to add a new DVD
void addDVD(Library& library) {
    std::string title, id, director, studio, genre, rating;
    int runtime, releaseYear;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Enter DVD title: ";
    std::getline(std::cin, title);
    
    std::cout << "Enter DVD ID: ";
    std::getline(std::cin, id);
    
    std::cout << "Enter director: ";
    std::getline(std::cin, director);
    
    std::cout << "Enter runtime (minutes): ";
    std::cin >> runtime;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Enter release year: ";
    std::cin >> releaseYear;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Enter studio: ";
    std::getline(std::cin, studio);
    
    std::cout << "Enter genre: ";
    std::getline(std::cin, genre);
    
    std::cout << "Enter rating (e.g., PG, PG-13, R): ";
    std::getline(std::cin, rating);
    
    auto dvd = std::make_shared<DVD>(title, id, director, runtime, 
                                     releaseYear, studio, genre, rating);
    
    // Add actors
    std::string actor;
    char addMore = 'y';
    
    while (addMore == 'y' || addMore == 'Y') {
        std::cout << "Enter actor name: ";
        std::getline(std::cin, actor);
        dvd->addActor(actor);
        
        std::cout << "Add another actor? (y/n): ";
        std::cin >> addMore;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    library.addItem(dvd);
}

// Function to add a new journal
void addJournal(Library& library) {
    std::string title, id, publisher, subject;
    int volume, issue, year, month;
    char isAcademicChar;
    bool isAcademic;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Enter journal title: ";
    std::getline(std::cin, title);
    
    std::cout << "Enter journal ID: ";
    std::getline(std::cin, id);
    
    std::cout << "Enter publisher: ";
    std::getline(std::cin, publisher);
    
    std::cout << "Enter volume: ";
    std::cin >> volume;
    
    std::cout << "Enter issue: ";
    std::cin >> issue;
    
    std::cout << "Enter publication year: ";
    std::cin >> year;
    
    std::cout << "Enter publication month (1-12): ";
    std::cin >> month;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Enter subject: ";
    std::getline(std::cin, subject);
    
    std::cout << "Is this an academic journal? (y/n): ";
    std::cin >> isAcademicChar;
    isAcademic = (isAcademicChar == 'y' || isAcademicChar == 'Y');
    
    auto journal = std::make_shared<Journal>(title, id, publisher, volume, issue, 
                                            year, month, subject, isAcademic);
    library.addItem(journal);
}

// Function to add a new item (calls appropriate function based on item type)
void addItem(Library& library) {
    int itemType;
    
    std::cout << "\n===== Add New Item =====" << std::endl;
    std::cout << "1. Book" << std::endl;
    std::cout << "2. DVD" << std::endl;
    std::cout << "3. Journal" << std::endl;
    std::cout << "Enter item type (1-3): ";
    std::cin >> itemType;
    
    switch (itemType) {
        case 1:
            addBook(library);
            break;
        case 2:
            addDVD(library);
            break;
        case 3:
            addJournal(library);
            break;
        default:
            std::cout << "Invalid item type." << std::endl;
    }
}

// Function to remove an item
void removeItem(Library& library) {
    std::string itemId;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "\n===== Remove Item =====" << std::endl;
    std::cout << "Enter item ID to remove: ";
    std::getline(std::cin, itemId);
    
    library.removeItem(itemId);
}

// Function to search for an item by title
void searchByTitle(const Library& library) {
    std::string titleQuery;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "\n===== Search by Title =====" << std::endl;
    std::cout << "Enter title to search for: ";
    std::getline(std::cin, titleQuery);
    
    auto results = library.searchByTitle(titleQuery);
    
    std::cout << "Search results for \"" << titleQuery << "\":" << std::endl;
    if (results.empty()) {
        std::cout << "No items found." << std::endl;
    } else {
        for (size_t i = 0; i < results.size(); ++i) {
            std::cout << (i + 1) << ". " << results[i]->getItemType() << ": " 
                      << results[i]->getTitle() << " (ID: " << results[i]->getItemId() << ")" << std::endl;
        }
        
        // Ask if user wants to see details of a specific item
        char viewDetails;
        std::cout << "\nView details of an item? (y/n): ";
        std::cin >> viewDetails;
        
        if (viewDetails == 'y' || viewDetails == 'Y') {
            int selection;
            std::cout << "Enter item number: ";
            std::cin >> selection;
            
            if (selection >= 1 && selection <= static_cast<int>(results.size())) {
                results[selection - 1]->displayDetails();
            } else {
                std::cout << "Invalid selection." << std::endl;
            }
        }
    }
}

// Function to check out an item
void checkOutItem(Library& library) {
    std::string itemId, borrower, dueDate;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "\n===== Check Out Item =====" << std::endl;
    std::cout << "Enter item ID: ";
    std::getline(std::cin, itemId);
    
    auto item = library.findItem(itemId);
    if (!item) {
        std::cout << "Error: Item with ID " << itemId << " not found." << std::endl;
        return;
    }
    
    if (item->isCheckedOut()) {
        std::cout << "Error: Item is already checked out." << std::endl;
        return;
    }
    
    std::cout << "Enter borrower name: ";
    std::getline(std::cin, borrower);
    
    std::cout << "Enter due date (e.g., 2025-07-15): ";
    std::getline(std::cin, dueDate);
    
    library.checkOutItem(itemId, borrower, dueDate);
}

// Function to return an item
void returnItem(Library& library) {
    std::string itemId;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "\n===== Return Item =====" << std::endl;
    std::cout << "Enter item ID: ";
    std::getline(std::cin, itemId);
    
    library.returnItem(itemId);
}

// Main function
int main() {
    // Create a library
    Library library("Community Library", "123 Main Street, Anytown, USA");
    
    // Add some initial items to the library
    auto book1 = std::make_shared<Book>(
        "The C++ Programming Language", "B001", "Bjarne Stroustrup",
        "978-0321563842", 1376, "Addison-Wesley Professional", 2013, "Programming"
    );
    
    auto book2 = std::make_shared<Book>(
        "Effective Modern C++", "B002", "Scott Meyers",
        "978-1491903995", 334, "O'Reilly Media", 2014, "Programming"
    );
    
    auto dvd1 = std::make_shared<DVD>(
        "The Matrix", "D001", "Lana and Lilly Wachowski",
        136, 1999, "Warner Bros.", "Science Fiction", "R"
    );
    dvd1->addActor("Keanu Reeves");
    dvd1->addActor("Laurence Fishburne");
    dvd1->addActor("Carrie-Anne Moss");
    
    auto journal1 = std::make_shared<Journal>(
        "Journal of C++ Studies", "J001", "C++ Foundation",
        5, 2, 2023, 6, "Computer Science", true
    );
    
    // Add items to the library
    library.addItem(book1);
    library.addItem(book2);
    library.addItem(dvd1);
    library.addItem(journal1);
    
    int choice;
    bool running = true;
    
    std::cout << "Welcome to the Library Management System!" << std::endl;
    
    while (running) {
        displayMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                addItem(library);
                break;
            case 2:
                removeItem(library);
                break;
            case 3:
                searchByTitle(library);
                break;
            case 4:
                library.displayAllItems();
                break;
            case 5:
                checkOutItem(library);
                break;
            case 6:
                returnItem(library);
                break;
            case 7:
                library.displayCheckedOutItems();
                break;
            case 8:
                library.displayStatistics();
                break;
            case 9:
                std::cout << "Thank you for using the Library Management System. Goodbye!" << std::endl;
                running = false;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    
    return 0;
}
