#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class SyncThread
{
public:
    typedef std::function<void(void)> Function;

    SyncThread();
    ~SyncThread();

    void Start();
    void Exec( Function func );
private:
    void Run();
    bool interrupt;
    std::thread mThread;
    Function mFunc;
    std::mutex Mutex;
    std::condition_variable mRunWaiter;
};
