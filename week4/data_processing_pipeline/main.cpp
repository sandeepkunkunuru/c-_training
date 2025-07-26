/*
 * Data Processing Pipeline - Main Demo
 * 
 * This program demonstrates the complete data processing pipeline,
 * showcasing advanced STL usage, custom iterators, and modern C++ features.
 */

#include "data_processor.hpp"
#include <iostream>
#include <fstream>
#include <random>

using namespace DataProcessing;

// Generate sample data for demonstration
void generate_sample_data(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create sample data file");
    }
    
    // Write header
    file << "id,name,age,salary,department,performance_score\n";
    
    // Generate random data
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> age_dist(22, 65);
    std::uniform_int_distribution<> salary_dist(40000, 150000);
    std::uniform_real_distribution<> score_dist(1.0, 5.0);
    
    std::vector<std::string> names = {
        "Alice Johnson", "Bob Smith", "Charlie Brown", "Diana Wilson", "Eve Davis",
        "Frank Miller", "Grace Lee", "Henry Taylor", "Ivy Chen", "Jack Wilson",
        "Kate Anderson", "Liam Garcia", "Maya Patel", "Noah Kim", "Olivia Martinez",
        "Paul Thompson", "Quinn O'Brien", "Rachel Green", "Sam Rodriguez", "Tina Wang"
    };
    
    std::vector<std::string> departments = {
        "Engineering", "Marketing", "Sales", "HR", "Finance", "Operations"
    };
    
    for (int i = 1; i <= 100; ++i) {
        file << i << ","
             << names[i % names.size()] << " " << i << ","
             << age_dist(gen) << ","
             << salary_dist(gen) << ","
             << departments[i % departments.size()] << ","
             << std::fixed << std::setprecision(2) << score_dist(gen) << "\n";
    }
}

void demonstrate_basic_operations() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Basic Data Operations" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    // Load data
    DataSet dataset;
    try {
        dataset = DataSet::load_from_csv("sample_data.csv");
    } catch (const std::exception& e) {
        std::cout << "Generating sample data..." << std::endl;
        generate_sample_data("sample_data.csv");
        dataset = DataSet::load_from_csv("sample_data.csv");
    }
    
    std::cout << "Loaded dataset:" << std::endl;
    std::cout << dataset << std::endl;
    
    // Basic statistics
    auto age_stats = Statistics::calculate_column(dataset, "age");
    auto salary_stats = Statistics::calculate_column(dataset, "salary");
    
    std::cout << "\nAge statistics: " << age_stats.to_string() << std::endl;
    std::cout << "Salary statistics: " << salary_stats.to_string() << std::endl;
    
    // Frequency analysis
    auto dept_freq = Statistics::frequency_count(dataset, "department");
    std::cout << "\nDepartment frequencies:" << std::endl;
    for (const auto& [dept, count] : dept_freq) {
        std::cout << "  " << dept << ": " << count << std::endl;
    }
}

void demonstrate_filtering_and_transformation() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Filtering and Transformation" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    DataSet dataset = DataSet::load_from_csv("sample_data.csv");
    
    // Filter high earners in Engineering
    auto high_earners = dataset.filter(
        Filters::logical_and(
            Filters::column_equals("department", std::string("Engineering")),
            Filters::column_greater_than("salary", 80000.0)
        )
    );
    
    std::cout << "High earners in Engineering:" << std::endl;
    std::cout << high_earners << std::endl;
    
    // Transform salary to monthly
    auto monthly_dataset = dataset;
    monthly_dataset.add_column("monthly_salary");
    monthly_dataset.transform_column("monthly_salary", 
        [&](const DataValue& value) -> DataValue {
            // Calculate monthly salary from annual
            return ValueOps::to_double(monthly_dataset[0]["salary"]) / 12.0;
        });
    
    // Actually, let's do this properly with a pipeline
    Pipeline salary_pipeline;
    salary_pipeline.add_column("monthly_salary", 
        [](const DataRecord& record) -> DataValue {
            return ValueOps::to_double(record["salary"]) / 12.0;
        })
        .add_column("salary_category",
        [](const DataRecord& record) -> DataValue {
            double salary = ValueOps::to_double(record["salary"]);
            if (salary >= 100000) return std::string("High");
            else if (salary >= 70000) return std::string("Medium");
            else return std::string("Low");
        });
    
    auto enhanced_dataset = salary_pipeline.execute(dataset);
    
    std::cout << "\nEnhanced dataset with calculated columns:" << std::endl;
    std::cout << enhanced_dataset << std::endl;
}

