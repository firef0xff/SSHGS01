#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB19 :public In
{
public:
    enum
    {
        CONSUMPTION_A_COUNT = 50,
        BP3_A_COUNT = 50,
        BP5_A_COUNT = 50,
        CONSUMPTION_B_COUNT = 50,
        BP4_B_COUNT = 50,
        BP5_B_COUNT = 50
    };

    void Read();

    float consumption_a[CONSUMPTION_A_COUNT] = {0.0}; // DB19, REAL0, 50 расход канал А
    float bp3_a[BP3_A_COUNT] = {0.0}; // DB19, REAL200, 50 давление ВР3 канал А
    float bp5_a[BP5_A_COUNT] = {0.0}; // DB19, REAL400, 50 давление ВР5 канал В
    float consumption_b[CONSUMPTION_B_COUNT] = {0.0}; // DB19, REAL600, 50 расход канал В
    float bp4_b[BP4_B_COUNT] = {0.0}; // DB19, REAL800, 50 давление ВР4 канал В
    float bp5_b[BP5_B_COUNT] = {0.0}; // DB19, REAL1000, 50 давление ВР5 канал В 

private:
    friend class cpu::CpuMemory;
    DB19();
    DB19( const DB19& ) = delete;
    void operator = ( const DB19& ) = delete;

    enum
    {
        ARRAYS_COUNT = 6
    };

    uint64_t mGroupID = 0;
    wchar_t const* mAdresses[ ARRAYS_COUNT ] = {
        L"CPU/DB19.consumption_a",
        L"CPU/DB19.bp3_a",
        L"CPU/DB19.bp5_a",
        L"CPU/DB19.consumption_b",
        L"CPU/DB19.bp4_b",
        L"CPU/DB19.bp5_b"
    };

};

}

}
