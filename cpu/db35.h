#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

//таблица с ответапи по гидроиспытаниям
class DB35 : public In
{
public:

    void Read();

    bool& channel_a;            //62.0 испытание канал А
    bool& channel_b;            //62.1 испытание канала В

    float& s860ma;              //2 управляющий сигнал
    float& x_max_a;             //6 полное перключение в А
    float& x_max_b;             //10 полное переключение в В
    float& x_pos_0;             //14 сигнал переключения в 0
    float& test_press;          //18 испытание пробным давлением
    float& nominal_press;       //22 номинальное давление
    float& q_max_a;             //26 макс расход в А
    float& q_max_b;             //30 макс расход в В
    float& q_max;               //34 максимальный расход
    float& amp_1;               //38 амплитуда 1
    float& amp_2;               //42 амплитуда 2
    float& amp_3;               //46 амплитуда 3
    float& increment;           //50 инкремент частоты
    float& press_control_min;   //54 мин давдение управления
    float& press_control_max;   //58 макс давление управления

private:

    friend class cpu::CpuMemory;
    DB35();
    DB35( const DB35& ) = delete;
    void operator = ( const DB35& ) = delete;

    enum
    {
        BOOL_COUNT = 2,
        FLOAT_COUNT = 15
    };

    bool mBoolData[ BOOL_COUNT ] = {false};
    float mFloatData[ FLOAT_COUNT ] = { 0.0 };
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
        L"CPU/DB35.channel_a",
        L"CPU/DB35.channel_b",

        L"CPU/DB35.s860ma",
        L"CPU/DB35.x_max_a",
        L"CPU/DB35.x_max_b",
        L"CPU/DB35.x_pos_0",
        L"CPU/DB35.test_press",
        L"CPU/DB35.nominal_press",
        L"CPU/DB35.q_max_a",
        L"CPU/DB35.q_max_b",
        L"CPU/DB35.q_max",
        L"CPU/DB35.amp_1",
        L"CPU/DB35.amp_2",
        L"CPU/DB35.amp_3",
        L"CPU/DB35.increment",
        L"CPU/DB35.press_control_min",
        L"CPU/DB35.press_control_max"

    };
};

}//namespace data

}//namespace cpu
