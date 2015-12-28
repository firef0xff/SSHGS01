#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB36 : public In
{
public:

    void Read();

    bool& op20_ok;  //2.0 наружняя герметичность выполнение
    bool& op21_ok;  //2.1 внутренняя герметичность выполнение
    bool& op22_ok;  //2.2 зависимоть расход/ток выполнение
    bool& op23_ok;  //2.3 зависимость расход/давление выполнение
    bool& op24_ok;  //2.4 частотные характеристики выполнение
    bool& op25_ok;  //2.5 переходные харарактеристики выполнение
    bool& op20_end; //2.6 наружняя герметичность конец
    bool& op21_end; //2.7 внутренняя герметичность конец
    bool& op22_end; //3.0 зависимоть расход/ток конец
    bool& op23_end; //3.1 зависимость расход/давление конец
    bool& op24_end; //3.2 частотные характеристики конец
    bool& op25_end; //3.3 переходные харарактеристики конец

    bool& op22_a_ab_open;   //8.0   катушка а канал АБ открыты
    bool& op22_a_b_close;   //8.1   катушка а канал Б закрыт
    bool& op22_a_a_close;   //8.2   катушка а канал А закрыт
    bool& op22_b_ab_open;   //8.3   катушка б канал АБ открыты
    bool& op22_b_b_close;   //8.4   катушка б канал Б закрыт
    bool& op22_b_a_close;   //8.5   катушка б канал А закрыт
    bool& op24_ready;       // TODO
    float& op24_frequency;  //4.0   TODO


private:

    friend class cpu::CpuMemory;
    DB36();
    DB36( const DB36& ) = delete;
    void operator = ( const DB36& ) = delete;

    enum
    {
        BOOL_COUNT = 19,
        FLOAT_COUNT = 1
    };

    bool mBoolData[ BOOL_COUNT ] = {false};
    float mFloatData[ FLOAT_COUNT ] = { 0.0 };
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
        L"CPU/DB36.op20_ok",
        L"CPU/DB36.op21_ok",
        L"CPU/DB36.op22_ok",
        L"CPU/DB36.op23_ok",
        L"CPU/DB36.op24_ok",
        L"CPU/DB36.op25_ok",
        L"CPU/DB36.op20_end",
        L"CPU/DB36.op21_end",
        L"CPU/DB36.op22_end",
        L"CPU/DB36.op23_end",
        L"CPU/DB36.op24_end",
        L"CPU/DB36.op25_end",
        L"CPU/DB36.op22_a_ab_open",
        L"CPU/DB36.op22_a_b_close",
        L"CPU/DB36.op22_a_a_close",
        L"CPU/DB36.op22_b_ab_open",
        L"CPU/DB36.op22_b_b_close",
        L"CPU/DB36.op22_b_a_close",
        L"CPU/DB36.op24_ready",
        L"CPU/DB36.op24_frequency",
    };
};

}//namespace data

}//namespace cpu
