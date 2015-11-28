#pragma once
#include "data_block_base.h"
#include "inttypes.h"

namespace cpu
{
class CpuMemory;

namespace data
{

/// таблица входных параметров тестирования кейса гидро распределителей
class DB30 : public Out
{
public:
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
    friend class cpu::CpuMemory;
    DB30();
    DB30( const DB30& ) = delete;
    void operator = ( const DB30& ) = delete;

    enum
    {
        BOOL_COUNT = 19,
        FLOAT_COUNT = 15
    };

    bool mBoolData[ BOOL_COUNT ] = {false};
    float mFloatData[ FLOAT_COUNT ] = { 0.0 };
    uint64_t  mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
        //bool
        L"CPU/DB30.Current",
        L"CPU/DB30.Coil",
        L"CPU/DB30.TypeControl",
        L"CPU/DB30.DD1_open_a",
        L"CPU/DB30.DD2_open_a",
        L"CPU/DB30.DD3_open_a",
        L"CPU/DB30.TypeD_open_a",
        L"CPU/DB30.DD1_open_b",
        L"CPU/DB30.DD2_open_b",
        L"CPU/DB30.DD3_open_b",
        L"CPU/DB30.TypeD_open_b",
        L"CPU/DB30.DD1_close_a",
        L"CPU/DB30.DD2_close_a",
        L"CPU/DB30.DD3_close_a",
        L"CPU/DB30.TypeD_close_a",
        L"CPU/DB30.DD1_close_b",
        L"CPU/DB30.DD2_close_b",
        L"CPU/DB30.DD3_close_b",
        L"CPU/DB30.TypeD_close_b",

        //real
        L"CPU/DB30.Voltage",
        L"CPU/DB30.Delta_U",
        L"CPU/DB30.Q_max",
        L"CPU/DB30.Q_min",
        L"CPU/DB30.P_max_rab",
        L"CPU/DB30.P_min",
        L"CPU/DB30.P_narug_germ",
        L"CPU/DB30.Signal_vnutr_germ",
        L"CPU/DB30.Signal_PA_BT",
        L"CPU/DB30.Signal_PB_AT",
        L"CPU/DB30.Time_ON",
        L"CPU/DB30.Time_OFF",
        L"CPU/DB30.Min_P",
        L"CPU/DB30.Max_P",
        L"CPU/DB30.Time_P"
    };
};

}//namespace data


}//namespace cpu
