#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <deque>
#include <list>
#include <forward_list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iterator>
#include <iomanip>

// Helper function to print any container
template<typename Container>
void printContainer(const std::string& name, const Container& container) {
    std::cout << name << ": ";
    for (const auto& item : container) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

// Helper function to print a map
template<typename Map>
void printMap(const std::string& name, const Map& map) {
    std::cout << name << ": ";
    for (const auto& [key, value] : map) {
        std::cout << "[" << key << ":" << value << "] ";
    }
    std::cout << std::endl;
}

// Helper function to print a pair
template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& pair) {
    os << "(" << pair.first << "," << pair.second << ")";
    return os;
}

int main() {
    std::cout << "===== STL Sequence Containers =====" << std::endl;
    
    // Vector - Dynamic array
    std::cout << "\n--- Vector ---" << std::endl;
    std::vector<int> vec = {1, 2, 3, 4, 5};
    printContainer("Initial vector", vec);
    
    vec.push_back(6);  // Add element at the end
    printContainer("After push_back", vec);
    
    vec.pop_back();  // Remove last element
    printContainer("After pop_back", vec);
    
    vec.insert(vec.begin() + 2, 10);  // Insert at position
    printContainer("After insert", vec);
    
    vec.erase(vec.begin() + 2);  // Erase at position
    printContainer("After erase", vec);
    
    std::cout << "Size: " << vec.size() << ", Capacity: " << vec.capacity() << std::endl;
    vec.shrink_to_fit();  // Reduce capacity to fit size
    std::cout << "After shrink_to_fit - Size: " << vec.size() << ", Capacity: " << vec.capacity() << std::endl;
    
    // Array - Fixed-size array
    std::cout << "\n--- Array ---" << std::endl;
    std::array<int, 5> arr = {5, 4, 3, 2, 1};
    printContainer("Initial array", arr);
    
    std::sort(arr.begin(), arr.end());  // Sort the array
    printContainer("After sort", arr);
    
    std::cout << "Front: " << arr.front() << ", Back: " << arr.back() << std::endl;
    std::cout << "Size: " << arr.size() << ", Max size: " << arr.max_size() << std::endl;
    
    // Deque - Double-ended queue
    std::cout << "\n--- Deque ---" << std::endl;
    std::deque<int> deq = {10, 20, 30, 40, 50};
    printContainer("Initial deque", deq);
    
    deq.push_front(5);  // Add element at the beginning
    printContainer("After push_front", deq);
    
    deq.push_back(60);  // Add element at the end
    printContainer("After push_back", deq);
    
    deq.pop_front();  // Remove first element
    printContainer("After pop_front", deq);
    
    deq.pop_back();  // Remove last element
    printContainer("After pop_back", deq);
    
    // List - Doubly-linked list
    std::cout << "\n--- List ---" << std::endl;
    std::list<int> lst = {100, 200, 300, 400, 500};
    printContainer("Initial list", lst);
    
    lst.push_front(50);  // Add element at the beginning
    printContainer("After push_front", lst);
    
    lst.push_back(600);  // Add element at the end
    printContainer("After push_back", lst);
    
    auto it = std::find(lst.begin(), lst.end(), 300);
    if (it != lst.end()) {
        lst.insert(it, 250);  // Insert before 300
    }
    printContainer("After insert", lst);
    
    it = std::find(lst.begin(), lst.end(), 400);
    if (it != lst.end()) {
        lst.erase(it);  // Erase 400
    }
    printContainer("After erase", lst);
    
    lst.sort();  // Sort the list
    printContainer("After sort", lst);
    
    lst.reverse();  // Reverse the list
    printContainer("After reverse", lst);
    
    // Forward list - Singly-linked list
    std::cout << "\n--- Forward List ---" << std::endl;
    std::forward_list<int> flist = {10, 20, 30, 40, 50};
    printContainer("Initial forward_list", flist);
    
    flist.push_front(5);  // Add element at the beginning
    printContainer("After push_front", flist);
    
    flist.insert_after(flist.begin(), 15);  // Insert after first element
    printContainer("After insert_after", flist);
    
    flist.erase_after(flist.begin());  // Erase element after first
    printContainer("After erase_after", flist);
    
    flist.sort();  // Sort the forward list
    printContainer("After sort", flist);
    
    std::cout << "\n===== STL Associative Containers =====" << std::endl;
    
    // Set - Ordered unique elements
    std::cout << "\n--- Set ---" << std::endl;
    std::set<int> s = {50, 20, 60, 10, 30, 30};  // Note: duplicate 30 will be ignored
    printContainer("Initial set", s);
    
    s.insert(40);  // Insert element
    printContainer("After insert", s);
    
    s.erase(20);  // Erase element
    printContainer("After erase", s);
    
    auto setIt = s.find(30);
    if (setIt != s.end()) {
        std::cout << "Found 30 in set" << std::endl;
    }
    
    std::cout << "Count of 30: " << s.count(30) << std::endl;
    std::cout << "Count of 20: " << s.count(20) << std::endl;
    
    // Multiset - Ordered elements, duplicates allowed
    std::cout << "\n--- Multiset ---" << std::endl;
    std::multiset<int> ms = {50, 20, 60, 10, 30, 30};  // Note: duplicate 30 is allowed
    printContainer("Initial multiset", ms);
    
    ms.insert(40);  // Insert element
    printContainer("After insert", ms);
    
    ms.erase(30);  // Erase all occurrences of 30
    printContainer("After erase all 30s", ms);
    
    // Map - Ordered key-value pairs
    std::cout << "\n--- Map ---" << std::endl;
    std::map<std::string, int> m = {
        {"apple", 100},
        {"banana", 200},
        {"orange", 300}
    };
    printMap("Initial map", m);
    
    m["grape"] = 400;  // Insert using operator[]
    printMap("After insert", m);
    
    m.insert({"kiwi", 500});  // Insert using insert()
    printMap("After insert", m);
    
    m.erase("banana");  // Erase by key
    printMap("After erase", m);
    
    if (m.find("apple") != m.end()) {
        std::cout << "Found apple with value: " << m["apple"] << std::endl;
    }
    
    // Multimap - Ordered key-value pairs, duplicate keys allowed
    std::cout << "\n--- Multimap ---" << std::endl;
    std::multimap<std::string, int> mm = {
        {"apple", 100},
        {"banana", 200},
        {"apple", 150},  // Duplicate key
        {"orange", 300}
    };
    printMap("Initial multimap", mm);
    
    mm.insert({"banana", 250});  // Insert duplicate key
    printMap("After insert", mm);
    
    std::cout << "Count of 'apple': " << mm.count("apple") << std::endl;
    
    auto range = mm.equal_range("apple");
    std::cout << "Values for 'apple': ";
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << it->second << " ";
    }
    std::cout << std::endl;
    
    std::cout << "\n===== STL Unordered Associative Containers =====" << std::endl;
    
    // Unordered set - Hash table with unique elements
    std::cout << "\n--- Unordered Set ---" << std::endl;
    std::unordered_set<int> us = {50, 20, 60, 10, 30, 30};  // Note: duplicate 30 will be ignored
    printContainer("Initial unordered_set", us);
    
    us.insert(40);  // Insert element
    printContainer("After insert", us);
    
    us.erase(20);  // Erase element
    printContainer("After erase", us);
    
    std::cout << "Load factor: " << us.load_factor() << ", Bucket count: " << us.bucket_count() << std::endl;
    
    // Unordered multiset - Hash table with duplicate elements allowed
    std::cout << "\n--- Unordered Multiset ---" << std::endl;
    std::unordered_multiset<int> ums = {50, 20, 60, 10, 30, 30};  // Note: duplicate 30 is allowed
    printContainer("Initial unordered_multiset", ums);
    
    // Unordered map - Hash table with key-value pairs
    std::cout << "\n--- Unordered Map ---" << std::endl;
    std::unordered_map<std::string, int> um = {
        {"apple", 100},
        {"banana", 200},
        {"orange", 300}
    };
    printMap("Initial unordered_map", um);
    
    um["grape"] = 400;  // Insert using operator[]
    printMap("After insert", um);
    
    // Unordered multimap - Hash table with key-value pairs, duplicate keys allowed
    std::cout << "\n--- Unordered Multimap ---" << std::endl;
    std::unordered_multimap<std::string, int> umm = {
        {"apple", 100},
        {"banana", 200},
        {"apple", 150},  // Duplicate key
        {"orange", 300}
    };
    printMap("Initial unordered_multimap", umm);
    
    std::cout << "\n===== STL Container Adaptors =====" << std::endl;
    
    // Stack - LIFO data structure
    std::cout << "\n--- Stack ---" << std::endl;
    std::stack<int> stack;
    stack.push(10);
    stack.push(20);
    stack.push(30);
    
    std::cout << "Stack top: " << stack.top() << std::endl;
    std::cout << "Stack size: " << stack.size() << std::endl;
    
    stack.pop();
    std::cout << "After pop - Stack top: " << stack.top() << std::endl;
    
    // Queue - FIFO data structure
    std::cout << "\n--- Queue ---" << std::endl;
    std::queue<int> queue;
    queue.push(10);
    queue.push(20);
    queue.push(30);
    
    std::cout << "Queue front: " << queue.front() << ", back: " << queue.back() << std::endl;
    std::cout << "Queue size: " << queue.size() << std::endl;
    
    queue.pop();
    std::cout << "After pop - Queue front: " << queue.front() << std::endl;
    
    // Priority queue - Heap data structure
    std::cout << "\n--- Priority Queue ---" << std::endl;
    std::priority_queue<int> pq;
    pq.push(30);
    pq.push(10);
    pq.push(50);
    pq.push(20);
    
    std::cout << "Priority queue top (max element): " << pq.top() << std::endl;
    
    pq.pop();
    std::cout << "After pop - Priority queue top: " << pq.top() << std::endl;
    
    // Min priority queue
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;
    min_pq.push(30);
    min_pq.push(10);
    min_pq.push(50);
    min_pq.push(20);
    
    std::cout << "Min priority queue top (min element): " << min_pq.top() << std::endl;
    
    return 0;
}
