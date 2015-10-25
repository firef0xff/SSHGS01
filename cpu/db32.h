#pragma once
#include "data_block_base.h"

namespace cpu
{

namespace data
{

//таблица с ответапи по гидроиспытаниям
class DB32 : public In
{
public:
    DB32();

    void Read();

    //хз что
    bool& D_E1_OK;               //2.4 давление в системе достигнуто ( электро )
    bool& D_E1G_OK;              //2.5 давление в системе достигнуто ( электрогидравл )

    //test1
    bool& OP1_Min_D_YESa;        //2.0 минимальное давление пройдено ( кат. а )
    bool& OP1_Min_D_NOa;         //2.1 минимальное давление НЕ пройдено ( кат. а )
    bool& OP1_Min_D_YESb;        //2.2 минимальное давление пройдено ( кат. б )
    bool& OP1_Min_D_NOb;         //2.3 минимальное давление НЕ пройдено ( кат. б )
    bool& Voltage_Alarm_a;       //2.6  напряжение катушки а вне допуска
    bool& Voltage_Alarm_b;       //36.0 напряжение катушки б вне допуска
    bool& OP1_Max_D_YESa;        //2.7 максимальное давление пройдено ( кат. а )
    bool& OP1_Max_D_NOa;         //3.0 максимальное давление НЕ пройдено ( кат. а )
    bool& OP1_Max_D_YESb;        //3.1 максимальное давление пройдено ( кат. б )
    bool& OP1_Max_D_NOb;         //3.2 максимальное давление НЕ пройдено ( кат. б )

    float& OP1_Voltage_a;        //4.0 замерянное наряжение катушки а
    float& OP1_Current_a;        //8.0 замерянный ток катушки а
    float& OP1_Power_a;          //12.0 расчетная мощьность катушки а
    float& OP1_Resist_a;         //16.0 расчетное сопротивение катушки а
    float& OP1_Voltage_b;        //20.0 замерянное наряжение катушки б
    float& OP1_Current_b;        //24.0 замерянный ток катушки б
    float& OP1_Power_b;          //28.0 расчетная мощьность катушки а
    float& OP1_Resist_b;         //32.0 расчетное сопротивение катушки а

    //test2 ???
    bool& OP2_Tech_Yes_No;       //36.1 течь обнаружена, не обнаружена

    //test3
    float& OP3_Sred_Rashod;      //38.0 средний расход
    bool& OP3_Rashod_Norma;      //42.0 расход в норме
    bool& OP3_Rashod_VNorma;     //42.1 расход выше нормы

    //test4
    float& OP4_Q_Fakt_03Qmax_A;  //44.0 фактический расход 0,3Qmax(А)
    float& OP4_D_P_03Qmax_A;     //48.0 давление в канале P 0,3Qmax(А)
    float& OP4_D_A_03Qmax_A;     //52.0 давление в канале A 0,3Qmax(А)
    float& OP4_BP5_BP3_03Qmax_A; //116.0 перепад BP5/BP3 0,3Qmax(А)

    float& OP4_Q_Fakt_06Qmax_A;  //56.0 фактический расход 0,6Qmax(А)
    float& OP4_D_P_06Qmax_A;     //60.0 давление в канале P 0,6Qmax(А)
    float& OP4_D_A_06Qmax_A;     //64.0 давление в канале A 0,6Qmax(А)
    float& OP4_BP5_BP3_06Qmax_A; //120.0 перепад BP5/BP3 0,6Qmax(А)

    float& OP4_Q_Fakt_Qmax_A;    //68.0 фактический расход Qmax(А)
    float& OP4_D_P_Qmax_A;       //72.0 давление в канале P Qmax(А)
    float& OP4_D_A_Qmax_A;       //76.0 давление в канале A Qmax(А)
    float& OP4_BP5_BP3_Qmax_A;   //124.0 перепад BP5/BP3 Qmax(А)


    float& OP4_Q_Fakt_03Qmax_B;  //80.0 фактический расход 0,3Qmax(B)
    float& OP4_D_P_03Qmax_B;     //84.0 давление в канале P 0,3Qmax(B)
    float& OP4_D_A_03Qmax_B;     //88.0 давление в канале A 0,3Qmax(B)
    float& OP4_BP5_BP3_03Qmax_B; //128.0 перепад BP5/BP3 0,3Qmax(B)

