#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <random>
#include <functional>
#include <memory>

// Data structures for a simple data analysis system
struct DataPoint {
    int id;
    std::string category;
    double value;
    std::chrono::system_clock::time_point timestamp;
    
    // Default constructor required for STL containers
    DataPoint() 
        : id(0), category(""), value(0.0), 
          timestamp(std::chrono::system_clock::now()) {}
    
    DataPoint(int i, std::string c, double v) 
        : id(i), category(std::move(c)), value(v), 
          timestamp(std::chrono::system_clock::now()) {}
    
    // Copy constructor
    DataPoint(const DataPoint& other) = default;
    
    // Move constructor
    DataPoint(DataPoint&& other) = default;
    
    // Copy assignment operator
    DataPoint& operator=(const DataPoint& other) = default;
    
    // Move assignment operator
    DataPoint& operator=(DataPoint&& other) = default;
    
    // For sorting and comparison
    bool operator<(const DataPoint& other) const {
        return id < other.id;
    }
    
    // For equality comparison
    bool operator==(const DataPoint& other) const {
        return id == other.id;
    }
};

// Output stream operator for DataPoint
std::ostream& operator<<(std::ostream& os, const DataPoint& dp) {
    auto time_t = std::chrono::system_clock::to_time_t(dp.timestamp);
    os << "ID: " << dp.id 
       << ", Category: " << dp.category 
       << ", Value: " << std::fixed << std::setprecision(2) << dp.value
       << ", Time: " << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return os;
}

// DataAnalyzer class to demonstrate STL usage in a practical context
class DataAnalyzer {
private:
    std::vector<DataPoint> data;
    std::map<std::string, std::vector<DataPoint>> categorizedData;
    std::unordered_map<int, DataPoint> idIndex;
    
public:
    // Add a data point
    void addDataPoint(const DataPoint& point) {
        data.push_back(point);
        categorizedData[point.category].push_back(point);
        idIndex[point.id] = point;
    }
    
    // Generate random data points
    void generateRandomData(int count) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> idDist(1000, 9999);
        std::uniform_real_distribution<> valueDist(0.0, 1000.0);
        
        std::vector<std::string> categories = {"Technology", "Finance", "Healthcare", "Education", "Entertainment"};
        std::uniform_int_distribution<> catDist(0, categories.size() - 1);
        
        for (int i = 0; i < count; ++i) {
            int id = idDist(gen);
            std::string category = categories[catDist(gen)];
            double value = valueDist(gen);
            
            addDataPoint(DataPoint(id, category, value));
        }
        
