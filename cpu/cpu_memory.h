#pragma once
#include "db30.h"
#include "db31.h"
#include "db32.h"

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

    data::DB30 DB30;
    data::DB31 DB31;
    data::DB32 DB32;

private:
    CpuMemory();
    CpuMemory( CpuMemory const& ) = delete;
    CpuMemory& operator = ( CpuMemory const& ) = delete;
};

}//namespace cpu
