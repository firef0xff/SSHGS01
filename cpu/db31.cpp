#include "db31.h"

namespace cpu
{
namespace data
{

DB31::DB31():
    Start_Oper( mBoolData[0] ),
    Stop_Oper( mBoolData[1] ),
    Nasos_M2( mBoolData[2] ),
    N_Operation( mFloatData[0] )
{
}

void DB31::Write()
{}

}//namespace data
}//namespace cpu

