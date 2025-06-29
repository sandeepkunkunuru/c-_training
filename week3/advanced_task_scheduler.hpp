#ifndef ADVANCED_TASK_SCHEDULER_HPP
#define ADVANCED_TASK_SCHEDULER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <functional>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>
#include <optional>
#include <variant>
#include <any>
#include <memory>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <type_traits>

// Custom exceptions for the task scheduler
class TaskSchedulerException : public std::runtime_error {
public:
    explicit TaskSchedulerException(const std::string& message) 
        : std::runtime_error(message) {}
};

class TaskNotFoundException : public TaskSchedulerException {
public:
    explicit TaskNotFoundException(const std::string& message) 
        : TaskSchedulerException(message) {}
};

class DuplicateTaskException : public TaskSchedulerException {
public:
    explicit DuplicateTaskException(const std::string& message) 
        : TaskSchedulerException(message) {}
};

// Task priority levels
enum class Priority {
    LOW,
    MEDIUM,
    HIGH,
    CRITICAL
};

// Task status
enum class TaskStatus {
    PENDING,
    RUNNING,
    COMPLETED,
    FAILED,
    CANCELLED
};

// Forward declaration
template<typename ResultType>
class Task;

// Task result wrapper to handle exceptions and status
template<typename ResultType>
class TaskResult {
private:
    std::optional<ResultType> result_;
    std::optional<std::exception_ptr> exception_;
    TaskStatus status_;

public:
    TaskResult() : status_(TaskStatus::PENDING) {}
    
    void setResult(ResultType&& result) {
        result_ = std::move(result);
        status_ = TaskStatus::COMPLETED;
    }
    
    void setException(std::exception_ptr exception) {
        exception_ = exception;
        status_ = TaskStatus::FAILED;
    }
    
    void setStatus(TaskStatus status) {
        status_ = status;
    }
    
    TaskStatus getStatus() const {
        return status_;
    }
    
    bool hasResult() const {
        return result_.has_value();
    }
    
    bool hasException() const {
        return exception_.has_value();
    }
    
    // Get result (throws if there was an exception)
    ResultType getResult() const {
        if (hasException()) {
            std::rethrow_exception(*exception_);
        }
        if (!hasResult()) {
            throw TaskSchedulerException("Task has no result");
        }
        return *result_;
    }
    
    // Try to get result without throwing
    std::optional<ResultType> tryGetResult() const {
        return result_;
    }
};

// Specialization for void result type
template<>
class TaskResult<void> {
private:
    std::optional<std::exception_ptr> exception_;
    TaskStatus status_;

public:
    TaskResult() : status_(TaskStatus::PENDING) {}
    
    void setCompleted() {
        status_ = TaskStatus::COMPLETED;
    }
    
    void setException(std::exception_ptr exception) {
        exception_ = exception;
        status_ = TaskStatus::FAILED;
    }
    
    void setStatus(TaskStatus status) {
        status_ = status;
    }
    
    TaskStatus getStatus() const {
        return status_;
    }
    
    bool hasException() const {
        return exception_.has_value();
    }
    
    // Get result (throws if there was an exception)
    void getResult() const {
        if (hasException()) {
            std::rethrow_exception(*exception_);
        }
        if (status_ != TaskStatus::COMPLETED) {
            throw TaskSchedulerException("Task not completed");
        }
    }
};

// Base task interface using type erasure
class TaskBase {
public:
    virtual ~TaskBase() = default;
    virtual void execute() = 0;
    virtual TaskStatus getStatus() const = 0;
    virtual std::string getName() const = 0;
    virtual Priority getPriority() const = 0;
    virtual std::chrono::system_clock::time_point getScheduledTime() const = 0;
    virtual void cancel() = 0;
    virtual bool isCancellable() const = 0;
    virtual std::any getResultAsAny() const = 0;
};

