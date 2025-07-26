/*
 * Data Processing Pipeline - Header File
 * 
 * This header defines a comprehensive data processing pipeline that demonstrates
 * advanced STL usage, custom iterators, allocators, and modern C++ features.
 * 
 * The pipeline supports:
 * - CSV data loading and parsing
 * - Data transformation and filtering
 * - Statistical analysis
 * - Performance monitoring
 * - Memory-efficient processing
 */

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>
#include <chrono>
#include <functional>
#include <memory>
#include <optional>
#include <variant>
#include <type_traits>

namespace DataProcessing {

// Forward declarations
class DataRecord;
class DataSet;
class Pipeline;
class Statistics;

// Type aliases for better readability
using DataValue = std::variant<int, double, std::string>;
using DataRow = std::unordered_map<std::string, DataValue>;
using TransformFunction = std::function<DataValue(const DataValue&)>;
using FilterPredicate = std::function<bool(const DataRecord&)>;
using AggregateFunction = std::function<DataValue(const std::vector<DataValue>&)>;

// Utility functions for DataValue operations
namespace ValueOps {
    double to_double(const DataValue& value);
    std::string to_string(const DataValue& value);
    bool is_numeric(const DataValue& value);
    DataValue add(const DataValue& a, const DataValue& b);
    DataValue multiply(const DataValue& a, const DataValue& b);
    bool compare_less(const DataValue& a, const DataValue& b);
}

// Data Record - represents a single row of data
class DataRecord {
private:
    DataRow data_;
    
public:
    DataRecord() = default;
    explicit DataRecord(DataRow data) : data_(std::move(data)) {}
    
    // Access operators
    DataValue& operator[](const std::string& column) { return data_[column]; }
    const DataValue& operator[](const std::string& column) const;
    
    // Utility methods
    bool has_column(const std::string& column) const;
    std::vector<std::string> get_columns() const;
    size_t size() const { return data_.size(); }
    bool empty() const { return data_.empty(); }
    
    // Iteration support
    auto begin() { return data_.begin(); }
    auto end() { return data_.end(); }
    auto begin() const { return data_.begin(); }
    auto end() const { return data_.end(); }
    
    // Comparison operators
    bool operator==(const DataRecord& other) const { return data_ == other.data_; }
    bool operator!=(const DataRecord& other) const { return data_ != other.data_; }
    
    // String representation
    std::string to_string() const;
    
    friend std::ostream& operator<<(std::ostream& os, const DataRecord& record);
};

// Custom iterator for filtered data access
template<typename Iterator, typename Predicate>
class FilteredIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    using difference_type = typename std::iterator_traits<Iterator>::difference_type;
    using pointer = typename std::iterator_traits<Iterator>::pointer;
    using reference = typename std::iterator_traits<Iterator>::reference;
    
private:
    Iterator current_;
    Iterator end_;
    Predicate predicate_;
    
    void advance_to_valid() {
        while (current_ != end_ && !predicate_(*current_)) {
            ++current_;
        }
    }
    
public:
    FilteredIterator(Iterator current, Iterator end, Predicate pred)
        : current_(current), end_(end), predicate_(pred) {
        advance_to_valid();
    }
    
    reference operator*() const { return *current_; }
    pointer operator->() const { return current_.operator->(); }
    
    FilteredIterator& operator++() {
        ++current_;
        advance_to_valid();
        return *this;
    }
    
    FilteredIterator operator++(int) {
        auto temp = *this;
        ++(*this);
        return temp;
    }
    
    bool operator==(const FilteredIterator& other) const { return current_ == other.current_; }
    bool operator!=(const FilteredIterator& other) const { return current_ != other.current_; }
};

// Data Set - collection of data records with processing capabilities
class DataSet {
private:
    std::vector<DataRecord> records_;
    std::vector<std::string> columns_;
    
public:
    DataSet() = default;
    explicit DataSet(std::vector<std::string> columns) : columns_(std::move(columns)) {}
    
    // Data access
    void add_record(DataRecord record);
    const DataRecord& operator[](size_t index) const { return records_[index]; }
    DataRecord& operator[](size_t index) { return records_[index]; }
    
    // Size and capacity
    size_t size() const { return records_.size(); }
    bool empty() const { return records_.empty(); }
    void reserve(size_t capacity) { records_.reserve(capacity); }
    
    // Column management
    const std::vector<std::string>& get_columns() const { return columns_; }
    void add_column(const std::string& name);
    bool has_column(const std::string& name) const;
    
    // Iteration
    auto begin() { return records_.begin(); }
    auto end() { return records_.end(); }
    auto begin() const { return records_.begin(); }
    auto end() const { return records_.end(); }
    
