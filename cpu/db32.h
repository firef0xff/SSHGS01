#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

//таблица с ответапи по гидроиспытаниям
class DB32 : public In
{
public:

    void Read();

    //хз что
    bool& p_e_ok;               //2.4 давление в системе достигнуто ( электро )
    bool& p_eg_ok;              //2.5 давление в системе достигнуто ( электрогидравл )

    //test1
    bool& op1_min_ok_a;          //2.0 переключение при мин давлении А
    bool& op1_min_no_a;          //2.1 нет переключения при мин давлени А
    bool& op1_min_ok_b;          //2.2 переключение при мин давлении В
    bool& op1_min_no_b;          //2.3 нет переключения при мин давлени В
    bool& volt_alarm_a;          //2.6  напряжение В вне допуска
    bool& volt_alarm_b;          //36.0 напряжение катушки б вне допуска
    bool& op1_max_ok_a;          //2.7 переключение при макс давлении А
    bool& op1_max_o_a;           //3.0 нет переключения при макс давлени А
    bool& op1_max_ok_b;          //3.1 переключение при макс давлении В
    bool& op1_max_no_b;          //3.2 нет переключения при макс давлени В

    float& op1_volt_a;        //4.0 замерянное наряжение катушки а
    float& op1_current_a;        //8.0 замерянный ток катушки а
    float& op1_power_a;          //12.0 расчетная мощьность катушки а
    float& op1_resist_a;         //16.0 расчетное сопротивение катушки а
    float& op1_volt_b;        //20.0 замерянное наряжение катушки б
    float& op1_current_b;        //24.0 замерянный ток катушки б
    float& op1_power_b;          //28.0 расчетная мощьность катушки а
    float& op1_resist_b;         //32.0 расчетное сопротивение катушки а

    //test2 ???
    bool& op2_ok_no;       //36.1 течь обнаружена, не обнаружена //не используется

    //test3
    float& op3_consumption_1;       //38.0 средний расход
    bool& op3_consumption_2;        //42.0 расход в норме
    bool& op3_consumption_3;        //42.1 расход выше нормы

    //test4
    float& op4_q_03_a;          //44.0 фактический расход 0,3Qmax(А)
    float& op4_p_p_03_a;        //48.0 давление в канале P 0,3Qmax(А)
    float& op4_p_a_03_a;        //52.0 давление в канале A 0,3Qmax(А)
    float& op4_bp5_bp3_03_a;    //116.0 перепад BP5/BP3 0,3Qmax(А)

    float& op4_q_06_a;          //56.0 фактический расход 0,6Qmax(А)
    float& op4_p_p_06_a;        //60.0 давление в канале P 0,6Qmax(А)
    float& op4_p_a_06_a;        //64.0 давление в канале A 0,6Qmax(А)
    float& op4_bp5_bp3_06_a;    //120.0 перепад BP5/BP3 0,6Qmax(А)

    float& op4_q_max_a;         //68.0 фактический расход Qmax(А)
    float& op4_p_p_max_a;       //72.0 давление в канале P Qmax(А)
    float& op4_p_a_max_a;       //76.0 давление в канале A Qmax(А)
    float& op4_bp5_bp3_max_a;   //124.0 перепад BP5/BP3 Qmax(А)


    float& op4_q_03_b;          //80.0 фактический расход 0,3Qmax(B)
    float& op4_p_p_03_b;        //84.0 давление в канале P 0,3Qmax(B)
    float& op4_p_a_03_b;        //88.0 давление в канале A 0,3Qmax(B)
    float& op4_bp5_bp3_03_b;    //128.0 перепад BP5/BP3 0,3Qmax(B)

    float& op4_q_06_b;          //92.0 фактический расход 0,6Qmax(B)
    float& op4_p_p_06_b;        //96.0 давление в канале P 0,6Qmax(B)
    float& op4_p_a_06_b;        //100.0 давление в канале A 0,6Qmax(B)
    float& op4_bp5_bp3_06_b;    //132.0 перепад BP5/BP3 0,6Qmax(B)

