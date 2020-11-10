#include "task_manager.h"

#include <algorithm>
#include <thread>
#include "task_worker.h"
#include "locks/scoped_lock.h"
#include "log.h"

TaskManager::TaskManager()
    : tasks_lock("TasksLock"), workers_lock("WorkersLock"), maxWorkersCount(CalcMaxWorkersCount())
{
    assert(maxWorkersCount > 0);
    workers.reserve(maxWorkersCount);
    free_workers.reserve(maxWorkersCount);
}

TaskManager::~TaskManager()
{
    ShutDown();
}

TaskManager &TaskManager::GetInstance()
{
    static TaskManager manager;
    return manager;
}

void TaskManager::ShutDown()
{
    ScopedLock scopeWorkersLock(workers_lock);

    for(auto& worker : workers)
    {
        worker->MarkStopped();
        // If worker is waiting for a task, assign null task
        auto free_worker = std::find(free_workers.begin(), free_workers.end(), worker.get());
        if(free_worker != free_workers.end())
        {
            free_workers.erase(free_worker);
            worker->AssignTask(nullptr);
        }

        if(!worker->WaitForStop())
        {
            log_err("Worker %s refusing to shut down", worker->GetName().c_str());
        }
    }

    free_workers.clear();
    workers.clear();
}

void TaskManager::WaitForTask(const TaskHandleBase &task_handle)
{
    while (!task_handle.HasTaskResult())
    {
        auto next_task = GetNextTask();
        if(next_task.has_value())
            next_task.value()->RunTask();
        else
            std::this_thread::yield();
    }
}

void TaskManager::AddTask(std::unique_ptr<TaskBase> task)
{
    // Try to assign task to a worker immediately
    {
        ScopedLock scopeWorkersLock(workers_lock);

        // Instantly assign task, if there is any free workers
        if (!free_workers.empty())
        {
            auto worker = free_workers.back();
            free_workers.pop_back();
            worker->AssignTask(std::move(task));
            return;
        }

        // Create new worker and assign task to it, if we can
        if (workers.size() < maxWorkersCount)
        {
            auto *newWorker = new TaskWorker();
            workers.emplace_back(newWorker);

            auto workerThread = std::thread(&TaskWorker::StartWorker, newWorker, std::move(task),
                                            [this] { return GetNextTask(); },
                                            [this](TaskWorker *worker) { return AddFreeWorker(worker); });
            workerThread.detach();

            return;
        }
    }

    // Put a task in the queue, if no worker can run it now
    {
        ScopedLock scopeTasksLock(tasks_lock);
        waiting_tasks.push_back(std::move(task));
    }
}

std::optional<std::unique_ptr<TaskBase>> TaskManager::GetNextTask()
{
    if(!tasks_lock.TryAcquire())
        return {};
    ScopedLock scopeTasksLock(tasks_lock, true);

    if(waiting_tasks.empty())
        return {};

    auto result = std::move(waiting_tasks.back());
    waiting_tasks.pop_back();
    return result;
}

unsigned int TaskManager::CalcMaxWorkersCount()
{
    const auto result = std::thread::hardware_concurrency();
    if (result == 0)
        return 4; // 4 threads by default
    return result;
}

bool TaskManager::AddFreeWorker(TaskWorker *worker)
{
    if(!workers_lock.TryAcquire())
        return false;
    ScopedLock scopeWorkersLock(workers_lock, true);

    free_workers.push_back(worker);
    return true;
}
