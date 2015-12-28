#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{

class CpuMemory;

namespace data
{

class DB34 :public In
{
public:

    void Read();

    bool& op10_ok; //2.0 наружняя герметичность выполнение
    bool& op11_ok; //2.1 внутренняя герметичность выполнение
    bool& op12_ok; //2.2 зависимоть расход/ток выполнение
    bool& op13_ok; //2.3 зависимость расход/давление выполнение
    bool& op14_ok; //2.4 частотные характеристики выполнение
    bool& op15_ok; //2.5 переходные харарактеристики выполнение
    bool& op10_end; //2.6 наружняя герметичность конец
    bool& op11_end; //2.7 внутренняя герметичность конец
    bool& op12_end; //3.0 зависимоть расход/ток конец
    bool& op13_end; //3.1 зависимость расход/давление конец
    bool& op14_end; //3.2 частотные характеристики конец
    bool& op15_end; //3.3 переходные харарактеристики конец
    bool& op12_ab_open_a;   //Катушка а канал АБ открыты
    bool& op12_b_close_a;   //Катушка а канал Б закрыт
    bool& op12_a_close_a;   //Катушка а канал А закрыт
    bool& op12_ab_open_b;   //Катушка б канал АБ открыты
    bool& op12_b_close_b;   //Катушка б канал Б закрыт
    bool& op12_a_close_b;   //Катушка б канал А закрыт
    bool& op14_ready;       // TODO
    float& op14_frequency;  //4.0   TODO

private:
    friend class cpu::CpuMemory;
    DB34();
    DB34( const DB34& ) = delete;
    void operator = ( const DB34& ) = delete;

    enum
    {
        BOOL_COUNT = 19,
        FLOAT_COUNT = 1
    };

    bool mBoolData[ BOOL_COUNT ] = {false};
    float mFloatData[ FLOAT_COUNT ] = { 0.0 };
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
        //bool
        L"CPU/DB34.op10_ok",
        L"CPU/DB34.op11_ok",
        L"CPU/DB34.op12_ok",
        L"CPU/DB34.op13_ok",
        L"CPU/DB34.op14_ok",
        L"CPU/DB34.op15_ok",
        L"CPU/DB34.op10_end",
        L"CPU/DB34.op11_end",
        L"CPU/DB34.op12_end",
        L"CPU/DB34.op13_end",
        L"CPU/DB34.op14_end",
        L"CPU/DB34.op15_end",
        L"CPU/DB34.op12_ab_open_a",
        L"CPU/DB34.op12_b_close_a",
        L"CPU/DB34.op12_a_close_a",
        L"CPU/DB34.op12_ab_open_b",
        L"CPU/DB34.op12_b_close_b",
        L"CPU/DB34.op12_a_close_b",
        L"CPU/DB34.op14_ready",
        L"CPU/DB34.op14_frequency",
    };
};

}//namespace data
}//namespace cpu