    float& OP4_Q_Fakt_06Qmax_B;  //92.0 фактический расход 0,6Qmax(B)
    float& OP4_D_P_06Qmax_B;     //96.0 давление в канале P 0,6Qmax(B)
    float& OP4_D_A_06Qmax_B;     //100.0 давление в канале A 0,6Qmax(B)
    float& OP4_BP5_BP3_06Qmax_B; //132.0 перепад BP5/BP3 0,6Qmax(B)

    float& OP4_Q_Fakt_Qmax_B;    //104.0 фактический расход Qmax(B)
    float& OP4_D_P_Qmax_B;       //108.0 давление в канале P Qmax(B)
    float& OP4_D_A_Qmax_B;       //112.0 давление в канале A Qmax(B)
    float& OP4_BP5_BP3_Qmax_B;   //136.0 перепад BP5/BP3 Qmax(B)

    //test5
    bool& OP5_A_OK;    //140.0 пропускная способность соотвествует А
    bool& OP5_A_NO;    //140.1 пропускная способность не соотвествует А
    bool& OP5_B_OK;    //140.2 пропускная способность соотвествует В
    bool& OP5_B_NO;    //140.3 пропускная способность не соотвествует В

    //test6
    bool& OP6_MinD_MinUprD_YESa;    //140.4 "а" перекл. при мин. давл. (мин упр.)
    bool& OP6_MinD_MinUprD_NOa;     //140.5 "а" не перекл. при мин. давл. (мин упр.)
    bool& OP6_MinD_MaxUprD_YESa;    //140.6 "а" перекл. при мин. давл. (макс упр.)
    bool& OP6_MinD_MaxUprD_NOa;     //140.7 "а" перекл. при мин. давл. (макс упр.)
    bool& OP6_MaxD_MinUprD_YESa;    //141.0 "а" перекл. при макс. давл. (мин упр.)
    bool& OP6_MaxD_MinUprD_NOa;     //141.1 "а" перекл. при макс. давл. (мин упр.)
    bool& OP6_MaxD_MaxUprD_YESa;    //141.2 "а" перекл. при макс. давл. (макс упр.)
    bool& OP6_MaxD_MaxUprD_NOa;     //141.3 "а" перекл. при макс. давл. (макс упр.)

    bool& OP6_MinD_MinUprD_YESb;    //141.4 "б" перекл. при мин. давл. (мин упр.)
    bool& OP6_MinD_MinUprD_NOb;     //141.5 "б" не перекл. при мин. давл. (мин упр.)
    bool& OP6_MinD_MaxUprD_YESb;    //141.6 "б" перекл. при мин. давл. (макс упр.)
    bool& OP6_MinD_MaxUprD_NOb;     //141.7 "б" перекл. при мин. давл. (макс упр.)
    bool& OP6_MaxD_MinUprD_YESb;    //142.0 "б" перекл. при макс. давл. (мин упр.)
    bool& OP6_MaxD_MinUprD_NOb;     //142.1 "б" перекл. при макс. давл. (мин упр.)
    bool& OP6_MaxD_MaxUprD_YESb;    //142.2 "б" перекл. при макс. давл. (макс упр.)
    bool& OP6_MaxD_MaxUprD_NOb;     //142.3 "б" перекл. при макс. давл. (макс упр.)


    //test7
    bool& OP7_Min_D_YESa;  //142.4 "а" перекл. при мин. давлении (Vmin)
    bool& OP7_Min_D_NOa;   //142.5 "а" не перекл. при мин. давлении (Vmin)
    bool& OP7_Min_D_YESb;  //142.6 "б" перекл. при мин. давлении (Vmin)
    bool& OP7_Min_D_NOb;   //142.7 "б" не перекл. при мин. давлении (Vmin)
    bool& OP7_Max_D_YESa;  //143.0 "а" перекл. при макс. давлении (Vmin)
    bool& OP7_Max_D_NOa;   //143.1 "а" не перекл. при макс. давлении (Vmin)
    bool& OP7_Max_D_YESb;  //143.2 "б" перекл. при макс. давлении (Vmin)
    bool& OP7_Max_D_NOb;   //143.3 "б" не перекл. при макс. давлении (Vmin)

    //test8
    float& OP8_Time_on_a;       //144.0 время включения катуки "а"
    float& OP8_Time_off_a;      //148.0 время отключения катуки "а"
    float& OP8_Time_on_b;       //152.0 время включения катуки "б"
    float& OP8_Time_off_b;      //156.0 время отключения катуки "б"

