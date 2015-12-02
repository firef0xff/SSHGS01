#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB21 :public In
{
public:
    enum
    {
        COORDINATE_COUNT = 10000
    };

    void Read();

    float coordinate[COORDINATE_COUNT] = {0.0};                  //DB21 REAL0   10000 положение цилиндра
    
private:
    friend class cpu::CpuMemory;
    DB21();
    DB21( const DB21& ) = delete;
    void operator = ( const DB21& ) = delete;

    enum
    {
        ARRAYS_COUNT = 1
    };

    uint64_t mGroupID = 0;
    wchar_t const* mAdresses[ ARRAYS_COUNT ] = {
        L"CPU/DB21.coordinate"
    };

};

}

}
