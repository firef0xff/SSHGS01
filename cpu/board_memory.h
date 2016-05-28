#pragma once
#include "data_block_base.h"
#include <memory>
#include "board/custom_control_board.h"


namespace cpu
{
class CpuMemory;
namespace data
{

class BoardMem
{
public:
    void Start();
    void Stop();

    bool IsShim = true;
    int mOutputChannels = 0;

    int mMaxAmperage = 0;
    int mMinAmperage = 0;
    int mVSigAmpl = 0;
    int mVSigFreq = 0;

private:
    friend class cpu::CpuMemory;
    BoardMem();
    BoardMem( const BoardMem& ) = delete;
    void operator = ( const BoardMem& ) = delete;

    typedef ::control_board::CustomControlBoard CustomBoard;
    std::unique_ptr<CustomBoard> mBoard;
};

}

}
