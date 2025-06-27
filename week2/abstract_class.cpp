#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Abstract base class (interface)
class Drawable {
public:
    // Pure virtual function - makes this an abstract class
    virtual void draw() const = 0;
    
    // Virtual destructor
    virtual ~Drawable() {
        std::cout << "Drawable destructor called" << std::endl;
    }
};

// Another abstract base class (interface)
class Transformable {
public:
    // Pure virtual functions
    virtual void move(double x, double y) = 0;
    virtual void rotate(double angle) = 0;
    virtual void scale(double factor) = 0;
    
    // Virtual destructor
    virtual ~Transformable() {
        std::cout << "Transformable destructor called" << std::endl;
    }
};

// Concrete class implementing multiple interfaces
class GraphicObject : public Drawable, public Transformable {
protected:
    std::string name;
    double x, y;       // Position
    double rotation;   // Rotation angle in degrees
    double scaleFactor; // Scale factor
    
public:
    // Constructor
    GraphicObject(const std::string& objectName, double posX = 0, double posY = 0)
        : name(objectName), x(posX), y(posY), rotation(0), scaleFactor(1.0) {
        std::cout << "GraphicObject constructor called for " << name << std::endl;
    }
    
    // Destructor
    virtual ~GraphicObject() {
        std::cout << "GraphicObject destructor called for " << name << std::endl;
    }
    
    // Implementation of Transformable interface
    void move(double deltaX, double deltaY) override {
        x += deltaX;
        y += deltaY;
        std::cout << name << " moved to position (" << x << ", " << y << ")" << std::endl;
    }
    
    void rotate(double angle) override {
        rotation += angle;
        // Normalize rotation to 0-360 degrees
        while (rotation >= 360.0) rotation -= 360.0;
        while (rotation < 0.0) rotation += 360.0;
        
        std::cout << name << " rotated to " << rotation << " degrees" << std::endl;
    }
    
    void scale(double factor) override {
        if (factor > 0) {
            scaleFactor *= factor;
            std::cout << name << " scaled by factor " << factor 
                      << " (total scale: " << scaleFactor << ")" << std::endl;
        } else {
            std::cout << "Error: Scale factor must be positive." << std::endl;
        }
    }
    
    // Getter methods
    std::string getName() const { return name; }
    double getX() const { return x; }
    double getY() const { return y; }
    double getRotation() const { return rotation; }
    double getScaleFactor() const { return scaleFactor; }
    
    // Virtual method to get object type
    virtual std::string getType() const {
        return "GraphicObject";
    }
};

// Concrete derived class - Circle
class Circle : public GraphicObject {
private:
    double radius;
    std::string color;
    
public:
    // Constructor
    Circle(const std::string& name, double r, const std::string& c, 
           double posX = 0, double posY = 0)
        : GraphicObject(name, posX, posY), radius(r), color(c) {
        std::cout << "Circle constructor called for " << name << std::endl;
    }
    
    // Destructor
    ~Circle() override {
        std::cout << "Circle destructor called for " << name << std::endl;
    }
    
    // Implementation of Drawable interface
    void draw() const override {
        std::cout << "Drawing " << color << " circle '" << name 
                  << "' at (" << x << ", " << y << ") with radius " 
                  << (radius * scaleFactor) << ", rotation " << rotation << "°" << std::endl;
    }
    
    // Override scale to update radius
    void scale(double factor) override {
        GraphicObject::scale(factor);
    }
    
    // Circle-specific methods
    double getRadius() const {
        return radius * scaleFactor;
    }
    
    double getArea() const {
        return 3.14159 * radius * radius * scaleFactor * scaleFactor;
    }
    
    // Override getType
    std::string getType() const override {
        return "Circle";
    }
};

// Concrete derived class - Rectangle
class Rectangle : public GraphicObject {
private:
    double width;
    double height;
    std::string color;
    
public:
    // Constructor
    Rectangle(const std::string& name, double w, double h, const std::string& c,
              double posX = 0, double posY = 0)
        : GraphicObject(name, posX, posY), width(w), height(h), color(c) {
        std::cout << "Rectangle constructor called for " << name << std::endl;
    }
    
    // Destructor
    ~Rectangle() override {
        std::cout << "Rectangle destructor called for " << name << std::endl;
    }
    
    // Implementation of Drawable interface
    void draw() const override {
        std::cout << "Drawing " << color << " rectangle '" << name 
                  << "' at (" << x << ", " << y << ") with dimensions " 
                  << (width * scaleFactor) << "x" << (height * scaleFactor)
                  << ", rotation " << rotation << "°" << std::endl;
    }
    
    // Rectangle-specific methods
    double getWidth() const {
        return width * scaleFactor;
    }
    
    double getHeight() const {
        return height * scaleFactor;
    }
    
    double getArea() const {
        return width * height * scaleFactor * scaleFactor;
    }
    
    // Override getType
    std::string getType() const override {
        return "Rectangle";
    }
};

// Concrete derived class - Text
class Text : public GraphicObject {
private:
    std::string content;
    std::string fontName;
    int fontSize;
    std::string color;
    
public:
    // Constructor
    Text(const std::string& name, const std::string& text, const std::string& font,
         int size, const std::string& c, double posX = 0, double posY = 0)
        : GraphicObject(name, posX, posY), content(text), fontName(font), 
          fontSize(size), color(c) {
        std::cout << "Text constructor called for " << name << std::endl;
    }
    
