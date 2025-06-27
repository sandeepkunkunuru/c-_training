#include <iostream>
#include <string>

class Car {
private:
    std::string make;
    std::string model;
    int year;
    double price;
    
public:
    // Default constructor
    Car() {
        make = "Unknown";
        model = "Unknown";
        year = 0;
        price = 0.0;
        std::cout << "Default constructor called" << std::endl;
    }
    
    // Parameterized constructor
    Car(std::string carMake, std::string carModel, int carYear, double carPrice) {
        make = carMake;
        model = carModel;
        year = carYear;
        price = carPrice;
        std::cout << "Parameterized constructor called for " << make << " " << model << std::endl;
    }
    
    // Constructor with some default parameters
    Car(std::string carMake, std::string carModel, int carYear = 2023) {
        make = carMake;
        model = carModel;
        year = carYear;
        price = 0.0;
        std::cout << "Constructor with default parameters called for " << make << " " << model << std::endl;
    }
    
    // Copy constructor
    Car(const Car &other) {
        make = other.make;
        model = other.model;
        year = other.year;
        price = other.price;
        std::cout << "Copy constructor called for " << make << " " << model << std::endl;
    }
    
    // Destructor
    ~Car() {
        std::cout << "Destructor called for " << make << " " << model << std::endl;
    }
    
    // Method to display car information
    void displayInfo() {
        std::cout << "Car Information:" << std::endl;
        std::cout << "Make: " << make << std::endl;
        std::cout << "Model: " << model << std::endl;
        std::cout << "Year: " << year << std::endl;
        std::cout << "Price: $" << price << std::endl;
    }
    
    // Setter methods
    void setPrice(double newPrice) {
        if (newPrice >= 0) {
            price = newPrice;
        } else {
            std::cout << "Error: Price cannot be negative." << std::endl;
        }
    }
};

// Function that takes a Car object by value (triggers copy constructor)
void processCar(Car car) {
    std::cout << "Processing car inside function:" << std::endl;
    car.displayInfo();
    // When function ends, the car object goes out of scope and destructor is called
}

int main() {
    std::cout << "===== Constructors and Destructors Example =====" << std::endl;
    
    // Using default constructor
    std::cout << "\n1. Creating car1 with default constructor:" << std::endl;
    Car car1;
    car1.displayInfo();
    
    // Using parameterized constructor
    std::cout << "\n2. Creating car2 with parameterized constructor:" << std::endl;
    Car car2("Toyota", "Camry", 2022, 25000.0);
    car2.displayInfo();
    
    // Using constructor with default parameters
    std::cout << "\n3. Creating car3 with some default parameters:" << std::endl;
    Car car3("Honda", "Civic");  // Year will default to 2023
    car3.displayInfo();
    car3.setPrice(22000.0);
    std::cout << "After setting price:" << std::endl;
    car3.displayInfo();
    
    // Using copy constructor
    std::cout << "\n4. Creating car4 as a copy of car2:" << std::endl;
    Car car4 = car2;  // Copy constructor called
    car4.displayInfo();
    
    // Demonstrating copy constructor with function call
    std::cout << "\n5. Passing car2 to function (by value):" << std::endl;
    processCar(car2);
    
    // Creating a car in a nested block to demonstrate scope and destructor
    std::cout << "\n6. Creating car in a nested block:" << std::endl;
    {
        Car tempCar("Tesla", "Model 3", 2023, 45000.0);
        tempCar.displayInfo();
        std::cout << "End of nested block, tempCar will be destroyed" << std::endl;
    }  // Destructor for tempCar called here
    
    std::cout << "\n7. End of main function, all remaining cars will be destroyed" << std::endl;
    
    return 0;
}  // Destructors for car1, car2, car3, and car4 called here
