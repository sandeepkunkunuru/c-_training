#include <iostream>
#include <string>

// Define a simple class
class Person {
    // Data members (attributes)
    public:
        std::string name;
        int age;
    
    // Member functions (methods)
    public:
        // Method to display person information
        void displayInfo() {
            std::cout << "Name: " << name << std::endl;
            std::cout << "Age: " << age << " years old" << std::endl;
        }
        
        // Method to have the person greet
        void greet() {
            std::cout << "Hello, my name is " << name << "!" << std::endl;
        }
        
        // Method to check if the person is an adult
        bool isAdult() {
            return age >= 18;
        }
};

int main() {
    std::cout << "===== Basic Class Example =====" << std::endl;
    
    // Create an object of the Person class
    Person person1;
    
    // Set values for the object's attributes
    person1.name = "Alice";
    person1.age = 25;
    
    // Call the object's methods
    std::cout << "\nInformation about person1:" << std::endl;
    person1.displayInfo();
    person1.greet();
    
    if (person1.isAdult()) {
        std::cout << person1.name << " is an adult." << std::endl;
    } else {
        std::cout << person1.name << " is not an adult." << std::endl;
    }
    
    // Create another object of the Person class
    Person person2;
    
    // Set values for the second object
    person2.name = "Bob";
    person2.age = 15;
    
    // Call the second object's methods
    std::cout << "\nInformation about person2:" << std::endl;
    person2.displayInfo();
    person2.greet();
    
    if (person2.isAdult()) {
        std::cout << person2.name << " is an adult." << std::endl;
    } else {
        std::cout << person2.name << " is not an adult." << std::endl;
    }
    
    // Demonstrate that objects are independent
    std::cout << "\nChanging person1's age..." << std::endl;
    person1.age = 30;
    
    std::cout << "Updated information about person1:" << std::endl;
    person1.displayInfo();
    
    std::cout << "Information about person2 (unchanged):" << std::endl;
    person2.displayInfo();
    
    return 0;
}
