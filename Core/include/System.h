#ifndef SYSTEM_H
#define SYSTEM_H

#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>
#include <atomic> // Include atomic for thread-safe boolean

class System
{
private:
    std::atomic<bool> isActive;
    std::thread updateThread;
    std::mutex mutex;

public:

    System() : isActive(false) {} // Initialize isActive to false
    ~System()
    {
        Kill();
    }

    virtual void Initialize()
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (!isActive)
        {
            isActive = true;
        }
    }

    void Kill()
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            isActive = false;
        }
        if (updateThread.joinable())
        {
            updateThread.join(); // Wait for the thread to finish
        }
    }

    bool IsActive() const
    {
        return isActive.load();
    }

    virtual const char* GetName() const = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void FixedUpdate() = 0;

};

#endif // SYSTEM_H
