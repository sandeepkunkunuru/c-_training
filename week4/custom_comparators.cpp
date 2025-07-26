/*
 * Custom Comparators and Hash Functions
 * 
 * This program demonstrates how to create and use custom comparators and hash functions
 * with STL containers. This is essential for working with user-defined types and
 * implementing custom sorting/hashing logic.
 * 
 * Key Learning Points:
 * - Function objects vs lambda comparators
 * - Custom hash functions for unordered containers
 * - Multi-criteria sorting
 * - Performance implications of different approaches
 */

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <string>
#include <iomanip>

// Example class for demonstration
struct Person {
    std::string name;
    int age;
    double salary;
    std::string department;
    
    Person(const std::string& n, int a, double s, const std::string& d)
        : name(n), age(a), salary(s), department(d) {}
    
    // Default equality operator (needed for unordered containers)
    bool operator==(const Person& other) const {
        return name == other.name && age == other.age && 
               salary == other.salary && department == other.department;
    }
    
    void print() const {
        std::cout << std::setw(15) << name 
                  << std::setw(5) << age 
                  << std::setw(10) << std::fixed << std::setprecision(0) << salary 
                  << std::setw(15) << department << std::endl;
    }
};

// 1. Function Object Comparators
struct PersonAgeComparator {
    bool operator()(const Person& a, const Person& b) const {
        return a.age < b.age;
    }
};

struct PersonSalaryComparator {
    bool operator()(const Person& a, const Person& b) const {
        return a.salary < b.salary;
    }
};

// Multi-criteria comparator
struct PersonMultiComparator {
    bool operator()(const Person& a, const Person& b) const {
        if (a.department != b.department) {
            return a.department < b.department;
        }
        if (a.salary != b.salary) {
            return a.salary > b.salary; // Higher salary first within department
        }
        return a.age < b.age; // Younger first if same salary
    }
};

// 2. Custom Hash Functions
struct PersonHasher {
    std::size_t operator()(const Person& p) const {
        // Combine hash values of different fields
        std::size_t h1 = std::hash<std::string>{}(p.name);
        std::size_t h2 = std::hash<int>{}(p.age);
        std::size_t h3 = std::hash<double>{}(p.salary);
        std::size_t h4 = std::hash<std::string>{}(p.department);
        
        // Simple hash combination (there are better methods)
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
    }
};

// Advanced hash combination using boost-style hash_combine
template<class T>
inline void hash_combine(std::size_t& seed, const T& v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

struct PersonAdvancedHasher {
    std::size_t operator()(const Person& p) const {
        std::size_t seed = 0;
        hash_combine(seed, p.name);
        hash_combine(seed, p.age);
        hash_combine(seed, p.salary);
        hash_combine(seed, p.department);
        return seed;
    }
};

// 3. Generic Comparator Factory
template<typename T, typename MemberType>
class MemberComparator {
private:
    MemberType T::*member;
    bool ascending;
    
public:
    MemberComparator(MemberType T::*m, bool asc = true) : member(m), ascending(asc) {}
    
    bool operator()(const T& a, const T& b) const {
        if (ascending) {
            return a.*member < b.*member;
        } else {
            return a.*member > b.*member;
        }
    }
};

// Helper function to create member comparators
template<typename T, typename MemberType>
auto make_member_comparator(MemberType T::*member, bool ascending = true) {
    return MemberComparator<T, MemberType>(member, ascending);
}

void demonstrateBasicComparators() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Basic Comparator Demonstrations" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::vector<Person> people = {
        {"Alice", 30, 75000, "Engineering"},
        {"Bob", 25, 65000, "Marketing"},
        {"Charlie", 35, 85000, "Engineering"},
        {"Diana", 28, 70000, "HR"},
        {"Eve", 32, 80000, "Marketing"},
        {"Frank", 29, 72000, "Engineering"}
    };
    
    std::cout << "\nOriginal data:" << std::endl;
    std::cout << std::setw(15) << "Name" << std::setw(5) << "Age" 
              << std::setw(10) << "Salary" << std::setw(15) << "Department" << std::endl;
    std::cout << std::string(45, '-') << std::endl;
    for (const auto& person : people) {
        person.print();
    }
    
    // Sort by age using function object
    auto peopleByAge = people;
    std::sort(peopleByAge.begin(), peopleByAge.end(), PersonAgeComparator{});
    
    std::cout << "\nSorted by age (function object):" << std::endl;
    std::cout << std::setw(15) << "Name" << std::setw(5) << "Age" 
              << std::setw(10) << "Salary" << std::setw(15) << "Department" << std::endl;
    std::cout << std::string(45, '-') << std::endl;
    for (const auto& person : peopleByAge) {
        person.print();
    }
    
    // Sort by salary using lambda
    auto peopleBySalary = people;
    std::sort(peopleBySalary.begin(), peopleBySalary.end(),
              [](const Person& a, const Person& b) {
                  return a.salary > b.salary; // Descending order
              });
    
    std::cout << "\nSorted by salary (lambda, descending):" << std::endl;
    std::cout << std::setw(15) << "Name" << std::setw(5) << "Age" 
              << std::setw(10) << "Salary" << std::setw(15) << "Department" << std::endl;
    std::cout << std::string(45, '-') << std::endl;
    for (const auto& person : peopleBySalary) {
        person.print();
    }
    
    // Multi-criteria sorting
    auto peopleMulti = people;
    std::sort(peopleMulti.begin(), peopleMulti.end(), PersonMultiComparator{});
    
    std::cout << "\nSorted by department, then salary (desc), then age:" << std::endl;
    std::cout << std::setw(15) << "Name" << std::setw(5) << "Age" 
              << std::setw(10) << "Salary" << std::setw(15) << "Department" << std::endl;
    std::cout << std::string(45, '-') << std::endl;
    for (const auto& person : peopleMulti) {
        person.print();
    }
}