    // Filtered iteration
    template<typename Predicate>
    auto filtered_begin(Predicate pred) {
        return FilteredIterator(records_.begin(), records_.end(), pred);
    }
    
    template<typename Predicate>
    auto filtered_end(Predicate pred) {
        return FilteredIterator(records_.end(), records_.end(), pred);
    }
    
    // Data operations
    DataSet filter(FilterPredicate predicate) const;
    void transform_column(const std::string& column, TransformFunction func);
    void sort_by_column(const std::string& column, bool ascending = true);
    
    // Aggregation operations
    DataValue aggregate_column(const std::string& column, AggregateFunction func) const;
    std::unordered_map<std::string, DataValue> group_by_aggregate(
        const std::string& group_column, 
        const std::string& value_column, 
        AggregateFunction func) const;
    
    // I/O operations
    static DataSet load_from_csv(const std::string& filename);
    void save_to_csv(const std::string& filename) const;
    
    // String representation
    std::string to_string(size_t max_rows = 10) const;
    
    friend std::ostream& operator<<(std::ostream& os, const DataSet& dataset);
};

// Statistics calculator
class Statistics {
public:
    struct DescriptiveStats {
        double mean;
        double median;
        double std_dev;
        double min_val;
        double max_val;
        size_t count;
        
        std::string to_string() const;
    };
    
    static DescriptiveStats calculate(const std::vector<DataValue>& values);
    static DescriptiveStats calculate_column(const DataSet& dataset, const std::string& column);
    
    // Correlation analysis
    static double correlation(const DataSet& dataset, 
                            const std::string& col1, 
                            const std::string& col2);
    
    // Frequency analysis
    static std::unordered_map<std::string, size_t> frequency_count(
        const DataSet& dataset, const std::string& column);
};

// Performance monitor
class PerformanceMonitor {
private:
    std::chrono::high_resolution_clock::time_point start_time_;
    std::string operation_name_;
    
public:
    explicit PerformanceMonitor(std::string operation_name);
    ~PerformanceMonitor();
    
    double elapsed_ms() const;
    
    // Disable copy and move
    PerformanceMonitor(const PerformanceMonitor&) = delete;
    PerformanceMonitor& operator=(const PerformanceMonitor&) = delete;
    PerformanceMonitor(PerformanceMonitor&&) = delete;
    PerformanceMonitor& operator=(PerformanceMonitor&&) = delete;
};

// Macro for easy performance monitoring
#define MONITOR_PERFORMANCE(name) PerformanceMonitor _monitor(name)

// Processing Pipeline - chains operations together
class Pipeline {
private:
    std::vector<std::function<void(DataSet&)>> operations_;
    
public:
    Pipeline() = default;
    
    // Add operations to the pipeline
    Pipeline& filter(FilterPredicate predicate);
    Pipeline& transform(const std::string& column, TransformFunction func);
    Pipeline& sort_by(const std::string& column, bool ascending = true);
    Pipeline& add_column(const std::string& name, 
                        std::function<DataValue(const DataRecord&)> calculator);
    
    // Execute the pipeline
    DataSet execute(DataSet input) const;
    
    // Clear the pipeline
    void clear() { operations_.clear(); }
    
    // Get number of operations
    size_t size() const { return operations_.size(); }
};

// Common aggregate functions
namespace Aggregates {
    extern const AggregateFunction Sum;
    extern const AggregateFunction Mean;
    extern const AggregateFunction Min;
    extern const AggregateFunction Max;
    extern const AggregateFunction Count;
    extern const AggregateFunction StdDev;
}

// Common filter predicates
namespace Filters {
    FilterPredicate column_equals(const std::string& column, const DataValue& value);
    FilterPredicate column_greater_than(const std::string& column, const DataValue& value);
    FilterPredicate column_less_than(const std::string& column, const DataValue& value);
    FilterPredicate column_contains(const std::string& column, const std::string& substring);
    FilterPredicate logical_and(FilterPredicate a, FilterPredicate b);
    FilterPredicate logical_or(FilterPredicate a, FilterPredicate b);
    FilterPredicate logical_not(FilterPredicate pred);
}

// Common transform functions
namespace Transforms {
    extern const TransformFunction ToUpper;
    extern const TransformFunction ToLower;
    extern const TransformFunction Square;
    extern const TransformFunction SquareRoot;
    extern const TransformFunction Absolute;
    
    TransformFunction multiply_by(double factor);
    TransformFunction add_constant(double constant);
    TransformFunction string_replace(const std::string& from, const std::string& to);
}

} // namespace DataProcessing
