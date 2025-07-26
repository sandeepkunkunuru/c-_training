/*
 * Data Processing Pipeline - Implementation File
 * 
 * This file implements the data processing pipeline functionality,
 * demonstrating advanced STL usage and modern C++ features.
 */

#include "data_processor.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <stdexcept>

namespace DataProcessing {

// ValueOps implementations
namespace ValueOps {
    double to_double(const DataValue& value) {
        return std::visit([](const auto& v) -> double {
            if constexpr (std::is_same_v<std::decay_t<decltype(v)>, int>) {
                return static_cast<double>(v);
            } else if constexpr (std::is_same_v<std::decay_t<decltype(v)>, double>) {
                return v;
            } else {
                try {
                    return std::stod(v);
                } catch (...) {
                    return 0.0;
                }
            }
        }, value);
    }
    
    std::string to_string(const DataValue& value) {
        return std::visit([](const auto& v) -> std::string {
            if constexpr (std::is_same_v<std::decay_t<decltype(v)>, std::string>) {
                return v;
            } else {
                return std::to_string(v);
            }
        }, value);
    }
    
    bool is_numeric(const DataValue& value) {
        return std::holds_alternative<int>(value) || std::holds_alternative<double>(value);
    }
    
    DataValue add(const DataValue& a, const DataValue& b) {
        if (is_numeric(a) && is_numeric(b)) {
            return to_double(a) + to_double(b);
        }
        return to_string(a) + to_string(b);
    }
    
    DataValue multiply(const DataValue& a, const DataValue& b) {
        if (is_numeric(a) && is_numeric(b)) {
            return to_double(a) * to_double(b);
        }
        throw std::invalid_argument("Cannot multiply non-numeric values");
    }
    
