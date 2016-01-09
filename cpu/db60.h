#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB60 :public In
{
public:
    ~DB60(){}
    void Read();

    bool& OP15_25_Opor_1;
    bool& OP15_25_Opor_2;
    bool& OP15_25_Opor_3;    
    int& OP15_25_count;       //DB60,INT60

private:
    friend class cpu::CpuMemory;
    DB60();
    DB60( const DB60& ) = delete;
    void operator = ( const DB60& ) = delete;

    enum
    {
        BOOL_COUNT = 3,
        INT_COUNT = 1
    };

    bool mBoolData[ BOOL_COUNT ];
    int mIntData[ INT_COUNT ];
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT+INT_COUNT ] = {
        //bool
        L"CPU/DB60.OP15_25_Opor_1",
        L"CPU/DB60.OP15_25_Opor_2",
        L"CPU/DB60.OP15_25_Opor_3",
        //int
        L"CPU/DB60.OP15_25_count"
    };
};

}
}
