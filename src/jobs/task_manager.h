#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <optional>
#include <type_traits>
#include "task_handle.h"
#include "task.h"
#include "locks/spin_lock.h"

class TaskWorker;

class TaskManager
{
private:
    TaskManager();

public:
    TaskManager(const TaskManager&) = delete;
    TaskManager(TaskManager&&) = delete;

    ~TaskManager();

    static TaskManager& GetInstance();

    //~ Templates magic. Hurts to understand
    template<class Function, class... Args>
    using ResultType = std::invoke_result_t<std::decay_t<Function>, std::decay_t<Args>...>;

    // Puts task to the execution queue
    template<class Function, class... Args>
    std::shared_ptr<TaskHandle<TaskManager::ResultType<Function, Args...>>> RunTask(Function&& func, Args&&... args)
    {
        using TaskResultType = ResultType<Function, Args...>;

        auto handle = std::make_shared<TaskHandle<TaskResultType>>(std::bind(&TaskManager::WaitForTask, this, std::placeholders::_1));
        auto bound_func = std::bind(std::forward<Function>(func), std::forward<Args>(args)...);
        Task<TaskResultType>* task = new Task<TaskResultType>(bound_func, handle);
        AddTask(std::unique_ptr<TaskBase>(task));
        return handle;
    }

    // Puts task to the execution queue
    template<class Function, class... Args>
    TaskManager::ResultType<Function, Args...> RunAndWaitForTask(Function&& func, Args&&... args)
    {
        auto handle = RunTask(std::forward<Function>(func), std::forward<Args>(args)...);
        return handle->WaitForTaskResult();
    }

protected:

    void ShutDown();

    // Blocks current thread until task with this handle will be finished (handle.HasTaskResult() is true)
    void WaitForTask(const TaskHandleBase& task_handle);

    void AddTask(std::unique_ptr<TaskBase> task);

    bool AddFreeWorker(TaskWorker* worker);

    std::optional<std::unique_ptr<TaskBase>> GetNextTask();

private:
    static unsigned int CalcMaxWorkersCount();

    SpinLock tasks_lock;
    std::vector<std::unique_ptr<TaskBase>> waiting_tasks;

    // TODO: Can use a lock-free container, probably?
    SpinLock workers_lock;
    std::vector<std::unique_ptr<TaskWorker>> workers;
    std::vector<TaskWorker*> free_workers;

    unsigned int maxWorkersCount = 0;

};
