#pragma once

#include "lock_base.h"

#include <atomic>

class SpinLock : public LockBase
{
public:
    SpinLock(const optional_sting& name);

    virtual bool TryAcquire();
    virtual void Acquire();
    virtual void Release();

protected:
    std::atomic_flag is_locked;
};

inline SpinLock::SpinLock(const optional_sting& name)
: LockBase(name)
{
    lock_class_name = "SpinLock";
    is_locked.clear();
}

inline bool SpinLock::TryAcquire()
{
    const bool is_already_locked = is_locked.test_and_set(std::memory_order_acquire);
    return !is_already_locked;
}

inline void SpinLock::Release()
{
    is_locked.clear(std::memory_order_release);
}
