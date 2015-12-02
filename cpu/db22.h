#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB22 :public In
{
public:
    enum
    {
        REF_COUNT = 100,
        OUTPUT_CURRENT_COUNT = 100
    };

    void Read();

    float ref[REF_COUNT] = {0.0};                //DB22,REAL0,100;опорный сигнал
    float output_current[OUTPUT_CURRENT_COUNT] = {0.0};  //DB22,REAL400,100;выходной ток

private:
    friend class cpu::CpuMemory;
    DB22();
    DB22( const DB22& ) = delete;
    void operator = ( const DB22& ) = delete;

    enum
    {
        ARRAYS_COUNT = 2
    };

    uint64_t mGroupID = 0;
    wchar_t const* mAdresses[ ARRAYS_COUNT ] = {
        L"CPU/DB22.ref",
        L"CPU/DB22.output_current"
    };

};

}

}
