#pragma once

#include <optional>
#include <sstream>
#include <string>

class LockBase
{
public:
    using optional_sting = std::optional<std::string>;

    explicit LockBase(optional_sting name = optional_sting());

    virtual bool TryAcquire() = 0;
    virtual void Acquire() = 0;
    virtual void Release() = 0;

    template<class StreamClass>
    friend StreamClass& operator<<(StreamClass& stream, const LockBase& lock);

    // Copying locks is not allowed _for now_ to avoid easy-to-make mistakes
    LockBase(const LockBase& other) = delete;
    LockBase& operator=(const LockBase& other) = delete;

    // Moving locks is not allowed _for now_ to avoid easy-to-make mistakes
    LockBase(LockBase&& other) = delete;
    LockBase& operator=(LockBase&& other) = delete;

    void UpdateName(const std::string& new_name);
    void UpdateName(std::string&& new_name);

protected:
    std::string lock_class_name;
    std::string lock_name;
};

inline LockBase::LockBase(optional_sting name)
: lock_class_name("LockBase")
{
    if(name.has_value())
        lock_name = name.value();
    else
        lock_name = "Unnamed";
}

template<class StreamClass>
StreamClass& operator<<(StreamClass& stream, const LockBase& lock)
{
    stream << lock.lock_class_name << "[" << &lock << "] " << lock.lock_name;
    return stream;
}

inline void LockBase::UpdateName(const std::string &new_name)
{
    lock_name = new_name;
}

inline void LockBase::UpdateName(std::string &&new_name)
{
    lock_name = std::move(new_name);
}
