#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <deque>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iterator>
#include <random>
#include <chrono>
#include <iomanip>

// Custom data structure for demonstrating algorithms
struct Person {
    std::string name;
    int age;
    double salary;
    
    Person(std::string n, int a, double s) : name(std::move(n)), age(a), salary(s) {}
    
    // For sorting and comparison
    bool operator<(const Person& other) const {
        return age < other.age;
    }
    
    // For equality comparison
    bool operator==(const Person& other) const {
        return name == other.name && age == other.age && salary == other.salary;
    }
};

// Output stream operator for Person
std::ostream& operator<<(std::ostream& os, const Person& p) {
    os << "{" << p.name << ", " << p.age << ", $" << std::fixed << std::setprecision(2) << p.salary << "}";
    return os;
}

// Helper function to print any container
template<typename Container>
void printContainer(const std::string& name, const Container& container) {
    std::cout << name << ": ";
    for (const auto& item : container) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

// Helper function to print a container of pairs
template<typename Container>
void printPairContainer(const std::string& name, const Container& container) {
    std::cout << name << ": ";
    for (const auto& [first, second] : container) {
        std::cout << "(" << first << "," << second << ") ";
    }
    std::cout << std::endl;
}

int main() {
    // Seed for random number generation
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(static_cast<unsigned int>(seed));
    
    std::cout << "===== Non-modifying Sequence Operations =====" << std::endl;
    
    // Create a vector for demonstration
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    printContainer("Original vector", v);
    
    // all_of, any_of, none_of
    bool allPositive = std::all_of(v.begin(), v.end(), [](int x) { return x > 0; });
    bool anyEven = std::any_of(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
    bool noneNegative = std::none_of(v.begin(), v.end(), [](int x) { return x < 0; });
    
    std::cout << "All positive? " << std::boolalpha << allPositive << std::endl;
    std::cout << "Any even? " << anyEven << std::endl;
    std::cout << "None negative? " << noneNegative << std::endl;
    
    // for_each
    std::cout << "\nfor_each (print squares): ";
    std::for_each(v.begin(), v.end(), [](int x) { std::cout << x * x << " "; });
    std::cout << std::endl;
    
    // count, count_if
    int countFives = std::count(v.begin(), v.end(), 5);
    int countOdd = std::count_if(v.begin(), v.end(), [](int x) { return x % 2 != 0; });
    
    std::cout << "Count of 5: " << countFives << std::endl;
    std::cout << "Count of odd numbers: " << countOdd << std::endl;
    
    // find, find_if, find_if_not
    auto it1 = std::find(v.begin(), v.end(), 9);
    auto it2 = std::find_if(v.begin(), v.end(), [](int x) { return x > 5; });
    auto it3 = std::find_if_not(v.begin(), v.end(), [](int x) { return x < 5; });
    
    if (it1 != v.end()) std::cout << "Found 9 at position: " << std::distance(v.begin(), it1) << std::endl;
    if (it2 != v.end()) std::cout << "First element > 5: " << *it2 << " at position: " << std::distance(v.begin(), it2) << std::endl;
    if (it3 != v.end()) std::cout << "First element not < 5: " << *it3 << " at position: " << std::distance(v.begin(), it3) << std::endl;
    
    // adjacent_find
    auto adjIt = std::adjacent_find(v.begin(), v.end());
    if (adjIt != v.end()) {
        std::cout << "Adjacent equal elements: " << *adjIt << " and " << *(adjIt + 1) 
                  << " at position: " << std::distance(v.begin(), adjIt) << std::endl;
    }
    
    // search, search_n
    std::vector<int> subseq = {5, 9, 2};
    auto searchIt = std::search(v.begin(), v.end(), subseq.begin(), subseq.end());
    if (searchIt != v.end()) {
        std::cout << "Subsequence found at position: " << std::distance(v.begin(), searchIt) << std::endl;
    }
    
    auto searchNIt = std::search_n(v.begin(), v.end(), 2, 5);
    if (searchNIt != v.end()) {
        std::cout << "Found 2 consecutive 5's at position: " << std::distance(v.begin(), searchNIt) << std::endl;
    }
    
    std::cout << "\n===== Modifying Sequence Operations =====" << std::endl;
    
    // Create a copy of the original vector
    std::vector<int> v2 = v;
    printContainer("Original vector", v2);
    
    // copy, copy_if
    std::vector<int> v3(v2.size());
    std::copy(v2.begin(), v2.end(), v3.begin());
    printContainer("After copy", v3);
    
    std::vector<int> v4;
    std::copy_if(v2.begin(), v2.end(), std::back_inserter(v4), [](int x) { return x % 2 == 0; });
    printContainer("After copy_if (even numbers)", v4);
    
    // copy_n
    std::vector<int> v5(3);
    std::copy_n(v2.begin(), 3, v5.begin());
    printContainer("After copy_n (first 3 elements)", v5);
    
    // move
    std::vector<std::string> src = {"Hello", "World", "C++", "Algorithms"};
    std::vector<std::string> dst(src.size());
    std::move(src.begin(), src.end(), dst.begin());
    printContainer("After move (destination)", dst);
    printContainer("After move (source)", src);  // Source elements are in valid but unspecified state
    
    // transform
    std::vector<int> v6 = v2;
    std::transform(v6.begin(), v6.end(), v6.begin(), [](int x) { return x * 2; });
    printContainer("After transform (double each element)", v6);
    
    std::vector<int> v7(v2.size());
    std::transform(v2.begin(), v2.end(), v6.begin(), v7.begin(), std::plus<>());
    printContainer("After transform (sum of v2 and v6)", v7);
    
    // replace, replace_if
    std::vector<int> v8 = v2;
    std::replace(v8.begin(), v8.end(), 5, 50);
    printContainer("After replace (5 -> 50)", v8);
    
    std::replace_if(v8.begin(), v8.end(), [](int x) { return x > 10; }, 10);
    printContainer("After replace_if (values > 10 -> 10)", v8);
    
    // fill, fill_n
    std::vector<int> v9(5);
    std::fill(v9.begin(), v9.end(), 7);
    printContainer("After fill (with 7)", v9);
    
    std::fill_n(v9.begin(), 3, 9);
    printContainer("After fill_n (first 3 elements with 9)", v9);
    
    // generate, generate_n
    std::vector<int> v10(5);
    std::generate(v10.begin(), v10.end(), [n = 0]() mutable { return n++; });
    printContainer("After generate (sequential numbers)", v10);
    
    std::generate_n(v10.begin(), 3, [n = 10]() mutable { return n++; });
    printContainer("After generate_n (first 3 elements)", v10);
    
    // remove, remove_if
    std::vector<int> v11 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto newEnd = std::remove(v11.begin(), v11.end(), 5);
    v11.erase(newEnd, v11.end());
    printContainer("After remove (5)", v11);
    
    newEnd = std::remove_if(v11.begin(), v11.end(), [](int x) { return x % 2 == 0; });
    v11.erase(newEnd, v11.end());
    printContainer("After remove_if (even numbers)", v11);
    
    // unique
    std::vector<int> v12 = {1, 1, 2, 2, 3, 3, 3, 4, 5, 5};
    auto uniqueEnd = std::unique(v12.begin(), v12.end());
    v12.erase(uniqueEnd, v12.end());
    printContainer("After unique", v12);
    
    // reverse
    std::vector<int> v13 = {1, 2, 3, 4, 5};
    std::reverse(v13.begin(), v13.end());
    printContainer("After reverse", v13);
    
    // rotate
    std::vector<int> v14 = {1, 2, 3, 4, 5, 6, 7};
    std::rotate(v14.begin(), v14.begin() + 3, v14.end());
    printContainer("After rotate (by 3 positions)", v14);
    
    // shuffle
    std::vector<int> v15 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::shuffle(v15.begin(), v15.end(), generator);
    printContainer("After shuffle", v15);
    
    std::cout << "\n===== Sorting Operations =====" << std::endl;
    
    // sort
    std::vector<int> v16 = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    std::sort(v16.begin(), v16.end());
    printContainer("After sort (ascending)", v16);
    
    std::sort(v16.begin(), v16.end(), std::greater<>());
    printContainer("After sort (descending)", v16);
    
    // partial_sort
    std::vector<int> v17 = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    std::partial_sort(v17.begin(), v17.begin() + 4, v17.end());
    printContainer("After partial_sort (first 4 elements)", v17);
    
    // nth_element
    std::vector<int> v18 = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    std::nth_element(v18.begin(), v18.begin() + 4, v18.end());
    printContainer("After nth_element (5th element)", v18);
    std::cout << "5th element: " << v18[4] << std::endl;
    
    // sort_heap
    std::vector<int> v19 = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    std::make_heap(v19.begin(), v19.end());
    printContainer("After make_heap", v19);
    
    std::sort_heap(v19.begin(), v19.end());
    printContainer("After sort_heap", v19);
    
    // is_sorted
    bool sorted = std::is_sorted(v19.begin(), v19.end());
    std::cout << "Is sorted? " << sorted << std::endl;
    
    std::cout << "\n===== Binary Search Operations =====" << std::endl;
    
    // Create a sorted vector
    std::vector<int> v20 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // lower_bound, upper_bound
    auto lb = std::lower_bound(v20.begin(), v20.end(), 5);
    auto ub = std::upper_bound(v20.begin(), v20.end(), 5);
    
    std::cout << "Lower bound of 5: " << *lb << " at position: " << std::distance(v20.begin(), lb) << std::endl;
    std::cout << "Upper bound of 5: " << *ub << " at position: " << std::distance(v20.begin(), ub) << std::endl;
    
    // equal_range
    auto range = std::equal_range(v20.begin(), v20.end(), 5);
    std::cout << "Equal range of 5: [" << std::distance(v20.begin(), range.first) << ", " 
              << std::distance(v20.begin(), range.second) << ")" << std::endl;
    
    // binary_search
    bool found = std::binary_search(v20.begin(), v20.end(), 5);
    bool notFound = std::binary_search(v20.begin(), v20.end(), 11);
    
    std::cout << "Binary search for 5: " << found << std::endl;
    std::cout << "Binary search for 11: " << notFound << std::endl;
    
    std::cout << "\n===== Merge Operations =====" << std::endl;
    
    // Create two sorted vectors
    std::vector<int> v21 = {1, 3, 5, 7, 9};
    std::vector<int> v22 = {2, 4, 6, 8, 10};
    
    // merge
    std::vector<int> v23(v21.size() + v22.size());
    std::merge(v21.begin(), v21.end(), v22.begin(), v22.end(), v23.begin());
    printContainer("After merge", v23);
    
    // inplace_merge
    std::vector<int> v24;
    v24.insert(v24.end(), v21.begin(), v21.end());
    v24.insert(v24.end(), v22.begin(), v22.end());
    printContainer("Before inplace_merge", v24);
    
    std::inplace_merge(v24.begin(), v24.begin() + v21.size(), v24.end());
    printContainer("After inplace_merge", v24);
    
    std::cout << "\n===== Set Operations =====" << std::endl;
    
    // Create two sets
    std::vector<int> v25 = {1, 2, 3, 4, 5};
    std::vector<int> v26 = {4, 5, 6, 7, 8};
    
    // set_union
    std::vector<int> v27(v25.size() + v26.size());
    auto unionEnd = std::set_union(v25.begin(), v25.end(), v26.begin(), v26.end(), v27.begin());
    v27.resize(std::distance(v27.begin(), unionEnd));
    printContainer("Set union", v27);
    
    // set_intersection
    std::vector<int> v28(std::min(v25.size(), v26.size()));
    auto intersectionEnd = std::set_intersection(v25.begin(), v25.end(), v26.begin(), v26.end(), v28.begin());
    v28.resize(std::distance(v28.begin(), intersectionEnd));
    printContainer("Set intersection", v28);
    
    // set_difference
    std::vector<int> v29(v25.size());
    auto differenceEnd = std::set_difference(v25.begin(), v25.end(), v26.begin(), v26.end(), v29.begin());
    v29.resize(std::distance(v29.begin(), differenceEnd));
    printContainer("Set difference (v25 - v26)", v29);
    
    // set_symmetric_difference
    std::vector<int> v30(v25.size() + v26.size());
    auto symDiffEnd = std::set_symmetric_difference(v25.begin(), v25.end(), v26.begin(), v26.end(), v30.begin());
    v30.resize(std::distance(v30.begin(), symDiffEnd));
    printContainer("Set symmetric difference", v30);
    
    std::cout << "\n===== Numeric Operations =====" << std::endl;
    
    std::vector<int> v31 = {1, 2, 3, 4, 5};
    
    // accumulate
    int sum = std::accumulate(v31.begin(), v31.end(), 0);
    int product = std::accumulate(v31.begin(), v31.end(), 1, std::multiplies<>());
    
    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Product: " << product << std::endl;
    
    // inner_product
    std::vector<int> v32 = {10, 20, 30, 40, 50};
    int innerProduct = std::inner_product(v31.begin(), v31.end(), v32.begin(), 0);
    std::cout << "Inner product: " << innerProduct << std::endl;
    
    // adjacent_difference
    std::vector<int> v33(v31.size());
    std::adjacent_difference(v31.begin(), v31.end(), v33.begin());
    printContainer("Adjacent difference", v33);
    
    // partial_sum
    std::vector<int> v34(v31.size());
    std::partial_sum(v31.begin(), v31.end(), v34.begin());
    printContainer("Partial sum", v34);
    
    // iota
    std::vector<int> v35(10);
    std::iota(v35.begin(), v35.end(), 1);
    printContainer("Iota (starting from 1)", v35);
    
    std::cout << "\n===== Custom Data Type Example =====" << std::endl;
    
    // Create a vector of Person objects
    std::vector<Person> people = {
        {"Alice", 30, 75000.0},
        {"Bob", 25, 65000.0},
        {"Charlie", 35, 85000.0},
        {"David", 28, 70000.0},
        {"Eve", 32, 90000.0}
    };
    
    // Print original vector
    std::cout << "Original people vector:" << std::endl;
    for (const auto& person : people) {
        std::cout << "  " << person << std::endl;
    }
    
    // Sort by age (using operator<)
    std::sort(people.begin(), people.end());
    
    std::cout << "\nAfter sorting by age:" << std::endl;
    for (const auto& person : people) {
        std::cout << "  " << person << std::endl;
    }
    
    // Sort by name
    std::sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
        return a.name < b.name;
    });
    
    std::cout << "\nAfter sorting by name:" << std::endl;
    for (const auto& person : people) {
        std::cout << "  " << person << std::endl;
    }
    
    // Sort by salary (descending)
    std::sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
        return a.salary > b.salary;
    });
    
    std::cout << "\nAfter sorting by salary (descending):" << std::endl;
    for (const auto& person : people) {
        std::cout << "  " << person << std::endl;
    }
    
    // Find a person by name
    auto it = std::find_if(people.begin(), people.end(), [](const Person& p) {
        return p.name == "Charlie";
    });
    
    if (it != people.end()) {
        std::cout << "\nFound Charlie: " << *it << std::endl;
    }
    
    // Count people with salary > 70000
    int highEarners = std::count_if(people.begin(), people.end(), [](const Person& p) {
        return p.salary > 70000.0;
    });
    
    std::cout << "Number of people with salary > $70,000: " << highEarners << std::endl;
    
    // Calculate average age
    double avgAge = std::accumulate(people.begin(), people.end(), 0.0, 
                                   [](double sum, const Person& p) { return sum + p.age; }) / people.size();
    
    std::cout << "Average age: " << avgAge << std::endl;
    
    // Calculate total salary
    double totalSalary = std::accumulate(people.begin(), people.end(), 0.0, 
                                        [](double sum, const Person& p) { return sum + p.salary; });
    
    std::cout << "Total salary: $" << std::fixed << std::setprecision(2) << totalSalary << std::endl;
    
    return 0;
}
