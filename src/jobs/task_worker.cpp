#include "task_worker.h"

#include "log.h"
#include "task_base.h"
#include <sstream>

TaskWorker::TaskWorker()
    : should_work(true)
    , stopped(false)
{
}

void TaskWorker::StartWorker(std::unique_ptr<TaskBase>&& first_task, const TaskWorker::get_task_func& get_next_task,
    const TaskWorker::release_func& release_worker)
{
    log_info("Staring worker thread");

    {
        std::stringstream name_stream;
        name_stream << std::this_thread::get_id();
        name = std::move(name_stream.str());
    }

    first_task->RunTask();
    first_task.reset(); // Deleting task explicitly since there is an endless cycle below and first_task won't be destroyed until the end of the program

    while (should_work.load(std::memory_order_acquire)) {
        // Try to get a new task immediately
        {
            auto opt_task = get_next_task();
            if (opt_task) {
                auto task = std::move(opt_task.value());
                task->RunTask();
                continue;
            }
        }

        // Wait for the next task
        {
            if (!release_worker(this))
                continue;

            auto task_future = task_promise.get_future();
            auto task = task_future.get();
            // Reset promise to be able to reuse it
            task_promise = std::promise<std::unique_ptr<TaskBase>>();
            if (task)
                task->RunTask();
        }
    }

    stopped.store(true, std::memory_order_release);
}

void TaskWorker::AssignTask(std::unique_ptr<TaskBase>&& task)
{
    std::atomic_thread_fence(std::memory_order_release);
    task_promise.set_value(std::move(task));
}

void TaskWorker::MarkStopped()
{
    should_work.store(false, std::memory_order_release);
}

bool TaskWorker::WaitForStop()
{
    // TODO: It's better to add a timeout
    while (!stopped.load(std::memory_order_acquire))
        std::this_thread::yield();
    return true;
}

const std::string& TaskWorker::GetName() const
{
    return name;
}
