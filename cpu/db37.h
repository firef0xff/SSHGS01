#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

//таблица с ответапи по гидроиспытаниям
class DB37 : public In
{
public:

    void Read();

    bool& s4_20ma; //2.0 сигнал 4-20 мА
    bool& s0_20ma; //2.1 сигнал 0-20 мА
    bool& s10v;    //2.2 синал +/-10 В
    bool& s10ma;   //2.3 сигнал +/-10 мА
    bool& s15ma;   //2.4 сигнал +/-15 мА
    bool& s20ma;   //2.5 сигнал +/-20 мА
    bool& s40ma;   //2.6 сигнал +/-40 мА

    float& nominal_volt;        //4 номинальное напряжение платы
    float& output_current_max;  //8 макс вых ток платы
    float& resistance;          //12 сопротивление катушки распределителя

private:

    friend class cpu::CpuMemory;
    DB37();
    DB37( const DB37& ) = delete;
    void operator = ( const DB37& ) = delete;

    enum
    {
        BOOL_COUNT = 7,
        FLOAT_COUNT = 3
    };

    bool mBoolData[ BOOL_COUNT ] = {false};
    float mFloatData[ FLOAT_COUNT ] = { 0.0 };
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
        //bool
        L"CPU/DB37.s4_20ma",
        L"CPU/DB37.s0_20ma",
        L"CPU/DB37.s10v",
        L"CPU/DB37.s10ma",
        L"CPU/DB37.s15ma",
        L"CPU/DB37.s20ma",
        L"CPU/DB37.s40ma",

        L"CPU/DB37.nominal_volt",
        L"CPU/DB37.output_current_max",
        L"CPU/DB37.resistance"
    };
};

}//namespace data

}//namespace cpu