        std::cout << "Generated " << count << " random data points\n";
    }
    
    // Get all data points
    const std::vector<DataPoint>& getAllData() const {
        return data;
    }
    
    // Get data points by category
    std::vector<DataPoint> getDataByCategory(const std::string& category) const {
        auto it = categorizedData.find(category);
        if (it != categorizedData.end()) {
            return it->second;
        }
        return {};
    }
    
    // Get data point by ID
    std::optional<DataPoint> getDataById(int id) const {
        auto it = idIndex.find(id);
        if (it != idIndex.end()) {
            return it->second;
        }
        return std::nullopt;
    }
    
    // Get all categories
    std::set<std::string> getAllCategories() const {
        std::set<std::string> categories;
        for (const auto& [category, _] : categorizedData) {
            categories.insert(category);
        }
        return categories;
    }
    
    // Calculate statistics for all data
    void calculateStatistics() const {
        if (data.empty()) {
            std::cout << "No data available for statistics\n";
            return;
        }
        
        // Calculate min, max, average using STL algorithms
        auto minmax = std::minmax_element(data.begin(), data.end(), 
            [](const DataPoint& a, const DataPoint& b) { return a.value < b.value; });
            
        double sum = std::accumulate(data.begin(), data.end(), 0.0, 
            [](double acc, const DataPoint& dp) { return acc + dp.value; });
            
        double average = sum / data.size();
        
        std::cout << "Statistics for all data:\n";
        std::cout << "  Count: " << data.size() << "\n";
        std::cout << "  Minimum value: " << minmax.first->value << " (ID: " << minmax.first->id << ")\n";
        std::cout << "  Maximum value: " << minmax.second->value << " (ID: " << minmax.second->id << ")\n";
        std::cout << "  Average value: " << std::fixed << std::setprecision(2) << average << "\n";
        
        // Calculate median
        std::vector<double> values;
        std::transform(data.begin(), data.end(), std::back_inserter(values),
            [](const DataPoint& dp) { return dp.value; });
            
        std::sort(values.begin(), values.end());
        double median;
        if (values.size() % 2 == 0) {
            median = (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2;
        } else {
            median = values[values.size() / 2];
        }
        
        std::cout << "  Median value: " << std::fixed << std::setprecision(2) << median << "\n";
        
        // Calculate standard deviation
        double sq_sum = std::accumulate(values.begin(), values.end(), 0.0,
            [average](double acc, double val) { return acc + (val - average) * (val - average); });
            
        double std_dev = std::sqrt(sq_sum / values.size());
        std::cout << "  Standard deviation: " << std::fixed << std::setprecision(2) << std_dev << "\n";
    }
    
    // Calculate statistics by category
    void calculateStatisticsByCategory() const {
        std::cout << "\nStatistics by category:\n";
        
        for (const auto& [category, categoryData] : categorizedData) {
            if (categoryData.empty()) continue;
            
            auto minmax = std::minmax_element(categoryData.begin(), categoryData.end(), 
                [](const DataPoint& a, const DataPoint& b) { return a.value < b.value; });
                
            double sum = std::accumulate(categoryData.begin(), categoryData.end(), 0.0, 
                [](double acc, const DataPoint& dp) { return acc + dp.value; });
                
            double average = sum / categoryData.size();
            
            std::cout << "Category: " << category << "\n";
            std::cout << "  Count: " << categoryData.size() << "\n";
            std::cout << "  Minimum value: " << minmax.first->value << "\n";
            std::cout << "  Maximum value: " << minmax.second->value << "\n";
            std::cout << "  Average value: " << std::fixed << std::setprecision(2) << average << "\n";
            
            // Calculate percentage of total
            double percentage = (categoryData.size() * 100.0) / data.size();
            std::cout << "  Percentage of total: " << std::fixed << std::setprecision(2) << percentage << "%\n";
        }
    }
    
    // Find data points within a value range
    std::vector<DataPoint> findDataInRange(double minValue, double maxValue) const {
        std::vector<DataPoint> result;
        std::copy_if(data.begin(), data.end(), std::back_inserter(result),
            [minValue, maxValue](const DataPoint& dp) {
                return dp.value >= minValue && dp.value <= maxValue;
            });
        return result;
    }
    
    // Sort data by value (ascending or descending)
    std::vector<DataPoint> getSortedByValue(bool ascending = true) const {
        std::vector<DataPoint> sortedData = data;
        if (ascending) {
            std::sort(sortedData.begin(), sortedData.end(),
                [](const DataPoint& a, const DataPoint& b) { return a.value < b.value; });
        } else {
            std::sort(sortedData.begin(), sortedData.end(),
                [](const DataPoint& a, const DataPoint& b) { return a.value > b.value; });
        }
        return sortedData;
    }
    
    // Get top N data points by value
    std::vector<DataPoint> getTopN(int n) const {
        if (n <= 0 || data.empty()) return {};
        
        std::vector<DataPoint> result = data;
        
        // Use partial_sort to get top N elements
        std::partial_sort(result.begin(), result.begin() + std::min(n, static_cast<int>(result.size())), 
                         result.end(),
                         [](const DataPoint& a, const DataPoint& b) { return a.value > b.value; });
        
        // Resize to at most n elements
        if (result.size() > static_cast<size_t>(n)) {
            result.resize(n);
        }
        
        return result;
    }
    
    // Export data to CSV
    void exportToCSV(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return;
        }
        
        // Write header
        file << "ID,Category,Value,Timestamp\n";
        
        // Write data
        for (const auto& point : data) {
            auto time_t = std::chrono::system_clock::to_time_t(point.timestamp);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
            
            file << point.id << ","
                 << point.category << ","
                 << std::fixed << std::setprecision(2) << point.value << ","
                 << ss.str() << "\n";
        }
        
        std::cout << "Data exported to " << filename << std::endl;
    }
    
    // Filter data using a predicate function
    template<typename Predicate>
    std::vector<DataPoint> filterData(Predicate predicate) const {
        std::vector<DataPoint> result;
        std::copy_if(data.begin(), data.end(), std::back_inserter(result), predicate);
        return result;
    }
    
    // Transform data using a transformation function
    template<typename Transform>
    std::vector<DataPoint> transformData(Transform transform) const {
        std::vector<DataPoint> result = data;
        std::transform(data.begin(), data.end(), result.begin(), transform);
        return result;
    }
};

// Demo function to show usage of the DataAnalyzer class
void runDataAnalysisDemo() {
    std::cout << "===== Data Analysis System Demo =====\n\n";
    
    // Create a data analyzer and generate random data
    DataAnalyzer analyzer;
    analyzer.generateRandomData(50);
    
    // Calculate overall statistics
    std::cout << "\n1. Overall Statistics:\n";
    analyzer.calculateStatistics();
    
    // Calculate statistics by category
    std::cout << "\n2. Statistics by Category:\n";
    analyzer.calculateStatisticsByCategory();
    
    // Find data in a specific range
    std::cout << "\n3. Data points with values between 300 and 500:\n";
    auto rangeData = analyzer.findDataInRange(300, 500);
    std::cout << "Found " << rangeData.size() << " data points in range\n";
    for (size_t i = 0; i < std::min(size_t(5), rangeData.size()); ++i) {
        std::cout << "  " << rangeData[i] << "\n";
    }
    if (rangeData.size() > 5) {
        std::cout << "  ... and " << (rangeData.size() - 5) << " more\n";
    }
    
    // Get top 5 data points by value
    std::cout << "\n4. Top 5 data points by value:\n";
    auto topData = analyzer.getTopN(5);
    for (const auto& point : topData) {
        std::cout << "  " << point << "\n";
    }
    
    // Filter data using a custom predicate
    std::cout << "\n5. Technology data points with value > 500:\n";
    auto filteredData = analyzer.filterData([](const DataPoint& dp) {
        return dp.category == "Technology" && dp.value > 500;
    });
    
    std::cout << "Found " << filteredData.size() << " matching data points\n";
    for (const auto& point : filteredData) {
        std::cout << "  " << point << "\n";
    }
    
    // Export data to CSV
    analyzer.exportToCSV("data_analysis.csv");
}

int main() {
    runDataAnalysisDemo();
    return 0;
}
