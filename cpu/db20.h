#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB20 :public In
{
public:
    enum
    {
        SIGNAL_COUNT = 15000
    };

    void Read();

    float signal[SIGNAL_COUNT] = {0.0};          //DB20 REAL0   15000  опорный сигнал

private:
    friend class cpu::CpuMemory;
    DB20();
    DB20( const DB20& ) = delete;
    void operator = ( const DB20& ) = delete;

    enum
    {
        ARRAYS_COUNT = 1
    };

    uint64_t mGroupID = 0;
    wchar_t const* mAdresses[ ARRAYS_COUNT ] = {
        L"CPU/DB20.signal"
    };

};

}

}
