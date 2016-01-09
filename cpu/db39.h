#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB39 : public In
{
public:

    void Read();

    bool& op30_ok;  //2.0   питание плат выполнение
    bool& op31_ok;  //2.1   зависимость вых ток/ опорон игнал выполнение
    bool& op32_ok;  //2.2   испытание ГЦ выполнение
    bool& op30_end; //2.3   питание плат конец
    bool& op31_end; //2.4   зависимость вых ток/ опорон игнал конец
    bool& op32_end; //2.5   испытание ГЦ конец

private:

    friend class cpu::CpuMemory;
    DB39();
    DB39( const DB39& ) = delete;
    void operator = ( const DB39& ) = delete;

    enum
    {
        BOOL_COUNT = 6,
        FLOAT_COUNT = 0
    };

    bool mBoolData[ BOOL_COUNT ];
//    float mFloatData[ FLOAT_COUNT ];
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
        L"CPU/DB39.op30_ok",
        L"CPU/DB39.op31_ok",
        L"CPU/DB39.op32_ok",
        L"CPU/DB39.op30_end",
        L"CPU/DB39.op31_end",
        L"CPU/DB39.op32_end"
    };
};

}//namespace data

}//namespace cpu
