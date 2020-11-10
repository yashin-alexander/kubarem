#pragma once

// Abstract templateless task handle class
class TaskHandleBase
{
public:
    // Is connected task finished work and passed the result to the handle
    virtual bool HasTaskResult() const = 0;
};


