#include "db32.h"
#include "../myOPC/miniOPC.h"
#include "memory.h"
namespace cpu
{
namespace data
{
DB32::DB32():
    //хз что
    p_e_ok( mBoolData[0] ),               //2.4 давление в системе достигнуто ( электро )
    p_eg_ok( mBoolData[1] ),              //2.5 давление в системе достигнуто ( электрогидравл )

    //test1
    op1_min_ok_a( mBoolData[2] ),        //2.0 минимальное давление пройдено ( кат. а )
    op1_min_no_a( mBoolData[3] ),         //2.1 минимальное давление НЕ пройдено ( кат. а )
    op1_min_ok_b( mBoolData[4] ),        //2.2 минимальное давление пройдено ( кат. б )
    op1_min_no_b( mBoolData[5] ),         //2.3 минимальное давление НЕ пройдено ( кат. б )
    volt_alarm_a( mBoolData[6] ),       //2.6  напряжение катушки а вне допуска
    volt_alarm_b( mBoolData[7] ),       //36.0 напряжение катушки б вне допуска
    op1_max_ok_a( mBoolData[8] ),        //2.7 максимальное давление пройдено ( кат. а )
    op1_max_o_a( mBoolData[9] ),         //3.0 максимальное давление НЕ пройдено ( кат. а )
    op1_max_ok_b( mBoolData[10] ),        //3.1 максимальное давление пройдено ( кат. б )
    op1_max_no_b( mBoolData[11] ),         //3.2 максимальное давление НЕ пройдено ( кат. б )

    op1_volt_a( mFloatData[0] ),        //4.0 замерянное наряжение катушки а
    op1_current_a( mFloatData[1] ),        //8.0 замерянный ток катушки а
    op1_power_a( mFloatData[2] ),          //12.0 расчетная мощьность катушки а
    op1_resist_a( mFloatData[3] ),         //16.0 расчетное сопротивение катушки а
    op1_volt_b( mFloatData[4] ),        //20.0 замерянное наряжение катушки б
    op1_current_b( mFloatData[5] ),        //24.0 замерянный ток катушки б
    op1_power_b( mFloatData[6] ),          //28.0 расчетная мощьность катушки а
    op1_resist_b( mFloatData[7] ),         //32.0 расчетное сопротивение катушки а

    //test2 ???
    op2_ok_no( mBoolData[12] ),       //36.1 течь обнаружена, не обнаружена

    //test3
    op3_consumption_1( mFloatData[8] ),      //38.0 средний расход
    op3_consumption_2( mBoolData[13] ),      //42.0 расход в норме
    op3_consumption_3( mBoolData[14] ),     //42.1 расход выше нормы

    //test4
    op4_q_03_a( mFloatData[9] ),  //44.0 фактический расход 0,3Qmax(А)
    op4_p_p_03_a( mFloatData[10] ),     //48.0 давление в канале P 0,3Qmax(А)
    op4_p_a_03_a( mFloatData[11] ),     //52.0 давление в канале A 0,3Qmax(А)
    op4_bp5_bp3_03_a( mFloatData[12] ), //116.0 перепад BP5/BP3 0,3Qmax(А)

    op4_q_06_a( mFloatData[13] ),  //56.0 фактический расход 0,6Qmax(А)
    op4_p_p_06_a( mFloatData[14] ),     //60.0 давление в канале P 0,6Qmax(А)
    op4_p_a_06_a( mFloatData[15] ),     //64.0 давление в канале A 0,6Qmax(А)
    op4_bp5_bp3_06_a( mFloatData[16] ), //120.0 перепад BP5/BP3 0,6Qmax(А)

    op4_q_max_a( mFloatData[17] ),    //68.0 фактический расход Qmax(А)
    op4_p_p_max_a( mFloatData[18] ),       //72.0 давление в канале P Qmax(А)
    op4_p_a_max_a( mFloatData[19] ),       //76.0 давление в канале A Qmax(А)
    op4_bp5_bp3_max_a( mFloatData[20] ),   //124.0 перепад BP5/BP3 Qmax(А)