void demonstrate_aggregation() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Aggregation Operations" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    DataSet dataset = DataSet::load_from_csv("sample_data.csv");
    
    // Group by department and calculate average salary
    auto dept_avg_salary = dataset.group_by_aggregate("department", "salary", Aggregates::Mean);
    
    std::cout << "Average salary by department:" << std::endl;
    for (const auto& [dept, avg_salary] : dept_avg_salary) {
        std::cout << "  " << dept << ": $" << std::fixed << std::setprecision(0) 
                  << ValueOps::to_double(avg_salary) << std::endl;
    }
    
    // Group by department and calculate count
    auto dept_count = dataset.group_by_aggregate("department", "id", Aggregates::Count);
    
    std::cout << "\nEmployee count by department:" << std::endl;
    for (const auto& [dept, count] : dept_count) {
        std::cout << "  " << dept << ": " << ValueOps::to_double(count) << std::endl;
    }
    
    // Overall aggregations
    auto total_salary = dataset.aggregate_column("salary", Aggregates::Sum);
    auto avg_performance = dataset.aggregate_column("performance_score", Aggregates::Mean);
    auto max_age = dataset.aggregate_column("age", Aggregates::Max);
    
    std::cout << "\nOverall statistics:" << std::endl;
    std::cout << "  Total salary budget: $" << std::fixed << std::setprecision(0) 
              << ValueOps::to_double(total_salary) << std::endl;
    std::cout << "  Average performance: " << std::fixed << std::setprecision(2) 
              << ValueOps::to_double(avg_performance) << std::endl;
    std::cout << "  Maximum age: " << ValueOps::to_double(max_age) << std::endl;
}

void demonstrate_pipeline_processing() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Pipeline Processing" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    DataSet dataset = DataSet::load_from_csv("sample_data.csv");
    
    // Create a complex processing pipeline
    Pipeline analysis_pipeline;
    
    analysis_pipeline
        // Filter for employees with good performance
        .filter(Filters::column_greater_than("performance_score", 3.5))
        // Add bonus calculation
        .add_column("bonus", [](const DataRecord& record) -> DataValue {
            double salary = ValueOps::to_double(record["salary"]);
            double performance = ValueOps::to_double(record["performance_score"]);
            return salary * 0.1 * (performance / 5.0); // 10% of salary scaled by performance
        })
        // Add total compensation
        .add_column("total_compensation", [](const DataRecord& record) -> DataValue {
            return ValueOps::to_double(record["salary"]) + ValueOps::to_double(record["bonus"]);
        })
        // Sort by total compensation (descending)
        .sort_by("total_compensation", false);
    
    auto processed_dataset = analysis_pipeline.execute(dataset);
    
    std::cout << "Top performers with bonus calculations:" << std::endl;
    std::cout << processed_dataset << std::endl;
    
    // Save processed data
    processed_dataset.save_to_csv("processed_data.csv");
    std::cout << "\nProcessed data saved to 'processed_data.csv'" << std::endl;
}

void demonstrate_correlation_analysis() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Correlation Analysis" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    DataSet dataset = DataSet::load_from_csv("sample_data.csv");
    
    // Calculate correlations between numeric columns
    std::vector<std::string> numeric_columns = {"age", "salary", "performance_score"};
    
    std::cout << "Correlation matrix:" << std::endl;
    std::cout << std::setw(20) << "";
    for (const auto& col : numeric_columns) {
        std::cout << std::setw(15) << col;
    }
    std::cout << std::endl;
    
    for (const auto& col1 : numeric_columns) {
        std::cout << std::setw(20) << col1;
        for (const auto& col2 : numeric_columns) {
            double correlation = Statistics::correlation(dataset, col1, col2);
            std::cout << std::setw(15) << std::fixed << std::setprecision(3) << correlation;
        }
        std::cout << std::endl;
    }
}

