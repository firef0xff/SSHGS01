#pragma once
#include "db30.h"
#include "db31.h"
#include "db32.h"

#include "db33.h"
#include "db34.h"
#include "db35.h"
#include "db36.h"

#include "db2.h"
#include "db3.h"
#include "db9.h"
#include "db10.h"
#include "db23.h"
#include "db11.h"

#include "db37.h"
#include "db38.h"
#include "db39.h"
#include "db22.h"

#include "db40.h"
#include "db50.h"
#include "db60.h"

#include "m1.h"
#include "i1.h"

namespace cpu
{

class CpuMemory
{
public:
    ~CpuMemory();
    static CpuMemory& Instance();

    void Syncronise();
    void ReadAll();
    void WriteAll();

    data::DB30 DB30;    //параметры для испытания гидроаппарутуры
    data::DB31 DB31;    //диспетчер запуска
    data::DB32 DB32;    //результаты испытаний гидроаппарутуры

    data::DB33 DB33;    //параметры для испытаний серво оборудования с блоком управления
    data::DB34 DB34;    //биты завершения испытаний серво оборудования с блоком управления
    data::DB35 DB35;    //параметры для испытаний серво оборудования с катушками управления
    data::DB36 DB36;    //биты завершения испытаний серво оборудования с катушками управления

    data::DB2   DB2;    //результаты испытания гесметичности пр11
    data::DB3   DB3;    //результаты пр12
    data::DB9   DB9;    //результаты пр13

    data::DB10  DB10;   //результаты пр14.1
    data::DB23  DB23;   //результаты пр14.2

    data::DB11  DB11;

    data::DB37 DB37;    //параметры для испытания контрольных плат
    data::DB38 DB38;    //параметры для испытания гидроцилиндров
    data::DB39 DB39;    //биты завершения испытаний гироцилиндров и плат

    data::DB22 DB22;

    data::DB40 DB40;    //аварийные биты (исключения)
    data::DB50 DB50;    //данные датчиков
    data::DB60 DB60;

    data::M1   M1;      //управляющие биты ручного режима
    data::I1   I1;      //контрольные биты ручного режима

private:
    CpuMemory();
    CpuMemory( CpuMemory const& ) = delete;
    CpuMemory& operator = ( CpuMemory const& ) = delete;
};

}//namespace cpu