    op4_q_03_b( mFloatData[21] ),  //80.0 фактический расход 0,3Qmax(B)
    op4_p_p_03_b( mFloatData[22] ),     //84.0 давление в канале P 0,3Qmax(B)
    op4_p_a_03_b( mFloatData[23] ),     //88.0 давление в канале A 0,3Qmax(B)
    op4_bp5_bp3_03_b( mFloatData[24] ), //128.0 перепад BP5/BP3 0,3Qmax(B)

    op4_q_06_b( mFloatData[25] ),  //92.0 фактический расход 0,6Qmax(B)
    op4_p_p_06_b( mFloatData[26] ),     //96.0 давление в канале P 0,6Qmax(B)
    op4_p_a_06_b( mFloatData[27] ),     //100.0 давление в канале A 0,6Qmax(B)
    op4_bp5_bp3_06_b( mFloatData[28] ), //132.0 перепад BP5/BP3 0,6Qmax(B)

    op4_q_max_b( mFloatData[29] ),    //104.0 фактический расход Qmax(B)
    op4_p_p_max_b( mFloatData[30] ),       //108.0 давление в канале P Qmax(B)
    op4_p_a_max_b( mFloatData[31] ),       //112.0 давление в канале A Qmax(B)
    op4_bp5_bp3_max_b( mFloatData[32] ),   //136.0 перепад BP5/BP3 Qmax(B)

    //test5
    op5_ok_a( mBoolData[15] ),    //140.0 пропускная способность соотвествует А
    op5_no_a( mBoolData[16] ),    //140.1 пропускная способность не соотвествует А
    op5_ok_b( mBoolData[17] ),    //140.2 пропускная способность соотвествует В
    op5_no_b( mBoolData[18] ),    //140.3 пропускная способность не соотвествует В
    op5_pa( mFloatData[38] ),     //174   Расход на канале А
    op5_pb( mFloatData[39] ),     //174   Расход на канале Б
    //test6
    op6_min_min_ok_a( mBoolData[19] ),    //140.4 "а" перекл. при мин. давл. (мин упр.)
    op6_min_min_no_a( mBoolData[20] ),     //140.5 "а" не перекл. при мин. давл. (мин упр.)
    op6_min_max_ok_a( mBoolData[21] ),    //140.6 "а" перекл. при мин. давл. (макс упр.)
    op6_min_max_no_a( mBoolData[22] ),     //140.7 "а" перекл. при мин. давл. (макс упр.)
    op6_max_min_ok_a( mBoolData[23] ),    //141.0 "а" перекл. при макс. давл. (мин упр.)
    op6_max_min_no_b( mBoolData[24] ),     //141.1 "а" перекл. при макс. давл. (мин упр.)
    op6_max_max_ok_a( mBoolData[25] ),    //141.2 "а" перекл. при макс. давл. (макс упр.)
    op6_max_max_no_a( mBoolData[26] ),     //141.3 "а" перекл. при макс. давл. (макс упр.)

    op6_min_min_ok_b( mBoolData[27] ),    //141.4 "б" перекл. при мин. давл. (мин упр.)
    op6_min_min_no_b( mBoolData[28] ),     //141.5 "б" не перекл. при мин. давл. (мин упр.)
    op6_min_max_ok_b( mBoolData[29] ),    //141.6 "б" перекл. при мин. давл. (макс упр.)
    op6_min_max_no_b( mBoolData[30] ),     //141.7 "б" перекл. при мин. давл. (макс упр.)
    op6_max_min_ok_b( mBoolData[31] ),    //142.0 "б" перекл. при макс. давл. (мин упр.)
    op6_mex_min_no_b( mBoolData[32] ),     //142.1 "б" перекл. при макс. давл. (мин упр.)
    op6_max_max_ok_b( mBoolData[33] ),    //142.2 "б" перекл. при макс. давл. (макс упр.)
    op6_max_max_no_b( mBoolData[34] ),     //142.3 "б" перекл. при макс. давл. (макс упр.)