    bool compare_less(const DataValue& a, const DataValue& b) {
        if (is_numeric(a) && is_numeric(b)) {
            return to_double(a) < to_double(b);
        }
        return to_string(a) < to_string(b);
    }
}

// DataRecord implementations
const DataValue& DataRecord::operator[](const std::string& column) const {
    auto it = data_.find(column);
    if (it == data_.end()) {
        throw std::out_of_range("Column not found: " + column);
    }
    return it->second;
}

bool DataRecord::has_column(const std::string& column) const {
    return data_.find(column) != data_.end();
}

std::vector<std::string> DataRecord::get_columns() const {
    std::vector<std::string> columns;
    columns.reserve(data_.size());
    for (const auto& [key, value] : data_) {
        columns.push_back(key);
    }
    std::sort(columns.begin(), columns.end());
    return columns;
}

std::string DataRecord::to_string() const {
    std::ostringstream oss;
    oss << "{";
    bool first = true;
    for (const auto& [key, value] : data_) {
        if (!first) oss << ", ";
        oss << key << ": " << ValueOps::to_string(value);
        first = false;
    }
    oss << "}";
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const DataRecord& record) {
    return os << record.to_string();
}

// DataSet implementations
void DataSet::add_record(DataRecord record) {
    // Ensure all columns exist
    for (const auto& column : columns_) {
        if (!record.has_column(column)) {
            record[column] = std::string(""); // Default empty value
        }
    }
    records_.push_back(std::move(record));
}

void DataSet::add_column(const std::string& name) {
    if (std::find(columns_.begin(), columns_.end(), name) == columns_.end()) {
        columns_.push_back(name);
        
        // Add empty values to existing records
        for (auto& record : records_) {
            if (!record.has_column(name)) {
                record[name] = std::string("");
            }
        }
    }
}

bool DataSet::has_column(const std::string& name) const {
    return std::find(columns_.begin(), columns_.end(), name) != columns_.end();
}

DataSet DataSet::filter(FilterPredicate predicate) const {
    DataSet result(columns_);
    result.reserve(records_.size() / 2); // Reasonable initial capacity
    
    std::copy_if(records_.begin(), records_.end(), 
                 std::back_inserter(result.records_), predicate);
    
    return result;
}

void DataSet::transform_column(const std::string& column, TransformFunction func) {
    if (!has_column(column)) {
        throw std::invalid_argument("Column not found: " + column);
    }
    
    std::for_each(records_.begin(), records_.end(), 
                  [&](DataRecord& record) {
                      record[column] = func(record[column]);
                  });
}

void DataSet::sort_by_column(const std::string& column, bool ascending) {
    if (!has_column(column)) {
        throw std::invalid_argument("Column not found: " + column);
    }
    
    std::sort(records_.begin(), records_.end(), 
              [&](const DataRecord& a, const DataRecord& b) {
                  bool less = ValueOps::compare_less(a[column], b[column]);
                  return ascending ? less : !less;
              });
}

DataValue DataSet::aggregate_column(const std::string& column, AggregateFunction func) const {
    if (!has_column(column)) {
        throw std::invalid_argument("Column not found: " + column);
    }
    
    std::vector<DataValue> values;
    values.reserve(records_.size());
    
    std::transform(records_.begin(), records_.end(), 
                   std::back_inserter(values),
                   [&](const DataRecord& record) { return record[column]; });
    
    return func(values);
}

std::unordered_map<std::string, DataValue> DataSet::group_by_aggregate(
    const std::string& group_column, 
    const std::string& value_column, 
    AggregateFunction func) const {
    
    if (!has_column(group_column) || !has_column(value_column)) {
        throw std::invalid_argument("Column not found");
    }
    
    std::unordered_map<std::string, std::vector<DataValue>> groups;
    
    // Group the data
    for (const auto& record : records_) {
        std::string group_key = ValueOps::to_string(record[group_column]);
        groups[group_key].push_back(record[value_column]);
    }
    
    // Apply aggregation function to each group
    std::unordered_map<std::string, DataValue> result;
    for (const auto& [group, values] : groups) {
        result[group] = func(values);
    }
    
    return result;
}

DataSet DataSet::load_from_csv(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::string line;
    std::vector<std::string> columns;
    
    // Read header
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string column;
        while (std::getline(iss, column, ',')) {
            // Trim whitespace
            column.erase(0, column.find_first_not_of(" \t\r\n"));
            column.erase(column.find_last_not_of(" \t\r\n") + 1);
            columns.push_back(column);
        }
    }
    
    DataSet dataset(columns);
    
    // Read data rows
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string cell;
        DataRow row;
        size_t col_index = 0;
        
        while (std::getline(iss, cell, ',') && col_index < columns.size()) {
            // Trim whitespace
            cell.erase(0, cell.find_first_not_of(" \t\r\n"));
            cell.erase(cell.find_last_not_of(" \t\r\n") + 1);
            
            // Try to parse as number
            DataValue value;
            try {
                if (cell.find('.') != std::string::npos) {
                    value = std::stod(cell);
                } else {
                    value = std::stoi(cell);
                }
            } catch (...) {
                value = cell; // Keep as string
            }
            
            row[columns[col_index]] = value;
            ++col_index;
        }
        
        dataset.add_record(DataRecord(std::move(row)));
    }
    
    return dataset;
}

void DataSet::save_to_csv(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create file: " + filename);
    }
    
    // Write header
    for (size_t i = 0; i < columns_.size(); ++i) {
        if (i > 0) file << ",";
        file << columns_[i];
    }
    file << "\n";
    
    // Write data
    for (const auto& record : records_) {
        for (size_t i = 0; i < columns_.size(); ++i) {
            if (i > 0) file << ",";
            if (record.has_column(columns_[i])) {
                file << ValueOps::to_string(record[columns_[i]]);
            }
        }
        file << "\n";
    }
}

