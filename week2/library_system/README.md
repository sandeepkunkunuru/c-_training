# Library Management System

This is a mini-project for Week 2 of the C++ training program, focusing on Object-Oriented Programming concepts.

## Overview

The Library Management System demonstrates key OOP concepts including:
- Classes and Objects
- Inheritance
- Polymorphism
- Encapsulation
- Abstract Classes and Interfaces

## Features

- Add, remove, and search for library items
- Support for different item types (Books, DVDs, Journals)
- Check out and return items
- Track borrower information and due dates
- Display library statistics
- Interactive menu-based interface

## Class Structure

- `LibraryItem` (Abstract Base Class)
  - `Book` (Derived Class)
  - `DVD` (Derived Class)
  - `Journal` (Derived Class)
- `Library` (Management Class)

## Compilation and Execution

To compile the program:

```bash
g++ -std=c++17 main.cpp -o library_system
```

To run the program:

```bash
./library_system
```

## Usage

The program provides a menu-driven interface with the following options:
1. Add a new item (Book, DVD, or Journal)
2. Remove an item
3. Search for an item by title
4. Display all items
5. Check out an item
6. Return an item
7. Display checked out items
8. Display library statistics
9. Exit

## OOP Concepts Demonstrated

### Inheritance
- `Book`, `DVD`, and `Journal` classes inherit from the `LibraryItem` base class
- Each derived class extends the base functionality with type-specific attributes and behaviors

### Polymorphism
- Virtual functions in the base class are overridden in derived classes
- Objects of different derived classes can be stored and accessed through base class pointers
- Runtime behavior depends on the actual object type

### Encapsulation
- Data members are private/protected
- Public interfaces control access to internal data
- Implementation details are hidden from users

### Abstraction
- `LibraryItem` is an abstract base class with pure virtual functions
- Defines a common interface for all library items

## Learning Outcomes

Through this project, you will gain practical experience with:
- Designing class hierarchies
- Implementing inheritance relationships
- Using polymorphism for flexible code
- Applying encapsulation for data protection
- Working with abstract classes and interfaces
- Managing objects with smart pointers
- Building a complete object-oriented application
