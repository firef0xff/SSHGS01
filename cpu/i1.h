#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class I1 :public In
{
public:
    void Read();

    bool& SQ1;     //IX0.0    MXкран насоса М1
    bool& SQ3;     //IX0.2    MXкран насоса М2
    bool& SQ5;     //IX0.4    MXкран насоса М3.2
    bool& SQ7;     //IX0.6    MXкран насоса М3.1
    bool& SQ9;     //IX1.0    MXкран насоса М4
    bool& SQ11;    //IX1.2    MXкран насоса М5
    bool& SQ13;    //IX1.4    MXкран дренажа М1
    bool& SQ14;    //IX1.5    MXкран дренажа М2
    bool& SQ15;    //IX1.6    MXкран дренажа М4
    bool& SP1;     //IX2.0    MXфильтр всасывающий М1
    bool& SP2;     //IX2.1    MXфильтр всасывающий М2
    bool& SP3;     //IX2.2    MXфильтр всасывающий М3.2
    bool& SP4;     //IX2.3    MXфильтр всасывающий М3.1
    bool& SP5;     //IX2.4    MXфильтр всасывающий М4
    bool& SP6;     //IX2.5    MXфильтр всасывающий М5
    bool& SP7;     //IX2.6    MXфильтр тонкой очистки М1
    bool& SP8;     //IX2.7    MXфильтр грубой очистки М1
    bool& SP9;     //IX3.0    MXфильтр тонкой очистки М2
    bool& SP10;    //IX3.1    MXфильтр грубой очистки М2
    bool& SP11;    //IX3.2    MXфильтр тонкой очистки М4
    bool& SP12;    //IX3.3    MXфильтр тонкой очистки М3.2
    bool& SP13;    //IX3.4    MXфильтр тонкой очистки М3.1
    bool& SP14;    //IX3.5    MXфильтр контура охлаждения
    bool& SP15;    //IX3.6    MXфильтр тонкой очистки М12
    bool& SL1;     //IX4.0    MXуровень масла - норма
    bool& SL2;     //IX4.1    MXуровень масла -  предупреждение
    bool& SL3;     //IX4.2    MXуровень масла - авария
    bool& SL4;     //IX4.3    MXуровень масла верхний (поддон)
    bool& SL5;     //IX4.4    MXуровень масла нижний (поддон)
    bool& Error;   //MX44.6    MXне правильная комбинация вкл клапанов


private:
    friend class cpu::CpuMemory;
    I1();
    I1( const I1& ) = delete;
    void operator = ( const I1& ) = delete;

    enum
    {
        BOOL_COUNT = 30
    };

    bool mBoolData[ BOOL_COUNT ];
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT ] =
    {
        L"CPU/I1.SQ1",    //IX0.0    MXкран насоса М1
        L"CPU/I1.SQ3",    //IX0.2    MXкран насоса М2
        L"CPU/I1.SQ5",    //IX0.4    MXкран насоса М3.2
        L"CPU/I1.SQ7",    //IX0.6    MXкран насоса М3.1
        L"CPU/I1.SQ9",    //IX1.0    MXкран насоса М4
        L"CPU/I1.SQ11",    //IX1.2    MXкран насоса М5
        L"CPU/I1.SQ13",    //IX1.4    MXкран дренажа М1
        L"CPU/I1.SQ14",    //IX1.5    MXкран дренажа М2
        L"CPU/I1.SQ15",    //IX1.6    MXкран дренажа М4
        L"CPU/I1.SP1",    //IX2.0    MXфильтр всасывающий М1
        L"CPU/I1.SP2",    //IX2.1    MXфильтр всасывающий М2
        L"CPU/I1.SP3",    //IX2.2    MXфильтр всасывающий М3.2
        L"CPU/I1.SP4",    //IX2.3    MXфильтр всасывающий М3.1
        L"CPU/I1.SP5",    //IX2.4    MXфильтр всасывающий М4
        L"CPU/I1.SP6",    //IX2.5    MXфильтр всасывающий М5
        L"CPU/I1.SP7",    //IX2.6    MXфильтр тонкой очистки М1
        L"CPU/I1.SP8",    //IX2.7    MXфильтр грубой очистки М1
        L"CPU/I1.SP9",    //IX3.0    MXфильтр тонкой очистки М2
        L"CPU/I1.SP10",    //IX3.1    MXфильтр грубой очистки М2
        L"CPU/I1.SP11",    //IX3.2    MXфильтр тонкой очистки М4
        L"CPU/I1.SP12",    //IX3.3    MXфильтр тонкой очистки М3.2
        L"CPU/I1.SP13",    //IX3.4    MXфильтр тонкой очистки М3.1
        L"CPU/I1.SP14",    //IX3.5    MXфильтр контура охлаждения
        L"CPU/I1.SP15",    //IX3.6    MXфильтр тонкой очистки М12
        L"CPU/I1.SL1",    //IX4.0    MXуровень масла - норма
        L"CPU/I1.SL2",    //IX4.1    MXуровень масла -  предупреждение
        L"CPU/I1.SL3",    //IX4.2    MXуровень масла - авария
        L"CPU/I1.SL4",    //IX4.3    MXуровень масла верхний (поддон)
        L"CPU/I1.SL5",    //IX4.4    MXуровень масла нижний (поддон)
        L"CPU/I1.Error"    //MX44.6    MXне правильная комбинация вкл клапанов
    };

};

}//namespace data
}//namespace cpu