std::string DataSet::to_string(size_t max_rows) const {
    std::ostringstream oss;
    oss << "DataSet (" << records_.size() << " rows, " << columns_.size() << " columns)\n";
    
    // Print header
    for (size_t i = 0; i < columns_.size(); ++i) {
        if (i > 0) oss << "\t";
        oss << std::setw(12) << columns_[i];
    }
    oss << "\n" << std::string(columns_.size() * 13, '-') << "\n";
    
    // Print data rows
    size_t rows_to_show = std::min(max_rows, records_.size());
    for (size_t i = 0; i < rows_to_show; ++i) {
        for (size_t j = 0; j < columns_.size(); ++j) {
            if (j > 0) oss << "\t";
            if (records_[i].has_column(columns_[j])) {
                oss << std::setw(12) << ValueOps::to_string(records_[i][columns_[j]]);
            } else {
                oss << std::setw(12) << "";
            }
        }
        oss << "\n";
    }
    
    if (records_.size() > max_rows) {
        oss << "... (" << (records_.size() - max_rows) << " more rows)\n";
    }
    
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const DataSet& dataset) {
    return os << dataset.to_string();
}

// Statistics implementations
std::string Statistics::DescriptiveStats::to_string() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Count: " << count << ", Mean: " << mean << ", Median: " << median
        << ", StdDev: " << std_dev << ", Min: " << min_val << ", Max: " << max_val;
    return oss.str();
}

Statistics::DescriptiveStats Statistics::calculate(const std::vector<DataValue>& values) {
    if (values.empty()) {
        return {0.0, 0.0, 0.0, 0.0, 0.0, 0};
    }
    
    // Convert to doubles and filter numeric values
    std::vector<double> numeric_values;
    for (const auto& value : values) {
        if (ValueOps::is_numeric(value)) {
            numeric_values.push_back(ValueOps::to_double(value));
        }
    }
    
    if (numeric_values.empty()) {
        return {0.0, 0.0, 0.0, 0.0, 0.0, 0};
    }
    
    DescriptiveStats stats;
    stats.count = numeric_values.size();
    
    // Calculate mean
    stats.mean = std::accumulate(numeric_values.begin(), numeric_values.end(), 0.0) / stats.count;
    
    // Calculate min and max
    auto [min_it, max_it] = std::minmax_element(numeric_values.begin(), numeric_values.end());
    stats.min_val = *min_it;
    stats.max_val = *max_it;
    
    // Calculate median
    std::vector<double> sorted_values = numeric_values;
    std::sort(sorted_values.begin(), sorted_values.end());
    if (sorted_values.size() % 2 == 0) {
        stats.median = (sorted_values[sorted_values.size() / 2 - 1] + 
                       sorted_values[sorted_values.size() / 2]) / 2.0;
    } else {
        stats.median = sorted_values[sorted_values.size() / 2];
    }
    
    // Calculate standard deviation
    double variance = std::accumulate(numeric_values.begin(), numeric_values.end(), 0.0,
        [mean = stats.mean](double acc, double val) {
            return acc + (val - mean) * (val - mean);
        }) / stats.count;
    stats.std_dev = std::sqrt(variance);
    
    return stats;
}

Statistics::DescriptiveStats Statistics::calculate_column(const DataSet& dataset, const std::string& column) {
    if (!dataset.has_column(column)) {
        throw std::invalid_argument("Column not found: " + column);
    }
    
    std::vector<DataValue> values;
    values.reserve(dataset.size());
    
    std::transform(dataset.begin(), dataset.end(), std::back_inserter(values),
                   [&](const DataRecord& record) { return record[column]; });
    
    return calculate(values);
}

