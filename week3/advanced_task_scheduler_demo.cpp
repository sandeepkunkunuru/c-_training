#include "advanced_task_scheduler.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <thread>

// Helper function to convert TaskStatus to string
std::string statusToString(TaskStatus status) {
    switch (status) {
        case TaskStatus::PENDING: return "PENDING";
        case TaskStatus::RUNNING: return "RUNNING";
        case TaskStatus::COMPLETED: return "COMPLETED";
        case TaskStatus::FAILED: return "FAILED";
        case TaskStatus::CANCELLED: return "CANCELLED";
        default: return "UNKNOWN";
    }
}

// Helper function to convert Priority to string
std::string priorityToString(Priority priority) {
    switch (priority) {
        case Priority::LOW: return "LOW";
        case Priority::MEDIUM: return "MEDIUM";
        case Priority::HIGH: return "HIGH";
        case Priority::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

// Helper function to print task statistics
void printStatistics(const TaskScheduler::TaskStatistics& stats) {
    std::cout << "\n--- Task Scheduler Statistics ---\n";
    std::cout << "Total tasks:     " << stats.totalTasks << "\n";
    std::cout << "Completed tasks: " << stats.completedTasks << "\n";
    std::cout << "Failed tasks:    " << stats.failedTasks << "\n";
    std::cout << "Cancelled tasks: " << stats.cancelledTasks << "\n";
    std::cout << "Pending tasks:   " << stats.pendingTasks << "\n";
    std::cout << "Active threads:  " << stats.activeThreads << "\n";
    std::cout << "Max threads:     " << stats.maxThreads << "\n";
    std::cout << "-------------------------------\n";
}

// Helper function to print task list
void printTaskList(const std::vector<std::pair<std::string, TaskStatus>>& tasks) {
    std::cout << "\n--- Task List ---\n";
    std::cout << std::left << std::setw(30) << "Task Name" << "Status\n";
    std::cout << std::string(40, '-') << "\n";
    
    for (const auto& task : tasks) {
        std::cout << std::left << std::setw(30) << task.first 
                  << statusToString(task.second) << "\n";
    }
    std::cout << "---------------\n";
}

// Example tasks
int computeFactorial(int n) {
    if (n < 0) {
        throw std::invalid_argument("Factorial not defined for negative numbers");
    }
    
    int result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
        // Simulate some work
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return result;
}

std::string generateRandomString(int length) {
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, chars.size() - 1);
    
    std::string result;
    result.reserve(length);
    
    for (int i = 0; i < length; ++i) {
        result += chars[distribution(generator)];
        // Simulate some work
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    
    return result;
}

std::vector<int> sortNumbers(const std::vector<int>& numbers) {
    std::vector<int> result = numbers;
    std::sort(result.begin(), result.end());
    
    // Simulate some work
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    return result;
}

void longRunningTask() {
    for (int i = 0; i < 10; ++i) {
        std::cout << "Long running task: step " << (i + 1) << "/10\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void failingTask() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    throw std::runtime_error("This task is designed to fail");
}

int main() {
    std::cout << "===== Advanced Task Scheduler Demo =====\n\n";
    
    // Create a task scheduler with 4 threads
    TaskScheduler scheduler(4);
    
    std::cout << "Scheduling various tasks...\n";
    
    // Schedule tasks with different priorities and return types
    auto factorialTask = scheduler.scheduleTask<int>(
        "factorial_10", 
        []() { return computeFactorial(10); },
        Priority::HIGH
    );
    
    auto stringTask = scheduler.scheduleTask<std::string>(
        "random_string", 
        []() { return generateRandomString(20); },
        Priority::MEDIUM
    );
    
    std::vector<int> numbersToSort = {9, 5, 3, 7, 1, 8, 2, 6, 4, 0};
    auto sortTask = scheduler.scheduleTask<std::vector<int>>(
        "sort_numbers", 
        [numbersToSort]() { return sortNumbers(numbersToSort); },
        Priority::LOW
    );
    
    // Schedule a void task
    auto voidTask = scheduler.scheduleTask<void>(
        "long_running", 
        longRunningTask,
        Priority::LOW
    );
    
    // Schedule a task that will fail
    auto failTask = scheduler.scheduleTask<void>(
        "failing_task", 
        failingTask,
        Priority::MEDIUM
    );
    
    // Schedule a task for the future
    auto futureTask = scheduler.scheduleTask<std::string>(
        "future_task", 
        []() { 
            std::cout << "Future task executed!\n";
            return "I came from the future"; 
        },
        Priority::HIGH,
        std::chrono::system_clock::now() + std::chrono::seconds(5)
    );
    
    // Try to schedule a duplicate task (should throw)
    try {
        scheduler.scheduleTask<int>(
            "factorial_10", 
            []() { return computeFactorial(5); },
            Priority::MEDIUM
        );
    } catch (const DuplicateTaskException& e) {
        std::cout << "Expected exception caught: " << e.what() << "\n";
    }
    
    // Print initial task list
    printTaskList(scheduler.listTasks());
    printStatistics(scheduler.getStatistics());
    
    // Wait a bit for some tasks to complete
    std::cout << "\nWaiting for some tasks to complete...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // Print updated task list
    printTaskList(scheduler.listTasks());
    printStatistics(scheduler.getStatistics());
    
    // Try to cancel a task
    try {
        bool cancelled = scheduler.cancelTask("future_task");
        std::cout << "\nCancelled future_task: " << (cancelled ? "yes" : "no") << "\n";
    } catch (const TaskSchedulerException& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
    
    // Wait for specific task and get result
    try {
        std::cout << "\nWaiting for factorial_10 task to complete...\n";
        int result = scheduler.waitForTask<int>("factorial_10");
        std::cout << "factorial_10 result: " << result << "\n";
    } catch (const TaskSchedulerException& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
    
    // Try to get result from the failing task (should throw)
    try {
        std::cout << "\nTrying to get result from failing_task...\n";
        scheduler.waitForTask<void>("failing_task");
    } catch (const std::runtime_error& e) {
        std::cout << "Expected exception from failing_task: " << e.what() << "\n";
    }
    
    // Wait for all remaining tasks
    std::cout << "\nWaiting for all remaining tasks to complete...\n";
    scheduler.waitForAll();
    
    // Print final task list and statistics
    printTaskList(scheduler.listTasks());
    printStatistics(scheduler.getStatistics());
    
    // Get results from completed tasks
    try {
        auto randomString = scheduler.getTaskResult<std::string>("random_string").getResult();
        std::cout << "\nRandom string result: " << randomString << "\n";
        
        auto sortedNumbers = scheduler.getTaskResult<std::vector<int>>("sort_numbers").getResult();
        std::cout << "Sorted numbers result: ";
        for (int num : sortedNumbers) {
            std::cout << num << " ";
        }
        std::cout << "\n";
    } catch (const TaskSchedulerException& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
    
    // Clean up completed tasks
    std::cout << "\nCleaning up completed tasks...\n";
    scheduler.cleanup();
    
    // Print final task list after cleanup
    printTaskList(scheduler.listTasks());
    
    std::cout << "\n===== Advanced Task Scheduler Demo Complete =====\n";
    return 0;
}
