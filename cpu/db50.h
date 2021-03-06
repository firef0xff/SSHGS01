#pragma once
#include "data_block_base.h"
#include <inttypes.h>
#include <mutex>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB50 :public In
{
public:
    void Read();

    bool& ReadyToWork;  //MX49.0 готовность системы к работе

    int& Osh_M;         //DB50,W66
    int& Osh_DM;        //DB50,W128

    float& BP1;         //DB50,REAL2    давление после М1/М2 (real)
    float& BP2;         //DB50,REAL6    давление на сливе испытуемого аппарата (real)
    float& BP3;         //DB50,REAL10    давление в полости А (real)
    float& BP4;         //DB50,REAL14    давление в полости В (real)
    float& BP5;         //DB50,REAL18    давление в полости Р (real)
    float& BP6;         //DB50,REAL22    давление в полости Х (real)
    float& BT1;         //DB50,REAL26    температура масла в баке (real)
    float& BV1;         //DB50,REAL30    расход в полости Р (0-60 л/мин) (real)
    float& BV2;         //DB50,REAL34    расход в полости Р (60-1000 л/мин) (real)
    float& BV4;         //DB50,REAL38    расход утечки (0.5-25 л/мин) (real)
    float& BV3;         //DB50,REAL42    расход утечки (0.005-2 л/мин) (real)
    float& V1_do60V;    //DB50,REAL46    Вольтметр до 60В (real)
    float& V0_300B;     //DB50,REAL50    Вольтметр 0-300В (real)
    float& A1;          //DB50,REAL54    Амперметр постоянного тока (mA)
    float& A2;          //DB50,REAL58    Амперметр переменного тока (mA)
    float& A_Plata;     //DB50,REAL62    Ток платы управления
    float& YB1;         //DB50,REAL68    Регулировка расхода основного насоса М1 (real)
    float& YB2;         //DB50,REAL72    Регулировка расхода основного насоса М2 (real)
    float& YB3;         //DB50,REAL76    Настройка давления в системе (real)
    float& YB4;         //DB50,REAL80    Настройка давления в канале управления (real)
    float& YB5;         //DB50,REAL84    Нагрузка в канеле А (real)
    float& YB6;         //DB50,REAL88    Нагрузка в канале В (real)
    float& Z_U_Post;    //DB50,REAL92    Задание на регулятор постоянного напряжения
    float& Z_U_Per;     //DB50,REAL96    Задание на регулятор переменного напряжения
    float& Plata_1;     //DB50,REAL100    Задание на плату 4..20мА (real)
    float& Plata_2;     //DB50,REAL104    Задание на плату 0..20мА (real)
    float& Plata_3;     //DB50,REAL108    Задание на плату +/-10В
    float& Plata_4_5_6; //DB50,REAL112    Задание на плату +/-20/15/10мА
    float& Plata_7;     //DB50,REAL116    Задание на плату +/-20мА (1)
    float& Plata_8;     //DB50,REAL120    Задание на плату +/-20мА (2)
    float& Plata_860mA; //DB50,REAL124    Задание на плату 0..20мА (860мА)
    float& POS_1_REAL;  //DB50,REAL130    Позиция Г/Ц (1..10л/мин) 100мм
    float& POS_2_REAL;  //DB50,REAL134    Позиция Г/Ц (0..1л/мин) 100мм
    float& POS_3_REAL;  //DB50,REAL138    Положение золотника

    //DB31
    float& P_YB3;           //DB31.DBD22
    float& P_YB4;           //DB31.DBD26
    float& Manual_set;      //31 сигнал на ручном режиме
//    управление золотником
    float& man_zol_v;       //DB31.DBD30 ВЕРХНИЙ УРОВЕНЬ СИГНАЛА
    float& man_zol_n;       //DB31.DBD34 НИЖНИЙ УРОВЕНЬ СИГНАЛА
    float& P_YB5;           //DB31,REAL40 Нагрузка в канеле А
    float& P_YB6;           //DB31,REAL44 Нагрузка в канале В
    bool& man_tupe_sign;   //DB31.DBX38.0 ВЫБОР ТИПА ВЫХОДА -ток -напряжение

    std::mutex& ReadLocker();
    void WriteTask();
private:    

    friend class cpu::CpuMemory;
    DB50();
    DB50( const DB50& ) = delete;
    void operator = ( const DB50& ) = delete;

    enum
    {
        BOOL_COUNT = 2,
        INT_COUNT = 2,
        FLOAT_COUNT = 41
    };

    bool mBoolData[ BOOL_COUNT ];
    int mIntData[ INT_COUNT ];
    float mFloatData[ FLOAT_COUNT ];
    uint64_t mGroupID = 0;
    std::mutex mLock;

    wchar_t const* mAdresses[ BOOL_COUNT + INT_COUNT + FLOAT_COUNT ] = {
        //bool
        L"CPU/DB50.ReadyToWork",
        L"CPU/DB50.man_tupe_sign",

        //int
        L"CPU/DB50.Osh_M",
        L"CPU/DB50.Osh_DM",

        //real
        L"CPU/DB50.BP1",
        L"CPU/DB50.BP2",
        L"CPU/DB50.BP3",
        L"CPU/DB50.BP4",
        L"CPU/DB50.BP5",
        L"CPU/DB50.BP6",
        L"CPU/DB50.BT1",
        L"CPU/DB50.BV1",
        L"CPU/DB50.BV2",
        L"CPU/DB50.BV4",
        L"CPU/DB50.BV3",
        L"CPU/DB50.V1_do60V",
        L"CPU/DB50.V0_300B",
        L"CPU/DB50.A1",
        L"CPU/DB50.A2",
        L"CPU/DB50.A_Plata",
        L"CPU/DB50.YB3",
        L"CPU/DB50.YB4",
        L"CPU/DB50.YB5",
        L"CPU/DB50.YB6",
        L"CPU/DB50.Z_U_Post",
        L"CPU/DB50.Z_U_Per",
        L"CPU/DB50.Plata_1",
        L"CPU/DB50.Plata_2",
        L"CPU/DB50.Plata_3",
        L"CPU/DB50.Plata_4_5_6",
        L"CPU/DB50.Plata_7",
        L"CPU/DB50.Plata_8",
        L"CPU/DB50.Plata_860mA",
        L"CPU/DB50.POS_1_REAL",
        L"CPU/DB50.POS_2_REAL",
        L"CPU/DB50.POS_3_REAL",

        //write group
        L"CPU/DB50.YB1",
        L"CPU/DB50.YB2",
        //db31
        L"CPU/DB50.P_YB3",
        L"CPU/DB50.P_YB4",
        L"CPU/DB50.Manual_set",
        //
        L"CPU/DB50.man_zol_v",
        L"CPU/DB50.man_zol_n",
        L"CPU/DB50.P_YB5",
        L"CPU/DB50.P_YB6",
    };
};

}

}
