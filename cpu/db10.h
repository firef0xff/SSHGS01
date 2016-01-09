#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB10 :public In
{
public:
    enum
    {
        SIGNAL_COUNT = 1000
    };

    void Read();

    float signal[SIGNAL_COUNT];          //DB10 REAL0   15000  опорный сигнал

private:
    friend class cpu::CpuMemory;
    DB10();
    DB10( const DB10& ) = delete;
    void operator = ( const DB10& ) = delete;

    enum
    {
        ARRAYS_COUNT = 1
    };

    uint64_t mGroupID = 0;
    wchar_t const* mAdresses[ ARRAYS_COUNT ] = {
        L"CPU/DB10.signal"
    };
};

}

}
