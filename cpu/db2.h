#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB2 :public In
{
public:
    enum
    {
        REF_A_COUNT = 100,
        CONSUMPTION_A_COUNT = 100,
        REF_B_COUNT = 100,
        CONSUMPTION_B_COUNT = 100
    };

    void Read();

    float ref_a[REF_A_COUNT] = {0.0};                  //DB2,REAL0,100 опорный сигнал катушки а
    float consumption_a[CONSUMPTION_A_COUNT] = {0.0};  //DB2,REAL0,400 расход катушки а
    float ref_b[REF_B_COUNT] = {0.0};                  //DB2,REAL0,800 опорный сигнал катушки б
    float consumption_b[CONSUMPTION_B_COUNT] = {0.0};  //DB2,REAL0,1200 расход катушки б
private:
    friend class cpu::CpuMemory;
    DB2();
    DB2( const DB2& ) = delete;
    void operator = ( const DB2& ) = delete;

    enum
    {
        ARRAYS_COUNT = 4,
    };

    uint64_t mGroupID = 0;
    wchar_t const* mAdresses[ ARRAYS_COUNT ] = {
        L"CPU/DB2.ref_a",
        L"CPU/DB2.consumption_a",
        L"CPU/DB2.ref_b",
        L"CPU/DB2.consumption_b",
    };

};

}

}