    OP6_MinD_MinUpr_YESa_off( mBoolData[71] ),  //X182.0;RW;TST=;CMT=а перекл.при мин.давлении (мин.упр)
    OP6_MinD_MinUpr_NOa_off ( mBoolData[72] ),  //X182.1;RW;TST=;CMT=а НЕ перекл.при мин.давлении (мин.упр)
    OP6_MinD_MaxUpr_YESa_off( mBoolData[73] ),  //X182.2;RW;TST=;CMT=а перекл.при мин.давлении (макс.упр)
    OP6_MinD_MaxUpr_NOa_off ( mBoolData[74] ),  //X182.3;RW;TST=;CMT=а НЕ перекл.при мин.давлении (макс.упр)
    OP6_MaxD_MinUpr_YESa_off( mBoolData[75] ),  //X182.4;RW;TST=;CMT=а перекл.при макс.давлении (мин.упр)
    OP6_MaxD_MinUpr_NOa_off ( mBoolData[76] ),  //X182.5;RW;TST=;CMT=а НЕ перекл.при макс.давлении (мин.упр)
    OP6_MaxD_MaxUpr_YESa_off( mBoolData[77] ),  //X182.6;RW;TST=;CMT=а перекл.при макс.давлении (макс.упр)
    OP6_MaxD_MaxUpr_NOa_off ( mBoolData[78] ),  //X182.7;RW;TST=;CMT=а НЕ перекл.при макс.давлении (макс.упр)
    OP6_MinD_MinUpr_YESb_off( mBoolData[79] ),  //X183.0;RW;TST=;CMT=b перекл.при мин.давлении (мин.упр)
    OP6_MinD_MinUpr_NOb_off ( mBoolData[80] ),  //X183.1;RW;TST=;CMT=b НЕ перекл.при мин.давлении (мин.упр)
    OP6_MinD_MaxUpr_YESb_off( mBoolData[81] ),  //X183.2;RW;TST=;CMT=b перекл.при мин.давлении (макс.упр)
    OP6_MinD_MaxUpr_NOb_off ( mBoolData[82] ),  //X183.3;RW;TST=;CMT=b НЕ перекл.при мин.давлении (макс.упр)
    OP6_MaxD_MinUpr_YESb_off( mBoolData[83] ),  //X183.4;RW;TST=;CMT=b перекл.при макс.давлении (мин.упр)
    OP6_MaxD_MinUpr_NOb_off ( mBoolData[84] ),  //X183.5;RW;TST=;CMT=b НЕ перекл.при макс.давлении (мин.упр)
    OP6_MaxD_MaxUpr_YESb_off( mBoolData[85] ),  //X183.6;RW;TST=;CMT=b перекл.при макс.давлении (макс.упр)
    OP6_MaxD_MaxUpr_NOb_off ( mBoolData[86] ),  //X183.7;RW;TST=;CMT=b НЕ перекл.при макс.давлении (макс.упр)


    //test7
    op7_min_ok_a( mBoolData[35] ),  //142.4 "а" перекл. при мин. давлении (Vmin)
    op7_min_no_a( mBoolData[36] ),   //142.5 "а" не перекл. при мин. давлении (Vmin)
    op7_min_ok_b( mBoolData[37] ),  //142.6 "б" перекл. при мин. давлении (Vmin)
    op7_min_no_b( mBoolData[38] ),   //142.7 "б" не перекл. при мин. давлении (Vmin)
    op7_max_ok_a( mBoolData[39] ),  //143.0 "а" перекл. при макс. давлении (Vmin)
    op7_max_no_a( mBoolData[40] ),   //143.1 "а" не перекл. при макс. давлении (Vmin)
    op7_max_ok_b( mBoolData[41] ),  //143.2 "б" перекл. при макс. давлении (Vmin)
    op7_max_no_b( mBoolData[42] ),   //143.3 "б" не перекл. при макс. давлении (Vmin)
    OP7_Min_D_YESa_off  ( mBoolData[87] ),      //X184.0;RW;TST=;CMT=а перекл.при мин.давлении (Vmin)
    OP7_Min_D_NOa_off   ( mBoolData[88] ),      //X184.1;RW;TST=;CMT=а НЕ перекл.при мин.давлении (Vmin)
    OP7_Min_D_YESb_off  ( mBoolData[89] ),      //X184.2;RW;TST=;CMT=b перекл.при мин.давлении (Vmin)
    OP7_Min_D_NOb_off   ( mBoolData[90] ),      //X184.3;RW;TST=;CMT=b НЕ перекл.при мин.давлении (Vmin)
    OP7_Max_D_YESa_off  ( mBoolData[91] ),      //X184.4;RW;TST=;CMT=а перекл.при макс.давлении (Vmin)
    OP7_Max_D_NOa_off   ( mBoolData[92] ),      //X184.5;RW;TST=;CMT=а НЕ перекл.при макс.давлении (Vmin)
    OP7_Max_D_YESb_off  ( mBoolData[93] ),      //X184.6;RW;TST=;CMT=b перекл.при макс.давлении (Vmin)
    OP7_Max_D_NOb_off   ( mBoolData[94] ),      //X184.7;RW;TST=;CMT=b НЕ перекл.при макс.давлении (Vmin)