    float& op4_q_max_b;         //104.0 фактический расход Qmax(B)
    float& op4_p_p_max_b;       //108.0 давление в канале P Qmax(B)
    float& op4_p_a_max_b;       //112.0 давление в канале A Qmax(B)
    float& op4_bp5_bp3_max_b;   //136.0 перепад BP5/BP3 Qmax(B)

    //test5
    bool& op5_ok_a;    //140.0 пропускная способность соотвествует А
    bool& op5_no_a;    //140.1 пропускная способность не соотвествует А
    bool& op5_ok_b;    //140.2 пропускная способность соотвествует В
    bool& op5_no_b;    //140.3 пропускная способность не соотвествует В

    //test6
    bool& op6_min_min_ok_a;    //140.4 "а" перекл. при мин. давл. (мин упр.)
    bool& op6_min_min_no_a;    //140.5 "а" не перекл. при мин. давл. (мин упр.)
    bool& op6_min_max_ok_a;    //140.6 "а" перекл. при мин. давл. (макс упр.)
    bool& op6_min_max_no_a;    //140.7 "а" перекл. при мин. давл. (макс упр.)
    bool& op6_max_min_ok_a;    //141.0 "а" перекл. при макс. давл. (мин упр.)
    bool& op6_max_min_no_b;    //141.1 "а" перекл. при макс. давл. (мин упр.)
    bool& op6_max_max_ok_a;    //141.2 "а" перекл. при макс. давл. (макс упр.)
    bool& op6_max_max_no_a;    //141.3 "а" перекл. при макс. давл. (макс упр.)

    bool& op6_min_min_ok_b;    //141.4 "б" перекл. при мин. давл. (мин упр.)
    bool& op6_min_min_no_b;    //141.5 "б" не перекл. при мин. давл. (мин упр.)
    bool& op6_min_max_ok_b;    //141.6 "б" перекл. при мин. давл. (макс упр.)
    bool& op6_min_max_no_b;    //141.7 "б" перекл. при мин. давл. (макс упр.)
    bool& op6_max_min_ok_b;    //142.0 "б" перекл. при макс. давл. (мин упр.)
    bool& op6_mex_min_no_b;    //142.1 "б" перекл. при макс. давл. (мин упр.)
    bool& op6_max_max_ok_b;    //142.2 "б" перекл. при макс. давл. (макс упр.)
    bool& op6_max_max_no_b;    //142.3 "б" перекл. при макс. давл. (макс упр.)


    //test7
    bool& op7_min_ok_a;  //142.4 "а" перекл. при мин. давлении (Vmin)
    bool& op7_min_no_a;   //142.5 "а" не перекл. при мин. давлении (Vmin)
    bool& op7_min_ok_b;  //142.6 "б" перекл. при мин. давлении (Vmin)
    bool& op7_min_no_b;   //142.7 "б" не перекл. при мин. давлении (Vmin)
    bool& op7_max_ok_a;  //143.0 "а" перекл. при макс. давлении (Vmin)
    bool& op7_max_no_a;   //143.1 "а" не перекл. при макс. давлении (Vmin)
    bool& op7_max_ok_b;  //143.2 "б" перекл. при макс. давлении (Vmin)
    bool& op7_max_no_b;   //143.3 "б" не перекл. при макс. давлении (Vmin)

    //test8
    float& op8_time_on_a;       //144.0 время включения катуки "а"
    float& op8_time_off_a;      //148.0 время отключения катуки "а"
    float& op8_time_on_b;       //152.0 время включения катуки "б"
    float& op8_time_off_b;      //156.0 время отключения катуки "б"

    bool& op8_open_ok_a;        //160.0 "а" время включения соответствует
    bool& op8_open_no_a;        //160.1 "а" время включения не соответствует
    bool& op8_close_ok_a;       //160.2 "а" время отключения соответствует
    bool& op8_close_no_a;       //160.3 "а" время отключения не соответствует
    bool& op8_open_ok_b;        //160.4 "б" время включения соответствует
    bool& op8_open_no_b;        //160.5 "б" время включения не соответствует
    bool& op8_close_ok_b;       //160.6 "б" время отключения соответствует
    bool& op8_close_no_b;       //160.7 "б" время отключения не соответствует

