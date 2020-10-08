#pragma once

#include "lock_base.h"

#include <atomic>

class SpinLock : public LockBase
{
public:
    explicit SpinLock(const optional_sting& name);

    bool TryAcquire() override;
    void Acquire() override;
    void Release() override;

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
