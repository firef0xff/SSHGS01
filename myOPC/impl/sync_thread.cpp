#include "sync_thread.h"

SyncThread::SyncThread():
    interrupt(false)
{}
SyncThread::~SyncThread()
{
    interrupt = true;
    mRunWaiter.notify_all();
    if ( mThread.joinable() )
        mThread.join();
}
void SyncThread::Start()
{
    std::unique_lock< std::mutex > lock( Mutex );
    mThread = std::thread( &SyncThread::Run, this );
    mRunWaiter.wait(lock);
}
void SyncThread::Exec( Function func )
{
    std::unique_lock< std::mutex > lock( Mutex );
    mFunc = func;
    mRunWaiter.notify_one();
    mRunWaiter.wait(lock);
}
void SyncThread::Run()
{
    std::unique_lock< std::mutex > lock( Mutex );
    for(;;)
    {
        try
        {
            mRunWaiter.notify_one();
            mRunWaiter.wait(lock);
            if ( interrupt )
                return;
            if (mFunc)
                mFunc();
        }
        catch(...)
        {

        }
    }
}
