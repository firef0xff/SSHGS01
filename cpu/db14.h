#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB14 :public In
{
public:
    enum
    {
        OPEN_REF_COUNT = 100,
        OPEN_CONSUMPTION_COUNT = 100,
        OPEN_BP3_COUNT = 100,
        OPEN_BP4_COUNT = 100,
        OPEN_BP5_COUNT = 100,
        CLOSE_REF_COUNT = 100,
        CLOSE_CONSUMPTION_COUNT = 100,
        CLOSE_BP3_COUNT = 100,
        CLOSE_BP4_COUNT = 100,
        CLOSE_BP5_COUNT = 100
    };

    

    void Read();

    float open_ref[OPEN_REF_COUNT] = {0.0};                     //DB14,REAL0,100 опорный сигнал на открытие
    float open_consumption[OPEN_CONSUMPTION_COUNT] = {0.0};     //DB14,REAL400,100 расход на открытие
    float open_bp3[OPEN_BP3_COUNT] = {0.0};                     //DB14,REAL800,100 давление ВР3 на открытие
    float open_bp4[OPEN_BP4_COUNT] = {0.0};                     //DB14,REAL1200,100 давление ВР4 на открытие
    float open_bp5[OPEN_BP5_COUNT] = {0.0};                     //DB14,REAL1600,100 давление ВР5 на открытие
    float close_ref[CLOSE_REF_COUNT] = {0.0};                   //DB14,REAL2000,100 опорный сигнал назакрытие
    float close_consumption[CLOSE_CONSUMPTION_COUNT] = {0.0};   //DB14,REAL2400,100 расход на закрытие
    float close_bp3[CLOSE_BP3_COUNT] = {0.0};                   //DB14,REAL2800,100 давление ВР3 на закрытие
    float close_bp4[CLOSE_BP4_COUNT] = {0.0};                   //DB14,REAL3200,100 давление ВР4 на закрытие
    float close_bp5[CLOSE_BP5_COUNT] = {0.0};                   //DB14,REAL3600,100 давление ВР5 на закрытие
private:
    friend class cpu::CpuMemory;
    DB14();
    DB14( const DB14& ) = delete;
    void operator = ( const DB14& ) = delete;

    enum
    {
        ARRAYS_COUNT = 10
    };

    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ ARRAYS_COUNT ] = {
        L"CPU/DB14.open_ref",
        L"CPU/DB14.open_consumption",
        L"CPU/DB14.open_bp3",
        L"CPU/DB14.open_bp4",
        L"CPU/DB14.open_bp5",
        L"CPU/DB14.close_ref",
        L"CPU/DB14.close_consumption",
        L"CPU/DB14.close_bp3",
        L"CPU/DB14.close_bp4",
        L"CPU/DB14.close_bp5"
    };

};

}

}
