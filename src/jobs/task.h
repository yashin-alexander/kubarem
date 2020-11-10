#pragma once
#include <functional>
#include <memory>
#include <thread>
#include <utility>
#include "task_base.h"
#include "task_handle.h"
#include "log.h"


template<class ResultType>
class Task : public TaskBase
{
public:

    Task(const std::function<ResultType()>& func, const std::weak_ptr<TaskHandle<ResultType>>& handle)
            : function_to_run(func), task_handle(handle)
    {}

    void RunTask() override
    {
        log_dbg("Running task %p", (void *) this);

        ResultType result = function_to_run();
        if (!task_handle.expired())
        {
            auto shared_handle = task_handle.lock();
            shared_handle->SetTaskResult(std::move(result));
        }

        log_dbg("Finished task %p", (void *) this);
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
        log_dbg("Running task %p", (void *) this);

        function_to_run();
        if (!task_handle.expired())
        {
            auto shared_handle = task_handle.lock();
            shared_handle->MarkFinished();
        }

        log_dbg("Finished task %p", (void *) this);
    }

protected:
    std::function<void()> function_to_run;
    std::weak_ptr<TaskHandle<void>> task_handle;
};
