#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{

class CpuMemory;

namespace data
{

class DB31 :public Out
{
public:

    void Write();
    void SendContinue();
    void SendNextAmp();
    void SetManualSignal( double persent );

    bool& Start_Oper;       //2.0 старт операции
    bool& Stop_Oper;        //2.1 стоп операции
    bool& Nasos_M2;         //2.2 основной насос М2
    bool& OP15_25_Continum; //DB60,120.3
    bool& Next_Amp; //DB31,16.0

    float& N_Operation;     //4.0 номер операции
    float& Q_5_5ma;         //8.0 расход по умолчанию
    float& Manual_set;      //31 сигнал на ручном режиме

private:    

    friend class cpu::CpuMemory;
    DB31();
    DB31( const DB31& ) = delete;
    void operator = ( const DB31& ) = delete;

    enum
    {
        BOOL_COUNT = 5,
        FLOAT_COUNT = 3
    };

    bool mBoolData[ BOOL_COUNT ];
    float mFloatData[ FLOAT_COUNT ];
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
        //bool
        L"CPU/DB31.Start_Oper",
        L"CPU/DB31.Stop_Oper",
        L"CPU/DB31.Nasos_M2",
        L"CPU/DB60.OP15_25_Continum",
        L"CPU/DB31.Next_Amp",

        L"CPU/DB31.N_Operation",
        L"CPU/DB31.Q_5_5ma",
        L"CPU/DB31.Manual_set",
    };
};

}//namespace data
}//namespace cpu
