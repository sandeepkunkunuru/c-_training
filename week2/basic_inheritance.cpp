#include <iostream>
#include <string>

// Base class (parent class)
class Vehicle {
protected:
    std::string make;
    std::string model;
    int year;
    double price;
    
public:
    // Constructor
    Vehicle(std::string vMake, std::string vModel, int vYear, double vPrice)
        : make(vMake), model(vModel), year(vYear), price(vPrice) {
        std::cout << "Vehicle constructor called" << std::endl;
    }
    
    // Destructor
    virtual ~Vehicle() {
        std::cout << "Vehicle destructor called" << std::endl;
    }
    
    // Getter methods
    std::string getMake() const { return make; }
    std::string getModel() const { return model; }
    int getYear() const { return year; }
    double getPrice() const { return price; }
    
    // Setter methods
    void setPrice(double newPrice) {
        if (newPrice >= 0) {
            price = newPrice;
        } else {
            std::cout << "Error: Price cannot be negative." << std::endl;
        }
    }
    
    // Method to display vehicle information
    virtual void displayInfo() const {
        std::cout << year << " " << make << " " << model << std::endl;
        std::cout << "Price: $" << price << std::endl;
    }
    
    // Method to calculate depreciation
    virtual double calculateDepreciation(int currentYear) const {
        int age = currentYear - year;
        if (age <= 0) return 0.0;
        
        // Simple depreciation model: 10% per year, up to 80%
        double depreciationRate = age * 0.1;
        if (depreciationRate > 0.8) depreciationRate = 0.8;
        
        return price * depreciationRate;
    }
};

// Derived class (child class) - Car inherits from Vehicle
class Car : public Vehicle {
private:
    int numDoors;
    double fuelEfficiency;  // miles per gallon
    std::string bodyType;   // sedan, SUV, hatchback, etc.
    
public:
    // Constructor
    Car(std::string cMake, std::string cModel, int cYear, double cPrice,
        int cDoors, double cFuelEfficiency, std::string cBodyType)
        : Vehicle(cMake, cModel, cYear, cPrice),  // Call base class constructor
          numDoors(cDoors), fuelEfficiency(cFuelEfficiency), bodyType(cBodyType) {
        std::cout << "Car constructor called" << std::endl;
    }
    
    // Destructor
    ~Car() override {
        std::cout << "Car destructor called" << std::endl;
    }
    
    // Getter methods
    int getNumDoors() const { return numDoors; }
    double getFuelEfficiency() const { return fuelEfficiency; }
    std::string getBodyType() const { return bodyType; }
    
    // Method to calculate fuel cost for a given distance
    double calculateFuelCost(double distance, double fuelPrice) const {
        if (fuelEfficiency <= 0) return 0.0;
        return (distance / fuelEfficiency) * fuelPrice;
    }
    
    // Override displayInfo method
    void displayInfo() const override {
        // Call base class method
        Vehicle::displayInfo();
        
        // Add car-specific information
        std::cout << "Body Type: " << bodyType << std::endl;
        std::cout << "Number of Doors: " << numDoors << std::endl;
        std::cout << "Fuel Efficiency: " << fuelEfficiency << " mpg" << std::endl;
    }
    
    // Override calculateDepreciation method
    double calculateDepreciation(int currentYear) const override {
        // Cars depreciate faster than the base vehicle model
        int age = currentYear - year;
        if (age <= 0) return 0.0;
        
        // Cars depreciate 15% per year, up to 90%
        double depreciationRate = age * 0.15;
        if (depreciationRate > 0.9) depreciationRate = 0.9;
        
        return price * depreciationRate;
    }
};

// Another derived class - Motorcycle inherits from Vehicle
class Motorcycle : public Vehicle {
private:
    std::string type;      // sport, cruiser, touring, etc.
    int engineSize;        // in cc
    bool hasSidecar;
    
public:
    // Constructor
    Motorcycle(std::string mMake, std::string mModel, int mYear, double mPrice,
               std::string mType, int mEngineSize, bool mHasSidecar)
        : Vehicle(mMake, mModel, mYear, mPrice),  // Call base class constructor
          type(mType), engineSize(mEngineSize), hasSidecar(mHasSidecar) {
        std::cout << "Motorcycle constructor called" << std::endl;
    }
    
