#pragma once

#include <atomic>
#include <cassert>
#include <functional>
#include <utility>
#include "task_handle_base.h"

template<class ResultType>
class TaskHandle : public TaskHandleBase
{
public:
    TaskHandle(const std::function<void(const TaskHandleBase &)> &handle_wait_function)
            : wait_function(handle_wait_function) {}

    TaskHandle(const TaskHandle<ResultType> &) = delete;
    TaskHandle(TaskHandle<ResultType> &&) = delete;

    // Sets result of the task for this handle. Typically called by the connected task
    void SetTaskResult(ResultType &&result)
    {
        assert(!has_result.load(std::memory_order_relaxed));

        task_result = std::move(result);
        has_result.store(true, std::memory_order_release);
    }

    // Returns the task result, when the task is finished. Until that moment blocks current thread
    ResultType WaitForTaskResult()
    {
        while (!has_result.load(std::memory_order_acquire))
            wait_function(*this);

        return task_result;
    }

    // Is connected task finished work and passed the result to the handle
    bool HasTaskResult() const override
    {
        return has_result.load(std::memory_order_relaxed);
    }

protected:
    std::atomic_bool has_result = false;
    ResultType task_result;

    std::function<void(const TaskHandleBase &)> wait_function;
};

// void specialization of the TaskHandle class
template<>
class TaskHandle<void> : public TaskHandleBase
{

public:
    TaskHandle(const std::function<void(const TaskHandleBase &)> &wait_function)
            : wait_function(wait_function) {}

    TaskHandle(const TaskHandle<void> &) = delete;
    TaskHandle(TaskHandle<void> &&) = delete;

    // Marks that task finished work. Typically called by connected the task
    void MarkFinished()
    {
        assert(!is_finished.load(std::memory_order_relaxed));
        is_finished.store(true, std::memory_order_release);
    }

    // Blocks current thread until the connected task is finished
    void WaitForTaskResult()
    {
        while (!is_finished.load(std::memory_order_acquire))
            wait_function(*this);
    }

    // Is connected task finished work and passed the result to the handle
    bool HasTaskResult() const override
    {
        return is_finished.load(std::memory_order_relaxed);
    }

protected:
    std::atomic_bool is_finished = false;

    std::function<void(const TaskHandleBase &)> wait_function;
};
