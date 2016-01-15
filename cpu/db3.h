#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB3 :public In
{
public:
    enum
    {
        OPEN_REF_COUNT = 100,
        OPEN_CONSUMPTION_COUNT = 100,
        CLOSE_REF_COUNT = 100,
        CLOSE_CONSUMPTION_COUNT = 100
    };

    

    void Read();

    float open_ref[OPEN_REF_COUNT];                     //DB3,REAL0,100 опорный сигнал на открытие
    float open_consumption[OPEN_CONSUMPTION_COUNT];     //DB3,REAL400,100 расход на открытие
    float close_ref[CLOSE_REF_COUNT];                   //DB3,REAL2000,100 опорный сигнал назакрытие
    float close_consumption[CLOSE_CONSUMPTION_COUNT];   //DB3,REAL2400,100 расход на закрытие
private:
    friend class cpu::CpuMemory;
    DB3();
    DB3( const DB3& ) = delete;
    void operator = ( const DB3& ) = delete;

    enum
    {
        ARRAYS_COUNT = 4
    };

    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ ARRAYS_COUNT ] = {
        L"CPU/DB3.open_ref",
        L"CPU/DB3.open_consumption",
        L"CPU/DB3.close_ref",
        L"CPU/DB3.close_consumption",
    };

};

}

}
