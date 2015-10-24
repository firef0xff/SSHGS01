#include "db32.h"

namespace cpu
{
namespace data
{
DB32::DB32():
    //хз что
    D_E1_OK( mBoolData[0] ),               //2.4 давление в системе достигнуто ( электро )
    D_E1G_OK( mBoolData[1] ),              //2.5 давление в системе достигнуто ( электрогидравл )

    //test1
    OP1_Min_D_YESa( mBoolData[2] ),        //2.0 минимальное давление пройдено ( кат. а )
    OP1_Min_D_NOa( mBoolData[3] ),         //2.1 минимальное давление НЕ пройдено ( кат. а )
    OP1_Min_D_YESb( mBoolData[4] ),        //2.2 минимальное давление пройдено ( кат. б )
    OP1_Min_D_NOb( mBoolData[5] ),         //2.3 минимальное давление НЕ пройдено ( кат. б )
    Voltage_Alarm_a( mBoolData[6] ),       //2.6  напряжение катушки а вне допуска
    Voltage_Alarm_b( mBoolData[7] ),       //36.0 напряжение катушки б вне допуска
    OP1_Max_D_YESa( mBoolData[8] ),        //2.7 максимальное давление пройдено ( кат. а )
    OP1_Max_D_NOa( mBoolData[9] ),         //3.0 максимальное давление НЕ пройдено ( кат. а )
    OP1_Max_D_YESb( mBoolData[10] ),        //3.1 максимальное давление пройдено ( кат. б )
    OP1_Max_D_NOb( mBoolData[11] ),         //3.2 максимальное давление НЕ пройдено ( кат. б )

    OP1_Voltage_a( mFloatData[0] ),        //4.0 замерянное наряжение катушки а
    OP1_Current_a( mFloatData[1] ),        //8.0 замерянный ток катушки а
    OP1_Power_a( mFloatData[2] ),          //12.0 расчетная мощьность катушки а
    OP1_Resist_a( mFloatData[3] ),         //16.0 расчетное сопротивение катушки а
    OP1_Voltage_b( mFloatData[4] ),        //20.0 замерянное наряжение катушки б
    OP1_Current_b( mFloatData[5] ),        //24.0 замерянный ток катушки б
    OP1_Power_b( mFloatData[6] ),          //28.0 расчетная мощьность катушки а
    OP1_Resist_b( mFloatData[7] ),         //32.0 расчетное сопротивение катушки а

    //test2 ???
    OP2_Tech_Yes_No( mBoolData[12] ),       //36.1 течь обнаружена, не обнаружена

    //test3
    OP3_Sred_Rashod( mFloatData[8] ),      //38.0 средний расход
    OP3_Rashod_Norma( mBoolData[13] ),      //42.0 расход в норме
    OP3_Rashod_VNorma( mBoolData[14] ),     //42.1 расход выше нормы

    //test4
    OP4_Q_Fakt_03Qmax_A( mFloatData[9] ),  //44.0 фактический расход 0,3Qmax(А)
    OP4_D_P_03Qmax_A( mFloatData[10] ),     //48.0 давление в канале P 0,3Qmax(А)
    OP4_D_A_03Qmax_A( mFloatData[11] ),     //52.0 давление в канале A 0,3Qmax(А)
    OP4_BP5_BP3_03Qmax_A( mFloatData[12] ), //116.0 перепад BP5/BP3 0,3Qmax(А)

    OP4_Q_Fakt_06Qmax_A( mFloatData[13] ),  //56.0 фактический расход 0,6Qmax(А)
    OP4_D_P_06Qmax_A( mFloatData[14] ),     //60.0 давление в канале P 0,6Qmax(А)
    OP4_D_A_06Qmax_A( mFloatData[15] ),     //64.0 давление в канале A 0,6Qmax(А)
    OP4_BP5_BP3_06Qmax_A( mFloatData[16] ), //120.0 перепад BP5/BP3 0,6Qmax(А)

    OP4_Q_Fakt_Qmax_A( mFloatData[17] ),    //68.0 фактический расход Qmax(А)
    OP4_D_P_Qmax_A( mFloatData[18] ),       //72.0 давление в канале P Qmax(А)
    OP4_D_A_Qmax_A( mFloatData[19] ),       //76.0 давление в канале A Qmax(А)
    OP4_BP5_BP3_Qmax_A( mFloatData[20] ),   //124.0 перепад BP5/BP3 Qmax(А)


