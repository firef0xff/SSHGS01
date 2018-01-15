#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB90 :public In
{
public:
    enum
    {
        DATA_COUNT = 1000
    };

    void Read();

    float Pressure[DATA_COUNT];

private:
    friend class cpu::CpuMemory;
    DB90();
    DB90( const DB90& ) = delete;
    void operator = ( const DB90& ) = delete;

    enum
    {
        ARRAYS_COUNT = 1
    };

    uint64_t mGroupID = 0;
    wchar_t const* mAdresses[ ARRAYS_COUNT ] = {
        L"CPU/DB90.Pressure"
    };
};

}

}