// Task implementation with templated result type
template<typename ResultType>
class Task : public TaskBase {
private:
    std::string name_;
    std::function<ResultType()> function_;
    TaskResult<ResultType> result_;
    Priority priority_;
    std::chrono::system_clock::time_point scheduledTime_;
    std::atomic<TaskStatus> status_;
    bool cancellable_;
    
public:
    Task(std::string name, 
         std::function<ResultType()> function,
         Priority priority = Priority::MEDIUM,
         std::chrono::system_clock::time_point scheduledTime = std::chrono::system_clock::now(),
         bool cancellable = true)
        : name_(std::move(name))
        , function_(std::move(function))
        , priority_(priority)
        , scheduledTime_(scheduledTime)
        , status_(TaskStatus::PENDING)
        , cancellable_(cancellable) {}
    
    void execute() override {
        if (status_ == TaskStatus::CANCELLED) {
            return;
        }
        
        status_ = TaskStatus::RUNNING;
        
        try {
            if constexpr (std::is_same_v<ResultType, void>) {
                function_();
                result_.setCompleted();
            } else {
                result_.setResult(function_());
            }
            status_ = TaskStatus::COMPLETED;
        } catch (...) {
            result_.setException(std::current_exception());
            status_ = TaskStatus::FAILED;
        }
    }
    
    TaskStatus getStatus() const override {
        return status_;
    }
    
    std::string getName() const override {
        return name_;
    }
    
    Priority getPriority() const override {
        return priority_;
    }
    
    std::chrono::system_clock::time_point getScheduledTime() const override {
        return scheduledTime_;
    }
    
    void cancel() override {
        if (cancellable_ && status_ == TaskStatus::PENDING) {
            status_ = TaskStatus::CANCELLED;
            result_.setStatus(TaskStatus::CANCELLED);
        }
    }
    
    bool isCancellable() const override {
        return cancellable_;
    }
    
    const TaskResult<ResultType>& getResult() const {
        return result_;
    }
    
    std::any getResultAsAny() const override {
        if constexpr (std::is_same_v<ResultType, void>) {
            return std::any();
        } else {
            if (result_.hasResult()) {
                return std::any(result_.tryGetResult());
            }
            return std::any();
        }
    }
};

// Task comparator for priority queue
struct TaskComparator {
    bool operator()(const std::shared_ptr<TaskBase>& a, const std::shared_ptr<TaskBase>& b) const {
        // First compare by priority
        if (a->getPriority() != b->getPriority()) {
            return a->getPriority() < b->getPriority();
        }
        
        // Then by scheduled time
        return a->getScheduledTime() > b->getScheduledTime();
    }
};

// Thread pool for executing tasks
class ThreadPool {
private:
    std::vector<std::thread> workers_;
    std::priority_queue<std::shared_ptr<TaskBase>, 
                        std::vector<std::shared_ptr<TaskBase>>, 
                        TaskComparator> tasks_;
    std::mutex queueMutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_;
    std::atomic<size_t> activeThreads_;
    size_t maxThreads_;

public:
    explicit ThreadPool(size_t threads = std::thread::hardware_concurrency())
        : stop_(false), activeThreads_(0), maxThreads_(threads) {
        
        for (size_t i = 0; i < threads; ++i) {
            workers_.emplace_back([this] {
                while (true) {
                    std::shared_ptr<TaskBase> task;
                    
                    {
                        std::unique_lock<std::mutex> lock(queueMutex_);
                        condition_.wait(lock, [this] { 
                            return stop_ || !tasks_.empty(); 
                        });
                        
                        if (stop_ && tasks_.empty()) {
                            return;
                        }
                        
                        task = tasks_.top();
                        tasks_.pop();
                    }
                    
                    ++activeThreads_;
                    task->execute();
                    --activeThreads_;
                }
            });
        }
    }
    
    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            stop_ = true;
        }
        
        condition_.notify_all();
        
        for (std::thread& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }
    
    void enqueue(std::shared_ptr<TaskBase> task) {
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            if (stop_) {
                throw TaskSchedulerException("ThreadPool has been stopped");
            }
            tasks_.push(std::move(task));
        }
        condition_.notify_one();
    }
    
    size_t getQueueSize() const {
        std::unique_lock<std::mutex> lock(const_cast<std::mutex&>(queueMutex_));
        return tasks_.size();
    }
    
    size_t getActiveThreadCount() const {
        return activeThreads_;
    }
    
    size_t getMaxThreadCount() const {
        return maxThreads_;
    }
};

