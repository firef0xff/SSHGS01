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

    bool& Start_Oper;       //2.0 старт операции
    bool& Stop_Oper;        //2.1 стоп операции
    bool& Nasos_M2;         //2.2 основной насос М2
    float& N_Operation;     //4.0 номер операции
    float& Q_5_5ma;         //8.0 расход по умолчанию

private:
    friend class cpu::CpuMemory;
    DB31();
    DB31( const DB31& ) = delete;
    void operator = ( const DB31& ) = delete;

    enum
    {
        BOOL_COUNT = 3,
        FLOAT_COUNT = 2
    };

    bool mBoolData[ BOOL_COUNT ] = {false};
    float mFloatData[ FLOAT_COUNT ] = { 0.0 };
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
        //bool
        L"CPU/DB31.Start_Oper",
        L"CPU/DB31.Stop_Oper",
        L"CPU/DB31.Nasos_M2",

        L"CPU/DB31.N_Operation",
        L"CPU/DB31.Q_5_5ma"
    };
};

}//namespace data
}//namespace cpu
