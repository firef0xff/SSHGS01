#pragma once
#include "data_block_base.h"

namespace cpu
{

namespace data
{

/// таблица входных параметров тестирования кейса гидро распределителей
class DB30 : public Out
{
public:
    DB30();

    void Write();

    bool& Current;        //2.0 0 - постоянный ток, 1 - переменный
    bool& Coil;           //2.1 0 - 1 катушка, 1 - 2 катушки
    bool& TypeControl;    //2.2 0 - электронное управление, 1 - электрогидравлическое управление

    float& Voltage;          //4.0 напряжение питания ( 24, 48, 110, 220 )
    float& Delta_U;          //8.0 допустимое отклонение напряжения питания

    float& Q_max;            //12.0 максимальный расход
    float& Q_min;            //16.0 допустимое значение утечки
    float& P_max_rab;        //20.0 максимальное рабочее давление
    float& P_min;            //24.0 минимальное давление испытания
    float& P_narug_germ;     //28.0 давление для проверки наружной герметичности
    float& Signal_vnutr_germ;//32.0 сигнал для проверки внутренней герметичности 0 - ничего 1 - катушка А 2 - катушка Б

    float& Signal_PA_BT;     //36.0 сигнал PA_BT 0 - ничего 1 - катушка А 2 - катушка Б
    float& Signal_PB_AT;     //40.0 сигнал PB_AT 0 - ничего 1 - катушка А 2 - катушка Б
    float& Time_ON;          //44.0 время включения распределителя
    float& Time_OFF;         //48.0 время выключения распределителя

    float& Min_P;            //52.0 минимальное давление управления
    float& Max_P;            //56.0 максимальное давление управления
    float& Time_P;           //60.0 давление для испытания "время срабатывания"

    bool& DD1_open_a;       //64.0 ДД1 включение а
    bool& DD2_open_a;       //64.1 ДД2 включение а
    bool& DD3_open_a;       //64.2 ДД3 включение а
    bool& TypeD_open_a;     //64.3 TуpeD 0 - падение 1 - увеличение

    bool& DD1_open_b;       //64.4 ДД1 включение б
    bool& DD2_open_b;       //64.5 ДД2 включение б
    bool& DD3_open_b;       //64.6 ДД3 включение б
    bool& TypeD_open_b;     //64.7 TуpeD 0 - падение 1 - увеличение

    bool& DD1_close_a;       //65.0 ДД1 выключение а
    bool& DD2_close_a;       //65.1 ДД2 выключение а
    bool& DD3_close_a;       //65.2 ДД3 выключение а
    bool& TypeD_close_a;     //65.3 TуpeD 0 - падение 1 - увеличение

    bool& DD1_close_b;       //65.4 ДД1 выключение б
    bool& DD2_close_b;       //65.5 ДД2 выключение б
    bool& DD3_close_b;       //65.6 ДД3 выключение б
    bool& TypeD_close_b;     //65.7 TуpeD 0 - падение 1 - увеличение


private:
    enum
    {
        BOOL_COUNT = 19,
        FLOAT_COUNT = 14
    };

    bool mBoolData[ BOOL_COUNT ] = {false};
    float mFloatData[ FLOAT_COUNT ] = { 0.0 };

    wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
        //bool
        L"S7:[S7 connection_4]DB30,X2.0",   //Current
        L"S7:[S7 connection_4]DB30,X2.1",   //Coil
        L"S7:[S7 connection_4]DB30,X2.2",   //TypeControl
        L"S7:[S7 connection_4]DB30,X64.0",   //DD1_open_a
        L"S7:[S7 connection_4]DB30,X64.1",   //DD2_open_a
        L"S7:[S7 connection_4]DB30,X64.2",   //DD3_open_a
        L"S7:[S7 connection_4]DB30,X64.3",   //TypeD_open_a
        L"S7:[S7 connection_4]DB30,X64.4",   //DD1_open_b
        L"S7:[S7 connection_4]DB30,X64.5",   //DD2_open_b
        L"S7:[S7 connection_4]DB30,X64.6",   //DD3_open_b
        L"S7:[S7 connection_4]DB30,X64.7",   //TypeD_open_b
        L"S7:[S7 connection_4]DB30,X65.0",   //DD1_close_a
        L"S7:[S7 connection_4]DB30,X65.1",   //DD2_close_a
        L"S7:[S7 connection_4]DB30,X65.2",   //DD3_close_a
        L"S7:[S7 connection_4]DB30,X65.3",   //TypeD_close_a
        L"S7:[S7 connection_4]DB30,X65.4",   //DD1_close_b
        L"S7:[S7 connection_4]DB30,X65.5",   //DD2_close_b
        L"S7:[S7 connection_4]DB30,X65.6",   //DD3_close_b
        L"S7:[S7 connection_4]DB30,X65.7",   //TypeD_close_b

        //real
        L"S7:[S7 connection_4]DB30,REAL4",  //Voltage
        L"S7:[S7 connection_4]DB30,REAL8",  //Delta_U
        L"S7:[S7 connection_4]DB30,REAL12", //Q_max
        L"S7:[S7 connection_4]DB30,REAL16", //Q_min
        L"S7:[S7 connection_4]DB30,REAL20", //P_max_rab
        L"S7:[S7 connection_4]DB30,REAL24", //P_min
        L"S7:[S7 connection_4]DB30,REAL28", //P_narug_germ
        L"S7:[S7 connection_4]DB30,REAL32", //Signal_vnutr_germ
        L"S7:[S7 connection_4]DB30,REAL36", //Signal_PA_BT
        L"S7:[S7 connection_4]DB30,REAL40", //Signal_PB_AT
        L"S7:[S7 connection_4]DB30,REAL44", //Time_ON
        L"S7:[S7 connection_4]DB30,REAL48", //Time_OFF
        L"S7:[S7 connection_4]DB30,REAL52", //Min_P
        L"S7:[S7 connection_4]DB30,REAL56"  //Max_P
        L"S7:[S7 connection_4]DB30,REAL60"  //Time_P
    };
};

}//namespace data


}//namespace cpu
