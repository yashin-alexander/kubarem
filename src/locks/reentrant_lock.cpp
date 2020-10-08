#include "reentrant_lock.h"

#include <cassert>

bool ReentrantLock::TryAcquire()
{
    int expected_count = 0;
    if(lock_count.compare_exchange_weak(expected_count, 1, std::memory_order_acquire))
    {
        assert(owner_thread_id == thread_id());
        owner_thread_id = std::this_thread::get_id();
        return true;
    }

    if(owner_thread_id == std::this_thread::get_id())
    {
        assert(lock_count > 0);
        lock_count.fetch_add(1, std::memory_order_relaxed);
        return true;
    }

    return false;
}

void ReentrantLock::Release()
{
    assert(owner_thread_id == std::this_thread::get_id());
    assert(lock_count > 0);

    if(lock_count > 1)
    {
        lock_count.fetch_add(-1, std::memory_order_relaxed);
        return;
    }

    owner_thread_id = thread_id();
    lock_count.store(0, std::memory_order_release);
}
