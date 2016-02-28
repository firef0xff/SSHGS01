#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB11 :public In
{
public:
    enum
    {
        COORDINATE_COUNT = 10000
    };

    void Read();

    float coordinate[COORDINATE_COUNT];                  //DB11 REAL0   10000 положение цилиндра
    
private:
    friend class cpu::CpuMemory;
    DB11();
    DB11( const DB11& ) = delete;
    void operator = ( const DB11& ) = delete;

    enum
    {
        ARRAYS_COUNT = 1
    };

    uint64_t mGroupID = 0;
    wchar_t const* mAdresses[ ARRAYS_COUNT ] = {
        L"CPU/DB11.coordinate"
    };

};

}

}
