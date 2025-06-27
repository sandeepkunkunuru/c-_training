#ifndef BOOK_H
#define BOOK_H

#include "library_item.h"
#include <string>
#include <iostream>
#include <iomanip>

class Book : public LibraryItem {
private:
    std::string author;
    std::string isbn;
    int pageCount;
    std::string publisher;
    int publicationYear;
    std::string genre;
    
public:
    // Constructor
    Book(const std::string& title, const std::string& id, const std::string& bookAuthor,
         const std::string& bookIsbn, int pages, const std::string& pub, 
         int year, const std::string& bookGenre)
        : LibraryItem(title, id, 0.50, 21),  // $0.50 daily fine, 21 days loan period
          author(bookAuthor), isbn(bookIsbn), pageCount(pages),
          publisher(pub), publicationYear(year), genre(bookGenre) {}
    
    // Implementation of pure virtual methods
    std::string getItemType() const override {
        return "Book";
    }
    
    void displayDetails() const override {
        std::cout << "\n===== Book Details =====" << std::endl;
        std::cout << "Title: " << title << std::endl;
        std::cout << "Author: " << author << std::endl;
        std::cout << "ID: " << itemId << std::endl;
        std::cout << "ISBN: " << isbn << std::endl;
        std::cout << "Genre: " << genre << std::endl;
        std::cout << "Publisher: " << publisher << std::endl;
        std::cout << "Publication Year: " << publicationYear << std::endl;
        std::cout << "Page Count: " << pageCount << std::endl;
        std::cout << "Status: " << (checkedOut ? "Checked Out" : "Available") << std::endl;
        
        if (checkedOut) {
            std::cout << "Borrower: " << borrowerName << std::endl;
            std::cout << "Due Date: " << dueDate << std::endl;
        }
        
        std::cout << "Daily Fine: $" << std::fixed << std::setprecision(2) << dailyFine << std::endl;
        std::cout << "Maximum Loan Period: " << maxLoanDays << " days" << std::endl;
    }
    
    // Book-specific methods
    std::string getAuthor() const { return author; }
    std::string getIsbn() const { return isbn; }
    int getPageCount() const { return pageCount; }
    std::string getPublisher() const { return publisher; }
    int getPublicationYear() const { return publicationYear; }
    std::string getGenre() const { return genre; }
    
    void setAuthor(const std::string& newAuthor) { author = newAuthor; }
    void setIsbn(const std::string& newIsbn) { isbn = newIsbn; }
    void setPageCount(int newPageCount) { pageCount = newPageCount; }
    void setPublisher(const std::string& newPublisher) { publisher = newPublisher; }
    void setPublicationYear(int newYear) { publicationYear = newYear; }
    void setGenre(const std::string& newGenre) { genre = newGenre; }
};

#endif // BOOK_H
