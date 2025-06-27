#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Base class with virtual functions
class Shape {
protected:
    std::string color;
    
public:
    // Constructor
    Shape(const std::string& c) : color(c) {
        std::cout << "Shape constructor called" << std::endl;
    }
    
    // Virtual destructor - essential for proper cleanup in polymorphic classes
    virtual ~Shape() {
        std::cout << "Shape destructor called" << std::endl;
    }
    
    // Pure virtual function - makes Shape an abstract class
    virtual double area() const = 0;
    
    // Pure virtual function
    virtual double perimeter() const = 0;
    
    // Virtual function with implementation
    virtual void draw() const {
        std::cout << "Drawing a " << color << " shape." << std::endl;
    }
    
    // Non-virtual function
    std::string getColor() const {
        return color;
    }
    
    // Virtual function to get shape type
    virtual std::string getType() const {
        return "Shape";
    }
    
    // Virtual function to scale the shape
    virtual void scale(double factor) = 0;
    
    // Friend function for output
    friend std::ostream& operator<<(std::ostream& os, const Shape& shape);
};

// Friend function implementation
std::ostream& operator<<(std::ostream& os, const Shape& shape) {
    os << shape.getType() << " (Color: " << shape.color << ")";
    os << "\n  Area: " << shape.area();
    os << "\n  Perimeter: " << shape.perimeter();
    return os;
}

// Derived class - Circle
class Circle : public Shape {
private:
    double radius;
    const double PI = 3.14159265358979323846;
    
public:
    // Constructor
    Circle(const std::string& c, double r) : Shape(c), radius(r) {
        std::cout << "Circle constructor called" << std::endl;
    }
    
    // Destructor
    ~Circle() override {
        std::cout << "Circle destructor called" << std::endl;
    }
    
    // Implementation of pure virtual function
    double area() const override {
        return PI * radius * radius;
    }
    
    // Implementation of pure virtual function
    double perimeter() const override {
        return 2 * PI * radius;
    }
    
    // Override draw method
    void draw() const override {
        std::cout << "Drawing a " << color << " circle with radius " << radius << std::endl;
    }
    
    // Override getType method
    std::string getType() const override {
        return "Circle";
    }
    
    // Implementation of scale method
    void scale(double factor) override {
        if (factor > 0) {
            radius *= factor;
            std::cout << "Circle scaled by factor " << factor << ". New radius: " << radius << std::endl;
        } else {
            std::cout << "Error: Scale factor must be positive." << std::endl;
        }
    }
    
    // Circle-specific method
    double getDiameter() const {
        return 2 * radius;
    }
};

// Derived class - Rectangle
class Rectangle : public Shape {
private:
    double width;
    double height;
    
public:
    // Constructor
    Rectangle(const std::string& c, double w, double h) : Shape(c), width(w), height(h) {
        std::cout << "Rectangle constructor called" << std::endl;
    }
    
    // Destructor
    ~Rectangle() override {
        std::cout << "Rectangle destructor called" << std::endl;
    }
    
    // Implementation of pure virtual function
    double area() const override {
        return width * height;
    }
    
    // Implementation of pure virtual function
    double perimeter() const override {
        return 2 * (width + height);
    }
    
    // Override draw method
    void draw() const override {
        std::cout << "Drawing a " << color << " rectangle with width " << width 
                  << " and height " << height << std::endl;
    }
    
    // Override getType method
    std::string getType() const override {
        return "Rectangle";
    }
    
    // Implementation of scale method
    void scale(double factor) override {
        if (factor > 0) {
            width *= factor;
            height *= factor;
            std::cout << "Rectangle scaled by factor " << factor 
                      << ". New dimensions: " << width << " x " << height << std::endl;
        } else {
            std::cout << "Error: Scale factor must be positive." << std::endl;
        }
    }
    
    // Rectangle-specific method
    bool isSquare() const {
        return width == height;
    }
};

// Derived class - Triangle
class Triangle : public Shape {
private:
    double side1;
    double side2;
    double side3;
    
public:
    // Constructor
    Triangle(const std::string& c, double s1, double s2, double s3) 
        : Shape(c), side1(s1), side2(s2), side3(s3) {
        std::cout << "Triangle constructor called" << std::endl;
    }
    
    // Destructor
    ~Triangle() override {
        std::cout << "Triangle destructor called" << std::endl;
    }
    
    // Implementation of pure virtual function
    double area() const override {
        // Using Heron's formula
        double s = (side1 + side2 + side3) / 2;
        return std::sqrt(s * (s - side1) * (s - side2) * (s - side3));
    }
    
    // Implementation of pure virtual function
    double perimeter() const override {
        return side1 + side2 + side3;
    }
    
    // Override draw method
    void draw() const override {
        std::cout << "Drawing a " << color << " triangle with sides " 
                  << side1 << ", " << side2 << ", " << side3 << std::endl;
    }
    
    // Override getType method
    std::string getType() const override {
        return "Triangle";
    }
    
