#pragma once

#include <array>
#include <cassert>

enum class TaskPriority
{
    VeryHigh,
    High,
    Normal,
    Low,
    VeryLow
};

constexpr static auto AllTaskPriorities = std::array
{
    TaskPriority::VeryHigh,
    TaskPriority::High,
    TaskPriority::Normal,
    TaskPriority::Low,
    TaskPriority::VeryLow,
};

constexpr static const char* GetPriorityName(TaskPriority priority)
{
    switch (priority)
    {
        case TaskPriority::VeryHigh:
            return "VeryHigh";
        case TaskPriority::High:
            return "High";
        case TaskPriority::Normal:
            return "Normal";
        case TaskPriority::Low:
            return "Low";
        case TaskPriority::VeryLow:
            return "VeryLow";
        default:
            assert(false);
            return "Unknown";
    }
}

// Abstract templateless task class
class TaskBase
{
public:
    virtual void RunTask() = 0;
};
