#include <iostream>
#include <string>

class Rectangle {
private:
    double length;
    double width;
    std::string color;
    int id;
    
    // Static member to keep track of the number of rectangles created
    static int nextId;
    
public:
    // Default constructor
    Rectangle() {
        length = 1.0;
        width = 1.0;
        color = "White";
        id = nextId++;
        std::cout << "Default constructor: Created Rectangle #" << id << std::endl;
    }
    
    // Parameterized constructor
    Rectangle(double l, double w) {
        length = (l > 0) ? l : 1.0;  // Validate input
        width = (w > 0) ? w : 1.0;   // Validate input
        color = "White";
        id = nextId++;
        std::cout << "Parameterized constructor: Created Rectangle #" << id 
                  << " with dimensions " << length << " x " << width << std::endl;
    }
    
    // Fully parameterized constructor
    Rectangle(double l, double w, std::string c) {
        length = (l > 0) ? l : 1.0;  // Validate input
        width = (w > 0) ? w : 1.0;   // Validate input
        color = c;
        id = nextId++;
        std::cout << "Fully parameterized constructor: Created " << color 
                  << " Rectangle #" << id << " with dimensions " 
                  << length << " x " << width << std::endl;
    }
    
    // Copy constructor
    Rectangle(const Rectangle &other) {
        length = other.length;
        width = other.width;
        color = other.color;
        id = nextId++;  // Assign a new ID
        std::cout << "Copy constructor: Created Rectangle #" << id 
                  << " as a copy of Rectangle #" << other.id << std::endl;
    }
    
    // Destructor
    ~Rectangle() {
        std::cout << "Destructor: Destroyed " << color << " Rectangle #" << id << std::endl;
    }
    
    // Getter methods
    double getLength() const {
        return length;
    }
    
    double getWidth() const {
        return width;
    }
    
    std::string getColor() const {
        return color;
    }
    
    int getId() const {
        return id;
    }
    
    // Setter methods
    void setLength(double l) {
        if (l > 0) {
            length = l;
        } else {
            std::cout << "Error: Length must be positive." << std::endl;
        }
    }
    
    void setWidth(double w) {
        if (w > 0) {
            width = w;
        } else {
            std::cout << "Error: Width must be positive." << std::endl;
        }
    }
    
    void setColor(std::string c) {
        color = c;
    }
    
    // Calculate area
    double calculateArea() const {
        return length * width;
    }
    
    // Calculate perimeter
    double calculatePerimeter() const {
        return 2 * (length + width);
    }
    
    // Display rectangle information
    void displayInfo() const {
        std::cout << "Rectangle #" << id << " Information:" << std::endl;
        std::cout << "Dimensions: " << length << " x " << width << std::endl;
        std::cout << "Color: " << color << std::endl;
        std::cout << "Area: " << calculateArea() << std::endl;
        std::cout << "Perimeter: " << calculatePerimeter() << std::endl;
    }
    
    // Static method to get the total number of rectangles created
    static int getTotalRectangles() {
        return nextId;
    }
};

// Initialize static member
int Rectangle::nextId = 1;

// Function that takes a Rectangle by value (triggers copy constructor)
void processRectangle(Rectangle rect) {
    std::cout << "Processing rectangle inside function:" << std::endl;
    rect.displayInfo();
    // When function ends, the rect object goes out of scope and destructor is called
}

int main() {
    std::cout << "===== Rectangle Class with Constructors and Destructors =====" << std::endl;
    
    // Using default constructor
    std::cout << "\n1. Creating rect1 with default constructor:" << std::endl;
    Rectangle rect1;
    rect1.displayInfo();
    
    // Using parameterized constructor with dimensions
    std::cout << "\n2. Creating rect2 with parameterized constructor:" << std::endl;
    Rectangle rect2(5.0, 3.0);
    rect2.displayInfo();
    
    // Using fully parameterized constructor
    std::cout << "\n3. Creating rect3 with fully parameterized constructor:" << std::endl;
    Rectangle rect3(4.0, 4.0, "Blue");
    rect3.displayInfo();
    
    // Using copy constructor
    std::cout << "\n4. Creating rect4 as a copy of rect3:" << std::endl;
    Rectangle rect4 = rect3;
    rect4.displayInfo();
    
    // Modifying the copied rectangle
    std::cout << "\n5. Modifying rect4:" << std::endl;
    rect4.setLength(6.0);
    rect4.setWidth(2.0);
    rect4.setColor("Red");
    rect4.displayInfo();
    
    // Show that the original rectangle is unchanged
    std::cout << "\n6. Original rect3 is unchanged:" << std::endl;
    rect3.displayInfo();
    
    // Demonstrating copy constructor with function call
    std::cout << "\n7. Passing rect2 to function (by value):" << std::endl;
    processRectangle(rect2);
    
    // Creating a rectangle in a nested block to demonstrate scope and destructor
    std::cout << "\n8. Creating rectangle in a nested block:" << std::endl;
    {
        Rectangle tempRect(7.0, 2.0, "Green");
        tempRect.displayInfo();
        std::cout << "End of nested block, tempRect will be destroyed" << std::endl;
    }  // Destructor for tempRect called here
    
    // Display total number of rectangles created
    std::cout << "\n9. Total number of rectangles created: " 
              << Rectangle::getTotalRectangles() - 1 << std::endl;
    
    std::cout << "\n10. End of main function, all remaining rectangles will be destroyed" << std::endl;
    
    return 0;
}  // Destructors for rect1, rect2, rect3, and rect4 called here