void demonstrateGenericComparators() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Generic Comparator Demonstrations" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::vector<Person> people = {
        {"Alice", 30, 75000, "Engineering"},
        {"Bob", 25, 65000, "Marketing"},
        {"Charlie", 35, 85000, "Engineering"},
        {"Diana", 28, 70000, "HR"}
    };
    
    // Using generic member comparator
    auto peopleByName = people;
    std::sort(peopleByName.begin(), peopleByName.end(),
              make_member_comparator(&Person::name));
    
    std::cout << "\nSorted by name (generic comparator):" << std::endl;
    std::cout << std::setw(15) << "Name" << std::setw(5) << "Age" 
              << std::setw(10) << "Salary" << std::setw(15) << "Department" << std::endl;
    std::cout << std::string(45, '-') << std::endl;
    for (const auto& person : peopleByName) {
        person.print();
    }
    
    // Using std::function for dynamic comparator selection
    std::map<std::string, std::function<bool(const Person&, const Person&)>> comparators = {
        {"age", [](const Person& a, const Person& b) { return a.age < b.age; }},
        {"salary", [](const Person& a, const Person& b) { return a.salary < b.salary; }},
        {"name", [](const Person& a, const Person& b) { return a.name < b.name; }},
        {"department", [](const Person& a, const Person& b) { return a.department < b.department; }}
    };
    
    std::string sortBy = "salary";
    auto peopleDynamic = people;
    std::sort(peopleDynamic.begin(), peopleDynamic.end(), comparators[sortBy]);
    
    std::cout << "\nSorted by " << sortBy << " (dynamic comparator):" << std::endl;
    std::cout << std::setw(15) << "Name" << std::setw(5) << "Age" 
              << std::setw(10) << "Salary" << std::setw(15) << "Department" << std::endl;
    std::cout << std::string(45, '-') << std::endl;
    for (const auto& person : peopleDynamic) {
        person.print();
    }
}

void demonstrateSetWithComparators() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Set with Custom Comparators" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    // Set with custom comparator
    std::set<Person, PersonAgeComparator> peopleByAge;
    std::set<Person, PersonSalaryComparator> peopleBySalary;
    
    std::vector<Person> people = {
        {"Alice", 30, 75000, "Engineering"},
        {"Bob", 25, 65000, "Marketing"},
        {"Charlie", 35, 85000, "Engineering"},
        {"Diana", 28, 70000, "HR"}
    };
    
    for (const auto& person : people) {
        peopleByAge.insert(person);
        peopleBySalary.insert(person);
    }
    
    std::cout << "\nSet ordered by age:" << std::endl;
    std::cout << std::setw(15) << "Name" << std::setw(5) << "Age" 
              << std::setw(10) << "Salary" << std::setw(15) << "Department" << std::endl;
    std::cout << std::string(45, '-') << std::endl;
    for (const auto& person : peopleByAge) {
        person.print();
    }
    
    std::cout << "\nSet ordered by salary:" << std::endl;
    std::cout << std::setw(15) << "Name" << std::setw(5) << "Age" 
              << std::setw(10) << "Salary" << std::setw(15) << "Department" << std::endl;
    std::cout << std::string(45, '-') << std::endl;
    for (const auto& person : peopleBySalary) {
        person.print();
    }
}

void demonstrateHashFunctions() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Custom Hash Functions" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    // Unordered set with custom hash and equality
    std::unordered_set<Person, PersonHasher> uniquePeople;
    
    std::vector<Person> people = {
        {"Alice", 30, 75000, "Engineering"},
        {"Bob", 25, 65000, "Marketing"},
        {"Alice", 30, 75000, "Engineering"}, // Duplicate
        {"Charlie", 35, 85000, "Engineering"},
        {"Bob", 25, 65000, "Marketing"} // Duplicate
    };
    
    std::cout << "\nInserting people into unordered_set (duplicates will be ignored):" << std::endl;
    for (const auto& person : people) {
        auto result = uniquePeople.insert(person);
        std::cout << "Inserting " << person.name << ": " 
                  << (result.second ? "SUCCESS" : "DUPLICATE") << std::endl;
    }
    
    std::cout << "\nUnique people in set:" << std::endl;
    std::cout << std::setw(15) << "Name" << std::setw(5) << "Age" 
              << std::setw(10) << "Salary" << std::setw(15) << "Department" << std::endl;
    std::cout << std::string(45, '-') << std::endl;
    for (const auto& person : uniquePeople) {
        person.print();
    }
    
    // Demonstrate hash distribution
    std::cout << "\nHash values (first 10 characters of hex):" << std::endl;
    PersonHasher hasher;
    PersonAdvancedHasher advancedHasher;
    
    for (const auto& person : uniquePeople) {
        std::size_t hash1 = hasher(person);
        std::size_t hash2 = advancedHasher(person);
        std::cout << std::setw(15) << person.name 
                  << " Basic: " << std::hex << (hash1 & 0xFFFFFFFF)
                  << " Advanced: " << std::hex << (hash2 & 0xFFFFFFFF) << std::dec << std::endl;
    }
}

