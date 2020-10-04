#pragma once

template<class LockClass>
class ScopedLock final
{
public:
    explicit ScopedLock(LockClass& Lock)
    : LockRef(Lock)
    {
        LockRef.Acquire();
    }

    ~ScopedLock()
    {
        LockRef.Release();
    }

private:
    LockClass& LockRef;
    
    // Copying ScopedLock is not allowed _for now_ to avoid mistakes
    ScopedLock(const ScopedLock<LockClass>& other) = delete;
    ScopedLock<LockClass>& operator=(const ScopedLock<LockClass>& other) = delete;

    // Moving ScopedLock is not allowed _for now_ to avoid mistakes
    ScopedLock(ScopedLock<LockClass>&& other) = delete;
    ScopedLock<LockClass>& operator=(ScopedLock<LockClass>&& other) = delete;
};
