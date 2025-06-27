#include <iostream>
#include <string>
#include <vector>
#include <iomanip>  // For formatting output

// Define a Student class
class Student {
    // Data members (attributes)
    public:
        std::string name;
        std::string id;
        std::vector<double> grades;
    
    // Member functions (methods)
    public:
        // Method to add a grade
        void addGrade(double grade) {
            if (grade >= 0 && grade <= 100) {
                grades.push_back(grade);
                std::cout << "Grade " << grade << " added successfully." << std::endl;
            } else {
                std::cout << "Error: Grade must be between 0 and 100." << std::endl;
            }
        }
        
        // Method to calculate average grade
        double calculateAverage() {
            if (grades.empty()) {
                return 0.0;
            }
            
            double sum = 0.0;
            for (double grade : grades) {
                sum += grade;
            }
            
            return sum / grades.size();
        }
        
        // Method to get letter grade based on average
        char getLetterGrade() {
            double average = calculateAverage();
            
            if (average >= 90) {
                return 'A';
            } else if (average >= 80) {
                return 'B';
            } else if (average >= 70) {
                return 'C';
            } else if (average >= 60) {
                return 'D';
            } else {
                return 'F';
            }
        }
        
        // Method to display student information
        void displayInfo() {
            std::cout << "Student ID: " << id << std::endl;
            std::cout << "Name: " << name << std::endl;
            
            std::cout << "Grades: ";
            if (grades.empty()) {
                std::cout << "No grades recorded yet." << std::endl;
            } else {
                for (double grade : grades) {
                    std::cout << grade << " ";
                }
                std::cout << std::endl;
                
                std::cout << std::fixed << std::setprecision(2);
                std::cout << "Average Grade: " << calculateAverage() << std::endl;
                std::cout << "Letter Grade: " << getLetterGrade() << std::endl;
            }
        }
};

int main() {
    std::cout << "===== Student Class Example =====" << std::endl;
    
    // Create a student object
    Student student1;
    student1.name = "John Smith";
    student1.id = "S12345";
    
    // Display initial information
    std::cout << "\nInitial student information:" << std::endl;
    student1.displayInfo();
    
    // Add some grades
    std::cout << "\nAdding grades for " << student1.name << ":" << std::endl;
    student1.addGrade(85.5);
    student1.addGrade(92.0);
    student1.addGrade(78.5);
    student1.addGrade(90.0);
    
    // Try adding an invalid grade
    student1.addGrade(105.0);  // Should show an error
    
    // Display updated information
    std::cout << "\nUpdated student information:" << std::endl;
    student1.displayInfo();
    
    // Create another student
    Student student2;
    student2.name = "Emily Johnson";
    student2.id = "S67890";
    
    // Add different grades
    std::cout << "\nAdding grades for " << student2.name << ":" << std::endl;
    student2.addGrade(95.0);
    student2.addGrade(88.5);
    student2.addGrade(92.5);
    
    // Display second student's information
    std::cout << "\nInformation for " << student2.name << ":" << std::endl;
    student2.displayInfo();
    
    // Compare the two students
    std::cout << "\n===== Comparing Students =====" << std::endl;
    double avg1 = student1.calculateAverage();
    double avg2 = student2.calculateAverage();
    
    std::cout << student1.name << "'s average: " << avg1 << " (Grade " << student1.getLetterGrade() << ")" << std::endl;
    std::cout << student2.name << "'s average: " << avg2 << " (Grade " << student2.getLetterGrade() << ")" << std::endl;
    
    if (avg1 > avg2) {
        std::cout << student1.name << " has a higher average." << std::endl;
    } else if (avg2 > avg1) {
        std::cout << student2.name << " has a higher average." << std::endl;
    } else {
        std::cout << "Both students have the same average." << std::endl;
    }
    
    return 0;
}