// Main task scheduler class
class TaskScheduler {
private:
    ThreadPool threadPool_;
    std::unordered_map<std::string, std::shared_ptr<TaskBase>> tasks_;
    std::mutex tasksMutex_;
    
    // Task statistics
    struct Statistics {
        std::atomic<size_t> totalTasks{0};
        std::atomic<size_t> completedTasks{0};
        std::atomic<size_t> failedTasks{0};
        std::atomic<size_t> cancelledTasks{0};
    };
    
    Statistics stats_;

public:
    explicit TaskScheduler(size_t threadCount = std::thread::hardware_concurrency())
        : threadPool_(threadCount) {}
    
    // Schedule a task with a result
    template<typename ResultType>
    std::shared_ptr<Task<ResultType>> scheduleTask(
        const std::string& name,
        std::function<ResultType()> function,
        Priority priority = Priority::MEDIUM,
        std::chrono::system_clock::time_point scheduledTime = std::chrono::system_clock::now(),
        bool cancellable = true) {
        
        std::unique_lock<std::mutex> lock(tasksMutex_);
        
        if (tasks_.find(name) != tasks_.end()) {
            throw DuplicateTaskException("Task with name '" + name + "' already exists");
        }
        
        auto task = std::make_shared<Task<ResultType>>(
            name, std::move(function), priority, scheduledTime, cancellable);
        
        tasks_[name] = task;
        stats_.totalTasks++;
        
        // If the scheduled time is now or in the past, enqueue immediately
        if (scheduledTime <= std::chrono::system_clock::now()) {
            threadPool_.enqueue(task);
        } else {
            // Create a timer thread to enqueue the task at the scheduled time
            std::thread([this, task, scheduledTime]() {
                auto now = std::chrono::system_clock::now();
                if (now < scheduledTime) {
                    std::this_thread::sleep_until(scheduledTime);
                }
                threadPool_.enqueue(task);
            }).detach();
        }
        
        return task;
    }
    
    // Cancel a task by name
    bool cancelTask(const std::string& name) {
        std::unique_lock<std::mutex> lock(tasksMutex_);
        
        auto it = tasks_.find(name);
        if (it == tasks_.end()) {
            throw TaskNotFoundException("Task '" + name + "' not found");
        }
        
        auto task = it->second;
        if (task->isCancellable() && task->getStatus() == TaskStatus::PENDING) {
            task->cancel();
            stats_.cancelledTasks++;
            return true;
        }
        
        return false;
    }
    
    // Get task status by name
    TaskStatus getTaskStatus(const std::string& name) const {
        std::unique_lock<std::mutex> lock(const_cast<std::mutex&>(tasksMutex_));
        
        auto it = tasks_.find(name);
        if (it == tasks_.end()) {
            throw TaskNotFoundException("Task '" + name + "' not found");
        }
        
        auto status = it->second->getStatus();
        
        // Update statistics if needed
        if (status == TaskStatus::COMPLETED && 
            it->second->getStatus() != TaskStatus::COMPLETED) {
            const_cast<std::atomic<size_t>&>(stats_.completedTasks)++;
        } else if (status == TaskStatus::FAILED && 
                  it->second->getStatus() != TaskStatus::FAILED) {
            const_cast<std::atomic<size_t>&>(stats_.failedTasks)++;
        }
        
        return status;
    }
    
