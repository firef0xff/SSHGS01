#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB23 :public In
{
public:
    enum
    {
        COORDINATE_COUNT = 1000
    };

    void Read();

    float coordinate[COORDINATE_COUNT] = {0.0};                  //DB23 REAL0   15000 положение цилиндра
    
private:
    friend class cpu::CpuMemory;
    DB23();
    DB23( const DB23& ) = delete;
    void operator = ( const DB23& ) = delete;

    enum
    {
        ARRAYS_COUNT = 1
    };

    uint64_t mGroupID = 0;
    wchar_t const* mAdresses[ ARRAYS_COUNT ] = {
        L"CPU/DB23.coord"
    };

};

}

}
