#include "spin_lock.h"

#include <thread>

void SpinLock::Acquire()
{
    while(!TryAcquire())
    {
        std::this_thread::yield();
    }
}
