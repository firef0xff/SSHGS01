#include "db30.h"
#include "../myOPC/miniOPC.h"

namespace cpu
{

namespace data
{

DB30::DB30():
    Current( mBoolData[0] ),        // 0 - постоянный ток, 1 - переменный
    Coil( mBoolData[1] ),           // 0 - 1 катушка, 1 - 2 катушки
    TypeControl( mBoolData[2] ),    // 0 - электронное управление, 1 - электрогидравлическое управление

    Voltage( mFloatData[0] ),          // напряжение питания ( 24, 48, 110, 220 )
    Delta_U( mFloatData[1] ),          // допустимое отклонение напряжения питания

    Q_max( mFloatData[2] ),            //максимальный расход
    Q_min( mFloatData[3] ),            //допустимое значение утечки
    P_max_rab( mFloatData[4] ),        //максимальное рабочее давление
    P_min( mFloatData[5] ),            //минимальное давление испытания
    P_narug_germ( mFloatData[6] ),     //давление для проверки наружной герметичности
    Signal_vnutr_germ( mFloatData[7] ),//сигнал для проверки внутренней герметичности

    Signal_PA_BT( mFloatData[8] ),     //сигнал PA_BT
    Signal_PB_AT( mFloatData[9] ),     //сигнал PB_AT
    Time_ON( mFloatData[10] ),         //время включения распределителя
    Time_OFF( mFloatData[11] ),        //время выключения распределителя

    Min_P( mFloatData[12] ),           //минимальное давление управления
    Max_P( mFloatData[13] ),           //максимальное давление управления
    Time_P( mFloatData[14] ),          //давление для испытания "время срабатывания"

    DD1_open_a( mBoolData[3] ),        //ДД1 включение а
    DD2_open_a( mBoolData[4] ),        //ДД2 включение а
    DD3_open_a( mBoolData[5] ),        //ДД3 включение а
    TypeD_open_a( mBoolData[6] ),      //TуpeD 0 - падение 1 - увеличение

    DD1_open_b( mBoolData[7] ),        //ДД1 включение б
    DD2_open_b( mBoolData[8] ),        //ДД2 включение б
    DD3_open_b( mBoolData[9] ),        //ДД3 включение б
    TypeD_open_b( mBoolData[10] ),     //TуpeD 0 - падение 1 - увеличение

    DD1_close_a( mBoolData[11] ),      //ДД1 выключение а
    DD2_close_a( mBoolData[12] ),      //ДД2 выключение а
    DD3_close_a( mBoolData[13] ),      //ДД3 выключение а
    TypeD_close_a( mBoolData[14] ),    //TуpeD 0 - падение 1 - увеличение

    DD1_close_b( mBoolData[15] ),      //ДД1 выключение б
    DD2_close_b( mBoolData[16] ),      //ДД2 выключение б
    DD3_close_b( mBoolData[17] ),      //ДД3 выключение б
    TypeD_close_b( mBoolData[18] )     //TуpeD 0 - падение 1 - увеличение
{
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB30", mAdresses, BOOL_COUNT + FLOAT_COUNT );
}

void DB30::Write()
{
    opc::miniOPC::Instance().WriteMass( mGroupID, 0, BOOL_COUNT, static_cast<void*>( mBoolData ), opc::tBOOL );
    opc::miniOPC::Instance().WriteMass( mGroupID, BOOL_COUNT, FLOAT_COUNT, static_cast<void*>( mFloatData ), opc::tFLOAT );
}

}//namespace data

}//namespace cpu
