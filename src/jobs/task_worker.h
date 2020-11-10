#pragma once

#include <atomic>
#include <functional>
#include <future>
#include <memory>
#include <optional>
#include <string>

class TaskBase;

class TaskWorker
{
public:
    TaskWorker();

    using get_task_func = std::function<std::optional<std::unique_ptr<TaskBase>>()>;
    using release_func = std::function<bool(TaskWorker *)>;

    void StartWorker(std::unique_ptr<TaskBase> &&first_task, const get_task_func &get_next_task,
                                  const release_func &release_worker);

    void AssignTask(std::unique_ptr<TaskBase> &&task);

    void MarkStopped();

    bool WaitForStop();

    const std::string& GetName() const;

private:
    std::promise<std::unique_ptr<TaskBase>> task_promise;

    std::atomic_bool should_work;
    std::atomic_bool stopped;

    std::string name;
};