    bool& op8_fault_on_a;       //161.0 катушка "а" не включилась
    bool& op8_fault_off_a;      //161.1 катушка "а" не отключилась
    bool& op8_fault_on_b;       //161.2 катушка "б" не включилась
    bool& op8_fault_off_b;      //161.3 катушка "б" не отключилась

    //служебные
    float& T_oil;   //162.0 температура масла

    bool& op1_ok; //166.0 Выполнение операции 1
    bool& op2_ok; //166.1 Выполнение операции 2
    bool& op3_ok; //166.2 Выполнение операции 3
    bool& op4_ok; //166.3 Выполнение операции 4
    bool& op5_ok; //166.4 Выполнение операции 5
    bool& op6_ok; //166.5 Выполнение операции 6
    bool& op7_ok; //166.6 Выполнение операции 7
    bool& op8_ok; //166.7 Выполнение операции 8

    bool& op1_end;  //167.0 Конец операции 1
    bool& op2_end;  //167.1 Конец операции 2
    bool& op3_end;  //167.2 Конец операции 3
    bool& op4_end;  //167.3 Конец операции 4
    bool& op5_end;  //167.4 Конец операции 5
    bool& op6_end;  //167.5 Конец операции 6
    bool& op7_end;  //167.6 Конец операции 7
    bool& op8_end;  //167.7 Конец операции 8

private:

    friend class cpu::CpuMemory;
    DB32();
    DB32( const DB32& ) = delete;
    void operator = ( const DB32& ) = delete;

    enum
    {
        BOOL_COUNT = 71,
        FLOAT_COUNT = 38
    };

    bool mBoolData[ BOOL_COUNT ] = {false};
    float mFloatData[ FLOAT_COUNT ] = { 0.0 };
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
        //bool
        L"CPU/DB32.p_e_ok",
        L"CPU/DB32.p_eg_ok",

        L"CPU/DB32.op1_min_ok_a",
        L"CPU/DB32.op1_min_no_a",
        L"CPU/DB32.op1_min_ok_b",
        L"CPU/DB32.op1_min_no_b",
        L"CPU/DB32.volt_alarm_a",
        L"CPU/DB32.volt_alarm_b",
        L"CPU/DB32.op1_max_ok_a",
        L"CPU/DB32.op1_max_o_a",
        L"CPU/DB32.op1_max_ok_b",
        L"CPU/DB32.op1_max_no_b",

        L"CPU/DB32.op2_ok_no",

        L"CPU/DB32.op3_consumption_2",
        L"CPU/DB32.op3_consumption_3",

        L"CPU/DB32.op5_ok_a",
        L"CPU/DB32.op5_no_a",
        L"CPU/DB32.op5_ok_b",
        L"CPU/DB32.op5_no_b",

        L"CPU/DB32.op6_min_min_ok_a",
        L"CPU/DB32.op6_min_min_no_a",
        L"CPU/DB32.op6_min_max_ok_a",
        L"CPU/DB32.op6_min_max_no_a",
        L"CPU/DB32.op6_max_min_ok_a",
        L"CPU/DB32.op6_mex_min_no_a",
        L"CPU/DB32.op6_max_max_ok_a",
        L"CPU/DB32.op6_max_max_no_a",
        L"CPU/DB32.op6_min_min_ok_b",
        L"CPU/DB32.op6_min_min_no_b",
        L"CPU/DB32.op6_min_max_ok_b",
        L"CPU/DB32.op6_min_max_no_b",
        L"CPU/DB32.op6_max_min_ok_b",
        L"CPU/DB32.op6_max_min_no_b",
        L"CPU/DB32.op6_max_max_ok_b",
        L"CPU/DB32.op6_max_max_no_b",

