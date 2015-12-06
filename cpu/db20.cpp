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
        REF_COUNT = 20,
        COORDINATE_COUNT = 20
    };

    void Read();

    float ref[REF_COUNT] = {0.0};                //DB20 REAL0   20  опорный сигнал
    float coordinate[COORDINATE_COUNT] = {0.0};  //DB20 REAL80  20  положение цилиндра

private:
    friend class cpu::CpuMemory;
    DB20();
    DB20( const DB20& ) = delete;
    void operator = ( const DB20& ) = delete;

    enum
    {
        ARRAYS_COUNT = 2
    };

    uint64_t mGroupID = 0;
    wchar_t const* mAdresses[ ARRAYS_COUNT ] = {
        L"CPU/DB20.ref",
        L"CPU/DB20.coordinate"
    };

};

}

}