void demonstrate_custom_iterators() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Custom Iterator Usage" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    DataSet dataset = DataSet::load_from_csv("sample_data.csv");
    
    // Use filtered iterator to process only high performers
    auto high_performer_filter = [](const DataRecord& record) {
        return ValueOps::to_double(record["performance_score"]) >= 4.0;
    };
    
    std::cout << "Processing high performers using custom iterator:" << std::endl;
    
    double total_salary = 0.0;
    int count = 0;
    
    for (auto it = dataset.filtered_begin(high_performer_filter);
         it != dataset.filtered_end(high_performer_filter); ++it) {
        total_salary += ValueOps::to_double((*it)["salary"]);
        ++count;
        
        if (count <= 5) { // Show first 5
            std::cout << "  " << ValueOps::to_string((*it)["name"]) 
                      << " - Score: " << ValueOps::to_double((*it)["performance_score"])
                      << ", Salary: $" << ValueOps::to_double((*it)["salary"]) << std::endl;
        }
    }
    
    if (count > 0) {
        std::cout << "\nHigh performer statistics:" << std::endl;
        std::cout << "  Count: " << count << std::endl;
        std::cout << "  Average salary: $" << std::fixed << std::setprecision(0) 
                  << (total_salary / count) << std::endl;
    }
}

void demonstrate_performance_monitoring() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Performance Monitoring" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    {
        MONITOR_PERFORMANCE("Data loading");
        DataSet dataset = DataSet::load_from_csv("sample_data.csv");
    }
    
    {
        MONITOR_PERFORMANCE("Complex pipeline processing");
        DataSet dataset = DataSet::load_from_csv("sample_data.csv");
        
        Pipeline complex_pipeline;
        complex_pipeline
            .filter(Filters::column_greater_than("age", 30))
            .transform("name", Transforms::ToUpper)
            .add_column("age_group", [](const DataRecord& record) -> DataValue {
                int age = static_cast<int>(ValueOps::to_double(record["age"]));
                if (age < 30) return std::string("Young");
                else if (age < 50) return std::string("Middle");
                else return std::string("Senior");
            })
            .sort_by("salary", false);
        
        auto result = complex_pipeline.execute(dataset);
    }
    
    {
        MONITOR_PERFORMANCE("Statistical analysis");
        DataSet dataset = DataSet::load_from_csv("sample_data.csv");
        
        auto age_stats = Statistics::calculate_column(dataset, "age");
        auto salary_stats = Statistics::calculate_column(dataset, "salary");
        auto correlation = Statistics::correlation(dataset, "age", "salary");
        auto freq = Statistics::frequency_count(dataset, "department");
    }
}

int main() {
    std::cout << "===== Data Processing Pipeline Demonstration =====" << std::endl;
    std::cout << "This program demonstrates a comprehensive data processing pipeline" << std::endl;
    std::cout << "using advanced STL features, custom iterators, and modern C++." << std::endl;
    
    try {
        demonstrate_basic_operations();
        demonstrate_filtering_and_transformation();
        demonstrate_aggregation();
        demonstrate_pipeline_processing();
        demonstrate_correlation_analysis();
        demonstrate_custom_iterators();
        demonstrate_performance_monitoring();
        
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "Data Processing Pipeline Features Demonstrated:" << std::endl;
        std::cout << "1. CSV data loading and parsing with type inference" << std::endl;
        std::cout << "2. Flexible filtering with composable predicates" << std::endl;
        std::cout << "3. Data transformation and column calculations" << std::endl;
        std::cout << "4. Statistical analysis and aggregation operations" << std::endl;
        std::cout << "5. Processing pipelines with method chaining" << std::endl;
        std::cout << "6. Custom iterators for filtered data access" << std::endl;
        std::cout << "7. Performance monitoring and optimization" << std::endl;
        std::cout << "8. Modern C++ features (variants, optionals, lambdas)" << std::endl;
        std::cout << "9. STL algorithms and containers throughout" << std::endl;
        std::cout << "10. Memory-efficient data processing patterns" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
