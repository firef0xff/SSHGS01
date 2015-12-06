#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB12 :public In
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

    float ref_a[REF_A_COUNT] = {0.0};                  //DB12   REAL0   100 опорный сигнал А
    float consumption_a[CONSUMPTION_A_COUNT] = {0.0};  //DB12   REAL400 100 расход А
    float ref_b[REF_B_COUNT] = {0.0};                  //DB12   REAL800 100 опорный сигнал В
    float consumption_b[CONSUMPTION_B_COUNT] = {0.0};  //DB12   REAL1200    100 расход В
private:
    friend class cpu::CpuMemory;
    DB12();
    DB12( const DB12& ) = delete;
    void operator = ( const DB12& ) = delete;

    enum
    {
        ARRAYS_COUNT = 4,
    };

    uint64_t mGroupID = 0;
    wchar_t const* mAdresses[ ARRAYS_COUNT ] = {
        L"CPU/DB12.ref_a",
        L"CPU/DB12.consumption_a",
        L"CPU/DB12.ref_b",
        L"CPU/DB12.consumption_b",
    };

};

}

}
