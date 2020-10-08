#pragma once

#include "lock_base.h"
#include <cassert>

class UnnecessaryLock : public LockBase
{
public:
    explicit UnnecessaryLock(const optional_sting& name);

    bool TryAcquire() override;

    void Acquire() override;

    void Release() override;

#ifndef NDEBUG
protected:
    volatile bool is_locked = false;
#endif
};

inline UnnecessaryLock::UnnecessaryLock(const optional_sting& name)
: LockBase(name)
{
    lock_class_name = "UnnecessaryLock";
}

inline bool UnnecessaryLock::TryAcquire()
{
#ifndef NDEBUG
    assert(!is_locked);
    is_locked = true;
#endif
    return true;
}

inline void UnnecessaryLock::Acquire()
{
    TryAcquire();
}

inline void UnnecessaryLock::Release()
{
#ifndef NDEBUG
    assert(is_locked);
    is_locked = false;
#endif
}
