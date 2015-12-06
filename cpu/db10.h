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
        REF_COUNT = 20,
        COORDINATE_COUNT = 20
    };

    void Read();

    float ref[REF_COUNT] = {0.0};                //DB10 REAL0   20  опорный сигнал
    float coordinate[COORDINATE_COUNT] = {0.0};  //DB10 REAL80  20  положение цилиндра

private:
    friend class cpu::CpuMemory;
    DB10();
    DB10( const DB10& ) = delete;
    void operator = ( const DB10& ) = delete;

    enum
    {
        ARRAYS_COUNT = 2
    };

    uint64_t mGroupID = 0;
    wchar_t const* mAdresses[ ARRAYS_COUNT ] = {
        L"CPU/DB10.ref",
        L"CPU/DB10.coordinate"
    };

};

}

}