    OP4_Q_Fakt_03Qmax_B( mFloatData[21] ),  //80.0 фактический расход 0,3Qmax(B)
    OP4_D_P_03Qmax_B( mFloatData[22] ),     //84.0 давление в канале P 0,3Qmax(B)
    OP4_D_A_03Qmax_B( mFloatData[23] ),     //88.0 давление в канале A 0,3Qmax(B)
    OP4_BP5_BP3_03Qmax_B( mFloatData[24] ), //128.0 перепад BP5/BP3 0,3Qmax(B)

    OP4_Q_Fakt_06Qmax_B( mFloatData[25] ),  //92.0 фактический расход 0,6Qmax(B)
    OP4_D_P_06Qmax_B( mFloatData[26] ),     //96.0 давление в канале P 0,6Qmax(B)
    OP4_D_A_06Qmax_B( mFloatData[27] ),     //100.0 давление в канале A 0,6Qmax(B)
    OP4_BP5_BP3_06Qmax_B( mFloatData[28] ), //132.0 перепад BP5/BP3 0,6Qmax(B)

    OP4_Q_Fakt_Qmax_B( mFloatData[29] ),    //104.0 фактический расход Qmax(B)
    OP4_D_P_Qmax_B( mFloatData[30] ),       //108.0 давление в канале P Qmax(B)
    OP4_D_A_Qmax_B( mFloatData[31] ),       //112.0 давление в канале A Qmax(B)
    OP4_BP5_BP3_Qmax_B( mFloatData[32] ),   //136.0 перепад BP5/BP3 Qmax(B)

    //test5
    OP5_A_OK( mBoolData[15] ),    //140.0 пропускная способность соотвествует А
    OP5_A_NO( mBoolData[16] ),    //140.1 пропускная способность не соотвествует А
    OP5_B_OK( mBoolData[17] ),    //140.2 пропускная способность соотвествует В
    OP5_B_NO( mBoolData[18] ),    //140.3 пропускная способность не соотвествует В

    //test6
    OP6_MinD_MinUprD_YESa( mBoolData[19] ),    //140.4 "а" перекл. при мин. давл. (мин упр.)
    OP6_MinD_MinUprD_NOa( mBoolData[20] ),     //140.5 "а" не перекл. при мин. давл. (мин упр.)
    OP6_MinD_MaxUprD_YESa( mBoolData[21] ),    //140.6 "а" перекл. при мин. давл. (макс упр.)
    OP6_MinD_MaxUprD_NOa( mBoolData[22] ),     //140.7 "а" перекл. при мин. давл. (макс упр.)
    OP6_MaxD_MinUprD_YESa( mBoolData[23] ),    //141.0 "а" перекл. при макс. давл. (мин упр.)
    OP6_MaxD_MinUprD_NOa( mBoolData[24] ),     //141.1 "а" перекл. при макс. давл. (мин упр.)
    OP6_MaxD_MaxUprD_YESa( mBoolData[25] ),    //141.2 "а" перекл. при макс. давл. (макс упр.)
    OP6_MaxD_MaxUprD_NOa( mBoolData[26] ),     //141.3 "а" перекл. при макс. давл. (макс упр.)

    OP6_MinD_MinUprD_YESb( mBoolData[27] ),    //141.4 "б" перекл. при мин. давл. (мин упр.)
    OP6_MinD_MinUprD_NOb( mBoolData[28] ),     //141.5 "б" не перекл. при мин. давл. (мин упр.)
    OP6_MinD_MaxUprD_YESb( mBoolData[29] ),    //141.6 "б" перекл. при мин. давл. (макс упр.)
    OP6_MinD_MaxUprD_NOb( mBoolData[30] ),     //141.7 "б" перекл. при мин. давл. (макс упр.)
    OP6_MaxD_MinUprD_YESb( mBoolData[31] ),    //142.0 "б" перекл. при макс. давл. (мин упр.)
    OP6_MaxD_MinUprD_NOb( mBoolData[32] ),     //142.1 "б" перекл. при макс. давл. (мин упр.)
    OP6_MaxD_MaxUprD_YESb( mBoolData[33] ),    //142.2 "б" перекл. при макс. давл. (макс упр.)
    OP6_MaxD_MaxUprD_NOb( mBoolData[34] ),     //142.3 "б" перекл. при макс. давл. (макс упр.)


