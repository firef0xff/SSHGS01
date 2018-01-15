#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB91 :public In
{
public:
    enum
    {
        DATA_COUNT = 1000
    };

    void Read();

    float Value[DATA_COUNT];

private:
    friend class cpu::CpuMemory;
    DB91();
    DB91( const DB91& ) = delete;
    void operator = ( const DB91& ) = delete;

    enum
    {
        ARRAYS_COUNT = 1
    };

    uint64_t mGroupID = 0;
    wchar_t const* mAdresses[ ARRAYS_COUNT ] = {
        L"CPU/DB91.Value"
    };
};

}

}
