#ifndef JOURNAL_H
#define JOURNAL_H

#include "library_item.h"
#include <string>
#include <iostream>
#include <iomanip>

class Journal : public LibraryItem {
private:
    std::string publisher;
    int volume;
    int issue;
    int publicationYear;
    int publicationMonth;
    std::string subject;
    bool isAcademic;
    
public:
    // Constructor
    Journal(const std::string& title, const std::string& id, const std::string& journalPublisher,
            int journalVolume, int journalIssue, int year, int month, 
            const std::string& journalSubject, bool academic)
        : LibraryItem(title, id, 0.75, 14),  // $0.75 daily fine, 14 days loan period
          publisher(journalPublisher), volume(journalVolume), issue(journalIssue),
          publicationYear(year), publicationMonth(month), 
          subject(journalSubject), isAcademic(academic) {}
    
    // Implementation of pure virtual methods
    std::string getItemType() const override {
        return "Journal";
    }
    
    void displayDetails() const override {
        std::cout << "\n===== Journal Details =====" << std::endl;
        std::cout << "Title: " << title << std::endl;
        std::cout << "ID: " << itemId << std::endl;
        std::cout << "Publisher: " << publisher << std::endl;
        std::cout << "Volume: " << volume << ", Issue: " << issue << std::endl;
        std::cout << "Publication Date: " << publicationMonth << "/" << publicationYear << std::endl;
        std::cout << "Subject: " << subject << std::endl;
        std::cout << "Type: " << (isAcademic ? "Academic" : "Popular") << std::endl;
        std::cout << "Status: " << (checkedOut ? "Checked Out" : "Available") << std::endl;
        
        if (checkedOut) {
            std::cout << "Borrower: " << borrowerName << std::endl;
            std::cout << "Due Date: " << dueDate << std::endl;
        }
        
        std::cout << "Daily Fine: $" << std::fixed << std::setprecision(2) << dailyFine << std::endl;
        std::cout << "Maximum Loan Period: " << maxLoanDays << " days" << std::endl;
    }
    
    // Journal-specific methods
    std::string getPublisher() const { return publisher; }
    int getVolume() const { return volume; }
    int getIssue() const { return issue; }
    int getPublicationYear() const { return publicationYear; }
    int getPublicationMonth() const { return publicationMonth; }
    std::string getSubject() const { return subject; }
    bool getIsAcademic() const { return isAcademic; }
    
    void setPublisher(const std::string& newPublisher) { publisher = newPublisher; }
    void setVolume(int newVolume) { volume = newVolume; }
    void setIssue(int newIssue) { issue = newIssue; }
    void setPublicationYear(int newYear) { publicationYear = newYear; }
    void setPublicationMonth(int newMonth) { publicationMonth = newMonth; }
    void setSubject(const std::string& newSubject) { subject = newSubject; }
    void setIsAcademic(bool academic) { isAcademic = academic; }
    
    // Get citation format
    std::string getCitation() const {
        std::string citation = title + ". ";
        citation += "(" + std::to_string(publicationYear) + "). ";
        citation += "Vol. " + std::to_string(volume) + ", ";
        citation += "Issue " + std::to_string(issue) + ". ";
        citation += publisher + ".";
        return citation;
    }
};

#endif // JOURNAL_H