    bool& OP8_open_YES_a;       //160.0 "а" время включения соответствует
    bool& OP8_open_NO_a;        //160.1 "а" время включения не соответствует
    bool& OP8_close_YES_a;      //160.2 "а" время отключения соответствует
    bool& OP8_close_NO_a;       //160.3 "а" время отключения не соответствует
    bool& OP8_open_YES_b;       //160.4 "б" время включения соответствует
    bool& OP8_open_NO_b;        //160.5 "б" время включения не соответствует
    bool& OP8_close_YES_b;      //160.6 "б" время отключения соответствует
    bool& OP8_close_NO_b;       //160.7 "б" время отключения не соответствует

    bool& OP8_NO_Impuls_open_a;     //161.0 катушка "а" не включилась
    bool& OP8_NO_Impuls_close_a;    //161.1 катушка "а" не отключилась
    bool& OP8_NO_Impuls_open_b;     //161.2 катушка "б" не включилась
    bool& OP8_NO_Impuls_close_b;    //161.3 катушка "б" не отключилась

    //служебные
    float& Temperatura_masla;   //162.0 температура масла

    bool& OP1_Work; //166.0 Выполнение операции 1
    bool& OP2_Work; //166.1 Выполнение операции 2
    bool& OP3_Work; //166.2 Выполнение операции 3
    bool& OP4_Work; //166.3 Выполнение операции 4
    bool& OP5_Work; //166.4 Выполнение операции 5
    bool& OP6_Work; //166.5 Выполнение операции 6
    bool& OP7_Work; //166.6 Выполнение операции 7
    bool& OP8_Work; //166.7 Выполнение операции 8

    bool& OP1_End;  //167.0 Конец операции 1
    bool& OP2_End;  //167.1 Конец операции 2
    bool& OP3_End;  //167.2 Конец операции 3
    bool& OP4_End;  //167.3 Конец операции 4
    bool& OP5_End;  //167.4 Конец операции 5
    bool& OP6_End;  //167.5 Конец операции 6
    bool& OP7_End;  //167.6 Конец операции 7
    bool& OP8_End;  //167.7 Конец операции 8

private:
    enum
    {
        BOOL_COUNT = 71,
        FLOAT_COUNT = 38
    };

    bool mBoolData[ BOOL_COUNT ] = {false};
    float mFloatData[ FLOAT_COUNT ] = { 0.0 };

    wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
        //bool
        L"S7:[S7 connection_4]DB32,X2.4",   //D_E1_OK
        L"S7:[S7 connection_4]DB32,X2.5",   //D_E1G_OK

        L"S7:[S7 connection_4]DB32,X2.0",   //OP1_Min_D_YESa
        L"S7:[S7 connection_4]DB32,X2.1",   //OP1_Min_D_NOa
        L"S7:[S7 connection_4]DB32,X2.2",   //OP1_Min_D_YESb
        L"S7:[S7 connection_4]DB32,X2.3",   //OP1_Min_D_NOb
        L"S7:[S7 connection_4]DB32,X2.6",   //Voltage_Alarm_a
        L"S7:[S7 connection_4]DB32,X36.0",  //Voltage_Alarm_b
        L"S7:[S7 connection_4]DB32,X2.7",   //OP1_Max_D_YESa
        L"S7:[S7 connection_4]DB32,X3.0",   //OP1_Max_D_NOa
        L"S7:[S7 connection_4]DB32,X3.1",   //OP1_Max_D_YESb
        L"S7:[S7 connection_4]DB32,X3.2",   //OP1_Max_D_NOb

        L"S7:[S7 connection_4]DB32,X36.1",  //OP2_Tech_Yes_No

        L"S7:[S7 connection_4]DB32,X42.0",  //OP3_Rashod_Norma
        L"S7:[S7 connection_4]DB32,X42.1",  //OP3_Rashod_VNorma

        L"S7:[S7 connection_4]DB32,X140.0",   //OP5_A_OK
        L"S7:[S7 connection_4]DB32,X140.1",   //OP5_A_NO
        L"S7:[S7 connection_4]DB32,X140.2",   //OP5_B_OK
        L"S7:[S7 connection_4]DB32,X140.3",   //OP5_B_NO

