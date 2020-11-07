#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <type_traits>
#include <vector>
#include <utility>

template<class ResultType>
class TaskHandle;
class TaskBase;


class TaskManager
{
private:
    TaskManager() = default;

public:
    TaskManager(const TaskManager&) = delete;
    TaskManager(TaskManager&&) = delete;

    static TaskManager& GetInstance()
    {
        static TaskManager manager;
        return manager;
    }

    //~ Templates magic. Hurts to understand
    template<class Function, class... Args>
    using ResultType = std::invoke_result_t<std::decay_t<Function>, std::decay_t<Args>...>;

    // Puts task to the execution queue
    template<class Function, class... Args>
    std::shared_ptr<TaskHandle<TaskManager::ResultType<Function, Args...>>> RunTask(Function&& func, Args&&... args)
    {
        using TaskResultType = ResultType<Function, Args...>;

        auto handle = std::make_shared<TaskHandle<TaskResultType>>();
        auto bound_func = std::bind(std::forward<Function>(func), std::forward<Args...>(args)...);
        Task<TaskResultType>* task = new Task<TaskResultType>(bound_func, handle);
        AddTask(task);
        return handle;
    }

    // Blocks current thread until task with this handle will be finished (handle.HasTaskResult() is true)
    template<class ResultType>
    void WaitForTask(const TaskHandle<ResultType>& task_handle)
    {
        while (!task_handle.HasTaskResult())
        {
            assert(HasTasks());
            std::unique_ptr<TaskBase> next_task = GetNextTask();
            next_task->RunTask();
        }
    }

protected:
    void AddTask(TaskBase* task)
    {
        tasks.emplace_back(task);
    }

    bool HasTasks()
    {
        return !tasks.empty();
    }

    std::unique_ptr<TaskBase> GetNextTask()
    {
        auto result = std::move(tasks.back());
        tasks.pop_back();
        return result;
    }

    std::vector<std::unique_ptr<TaskBase>> tasks;

};

template<class ResultType>
class TaskHandle
{
public:
    TaskHandle() = default;
    TaskHandle(const TaskHandle<ResultType>&) = delete;
    TaskHandle(TaskHandle<ResultType>&&) = delete;

    // Sets result of the task for this handle. Typically called by the connected task
    void SetTaskResult(ResultType&& result)
    {
        assert(!has_result);
        has_result = true;
        task_result = std::move(result);
    }

    // Returns the task result, when the task is finished. Until that moment blocks current thread
    ResultType WaitForTaskResult()
    {
        if (!has_result)
            TaskManager::GetInstance().WaitForTask(*this);

        assert(has_result);
        return task_result;
    }

    // Is connected task finished work and passed the result to the handle
    bool HasTaskResult() const
    {
        return has_result;
    }

protected:
    bool has_result = false;

    ResultType task_result;
};

// void specialization of the TaskHandle class
template<>
class TaskHandle<void>
{

public:
    TaskHandle() = default;
    TaskHandle(const TaskHandle<void>&) = delete;
    TaskHandle(TaskHandle<void>&&) = delete;

    // Marks that task finished work. Typically called by connected the task
    void MarkFinished()
    {
        assert(!is_finished);
        is_finished = true;
    }

    // Blocks current thread until the connected task is finished
    void WaitForTaskResult()
    {
        if (!is_finished)
            TaskManager::GetInstance().WaitForTask(*this);
    }

    // Is connected task finished work and passed the result to the handle
    bool HasTaskResult() const
    {
        return is_finished;
    }

protected:
    bool is_finished = false;
};

// Generic templateless task class
class TaskBase
{
public:
    virtual void RunTask() = 0;
};

template<class ResultType>
class Task : public TaskBase
{
public:

    Task(const std::function<ResultType()>& func, const std::weak_ptr<TaskHandle<ResultType>>& handle)
        : function_to_run(func), task_handle(handle)
    {}

    void RunTask() override
    {
        ResultType result = function_to_run();
        if(!task_handle.expired())
        {
            auto shared_handle = task_handle.lock();
            shared_handle->SetTaskResult(std::move(result));
        }
    }

protected:
    std::function<ResultType()> function_to_run;
    std::weak_ptr<TaskHandle<ResultType>> task_handle;
};

// void specialization of the Task class
template<>
class Task<void> : public TaskBase
{
public:

    Task(std::function<void()> func, std::weak_ptr<TaskHandle<void>> handle)
            : function_to_run(std::move(func)), task_handle(std::move(handle))
    {}

    void RunTask() override
    {
        function_to_run();
        if(!task_handle.expired())
        {
            auto shared_handle = task_handle.lock();
            shared_handle->MarkFinished();
        }
    }

protected:
    std::function<void()> function_to_run;
    std::weak_ptr<TaskHandle<void>> task_handle;
};
