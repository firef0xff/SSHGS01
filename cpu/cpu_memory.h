#pragma once

namespace cpu
{

class CpuMemory
{
public:
    ~CpuMemory();
    static CpuMemory& Instance();

    void Syncronise();



private:
    CpuMemory();
    CpuMemory( CpuMemory const& ) = delete;
    CpuMemory& operator = ( CpuMemory const& ) = delete;
};

}//namespace cpu