    // Destructor
    ~Motorcycle() override {
        std::cout << "Motorcycle destructor called" << std::endl;
    }
    
    // Getter methods
    std::string getType() const { return type; }
    int getEngineSize() const { return engineSize; }
    bool getHasSidecar() const { return hasSidecar; }
    
    // Override displayInfo method
    void displayInfo() const override {
        // Call base class method
        Vehicle::displayInfo();
        
        // Add motorcycle-specific information
        std::cout << "Type: " << type << std::endl;
        std::cout << "Engine Size: " << engineSize << " cc" << std::endl;
        std::cout << "Sidecar: " << (hasSidecar ? "Yes" : "No") << std::endl;
    }
    
    // Override calculateDepreciation method
    double calculateDepreciation(int currentYear) const override {
        // Motorcycles depreciate at a different rate
        int age = currentYear - year;
        if (age <= 0) return 0.0;
        
        // Motorcycles depreciate 12% per year, up to 85%
        double depreciationRate = age * 0.12;
        if (depreciationRate > 0.85) depreciationRate = 0.85;
        
        return price * depreciationRate;
    }
};

// Function to demonstrate polymorphism
void showVehicleInfo(const Vehicle& vehicle, int currentYear) {
    std::cout << "\n----- Vehicle Information -----" << std::endl;
    vehicle.displayInfo();
    
    double depreciation = vehicle.calculateDepreciation(currentYear);
    double currentValue = vehicle.getPrice() - depreciation;
    
    std::cout << "Original Price: $" << vehicle.getPrice() << std::endl;
    std::cout << "Depreciation: $" << depreciation << std::endl;
    std::cout << "Current Value: $" << currentValue << std::endl;
}

int main() {
    std::cout << "===== Basic Inheritance Example =====" << std::endl;
    
    // Current year for depreciation calculation
    const int currentYear = 2025;
    
    // Create a base Vehicle object
    std::cout << "\n1. Creating a base Vehicle:" << std::endl;
    Vehicle genericVehicle("Generic", "Transport", 2020, 15000.0);
    showVehicleInfo(genericVehicle, currentYear);
    
    // Create a Car object
    std::cout << "\n2. Creating a Car (derived from Vehicle):" << std::endl;
    Car sedan("Toyota", "Camry", 2019, 25000.0, 4, 32.5, "Sedan");
    showVehicleInfo(sedan, currentYear);
    
    // Demonstrate car-specific method
    double distance = 500.0;  // miles
    double fuelPrice = 3.50;  // dollars per gallon
    double fuelCost = sedan.calculateFuelCost(distance, fuelPrice);
    std::cout << "Fuel cost for " << distance << " miles at $" 
              << fuelPrice << "/gallon: $" << fuelCost << std::endl;
    
    // Create a Motorcycle object
    std::cout << "\n3. Creating a Motorcycle (derived from Vehicle):" << std::endl;
    Motorcycle sportBike("Honda", "CBR600RR", 2021, 12000.0, "Sport", 600, false);
    showVehicleInfo(sportBike, currentYear);
    
    // Demonstrate polymorphism with an array of pointers
    std::cout << "\n4. Demonstrating polymorphism with an array of Vehicle pointers:" << std::endl;
    
    // Create an array of Vehicle pointers
    Vehicle* vehicles[3];
    vehicles[0] = new Vehicle("Generic", "Transport", 2020, 15000.0);
    vehicles[1] = new Car("Ford", "Mustang", 2018, 35000.0, 2, 22.5, "Sports Car");
    vehicles[2] = new Motorcycle("Harley-Davidson", "Street Glide", 2022, 22000.0, "Cruiser", 1750, false);
    
    // Loop through the array and call methods polymorphically
    for (int i = 0; i < 3; i++) {
        std::cout << "\nVehicle " << (i + 1) << ":" << std::endl;
        vehicles[i]->displayInfo();
        
        double depreciation = vehicles[i]->calculateDepreciation(currentYear);
        std::cout << "Depreciation: $" << depreciation << std::endl;
        std::cout << "Current Value: $" << (vehicles[i]->getPrice() - depreciation) << std::endl;
    }
    
    // Clean up dynamically allocated objects
    for (int i = 0; i < 3; i++) {
        delete vehicles[i];
    }
    
    return 0;
}