    //test8
    op8_time_on_a( mFloatData[33] ),       //144.0 время включения катуки "а"
    op8_time_off_a( mFloatData[34] ),      //148.0 время отключения катуки "а"
    op8_time_on_b( mFloatData[35] ),       //152.0 время включения катуки "б"
    op8_time_off_b( mFloatData[36] ),      //156.0 время отключения катуки "б"

    op8_open_ok_a( mBoolData[43] ),       //160.0 "а" время включения соответствует
    op8_open_no_a( mBoolData[44] ),        //160.1 "а" время включения не соответствует
    op8_close_ok_a( mBoolData[45] ),      //160.2 "а" время отключения соответствует
    op8_close_no_a( mBoolData[46] ),       //160.3 "а" время отключения не соответствует
    op8_open_ok_b( mBoolData[47] ),       //160.4 "б" время включения соответствует
    op8_open_no_b( mBoolData[48] ),        //160.5 "б" время включения не соответствует
    op8_close_ok_b( mBoolData[49] ),      //160.6 "б" время отключения соответствует
    op8_close_no_b( mBoolData[50] ),       //160.7 "б" время отключения не соответствует

    op8_fault_on_a( mBoolData[51] ),     //161.0 катушка "а" не включилась
    op8_fault_off_a( mBoolData[52] ),    //161.1 катушка "а" не отключилась
    op8_fault_on_b( mBoolData[53] ),     //161.2 катушка "б" не включилась
    op8_fault_off_b( mBoolData[54] ),    //161.3 катушка "б" не отключилась

    //служебные
    T_oil( mFloatData[37] ),   //162.0 температура масла

    op1_ok( mBoolData[55] ), //166.0 Выполнение операции 1
    op2_ok( mBoolData[56] ), //166.1 Выполнение операции 2
    op3_ok( mBoolData[57] ), //166.2 Выполнение операции 3
    op4_ok( mBoolData[58] ), //166.3 Выполнение операции 4
    op5_ok( mBoolData[59] ), //166.4 Выполнение операции 5
    op6_ok( mBoolData[60] ), //166.5 Выполнение операции 6
    op7_ok( mBoolData[61] ), //166.6 Выполнение операции 7
    op8_ok( mBoolData[62] ), //166.7 Выполнение операции 8