    // Destructor
    ~Text() override {
        std::cout << "Text destructor called for " << name << std::endl;
    }
    
    // Implementation of Drawable interface
    void draw() const override {
        std::cout << "Drawing " << color << " text '" << name 
                  << "' at (" << x << ", " << y << ") with content \"" 
                  << content << "\" using font " << fontName << " size " 
                  << static_cast<int>(fontSize * scaleFactor)
                  << ", rotation " << rotation << "°" << std::endl;
    }
    
    // Text-specific methods
    void setText(const std::string& newText) {
        content = newText;
        std::cout << "Text content updated to: " << content << std::endl;
    }
    
    std::string getText() const {
        return content;
    }
    
    // Override scale to update font size
    void scale(double factor) override {
        GraphicObject::scale(factor);
    }
    
    // Override getType
    std::string getType() const override {
        return "Text";
    }
};

// Composite class - Group (implements Composite pattern)
class Group : public Drawable, public Transformable {
private:
    std::string name;
    std::vector<std::shared_ptr<GraphicObject>> objects;
    
public:
    // Constructor
    Group(const std::string& groupName) : name(groupName) {
        std::cout << "Group constructor called for " << name << std::endl;
    }
    
    // Destructor
    ~Group() override {
        std::cout << "Group destructor called for " << name << std::endl;
    }
    
    // Add an object to the group
    void addObject(std::shared_ptr<GraphicObject> object) {
        objects.push_back(object);
        std::cout << "Added " << object->getType() << " '" << object->getName() 
                  << "' to group '" << name << "'" << std::endl;
    }
    
    // Implementation of Drawable interface
    void draw() const override {
        std::cout << "Drawing group '" << name << "' with " 
                  << objects.size() << " objects:" << std::endl;
        
        for (const auto& object : objects) {
            object->draw();
        }
    }
    
    // Implementation of Transformable interface
    void move(double deltaX, double deltaY) override {
        std::cout << "Moving all objects in group '" << name << "':" << std::endl;
        
        for (auto& object : objects) {
            object->move(deltaX, deltaY);
        }
    }
    
    void rotate(double angle) override {
        std::cout << "Rotating all objects in group '" << name << "':" << std::endl;
        
        for (auto& object : objects) {
            object->rotate(angle);
        }
    }
    
    void scale(double factor) override {
        std::cout << "Scaling all objects in group '" << name << "':" << std::endl;
        
        for (auto& object : objects) {
            object->scale(factor);
        }
    }
    
    // Group-specific methods
    std::string getName() const {
        return name;
    }
    
    size_t getObjectCount() const {
        return objects.size();
    }
};

// Function to demonstrate using objects through interfaces
void drawAndTransform(Drawable& drawable, Transformable& transformable) {
    std::cout << "\n--- Drawing and transforming object ---" << std::endl;
    
    // Draw the object
    drawable.draw();
    
    // Transform the object
    transformable.move(10, 5);
    transformable.rotate(45);
    transformable.scale(1.5);
    
    // Draw again after transformation
    drawable.draw();
    
    std::cout << "--- End of drawing and transforming ---\n" << std::endl;
}

int main() {
    std::cout << "===== Abstract Classes and Interfaces Example =====" << std::endl;
    
    // Create objects
    std::cout << "\n1. Creating graphic objects:" << std::endl;
    auto circle = std::make_shared<Circle>("MyCircle", 5.0, "Red", 0, 0);
    auto rectangle = std::make_shared<Rectangle>("MyRectangle", 10.0, 7.0, "Blue", 20, 20);
    auto text = std::make_shared<Text>("MyText", "Hello, C++!", "Arial", 12, "Black", 50, 50);
    
    // Draw individual objects
    std::cout << "\n2. Drawing individual objects:" << std::endl;
    circle->draw();
    rectangle->draw();
    text->draw();
    
    // Transform individual objects
    std::cout << "\n3. Transforming individual objects:" << std::endl;
    circle->move(5, 5);
    circle->draw();
    
    rectangle->rotate(30);
    rectangle->draw();
    
    text->scale(2.0);
    text->draw();
    
    // Create a group and add objects to it
    std::cout << "\n4. Creating a group and adding objects:" << std::endl;
    Group graphicsGroup("MyGroup");
    graphicsGroup.addObject(circle);
    graphicsGroup.addObject(rectangle);
    
    // Draw the group
    std::cout << "\n5. Drawing the group:" << std::endl;
    graphicsGroup.draw();
    
    // Transform the group
    std::cout << "\n6. Transforming the group:" << std::endl;
    graphicsGroup.move(10, 10);
    graphicsGroup.rotate(15);
    
    // Draw the group again after transformation
    std::cout << "\n7. Drawing the group after transformation:" << std::endl;
    graphicsGroup.draw();
    
    // Demonstrate using objects through interfaces
    std::cout << "\n8. Using objects through interfaces:" << std::endl;
    drawAndTransform(*text, *text);
    drawAndTransform(graphicsGroup, graphicsGroup);
    
    // Text-specific operations
    std::cout << "\n9. Text-specific operations:" << std::endl;
    text->setText("Updated text content");
    text->draw();
    
    std::cout << "\n10. Program ending, objects will be destroyed." << std::endl;
    
    return 0;
}
