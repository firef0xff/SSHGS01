#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB24 :public In
{
public:
    enum
    {
        COORDINATE_COUNT = 10000
    };

    void Read();

    float coordinate[COORDINATE_COUNT] = {0.0};                  //DB24 REAL0   10000 положение цилиндра
    
private:
    friend class cpu::CpuMemory;
    DB24();
    DB24( const DB24& ) = delete;
    void operator = ( const DB24& ) = delete;

    enum
    {
        ARRAYS_COUNT = 1
    };

    uint64_t mGroupID = 0;
    wchar_t const* mAdresses[ ARRAYS_COUNT ] = {
        L"CPU/DB24.coordinate"
    };

};

}

}
