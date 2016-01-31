#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

//таблица с ответапи по гидроиспытаниям
class DB35 : public Out
{
public:

    void Write();
    void Reset();

    bool& channel_a;            //62.0 испытание канал А
    bool& channel_b;            //62.1 испытание канала В
    bool& channel_k_a;          //62.2 катушка на канале А( а-0 б-1)
    bool& channel_k_b;          //62.3 катушка на канале Б( а-0 б-1)

    float& s860ma;              //2 управляющий сигнал
    float& x_max_a;             //6 полное перключение в А
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
        BOOL_COUNT = 4,
        FLOAT_COUNT = 13
    };

    bool mBoolData[ BOOL_COUNT ];
    float mFloatData[ FLOAT_COUNT ];
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
        L"CPU/DB35.channel_a",
        L"CPU/DB35.channel_b",
        L"CPU/DB35.channel_k_a",
        L"CPU/DB35.channel_k_b",

        L"CPU/DB35.s860ma",
        L"CPU/DB35.x_max_a",
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