double Statistics::correlation(const DataSet& dataset, const std::string& col1, const std::string& col2) {
    if (!dataset.has_column(col1) || !dataset.has_column(col2)) {
        throw std::invalid_argument("Column not found");
    }
    
    std::vector<double> x_values, y_values;
    
    for (const auto& record : dataset) {
        if (ValueOps::is_numeric(record[col1]) && ValueOps::is_numeric(record[col2])) {
            x_values.push_back(ValueOps::to_double(record[col1]));
            y_values.push_back(ValueOps::to_double(record[col2]));
        }
    }
    
    if (x_values.size() < 2) {
        return 0.0;
    }
    
    double mean_x = std::accumulate(x_values.begin(), x_values.end(), 0.0) / x_values.size();
    double mean_y = std::accumulate(y_values.begin(), y_values.end(), 0.0) / y_values.size();
    
    double numerator = 0.0, sum_sq_x = 0.0, sum_sq_y = 0.0;
    
    for (size_t i = 0; i < x_values.size(); ++i) {
        double dx = x_values[i] - mean_x;
        double dy = y_values[i] - mean_y;
        numerator += dx * dy;
        sum_sq_x += dx * dx;
        sum_sq_y += dy * dy;
    }
    
    double denominator = std::sqrt(sum_sq_x * sum_sq_y);
    return (denominator == 0.0) ? 0.0 : numerator / denominator;
}

std::unordered_map<std::string, size_t> Statistics::frequency_count(
    const DataSet& dataset, const std::string& column) {
    
    if (!dataset.has_column(column)) {
        throw std::invalid_argument("Column not found: " + column);
    }
    
    std::unordered_map<std::string, size_t> frequencies;
    
    for (const auto& record : dataset) {
        std::string value = ValueOps::to_string(record[column]);
        ++frequencies[value];
    }
    
    return frequencies;
}

// PerformanceMonitor implementations
PerformanceMonitor::PerformanceMonitor(std::string operation_name) 
    : start_time_(std::chrono::high_resolution_clock::now()), 
      operation_name_(std::move(operation_name)) {
    std::cout << "[PERF] Starting: " << operation_name_ << std::endl;
}

PerformanceMonitor::~PerformanceMonitor() {
    double elapsed = elapsed_ms();
    std::cout << "[PERF] Completed: " << operation_name_ 
              << " (" << std::fixed << std::setprecision(3) << elapsed << " ms)" << std::endl;
}

double PerformanceMonitor::elapsed_ms() const {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - start_time_);
    return duration.count() / 1000.0;
}

// Pipeline implementations
Pipeline& Pipeline::filter(FilterPredicate predicate) {
    operations_.push_back([predicate](DataSet& dataset) {
        dataset = dataset.filter(predicate);
    });
    return *this;
}

Pipeline& Pipeline::transform(const std::string& column, TransformFunction func) {
    operations_.push_back([column, func](DataSet& dataset) {
        dataset.transform_column(column, func);
    });
    return *this;
}

Pipeline& Pipeline::sort_by(const std::string& column, bool ascending) {
    operations_.push_back([column, ascending](DataSet& dataset) {
        dataset.sort_by_column(column, ascending);
    });
    return *this;
}

Pipeline& Pipeline::add_column(const std::string& name, 
                              std::function<DataValue(const DataRecord&)> calculator) {
    operations_.push_back([name, calculator](DataSet& dataset) {
        dataset.add_column(name);
        for (auto& record : dataset) {
            record[name] = calculator(record);
        }
    });
    return *this;
}

DataSet Pipeline::execute(DataSet input) const {
    MONITOR_PERFORMANCE("Pipeline execution");
    
    for (const auto& operation : operations_) {
        operation(input);
    }
    
    return input;
}

// Aggregate functions
namespace Aggregates {
    const AggregateFunction Sum = [](const std::vector<DataValue>& values) -> DataValue {
        double sum = 0.0;
        for (const auto& value : values) {
            if (ValueOps::is_numeric(value)) {
                sum += ValueOps::to_double(value);
            }
        }
        return sum;
    };
    
    const AggregateFunction Mean = [](const std::vector<DataValue>& values) -> DataValue {
        if (values.empty()) return 0.0;
        double sum = ValueOps::to_double(Sum(values));
        return sum / values.size();
    };
    
    const AggregateFunction Min = [](const std::vector<DataValue>& values) -> DataValue {
        if (values.empty()) return 0.0;
        auto min_val = std::min_element(values.begin(), values.end(), ValueOps::compare_less);
        return *min_val;
    };
    
