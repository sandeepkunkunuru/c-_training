#ifndef DVD_H
#define DVD_H

#include "library_item.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

class DVD : public LibraryItem {
private:
    std::string director;
    int runtime;  // in minutes
    int releaseYear;
    std::string studio;
    std::vector<std::string> actors;
    std::string genre;
    std::string rating;  // e.g., "PG", "PG-13", "R"
    
public:
    // Constructor
    DVD(const std::string& title, const std::string& id, const std::string& dvdDirector,
        int minutes, int year, const std::string& dvdStudio, 
        const std::string& dvdGenre, const std::string& dvdRating)
        : LibraryItem(title, id, 1.00, 7),  // $1.00 daily fine, 7 days loan period
          director(dvdDirector), runtime(minutes), releaseYear(year),
          studio(dvdStudio), genre(dvdGenre), rating(dvdRating) {}
    
    // Implementation of pure virtual methods
    std::string getItemType() const override {
        return "DVD";
    }
    
    void displayDetails() const override {
        std::cout << "\n===== DVD Details =====" << std::endl;
        std::cout << "Title: " << title << std::endl;
        std::cout << "Director: " << director << std::endl;
        std::cout << "ID: " << itemId << std::endl;
        std::cout << "Runtime: " << runtime << " minutes" << std::endl;
        std::cout << "Release Year: " << releaseYear << std::endl;
        std::cout << "Studio: " << studio << std::endl;
        std::cout << "Genre: " << genre << std::endl;
        std::cout << "Rating: " << rating << std::endl;
        
        if (!actors.empty()) {
            std::cout << "Actors: ";
            for (size_t i = 0; i < actors.size(); ++i) {
                std::cout << actors[i];
                if (i < actors.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }
        
        std::cout << "Status: " << (checkedOut ? "Checked Out" : "Available") << std::endl;
        
        if (checkedOut) {
            std::cout << "Borrower: " << borrowerName << std::endl;
            std::cout << "Due Date: " << dueDate << std::endl;
        }
        
        std::cout << "Daily Fine: $" << std::fixed << std::setprecision(2) << dailyFine << std::endl;
        std::cout << "Maximum Loan Period: " << maxLoanDays << " days" << std::endl;
    }
    
    // DVD-specific methods
    void addActor(const std::string& actor) {
        actors.push_back(actor);
    }
    
    std::string getDirector() const { return director; }
    int getRuntime() const { return runtime; }
    int getReleaseYear() const { return releaseYear; }
    std::string getStudio() const { return studio; }
    std::string getGenre() const { return genre; }
    std::string getRating() const { return rating; }
    const std::vector<std::string>& getActors() const { return actors; }
    
    void setDirector(const std::string& newDirector) { director = newDirector; }
    void setRuntime(int newRuntime) { runtime = newRuntime; }
    void setReleaseYear(int newYear) { releaseYear = newYear; }
    void setStudio(const std::string& newStudio) { studio = newStudio; }
    void setGenre(const std::string& newGenre) { genre = newGenre; }
    void setRating(const std::string& newRating) { rating = newRating; }
};

#endif // DVD_H
