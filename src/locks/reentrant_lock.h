#pragma once

#include "lock_base.h"

#include <atomic>
#include <thread>

class ReentrantLock : public LockBase
{
public:
    explicit ReentrantLock(const optional_sting& name);

    bool TryAcquire() override;

    void Acquire() override;

    void Release() override;

protected:
    using thread_id = std::thread::id;

    thread_id owner_thread_id = thread_id();
    std::atomic_int lock_count = 0;

    static_assert(std::atomic_int::is_always_lock_free, "atomic int is not lock free, need to use other type");
};

inline ReentrantLock::ReentrantLock(const LockBase::optional_sting& name)
: LockBase(name)
{
    lock_class_name = "ReentrantLock";
}

inline void ReentrantLock::Acquire()
{
    while(!TryAcquire())
    {
        std::this_thread::yield();
    }
}