    const AggregateFunction Max = [](const std::vector<DataValue>& values) -> DataValue {
        if (values.empty()) return 0.0;
        auto max_val = std::max_element(values.begin(), values.end(), ValueOps::compare_less);
        return *max_val;
    };
    
    const AggregateFunction Count = [](const std::vector<DataValue>& values) -> DataValue {
        return static_cast<int>(values.size());
    };
    
    const AggregateFunction StdDev = [](const std::vector<DataValue>& values) -> DataValue {
        auto stats = Statistics::calculate(values);
        return stats.std_dev;
    };
}

// Filter predicates
namespace Filters {
    FilterPredicate column_equals(const std::string& column, const DataValue& value) {
        return [column, value](const DataRecord& record) {
            return record.has_column(column) && 
                   ValueOps::to_string(record[column]) == ValueOps::to_string(value);
        };
    }
    
    FilterPredicate column_greater_than(const std::string& column, const DataValue& value) {
        return [column, value](const DataRecord& record) {
            return record.has_column(column) && 
                   !ValueOps::compare_less(record[column], value) &&
                   ValueOps::to_string(record[column]) != ValueOps::to_string(value);
        };
    }
    
    FilterPredicate column_less_than(const std::string& column, const DataValue& value) {
        return [column, value](const DataRecord& record) {
            return record.has_column(column) && 
                   ValueOps::compare_less(record[column], value);
        };
    }
    
    FilterPredicate column_contains(const std::string& column, const std::string& substring) {
        return [column, substring](const DataRecord& record) {
            if (!record.has_column(column)) return false;
            std::string value = ValueOps::to_string(record[column]);
            return value.find(substring) != std::string::npos;
        };
    }
    
    FilterPredicate logical_and(FilterPredicate a, FilterPredicate b) {
        return [a, b](const DataRecord& record) {
            return a(record) && b(record);
        };
    }
    
    FilterPredicate logical_or(FilterPredicate a, FilterPredicate b) {
        return [a, b](const DataRecord& record) {
            return a(record) || b(record);
        };
    }
    
    FilterPredicate logical_not(FilterPredicate pred) {
        return [pred](const DataRecord& record) {
            return !pred(record);
        };
    }
}

// Transform functions
namespace Transforms {
    const TransformFunction ToUpper = [](const DataValue& value) -> DataValue {
        std::string str = ValueOps::to_string(value);
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    };
    
    const TransformFunction ToLower = [](const DataValue& value) -> DataValue {
        std::string str = ValueOps::to_string(value);
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    };
    
    const TransformFunction Square = [](const DataValue& value) -> DataValue {
        if (ValueOps::is_numeric(value)) {
            double val = ValueOps::to_double(value);
            return val * val;
        }
        return value;
    };
    
    const TransformFunction SquareRoot = [](const DataValue& value) -> DataValue {
        if (ValueOps::is_numeric(value)) {
            double val = ValueOps::to_double(value);
            return std::sqrt(std::abs(val));
        }
        return value;
    };
    
    const TransformFunction Absolute = [](const DataValue& value) -> DataValue {
        if (ValueOps::is_numeric(value)) {
            double val = ValueOps::to_double(value);
            return std::abs(val);
        }
        return value;
    };
    
    TransformFunction multiply_by(double factor) {
        return [factor](const DataValue& value) -> DataValue {
            if (ValueOps::is_numeric(value)) {
                return ValueOps::to_double(value) * factor;
            }
            return value;
        };
    }
    
    TransformFunction add_constant(double constant) {
        return [constant](const DataValue& value) -> DataValue {
            if (ValueOps::is_numeric(value)) {
                return ValueOps::to_double(value) + constant;
            }
            return value;
        };
    }
    
    TransformFunction string_replace(const std::string& from, const std::string& to) {
        return [from, to](const DataValue& value) -> DataValue {
            std::string str = ValueOps::to_string(value);
            size_t pos = str.find(from);
            if (pos != std::string::npos) {
                str.replace(pos, from.length(), to);
            }
            return str;
        };
    }
}

} // namespace DataProcessing
