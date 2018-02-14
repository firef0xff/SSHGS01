#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB90 :public In
{
public:
    enum
    {
        DATA_COUNT = 1000
    };

    void Read();

    float Pressure_S1[DATA_COUNT];
    float Pressure_S2[DATA_COUNT];
    float Consumption_S1[DATA_COUNT];
    float Consumption_S2[DATA_COUNT];
    float Power[DATA_COUNT];
    float K_Consumption_S1[DATA_COUNT];
    float K_Consumption_S2[DATA_COUNT];
    float KPD[DATA_COUNT];
private:
    friend class cpu::CpuMemory;
    DB90();
    DB90( const DB90& ) = delete;
    void operator = ( const DB90& ) = delete;

    enum
    {
        ARRAYS_COUNT = 8
    };

    uint64_t mGroupID = 0;
    wchar_t const* mAdresses[ ARRAYS_COUNT ] = {
        L"CPU/DB90.Pressure_S1",
        L"CPU/DB90.Pressure_S2",
        L"CPU/DB90.Consumption_S1",
        L"CPU/DB90.Consumption_S2",
        L"CPU/DB90.Power",
        L"CPU/DB90.K_Consumption_S1",
        L"CPU/DB90.K_Consumption_S2",
        L"CPU/DB90.KPD",
    };
};

}

}