    op1_end( mBoolData[63] ),  //167.0 Конец операции 1
    op2_end( mBoolData[64] ),  //167.1 Конец операции 2
    op3_end( mBoolData[65] ),  //167.2 Конец операции 3
    op4_end( mBoolData[66] ),  //167.3 Конец операции 4
    op5_end( mBoolData[67] ),  //167.4 Конец операции 5
    op6_end( mBoolData[68] ),  //167.5 Конец операции 6
    op7_end( mBoolData[69] ),  //167.6 Конец операции 7
    op8_end( mBoolData[70] )  //167.7 Конец операции 8
{
    memset( mBoolData, 0, sizeof(mBoolData) );
    memset( mFloatData, 0, sizeof(mFloatData) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB32", mAdresses, BOOL_COUNT + FLOAT_COUNT );

    {
#ifdef DEMO
        //хз что
        p_e_ok = true;               //2.4 давление в системе достигнуто ( электро )
        p_eg_ok = true;              //2.5 давление в системе достигнуто ( электрогидравл )

        //test1
        op1_min_ok_a = true;        //2.0 минимальное давление пройдено ( кат. а )
        op1_min_no_a = false;         //2.1 минимальное давление НЕ пройдено ( кат. а )
        op1_min_ok_b = true;        //2.2 минимальное давление пройдено ( кат. б )
        op1_min_no_b = false;         //2.3 минимальное давление НЕ пройдено ( кат. б )
        volt_alarm_a = false;       //2.6  напряжение катушки а вне допуска
        volt_alarm_b = false;       //36.0 напряжение катушки б вне допуска
        op1_max_ok_a = true;        //2.7 максимальное давление пройдено ( кат. а )
        op1_max_o_a = false;         //3.0 максимальное давление НЕ пройдено ( кат. а )
        op1_max_ok_b = true;        //3.1 максимальное давление пройдено ( кат. б )
        op1_max_no_b = false;         //3.2 максимальное давление НЕ пройдено ( кат. б )

        op1_volt_a = 253;        //4.0 замерянное наряжение катушки а
        op1_current_a = 20;        //8.0 замерянный ток катушки а
        op1_power_a = 30;          //12.0 расчетная мощьность катушки а
        op1_resist_a = 53;         //16.0 расчетное сопротивение катушки а
        op1_volt_b = 10;        //20.0 замерянное наряжение катушки б
        op1_current_b = 63;        //24.0 замерянный ток катушки б
        op1_power_b = 10;          //28.0 расчетная мощьность катушки а
        op1_resist_b = 1;         //32.0 расчетное сопротивение катушки а

        //test2 ???
        op2_ok_no = false;       //36.1 течь обнаружена, не обнаружена //не используется

        //test3
        op3_consumption_1 = 321;      //38.0 средний расход
        op3_consumption_2 = true;      //42.0 расход в норме
        op3_consumption_3 = false;     //42.1 расход выше нормы

        //test4
        op4_q_03_a = 100;
        op4_p_p_03_a = 300;
        op4_p_a_03_a = 150;
        op4_bp5_bp3_03_a = 150;

        op4_q_06_a = 300;
        op4_p_p_06_a = 270;
        op4_p_a_06_a = 170;
        op4_bp5_bp3_06_a = 100;

        op4_q_max_a = 600;
        op4_p_p_max_a = 200;
        op4_p_a_max_a = 120;
        op4_bp5_bp3_max_a = 80;


        op4_q_03_b = 150;
        op4_p_p_03_b = 280;
        op4_p_a_03_b = 140;
        op4_bp5_bp3_03_b = 140;

        op4_q_06_b = 250;
        op4_p_p_06_b = 250;
        op4_p_a_06_b = 170;
        op4_bp5_bp3_06_b = 80;

        op4_q_max_b = 550;
        op4_p_p_max_b = 170;
        op4_p_a_max_b = 70;
        op4_bp5_bp3_max_b = 100;

        //test5
        op5_ok_a = true;    //140.0 пропускная способность соотвествует А
        op5_no_a = false;    //140.1 пропускная способность не соотвествует А
        op5_ok_b = true;    //140.2 пропускная способность соотвествует В
        op5_no_b = false;    //140.3 пропускная способность не соотвествует В

        //test6
        op6_min_min_ok_a = true;    //140.4 "а" перекл. при мин. давл. (мин упр.)
        op6_min_min_no_a = false;     //140.5 "а" не перекл. при мин. давл. (мин упр.)
        op6_min_max_ok_a = true;    //140.6 "а" перекл. при мин. давл. (макс упр.)
        op6_min_max_no_a = false;     //140.7 "а" перекл. при мин. давл. (макс упр.)
        op6_max_min_ok_a = true;    //141.0 "а" перекл. при макс. давл. (мин упр.)
        op6_max_min_no_b = false;     //141.1 "а" перекл. при макс. давл. (мин упр.)
        op6_max_max_ok_a = true;    //141.2 "а" перекл. при макс. давл. (макс упр.)
        op6_max_max_no_a = false;     //141.3 "а" перекл. при макс. давл. (макс упр.)

        op6_min_min_ok_b = true;    //141.4 "б" перекл. при мин. давл. (мин упр.)
        op6_min_min_no_b = false;     //141.5 "б" не перекл. при мин. давл. (мин упр.)
        op6_min_max_ok_b = true;    //141.6 "б" перекл. при мин. давл. (макс упр.)
        op6_min_max_no_b = false;     //141.7 "б" перекл. при мин. давл. (макс упр.)
        op6_max_min_ok_b = true;    //142.0 "б" перекл. при макс. давл. (мин упр.)
        op6_mex_min_no_b = false;     //142.1 "б" перекл. при макс. давл. (мин упр.)
        op6_max_max_ok_b = true;    //142.2 "б" перекл. при макс. давл. (макс упр.)
        op6_max_max_no_b = false;     //142.3 "б" перекл. при макс. давл. (макс упр.)


        //test7
        op7_min_ok_a = true;  //142.4 "а" перекл. при мин. давлении (Vmin)
        op7_min_no_a = false;   //142.5 "а" не перекл. при мин. давлении (Vmin)
        op7_min_ok_b = true;  //142.6 "б" перекл. при мин. давлении (Vmin)
        op7_min_no_b = false;   //142.7 "б" не перекл. при мин. давлении (Vmin)
        op7_max_ok_a = true;  //143.0 "а" перекл. при макс. давлении (Vmin)
        op7_max_no_a = false;   //143.1 "а" не перекл. при макс. давлении (Vmin)
        op7_max_ok_b = true;  //143.2 "б" перекл. при макс. давлении (Vmin)
        op7_max_no_b = false;   //143.3 "б" не перекл. при макс. давлении (Vmin)

        //test8
        op8_time_on_a = 50;       //144.0 время включения катуки "а"
        op8_time_off_a = 65;      //148.0 время отключения катуки "а"
        op8_time_on_b = 23;       //152.0 время включения катуки "б"
        op8_time_off_b = 25;      //156.0 время отключения катуки "б"

        op8_open_ok_a = true;       //160.0 "а" время включения соответствует
        op8_open_no_a = false;        //160.1 "а" время включения не соответствует
        op8_close_ok_a = true;      //160.2 "а" время отключения соответствует
        op8_close_no_a = false;       //160.3 "а" время отключения не соответствует
        op8_open_ok_b = true;       //160.4 "б" время включения соответствует
        op8_open_no_b = false;        //160.5 "б" время включения не соответствует
        op8_close_ok_b = true;      //160.6 "б" время отключения соответствует
        op8_close_no_b = false;       //160.7 "б" время отключения не соответствует

        op8_fault_on_a = false;     //161.0 катушка "а" не включилась
        op8_fault_off_a = false;    //161.1 катушка "а" не отключилась
        op8_fault_on_b = false;     //161.2 катушка "б" не включилась
        op8_fault_off_b = false;    //161.3 катушка "б" не отключилась

        //служебные
        T_oil = 66.3;   //162.0 температура масла

        op1_ok = true; //166.0 Выполнение операции 1
        op2_ok = true; //166.1 Выполнение операции 2
        op3_ok = true; //166.2 Выполнение операции 3
        op4_ok = true; //166.3 Выполнение операции 4
        op5_ok = true; //166.4 Выполнение операции 5
        op6_ok = true; //166.5 Выполнение операции 6
        op7_ok = true; //166.6 Выполнение операции 7
        op8_ok = true; //166.7 Выполнение операции 8

        op1_end = true;  //167.0 Конец операции 1
        op2_end = true;  //167.1 Конец операции 2
        op3_end = true;  //167.2 Конец операции 3
        op4_end = true;  //167.3 Конец операции 4
        op5_end = true;  //167.4 Конец операции 5
        op6_end = true;  //167.5 Конец операции 6
        op7_end = true;  //167.6 Конец операции 7
        op8_end = true;  //167.7 Конец операции 8

        opc::miniOPC::Instance().WriteMass( mGroupID, BOOL_COUNT, FLOAT_COUNT, static_cast<void*>( mFloatData ), opc::tFLOAT );
        opc::miniOPC::Instance().WriteMass( mGroupID, 0, BOOL_COUNT, static_cast<void*>( mBoolData ), opc::tBOOL );
#endif
    }
}

void DB32::Read()
{
    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
    if (!rez)
    {
        //ошибка подключения..
        return;
    }
    for (size_t i = 0; i < BOOL_COUNT + FLOAT_COUNT; i++)
    {
        if ( i < BOOL_COUNT )
            mBoolData[ i ] = rez[i].vDataValue.boolVal;
        else
            mFloatData[ i - BOOL_COUNT ] = rez[i].vDataValue.fltVal;
    }
    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}//namespace data
}//namespace cpu

