#ifndef SYSTEM_H
#define SYSTEM_H

#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>
#include <atomic> // Include atomic for thread-safe boolean

class System {
private:
    std::atomic<bool> isActive;
    std::thread updateThread;
    std::mutex mutex;

public:
    System() : isActive(false) {} // Initialize isActive to false
    ~System() {
        Kill();
    }

    virtual void Initialize(double deltaTime) {
        std::lock_guard<std::mutex> lock(mutex);
        if (!isActive) {
            isActive = true;
            updateThread = std::thread(&System::Update, this, deltaTime);
        }
    }

    void Update(double deltaTime) {
        while (IsActive()) {
            std::cout << "Running " << this->GetName() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(deltaTime * 1000)));
        }
    }

    void Kill() {
        {
            std::lock_guard<std::mutex> lock(mutex);
            isActive = false;
        }
        if (updateThread.joinable()) {
            updateThread.join(); // Wait for the thread to finish
        }
    }

    virtual void AddEntity(int entityId) = 0;

    virtual void RemoveEntity(int entityId) = 0;

    virtual const char* GetName() const = 0;

    bool IsActive() const {
        return isActive.load();
    }
};

#endif // SYSTEM_H
