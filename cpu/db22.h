#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB22 :public In
{
public:
    enum
    {
        SIGNAL_COUNT = 100,
        CURRENT_COUNT = 100
    };



    void Read();

    float signal[SIGNAL_COUNT] = {0.0};         //DB22,REAL0,100 опорный сигнал
    float current[CURRENT_COUNT] = {0.0};     //DB22,REAL400,100 выходной ток
private:
    friend class cpu::CpuMemory;
    DB22();
    DB22( const DB22& ) = delete;
    void operator = ( const DB22& ) = delete;

    enum
    {
        ARRAYS_COUNT = 10
    };

    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ ARRAYS_COUNT ] = {
        L"CPU/DB22.signal",
        L"CPU/DB22.current",
    };
};
}
}
