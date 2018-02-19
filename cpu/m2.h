#pragma once
#include "data_block_base.h"
#include <inttypes.h>
#include <mutex>

namespace cpu
{
class CpuMemory;
namespace data
{

class M2 : public Out
{
public:
    void Write();
    void Reset();                   // сброс всех битов в контроллере

    bool& Pump_Continue;          //m127.1 продолжить тест
    bool& Pump_Fail;          //m128.6 завершить тест

private:

    friend class cpu::CpuMemory;
    M2();
    M2( const M2& ) = delete;
    void operator = ( const M2& ) = delete;

    enum
    {
        BOOL_COUNT = 2
    };

    bool mBoolData[ BOOL_COUNT ];
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT ] =
    {
        L"CPU/M2.Pump_Continue", //m127.1 продолжить тест
        L"CPU/M2.Pump_Fail", //m128.6 завершить тест
    };
};

class M3 : public InOut
{
public:
    void Write();
    void Read();

    bool& StopOnError;          //m127.7 продолжить тест

private:

    friend class cpu::CpuMemory;
    M3();
    M3( const M3& ) = delete;
    void operator = ( const M3& ) = delete;

    enum
    {
        BOOL_COUNT = 1
    };

    bool mBoolData[ BOOL_COUNT ];
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT ] =
    {
        L"CPU/M3.StopOnError", //m127.7 продолжить тест
    };
};

class M4 : public InOut
{
public:
    void Write();
    void Read();

    bool& LubMonStart;
    bool& LubMonStop;
    std::mutex Mutex;
private:

    friend class cpu::CpuMemory;
    M4();
    M4( const M4& ) = delete;
    void operator = ( const M4& ) = delete;

    enum
    {
        BOOL_COUNT = 2
    };

    bool mBoolData[ BOOL_COUNT ];
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT ] =
    {
        L"CPU/M4.LubMonStart",
        L"CPU/M4.LubMonStop",
    };
};

}//namespace data

}//namespace cpu
