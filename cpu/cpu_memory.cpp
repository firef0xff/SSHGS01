#include "cpu_memory.h"
#include <memory>
#include <mutex>

namespace cpu
{

namespace
{
static std::unique_ptr< CpuMemory > CPU_MEMORY;
static std::mutex CPU_MEMORY_MUTEX;
}//namespace

CpuMemory::CpuMemory()
{}
CpuMemory::~CpuMemory()
{}

CpuMemory& CpuMemory::Instance()
{
    if ( !CPU_MEMORY )
    {
        std::lock_guard< std::mutex > lock( CPU_MEMORY_MUTEX );
        if ( !CPU_MEMORY )
        {
            CPU_MEMORY.reset( new CpuMemory() );
        }
    }
    return *CPU_MEMORY;
}

void CpuMemory::Syncronise()
{
    ReadAll();
    WriteAll();
}

void CpuMemory::ReadAll()
{
    DB32.Read();
    DB34.Read();
    DB36.Read();

    DB2.Read();
    DB12.Read();

    DB3.Read();
    DB13.Read();

    DB9.Read();
    DB19.Read();

    DB10.Read();
    DB23.Read();
    DB20.Read();
    DB24.Read();

    DB11.Read();
    DB21.Read();

    DB39.Read();
    DB22.Read();

    DB40.Read();
}
void CpuMemory::WriteAll()
{
    DB30.Write();
    DB31.Write();

    DB33.Write();
    DB35.Write();
    DB37.Write();
    DB38.Write();
}

}//namespace cpu