void demonstrateUnorderedMapWithCustomHash() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Unordered Map with Custom Hash" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    // Using Person as key in unordered_map
    std::unordered_map<Person, std::string, PersonAdvancedHasher> personRoles;
    
    std::vector<Person> people = {
        {"Alice", 30, 75000, "Engineering"},
        {"Bob", 25, 65000, "Marketing"},
        {"Charlie", 35, 85000, "Engineering"},
        {"Diana", 28, 70000, "HR"}
    };
    
    // Assign roles
    personRoles[people[0]] = "Senior Developer";
    personRoles[people[1]] = "Marketing Specialist";
    personRoles[people[2]] = "Lead Engineer";
    personRoles[people[3]] = "HR Manager";
    
    std::cout << "\nPerson -> Role mapping:" << std::endl;
    std::cout << std::setw(15) << "Name" << std::setw(5) << "Age" 
              << std::setw(10) << "Salary" << std::setw(15) << "Department"
              << std::setw(20) << "Role" << std::endl;
    std::cout << std::string(65, '-') << std::endl;
    
    for (const auto& [person, role] : personRoles) {
        person.print();
        std::cout << std::setw(20) << role << std::endl;
    }
    
    // Lookup performance test
    std::cout << "\nLookup test:" << std::endl;
    Person lookupPerson = people[1]; // Bob
    auto it = personRoles.find(lookupPerson);
    if (it != personRoles.end()) {
        std::cout << "Found " << lookupPerson.name << " with role: " << it->second << std::endl;
    } else {
        std::cout << "Person not found!" << std::endl;
    }
}

void demonstratePerformanceComparison() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Performance Comparison: Function Objects vs Lambdas" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    const size_t size = 100000;
    std::vector<Person> people;
    
    // Generate test data
    std::vector<std::string> names = {"Alice", "Bob", "Charlie", "Diana", "Eve", "Frank"};
    std::vector<std::string> departments = {"Engineering", "Marketing", "HR", "Finance"};
    
    for (size_t i = 0; i < size; ++i) {
        people.emplace_back(
            names[i % names.size()] + std::to_string(i),
            20 + (i % 40),
            50000 + (i % 50000),
            departments[i % departments.size()]
        );
    }
    
    auto measureTime = [](auto func) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count() / 1000.0; // Return milliseconds
    };
    
    // Test function object
    auto people1 = people;
    double funcObjTime = measureTime([&]() {
        std::sort(people1.begin(), people1.end(), PersonAgeComparator{});
    });
    
    // Test lambda
    auto people2 = people;
    double lambdaTime = measureTime([&]() {
        std::sort(people2.begin(), people2.end(), 
                  [](const Person& a, const Person& b) { return a.age < b.age; });
    });
    
    // Test generic comparator
    auto people3 = people;
    double genericTime = measureTime([&]() {
        std::sort(people3.begin(), people3.end(), 
                  make_member_comparator(&Person::age));
    });
    
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Sorting " << size << " Person objects by age:" << std::endl;
    std::cout << "Function Object: " << funcObjTime << " ms" << std::endl;
    std::cout << "Lambda:          " << lambdaTime << " ms" << std::endl;
    std::cout << "Generic:         " << genericTime << " ms" << std::endl;
    
    std::cout << "\nNote: Performance differences are usually minimal due to compiler optimizations." << std::endl;
    std::cout << "Choose based on readability and maintainability requirements." << std::endl;
}

int main() {
    std::cout << "===== Custom Comparators and Hash Functions =====" << std::endl;
    std::cout << "This program demonstrates various ways to customize sorting and hashing" << std::endl;
    std::cout << "behavior for user-defined types in STL containers." << std::endl;
    
    demonstrateBasicComparators();
    demonstrateGenericComparators();
    demonstrateSetWithComparators();
    demonstrateHashFunctions();
    demonstrateUnorderedMapWithCustomHash();
    demonstratePerformanceComparison();
    
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "Key Takeaways:" << std::endl;
    std::cout << "1. Function objects are reusable and can maintain state" << std::endl;
    std::cout << "2. Lambdas are convenient for one-off comparisons" << std::endl;
    std::cout << "3. Generic comparators provide flexibility" << std::endl;
    std::cout << "4. Custom hash functions enable using custom types as keys" << std::endl;
    std::cout << "5. Good hash functions distribute values evenly" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    return 0;
}