    // Implementation of scale method
    void scale(double factor) override {
        if (factor > 0) {
            side1 *= factor;
            side2 *= factor;
            side3 *= factor;
            std::cout << "Triangle scaled by factor " << factor 
                      << ". New sides: " << side1 << ", " << side2 << ", " << side3 << std::endl;
        } else {
            std::cout << "Error: Scale factor must be positive." << std::endl;
        }
    }
    
    // Triangle-specific method
    bool isEquilateral() const {
        return (side1 == side2) && (side2 == side3);
    }
};

// Function to demonstrate runtime polymorphism
void processShape(const Shape& shape) {
    std::cout << "\nProcessing shape: " << shape.getType() << std::endl;
    std::cout << "Area: " << shape.area() << std::endl;
    std::cout << "Perimeter: " << shape.perimeter() << std::endl;
    shape.draw();
}

// Function to demonstrate polymorphism with pointers
void scaleAndDrawShape(Shape* shape, double factor) {
    std::cout << "\nBefore scaling: " << std::endl;
    shape->draw();
    std::cout << "Area: " << shape->area() << std::endl;
    
    shape->scale(factor);
    
    std::cout << "After scaling: " << std::endl;
    shape->draw();
    std::cout << "Area: " << shape->area() << std::endl;
}

int main() {
    std::cout << "===== Polymorphism Example =====" << std::endl;
    
    // Cannot instantiate an abstract class
    // Shape shape("Red");  // This would cause a compilation error
    
    // 1. Compile-time polymorphism (function overloading)
    std::cout << "\n1. Compile-time polymorphism (function overloading):" << std::endl;
    
    // Create derived class objects
    Circle circle("Blue", 5.0);
    Rectangle rectangle("Green", 4.0, 6.0);
    Triangle triangle("Yellow", 3.0, 4.0, 5.0);
    
    // 2. Runtime polymorphism with references
    std::cout << "\n2. Runtime polymorphism with references:" << std::endl;
    processShape(circle);
    processShape(rectangle);
    processShape(triangle);
    
    // 3. Runtime polymorphism with pointers
    std::cout << "\n3. Runtime polymorphism with pointers:" << std::endl;
    Shape* shapePtr1 = &circle;
    Shape* shapePtr2 = &rectangle;
    Shape* shapePtr3 = &triangle;
    
    shapePtr1->draw();
    shapePtr2->draw();
    shapePtr3->draw();
    
    // 4. Scaling shapes using polymorphism
    std::cout << "\n4. Scaling shapes using polymorphism:" << std::endl;
    scaleAndDrawShape(&circle, 2.0);
    scaleAndDrawShape(&rectangle, 1.5);
    scaleAndDrawShape(&triangle, 3.0);
    
    // 5. Using vector of pointers to demonstrate polymorphism
    std::cout << "\n5. Using vector of pointers to demonstrate polymorphism:" << std::endl;
    std::vector<Shape*> shapes;
    shapes.push_back(&circle);
    shapes.push_back(&rectangle);
    shapes.push_back(&triangle);
    
    for (const auto& shape : shapes) {
        std::cout << "\n" << *shape << std::endl;  // Using the overloaded << operator
    }
    
    // 6. Using smart pointers for better memory management
    std::cout << "\n6. Using smart pointers for better memory management:" << std::endl;
    
    // Create shapes with smart pointers
    std::vector<std::unique_ptr<Shape>> smartShapes;
    
    // Cannot use the existing objects as they are stack-allocated
    // Instead, create new heap-allocated objects
    smartShapes.push_back(std::make_unique<Circle>("Purple", 3.0));
    smartShapes.push_back(std::make_unique<Rectangle>("Orange", 2.0, 7.0));
    smartShapes.push_back(std::make_unique<Triangle>("Red", 5.0, 5.0, 5.0));
    
    for (const auto& shape : smartShapes) {
        shape->draw();
        std::cout << "Area: " << shape->area() << std::endl;
        std::cout << "Perimeter: " << shape->perimeter() << std::endl;
    }
    
    // No need to manually delete - smart pointers handle cleanup automatically
    
    // 7. Accessing derived class specific methods
    std::cout << "\n7. Accessing derived class specific methods:" << std::endl;
    
    // Need to downcast to access derived class specific methods
    std::cout << "Circle diameter: " << circle.getDiameter() << std::endl;
    std::cout << "Is rectangle a square? " << (rectangle.isSquare() ? "Yes" : "No") << std::endl;
    std::cout << "Is triangle equilateral? " << (triangle.isEquilateral() ? "Yes" : "No") << std::endl;
    
    // Dynamic casting example
    std::cout << "\n8. Dynamic casting example:" << std::endl;
    Shape* genericShape = &circle;
    
    // Try to cast to different types
    Circle* circlePtr = dynamic_cast<Circle*>(genericShape);
    Rectangle* rectanglePtr = dynamic_cast<Rectangle*>(genericShape);
    
    if (circlePtr) {
        std::cout << "Successfully cast to Circle. Diameter: " << circlePtr->getDiameter() << std::endl;
    } else {
        std::cout << "Could not cast to Circle." << std::endl;
    }
    
    if (rectanglePtr) {
        std::cout << "Successfully cast to Rectangle." << std::endl;
    } else {
        std::cout << "Could not cast to Rectangle." << std::endl;
    }
    
    return 0;
}
