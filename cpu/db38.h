#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB38 : public Out
{
public:

    void Write();

    float& p_max;           //2 макс давление нагрузки ГЦ   
    float& p_function;      //6 давление при испытании фнкциониования
    float& q_nominal;       //10 ном расход ГЦ
    float& time_function;   //14 время перемещения
    float& time_tightness;  //18 время испытания наружней гермтичности

private:

    friend class cpu::CpuMemory;
    DB38();
    DB38( const DB38& ) = delete;
    void operator = ( const DB38& ) = delete;

    enum
    {
        BOOL_COUNT = 0,
        FLOAT_COUNT = 5
    };

//    bool mBoolData[ BOOL_COUNT ] = {false};
    float mFloatData[ FLOAT_COUNT ] = { 0.0 };
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
        L"CPU/DB38.p_max",
        L"CPU/DB38.p_function",
        L"CPU/DB38.q_nominal",
        L"CPU/DB38.time_function",
        L"CPU/DB38.time_tightness"
    };
};

}//namespace data

}//namespace cpu
