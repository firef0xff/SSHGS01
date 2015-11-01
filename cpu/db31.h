#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
namespace data
{

class DB31 :public Out
{
public:
    DB31();

    void Write();

    bool& Start_Oper;       //2.0 старт операции
    bool& Stop_Oper;        //2.1 стоп операции
    bool& Nasos_M2;         //2.2 основной насос М2
    float& N_Operation;     //4.0 номер операции

private:
    enum
    {
        BOOL_COUNT = 3,
        FLOAT_COUNT = 1
    };

    bool mBoolData[ BOOL_COUNT ] = {false};
    float mFloatData[ FLOAT_COUNT ] = { 0.0 };
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
        //bool
        L"S7:[S7 connection_4]DB31,X2.0",   //Start_Oper
        L"S7:[S7 connection_4]DB31,X2.1",   //Stop_Oper
        L"S7:[S7 connection_4]DB31,X2.2",   //Nasos_M2

        L"S7:[S7 connection_4]DB31,REAL4",  //N_Operation
    };
};

}//namespace data
}//namespace cpu