        L"S7:[S7 connection_4]DB32,X140.4",   //OP6_MinD_MinUprD_YESa
        L"S7:[S7 connection_4]DB32,X140.5",   //OP6_MinD_MinUprD_NOa
        L"S7:[S7 connection_4]DB32,X140.6",   //OP6_MinD_MaxUprD_YESa
        L"S7:[S7 connection_4]DB32,X140.7",   //OP6_MinD_MaxUprD_NOa
        L"S7:[S7 connection_4]DB32,X141.0",   //OP6_MaxD_MinUprD_YESa
        L"S7:[S7 connection_4]DB32,X141.1",   //OP6_MaxD_MinUprD_NOa
        L"S7:[S7 connection_4]DB32,X141.2",   //OP6_MaxD_MaxUprD_YESa
        L"S7:[S7 connection_4]DB32,X141.3",   //OP6_MaxD_MaxUprD_NOa
        L"S7:[S7 connection_4]DB32,X141.4",   //OP6_MinD_MinUprD_YESb
        L"S7:[S7 connection_4]DB32,X141.5",   //OP6_MinD_MinUprD_NOb
        L"S7:[S7 connection_4]DB32,X141.6",   //OP6_MinD_MaxUprD_YESb
        L"S7:[S7 connection_4]DB32,X141.7",   //OP6_MinD_MaxUprD_NOb
        L"S7:[S7 connection_4]DB32,X142.0",   //OP6_MaxD_MinUprD_YESb
        L"S7:[S7 connection_4]DB32,X142.1",   //OP6_MaxD_MinUprD_NOb
        L"S7:[S7 connection_4]DB32,X142.2",   //OP6_MaxD_MaxUprD_YESb
        L"S7:[S7 connection_4]DB32,X142.3",   //OP6_MaxD_MaxUprD_NOb

        L"S7:[S7 connection_4]DB32,X142.4",   //OP7_Min_D_YESa
        L"S7:[S7 connection_4]DB32,X142.5",   //OP7_Min_D_NOa
        L"S7:[S7 connection_4]DB32,X142.6",   //OP7_Min_D_YESb
        L"S7:[S7 connection_4]DB32,X142.7",   //OP7_Min_D_NOb
        L"S7:[S7 connection_4]DB32,X143.0",   //OP7_Max_D_YESa
        L"S7:[S7 connection_4]DB32,X143.1",   //OP7_Max_D_NOa
        L"S7:[S7 connection_4]DB32,X143.2",   //OP7_Max_D_YESb
        L"S7:[S7 connection_4]DB32,X143.3",   //OP7_Max_D_NOb

        L"S7:[S7 connection_4]DB32,X160.0",   //OP8_open_YES_a
        L"S7:[S7 connection_4]DB32,X160.1",   //OP8_open_NO_a
        L"S7:[S7 connection_4]DB32,X160.2",   //OP8_close_YES_a
        L"S7:[S7 connection_4]DB32,X160.3",   //OP8_close_NO_a
        L"S7:[S7 connection_4]DB32,X160.4",   //OP8_open_YES_b
        L"S7:[S7 connection_4]DB32,X160.5",   //OP8_open_NO_b
        L"S7:[S7 connection_4]DB32,X160.6",   //OP8_close_YES_b
        L"S7:[S7 connection_4]DB32,X160.7",   //OP8_close_NO_b

        L"S7:[S7 connection_4]DB32,X161.0",   //OP8_NO_Impuls_open_a
        L"S7:[S7 connection_4]DB32,X161.1",   //OP8_NO_Impuls_close_a
        L"S7:[S7 connection_4]DB32,X161.2",   //OP8_NO_Impuls_open_b
        L"S7:[S7 connection_4]DB32,X161.3",   //OP8_NO_Impuls_close_b

        L"S7:[S7 connection_4]DB32,X166.0",   //OP1_Work
        L"S7:[S7 connection_4]DB32,X166.1",   //OP2_Work
        L"S7:[S7 connection_4]DB32,X166.2",   //OP3_Work
        L"S7:[S7 connection_4]DB32,X166.3",   //OP4_Work
        L"S7:[S7 connection_4]DB32,X166.4",   //OP5_Work
        L"S7:[S7 connection_4]DB32,X166.5",   //OP6_Work
        L"S7:[S7 connection_4]DB32,X166.6",   //OP7_Work
        L"S7:[S7 connection_4]DB32,X166.7",   //OP8_Work

