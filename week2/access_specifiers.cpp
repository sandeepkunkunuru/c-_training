#include <iostream>
#include <string>

// Class demonstrating different access specifiers
class Employee {
private:
    // Private members - only accessible within the class
    std::string socialSecurityNumber;
    double salary;
    
protected:
    // Protected members - accessible within the class and derived classes
    std::string department;
    int yearsOfService;
    
public:
    // Public members - accessible from anywhere
    std::string name;
    std::string position;
    int employeeId;
    
    // Constructor
    Employee(std::string empName, int empId, std::string empPosition, 
             std::string empDepartment, double empSalary, std::string ssn) {
        name = empName;
        employeeId = empId;
        position = empPosition;
        department = empDepartment;
        salary = empSalary;
        socialSecurityNumber = ssn;
        yearsOfService = 0;
    }
    
    // Public methods to access and modify private members (getters and setters)
    double getSalary() const {
        return salary;
    }
    
    void setSalary(double newSalary) {
        if (newSalary >= 0) {
            salary = newSalary;
        } else {
            std::cout << "Error: Salary cannot be negative." << std::endl;
        }
    }
    
    // Only return last 4 digits of SSN for security
    std::string getLastFourSSN() const {
        if (socialSecurityNumber.length() >= 4) {
            return "XXX-XX-" + socialSecurityNumber.substr(socialSecurityNumber.length() - 4);
        }
        return "Invalid SSN";
    }
    
    // Protected members can be accessed by public methods
    std::string getDepartment() const {
        return department;
    }
    
    void setDepartment(std::string newDepartment) {
        department = newDepartment;
    }
    
    int getYearsOfService() const {
        return yearsOfService;
    }
    
    void incrementYearsOfService() {
        yearsOfService++;
    }
    
    // Method to display employee information
    void displayInfo() const {
        std::cout << "Employee Information:" << std::endl;
        std::cout << "ID: " << employeeId << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Position: " << position << std::endl;
        std::cout << "Department: " << department << std::endl;
        std::cout << "Years of Service: " << yearsOfService << std::endl;
        std::cout << "Salary: $" << salary << std::endl;
        std::cout << "SSN: " << getLastFourSSN() << std::endl;
    }
};

// Derived class to demonstrate protected inheritance
class Manager : public Employee {
private:
    int teamSize;
    double bonus;
    
public:
    // Constructor
    Manager(std::string empName, int empId, std::string empDepartment, 
            double empSalary, std::string ssn, int empTeamSize)
        : Employee(empName, empId, "Manager", empDepartment, empSalary, ssn) {
        teamSize = empTeamSize;
        bonus = 0.0;
    }
    
    // Methods specific to Manager
    void setTeamSize(int size) {
        if (size >= 0) {
            teamSize = size;
        } else {
            std::cout << "Error: Team size cannot be negative." << std::endl;
        }
    }
    
    int getTeamSize() const {
        return teamSize;
    }
    
    void setBonus(double amount) {
        if (amount >= 0) {
            bonus = amount;
        } else {
            std::cout << "Error: Bonus cannot be negative." << std::endl;
        }
    }
    
    double getBonus() const {
        return bonus;
    }
    
    // Method to calculate total compensation (salary + bonus)
    double getTotalCompensation() const {
        return getSalary() + bonus;
    }
    
    // Override displayInfo to include manager-specific information
    void displayInfo() const {
        // Call base class method
        Employee::displayInfo();
        
        // Add manager-specific information
        std::cout << "Team Size: " << teamSize << std::endl;
        std::cout << "Bonus: $" << bonus << std::endl;
        std::cout << "Total Compensation: $" << getTotalCompensation() << std::endl;
    }
    
    // Method to demonstrate access to protected members from base class
    void promote() {
        yearsOfService++;  // Can access protected member from base class
        std::cout << name << " has been promoted! Years of service increased to " 
                  << yearsOfService << "." << std::endl;
        
        // Cannot access private members of base class
        // socialSecurityNumber = "123-45-6789";  // This would cause a compilation error
    }
};

// Friend function demonstration
class Department {
private:
    std::string name;
    std::string location;
    int employeeCount;
    
    // Private method
    void updateEmployeeCount(int count) {
        employeeCount = count;
    }
    
public:
    Department(std::string deptName, std::string deptLocation) {
        name = deptName;
        location = deptLocation;
        employeeCount = 0;
    }
    
    std::string getName() const {
        return name;
    }
    
    std::string getLocation() const {
        return location;
    }
    
    int getEmployeeCount() const {
        return employeeCount;
    }
    
    // Declare a friend function
    friend void transferEmployee(Employee& emp, Department& from, Department& to);
};

// Friend function definition
void transferEmployee(Employee& emp, Department& from, Department& to) {
    // Friend function can access private members of Department
    from.updateEmployeeCount(from.employeeCount - 1);
    to.updateEmployeeCount(to.employeeCount + 1);
    
    // Update the employee's department
    emp.setDepartment(to.name);
    
    std::cout << emp.name << " has been transferred from " 
              << from.name << " to " << to.name << "." << std::endl;
}

int main() {
    std::cout << "===== Access Specifiers and Encapsulation Example =====" << std::endl;
    
    // Create an employee
    std::cout << "\n1. Creating a regular employee:" << std::endl;
    Employee emp1("John Smith", 1001, "Software Developer", "Engineering", 75000.0, "123-45-6789");
    emp1.displayInfo();
    
    // Demonstrate public access
    std::cout << "\n2. Accessing public members directly:" << std::endl;
    std::cout << "Name: " << emp1.name << std::endl;
    std::cout << "Position: " << emp1.position << std::endl;
    emp1.position = "Senior Software Developer";
    std::cout << "Updated position: " << emp1.position << std::endl;
    
    // Demonstrate private access through public methods
    std::cout << "\n3. Accessing private members through public methods:" << std::endl;
    std::cout << "Salary: $" << emp1.getSalary() << std::endl;
    std::cout << "SSN: " << emp1.getLastFourSSN() << std::endl;
    
    emp1.setSalary(80000.0);
    std::cout << "Updated salary: $" << emp1.getSalary() << std::endl;
    
    // Demonstrate protected access through inheritance
    std::cout << "\n4. Creating a manager (derived class):" << std::endl;
    Manager mgr1("Jane Doe", 2001, "Engineering", 100000.0, "987-65-4321", 5);
    mgr1.displayInfo();
    
    // Manager can access protected members of Employee
    std::cout << "\n5. Demonstrating protected member access through inheritance:" << std::endl;
    mgr1.promote();
    mgr1.displayInfo();
    
    // Demonstrate friend function
    std::cout << "\n6. Demonstrating friend function:" << std::endl;
    Department engineering("Engineering", "Building A");
    Department marketing("Marketing", "Building B");
    
    std::cout << "Initial departments:" << std::endl;
    std::cout << "Engineering employee count: " << engineering.getEmployeeCount() << std::endl;
    std::cout << "Marketing employee count: " << marketing.getEmployeeCount() << std::endl;
    
    transferEmployee(emp1, engineering, marketing);
    
    std::cout << "After transfer:" << std::endl;
    std::cout << "Engineering employee count: " << engineering.getEmployeeCount() << std::endl;
    std::cout << "Marketing employee count: " << marketing.getEmployeeCount() << std::endl;
    std::cout << "Employee's new department: " << emp1.getDepartment() << std::endl;
    
    return 0;
}