    // Get task result by name (type-erased version)
    std::any getTaskResultAsAny(const std::string& name) const {
        std::unique_lock<std::mutex> lock(const_cast<std::mutex&>(tasksMutex_));
        
        auto it = tasks_.find(name);
        if (it == tasks_.end()) {
            throw TaskNotFoundException("Task '" + name + "' not found");
        }
        
        return it->second->getResultAsAny();
    }
    
    // Get task result by name (typed version)
    template<typename ResultType>
    TaskResult<ResultType> getTaskResult(const std::string& name) const {
        std::unique_lock<std::mutex> lock(const_cast<std::mutex&>(tasksMutex_));
        
        auto it = tasks_.find(name);
        if (it == tasks_.end()) {
            throw TaskNotFoundException("Task '" + name + "' not found");
        }
        
        auto typedTask = std::dynamic_pointer_cast<Task<ResultType>>(it->second);
        if (!typedTask) {
            throw TaskSchedulerException("Task result type mismatch");
        }
        
        return typedTask->getResult();
    }
    
    // Wait for a task to complete
    template<typename ResultType>
    ResultType waitForTask(const std::string& name) {
        while (true) {
            try {
                auto status = getTaskStatus(name);
                if (status == TaskStatus::COMPLETED) {
                    return getTaskResult<ResultType>(name).getResult();
                } else if (status == TaskStatus::FAILED) {
                    getTaskResult<ResultType>(name).getResult(); // This will throw the original exception
                } else if (status == TaskStatus::CANCELLED) {
                    throw TaskSchedulerException("Task was cancelled");
                }
            } catch (const TaskNotFoundException&) {
                throw;
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    
    // Wait for all tasks to complete
    void waitForAll() {
        bool allDone = false;
        
        while (!allDone) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            
            std::unique_lock<std::mutex> lock(tasksMutex_);
            allDone = true;
            
            for (const auto& pair : tasks_) {
                auto status = pair.second->getStatus();
                if (status == TaskStatus::PENDING || status == TaskStatus::RUNNING) {
                    allDone = false;
                    break;
                }
            }
        }
    }
    
    // Get statistics
    struct TaskStatistics {
        size_t totalTasks;
        size_t completedTasks;
        size_t failedTasks;
        size_t cancelledTasks;
        size_t pendingTasks;
        size_t activeThreads;
        size_t maxThreads;
    };
    
    TaskStatistics getStatistics() const {
        std::unique_lock<std::mutex> lock(const_cast<std::mutex&>(tasksMutex_));
        
        size_t pending = 0;
        for (const auto& pair : tasks_) {
            if (pair.second->getStatus() == TaskStatus::PENDING) {
                pending++;
            }
        }
        
        return {
            stats_.totalTasks,
            stats_.completedTasks,
            stats_.failedTasks,
            stats_.cancelledTasks,
            pending,
            threadPool_.getActiveThreadCount(),
            threadPool_.getMaxThreadCount()
        };
    }
    
    // List all tasks with their status
    std::vector<std::pair<std::string, TaskStatus>> listTasks() const {
        std::unique_lock<std::mutex> lock(const_cast<std::mutex&>(tasksMutex_));
        
        std::vector<std::pair<std::string, TaskStatus>> result;
        result.reserve(tasks_.size());
        
        for (const auto& pair : tasks_) {
            result.emplace_back(pair.first, pair.second->getStatus());
        }
        
        return result;
    }
    
    // Remove completed/failed/cancelled tasks
    void cleanup() {
        std::unique_lock<std::mutex> lock(tasksMutex_);
        
        for (auto it = tasks_.begin(); it != tasks_.end();) {
            auto status = it->second->getStatus();
            if (status == TaskStatus::COMPLETED || 
                status == TaskStatus::FAILED || 
                status == TaskStatus::CANCELLED) {
                it = tasks_.erase(it);
            } else {
                ++it;
            }
        }
    }
};

#endif // ADVANCED_TASK_SCHEDULER_HPP