    //test7
    OP7_Min_D_YESa( mBoolData[35] ),  //142.4 "а" перекл. при мин. давлении (Vmin)
    OP7_Min_D_NOa( mBoolData[36] ),   //142.5 "а" не перекл. при мин. давлении (Vmin)
    OP7_Min_D_YESb( mBoolData[37] ),  //142.6 "б" перекл. при мин. давлении (Vmin)
    OP7_Min_D_NOb( mBoolData[38] ),   //142.7 "б" не перекл. при мин. давлении (Vmin)
    OP7_Max_D_YESa( mBoolData[39] ),  //143.0 "а" перекл. при макс. давлении (Vmin)
    OP7_Max_D_NOa( mBoolData[40] ),   //143.1 "а" не перекл. при макс. давлении (Vmin)
    OP7_Max_D_YESb( mBoolData[41] ),  //143.2 "б" перекл. при макс. давлении (Vmin)
    OP7_Max_D_NOb( mBoolData[42] ),   //143.3 "б" не перекл. при макс. давлении (Vmin)

    //test8
    OP8_Time_on_a( mFloatData[33] ),       //144.0 время включения катуки "а"
    OP8_Time_off_a( mFloatData[34] ),      //148.0 время отключения катуки "а"
    OP8_Time_on_b( mFloatData[35] ),       //152.0 время включения катуки "б"
    OP8_Time_off_b( mFloatData[36] ),      //156.0 время отключения катуки "б"

    OP8_open_YES_a( mBoolData[43] ),       //160.0 "а" время включения соответствует
    OP8_open_NO_a( mBoolData[44] ),        //160.1 "а" время включения не соответствует
    OP8_close_YES_a( mBoolData[45] ),      //160.2 "а" время отключения соответствует
    OP8_close_NO_a( mBoolData[46] ),       //160.3 "а" время отключения не соответствует
    OP8_open_YES_b( mBoolData[47] ),       //160.4 "б" время включения соответствует
    OP8_open_NO_b( mBoolData[48] ),        //160.5 "б" время включения не соответствует
    OP8_close_YES_b( mBoolData[49] ),      //160.6 "б" время отключения соответствует
    OP8_close_NO_b( mBoolData[50] ),       //160.7 "б" время отключения не соответствует

    OP8_NO_Impuls_open_a( mBoolData[51] ),     //161.0 катушка "а" не включилась
    OP8_NO_Impuls_close_a( mBoolData[52] ),    //161.1 катушка "а" не отключилась
    OP8_NO_Impuls_open_b( mBoolData[53] ),     //161.2 катушка "б" не включилась
    OP8_NO_Impuls_close_b( mBoolData[54] ),    //161.3 катушка "б" не отключилась

    //служебные
    Temperatura_masla( mFloatData[37] ),   //162.0 температура масла

    OP1_Work( mBoolData[55] ), //166.0 Выполнение операции 1
    OP2_Work( mBoolData[56] ), //166.1 Выполнение операции 2
    OP3_Work( mBoolData[57] ), //166.2 Выполнение операции 3
    OP4_Work( mBoolData[58] ), //166.3 Выполнение операции 4
    OP5_Work( mBoolData[59] ), //166.4 Выполнение операции 5
    OP6_Work( mBoolData[60] ), //166.5 Выполнение операции 6
    OP7_Work( mBoolData[61] ), //166.6 Выполнение операции 7
    OP8_Work( mBoolData[62] ), //166.7 Выполнение операции 8

    OP1_End( mBoolData[63] ),  //167.0 Конец операции 1
    OP2_End( mBoolData[64] ),  //167.1 Конец операции 2
    OP3_End( mBoolData[65] ),  //167.2 Конец операции 3
    OP4_End( mBoolData[66] ),  //167.3 Конец операции 4
    OP5_End( mBoolData[67] ),  //167.4 Конец операции 5
    OP6_End( mBoolData[68] ),  //167.5 Конец операции 6
    OP7_End( mBoolData[69] ),  //167.6 Конец операции 7
    OP8_End( mBoolData[70] )  //167.7 Конец операции 8
{
}

void DB32::Read()
{}

}//namespace data
}//namespace cpu

