#include "m2.h"
#include "miniOPC.h"
#include "memory.h"

namespace cpu
{
namespace data
{
M2::M2():
    Pump_Continue(mBoolData[0]),
    Pump_Fail(mBoolData[1])
{
    memset( mBoolData, 0, sizeof(mBoolData) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"M2", mAdresses, BOOL_COUNT );
}

void M2::Reset()
{
    memset( mBoolData, 0, sizeof(mBoolData) );
    Write();
}
void M2::Write()
{
   HRESULT res = E_FAIL;
   while ( res == E_FAIL )
       res = opc::miniOPC::Instance().WriteMass( mGroupID, 0, BOOL_COUNT, static_cast<void*>( mBoolData ), opc::tBOOL );

}


M3::M3():
    OP49_Continue(mBoolData[0])
{
    memset( mBoolData, 0, sizeof(mBoolData) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"M3", mAdresses, BOOL_COUNT );
}

void M3::Write()
{
   HRESULT res = E_FAIL;
   while ( res == E_FAIL )
       res = opc::miniOPC::Instance().WriteMass( mGroupID, 0, BOOL_COUNT, static_cast<void*>( mBoolData ), opc::tBOOL );

}
void M3::Read()
{
   OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
   if (!rez)
   {
       //ошибка подключения..
       return;
   }
   for (size_t i = 0; i < BOOL_COUNT; i++)
   {
       if ( i < BOOL_COUNT )
           mBoolData[ i ] = rez[i].vDataValue.boolVal;
   }
   opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}
}//namespace data
}//namespace cpu

