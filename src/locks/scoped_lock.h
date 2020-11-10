#pragma once

template<class LockClass>
class ScopedLock final
{
public:
    explicit ScopedLock(LockClass& Lock, bool is_lock_already_acquired = false)
    : LockRef(Lock)
    {
        if (!is_lock_already_acquired)
            LockRef.Acquire();
    }

    ~ScopedLock()
    {
        LockRef.Release();
    }

    // Copying ScopedLock is not allowed _for now_ to avoid mistakes
    ScopedLock(const ScopedLock<LockClass>& other) = delete;
    ScopedLock<LockClass>& operator=(const ScopedLock<LockClass>& other) = delete;

    // Moving ScopedLock is not allowed _for now_ to avoid mistakes
    ScopedLock(ScopedLock<LockClass>&& other) = delete;
    ScopedLock<LockClass>& operator=(ScopedLock<LockClass>&& other) = delete;

private:
    LockClass& LockRef;
};