        L"CPU/DB32.op7_min_ok_a",
        L"CPU/DB32.op7_min_no_a",
        L"CPU/DB32.op7_min_ok_b",
        L"CPU/DB32.op7_min_no_b",
        L"CPU/DB32.op7_max_ok_a",
        L"CPU/DB32.op7_max_no_a",
        L"CPU/DB32.op7_max_ok_b",
        L"CPU/DB32.op7_max_no_b",

        L"CPU/DB32.op8_open_ok_a",
        L"CPU/DB32.op8_open_no_a",
        L"CPU/DB32.op8_close_ok_a",
        L"CPU/DB32.op8_close_no_a",
        L"CPU/DB32.op8_open_ok_b",
        L"CPU/DB32.op8_open_no_b",
        L"CPU/DB32.op8_close_ok_b",
        L"CPU/DB32.op8_close_no_b",

        L"CPU/DB32.op8_fault_on_a",
        L"CPU/DB32.op8_fault_off_a",
        L"CPU/DB32.op8_fault_on_b",
        L"CPU/DB32.op8_fault_off_b",

        L"CPU/DB32.op1_ok",
        L"CPU/DB32.op2_ok",
        L"CPU/DB32.op3_ok",
        L"CPU/DB32.op4_ok",
        L"CPU/DB32.op5_ok",
        L"CPU/DB32.op6_ok",
        L"CPU/DB32.op7_ok",
        L"CPU/DB32.op8_ok",

        L"CPU/DB32.op1_end",
        L"CPU/DB32.op2_end",
        L"CPU/DB32.op3_end",
        L"CPU/DB32.op4_end",
        L"CPU/DB32.op5_end",
        L"CPU/DB32.op6_end",
        L"CPU/DB32.op7_end",
        L"CPU/DB32.op8_end",


        L"CPU/DB32.op1_volt_a",
        L"CPU/DB32.op1_current_a",
        L"CPU/DB32.op1_power_a",
        L"CPU/DB32.op1_resist_a",
        L"CPU/DB32.op1_volt_b",
        L"CPU/DB32.op1_current_b",
        L"CPU/DB32.op1_power_b",
        L"CPU/DB32.op1_resist_b",

        L"CPU/DB32.op3_consumption_1",

        L"CPU/DB32.op4_q_03_a",
        L"CPU/DB32.op4_p_p_03_a",
        L"CPU/DB32.op4_p_a_03_a",
        L"CPU/DB32.op4_bp5_bp3_03_a",

        L"CPU/DB32.op4_q_06_a",
        L"CPU/DB32.op4_p_p_06_a",
        L"CPU/DB32.op4_p_a_06_a",
        L"CPU/DB32.op4_bp5_bp3_06_a",

        L"CPU/DB32.op4_q_max_a",
        L"CPU/DB32.op4_p_p_max_a",
        L"CPU/DB32.op4_p_a_max_a",
        L"CPU/DB32.op4_bp5_bp3_max_a",

        L"CPU/DB32.op4_q_03_b",
        L"CPU/DB32.op4_p_p_03_b",
        L"CPU/DB32.op4_p_a_03_b",
        L"CPU/DB32.op4_bp5_bp3_03_b",

        L"CPU/DB32.op4_q_06_b",
        L"CPU/DB32.op4_p_p_06_b",
        L"CPU/DB32.op4_p_a_06_b",
        L"CPU/DB32.op4_bp5_bp3_06_b",

        L"CPU/DB32.op4_q_max_b",
        L"CPU/DB32.op4_p_p_max_b",
        L"CPU/DB32.op4_p_a_max_b",
        L"CPU/DB32.op4_bp5_bp3_max_b",

        L"CPU/DB32.op8_time_on_a",
        L"CPU/DB32.op8_time_off_a",
        L"CPU/DB32.op8_time_on_b",
        L"CPU/DB32.op8_time_off_b",

        L"CPU/DB32.T_oil"
    };
};

}//namespace data

}//namespace cpu
