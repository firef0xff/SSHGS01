#pragma once
#include "db30.h"
#include "db31.h"
#include "db32.h"

#include "db33.h"
#include "db34.h"
#include "db35.h"
#include "db36.h"

#include "db2.h"
#include "db12.h"
#include "db3.h"
#include "db22.h"
#include "db9.h"
#include "db19.h"

#include "db37.h"
#include "db38.h"
#include "db39.h"
#include "db40.h"

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
    data::DB12  DB12;   //результаты испытания гесметичности пр21
    data::DB3   DB3;    //результаты пр12
    data::DB22  DB22;   //результаты пр22
    data::DB9   DB9;    //результаты пр13
    data::DB19  DB19;   //результаты пр23


    data::DB37 DB37;    //параметры для испытания контрольных плат
    data::DB38 DB38;    //параметры для испытания гидроцилиндров
    data::DB39 DB39;    //биты завершения испытаний гироцилиндров и плат
    data::DB40 DB40;    //аварийные биты (исключения)

private:
    CpuMemory();
    CpuMemory( CpuMemory const& ) = delete;
    CpuMemory& operator = ( CpuMemory const& ) = delete;
};

}//namespace cpu
