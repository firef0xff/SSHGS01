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

}//namespace cpu