        L"S7:[S7 connection_4]DB32,X167.0",   //OP1_End
        L"S7:[S7 connection_4]DB32,X167.1",   //OP2_End
        L"S7:[S7 connection_4]DB32,X167.2",   //OP3_End
        L"S7:[S7 connection_4]DB32,X167.3",   //OP4_End
        L"S7:[S7 connection_4]DB32,X167.4",   //OP5_End
        L"S7:[S7 connection_4]DB32,X167.5",   //OP6_End
        L"S7:[S7 connection_4]DB32,X167.6",   //OP7_End
        L"S7:[S7 connection_4]DB32,X167.7",   //OP8_End

        //real
        L"S7:[S7 connection_4]DB32,REAL4",   //OP1_Voltage_a
        L"S7:[S7 connection_4]DB32,REAL8",   //OP1_Current_a
        L"S7:[S7 connection_4]DB32,REAL12",  //OP1_Power_a
        L"S7:[S7 connection_4]DB32,REAL16",  //OP1_Resist_a
        L"S7:[S7 connection_4]DB32,REAL20",  //OP1_Voltage_b
        L"S7:[S7 connection_4]DB32,REAL24",  //OP1_Current_b
        L"S7:[S7 connection_4]DB32,REAL28",  //OP1_Power_b
        L"S7:[S7 connection_4]DB32,REAL32",  //OP1_Resist_b

        L"S7:[S7 connection_4]DB32,REAL38",   //OP3_Sred_Rashod

        L"S7:[S7 connection_4]DB32,REAL44",   //OP4_Q_Fakt_03Qmax_A
        L"S7:[S7 connection_4]DB32,REAL48",   //OP4_D_P_03Qmax_A
        L"S7:[S7 connection_4]DB32,REAL52",   //OP4_D_A_03Qmax_A
        L"S7:[S7 connection_4]DB32,REAL116",  //OP4_BP5_BP3_03Qmax_A

        L"S7:[S7 connection_4]DB32,REAL56",   //OP4_Q_Fakt_06Qmax_A
        L"S7:[S7 connection_4]DB32,REAL60",   //OP4_D_P_06Qmax_A
        L"S7:[S7 connection_4]DB32,REAL64",   //OP4_D_A_06Qmax_A
        L"S7:[S7 connection_4]DB32,REAL120",  //OP4_BP5_BP3_06Qmax_A

        L"S7:[S7 connection_4]DB32,REAL68",   //OP4_Q_Fakt_Qmax_A
        L"S7:[S7 connection_4]DB32,REAL72",   //OP4_D_P_Qmax_A
        L"S7:[S7 connection_4]DB32,REAL76",   //OP4_D_A_Qmax_A
        L"S7:[S7 connection_4]DB32,REAL124",  //OP4_BP5_BP3_Qmax_A

        L"S7:[S7 connection_4]DB32,REAL80",   //OP4_Q_Fakt_03Qmax_B
        L"S7:[S7 connection_4]DB32,REAL84",   //OP4_D_P_03Qmax_B
        L"S7:[S7 connection_4]DB32,REAL88",   //OP4_D_A_03Qmax_B
        L"S7:[S7 connection_4]DB32,REAL128",  //OP4_BP5_BP3_03Qmax_B

        L"S7:[S7 connection_4]DB32,REAL92",   //OP4_Q_Fakt_06Qmax_B
        L"S7:[S7 connection_4]DB32,REAL96",   //OP4_D_P_06Qmax_B
        L"S7:[S7 connection_4]DB32,REAL100",  //OP4_D_A_06Qmax_B
        L"S7:[S7 connection_4]DB32,REAL132",  //OP4_BP5_BP3_06Qmax_B

        L"S7:[S7 connection_4]DB32,REAL104",  //OP4_Q_Fakt_Qmax_B
        L"S7:[S7 connection_4]DB32,REAL108",  //OP4_D_P_Qmax_B
        L"S7:[S7 connection_4]DB32,REAL112",  //OP4_D_A_Qmax_B
        L"S7:[S7 connection_4]DB32,REAL136",  //OP4_BP5_BP3_Qmax_B

        L"S7:[S7 connection_4]DB32,REAL144",  //OP8_Time_on_a
        L"S7:[S7 connection_4]DB32,REAL148",  //OP8_Time_off_a
        L"S7:[S7 connection_4]DB32,REAL152",  //OP8_Time_on_b
        L"S7:[S7 connection_4]DB32,REAL156",  //OP8_Time_off_b

        L"S7:[S7 connection_4]DB32,REAL162",  //Temperatura_masla
    };
};

}//namespace data

}//namespace cpu
