#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{

class CpuMemory;

namespace data
{

class DB33 :public Out
{
public:

    void Write();
    void Reset();

    bool& s4_20ma; //2.0 сигнал 4-20 мА
    bool& s0_20ma; //2.1 сигнал 0-20 мА
    bool& s10v; //2.2 сигнал +/- 10В
    bool& s0_10v; //2.2 сигнал 0..10В
    bool& s10ma; //2.3 сигнал +/- 10 мА
    bool& s15ma; //2.4 сигнал +/- 15мА
    bool& s20ma; //2.5 сиганл +/- 20 мА
    bool& s40ma; //2.6 сигнал +/- 40 мА
    bool& channel_a; //60.0 испытание канала А
    bool& channel_b; //60.1 испытание канала В
    bool& channel_k_a; //60.2 катушка на канале А( а-0 б-1)
    bool& channel_k_b; //60.3 катушка на канале Б( а-0 б-1)

    float& x_max_a;             //4 сигнал переключение в А       
    float& x_max_b;             //8 сигнал переключение в В     
    float& x_pos_0;             //12 сигнал переключение в 0  
    float& test_press;          //16 пробное давление   
    float& nominal_press;       //20 номинальное давление      
    float& q_max_a;             //24 макс. расход в А
    float& q_max_b;             //28 макс. расход в В 
    float& q_max;               //32 макс. допустимый расход
    float& amp_1;               //36 амплитуда 1     
    float& amp_2;               //40 амплитуда 2      
    float& amp_3;               //44 амплитуда 3
    float& increment;           //48 инкремент частоты         
    float& press_control_min;   //52 мин. давление управления       
    float& press_control_max;   //56 макс. давление управления 
    float& U_Plat;              //62 Напряжение на блоке управления
    float& start_frequency;     //31.48 Стартовая частота
    float& ampl_inc;            //31.52 Инкремент амплитуды
private:
    friend class cpu::CpuMemory;
    DB33();
    DB33( const DB33& ) = delete;
    void operator = ( const DB33& ) = delete;

    enum
    {
        BOOL_COUNT = 12,
        FLOAT_COUNT = 17
    };

    bool mBoolData[ BOOL_COUNT ];
    float mFloatData[ FLOAT_COUNT ];
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
        //bool
        L"CPU/DB33.s4_20ma",
        L"CPU/DB33.s0_20ma",
        L"CPU/DB33.s10v",
        L"CPU/DB33.s0_10v",
        L"CPU/DB33.s10ma",
        L"CPU/DB33.s15ma",
        L"CPU/DB33.s20ma",
        L"CPU/DB33.s40ma",
        L"CPU/DB33.channel_a",
        L"CPU/DB33.channel_b",
        L"CPU/DB33.channel_k_a",
        L"CPU/DB33.channel_k_b",

        L"CPU/DB33.x_max_a",
        L"CPU/DB33.x_max_b",
        L"CPU/DB33.x_pos_0",
        L"CPU/DB33.test_press",
        L"CPU/DB33.nominal_press",
        L"CPU/DB33.q_max_a",
        L"CPU/DB33.q_max_b",
        L"CPU/DB33.q_max",
        L"CPU/DB33.amp_1",
        L"CPU/DB33.amp_2",
        L"CPU/DB33.amp_3",
        L"CPU/DB33.increment",
        L"CPU/DB33.press_control_min",
        L"CPU/DB33.press_control_max",
        L"CPU/DB33.U_Plat",
        L"CPU/DB31.start_frequency",
        L"CPU/DB31.ampl_inc",
    };
};

}//namespace data
}//namespace cpu
