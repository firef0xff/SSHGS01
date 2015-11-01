#include <iostream>
#include "miniOPC.h"

int main()
{
    enum
    {
        BOOL_COUNT = 3,
        FLOAT_COUNT = 1
    };

    bool mWriteBoolData[ BOOL_COUNT ] = {false, true, true};
    float mWriteFloatData[ FLOAT_COUNT ] = { 100.0 };
    bool t = true;

    bool mBoolData[ BOOL_COUNT ] = {false};
    float mFloatData[ FLOAT_COUNT ] = { 0.0 };
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
        //bool
        L"S7:[S7 connection_4]DB31,X2.0",   //Start_Oper
        L"S7:[S7 connection_4]DB31,X2.1",   //Stop_Oper
        L"S7:[S7 connection_4]DB31,X2.2",   //Nasos_M2

        L"S7:[S7 connection_4]DB31,REAL4",  //N_Operation
    };

    opc::miniOPC& server = opc::miniOPC::Instance();

    mGroupID = server.AddGroup( L"DB31", mAdresses, BOOL_COUNT + FLOAT_COUNT );

    server.WriteValue( mGroupID, 0, static_cast<void*>( &t ), opc::tBOOL );
    auto res = server.Read( mGroupID );
    if ( res )
    {
        for (size_t i = 0; i < BOOL_COUNT + FLOAT_COUNT; i++)
        {
            if ( i < BOOL_COUNT )
                mBoolData[ i ] = res[i].vDataValue.boolVal;
            else
                mFloatData[ i - BOOL_COUNT ] = res[i].vDataValue.fltVal;
        }
        server.OpcMassFree( mGroupID, res );
    }

    for ( size_t i = 0; i < BOOL_COUNT; ++i )
    {
        std::cout << i << " = " << mBoolData[i] << std::endl;
    }
    for ( size_t i = 0; i < FLOAT_COUNT; ++i )
    {
        std::cout << BOOL_COUNT + i << " = " << mFloatData[i] << std::endl;
    }

    server.WriteMass( mGroupID, 0, BOOL_COUNT, static_cast<void*>( mWriteBoolData ), opc::tBOOL );
    server.WriteMass( mGroupID, BOOL_COUNT, FLOAT_COUNT, static_cast<void*>( mWriteFloatData ), opc::tFLOAT );

    res = server.Read( mGroupID );
    if ( res )
    {
        for (size_t i = 0; i < BOOL_COUNT + FLOAT_COUNT; i++)
        {
            if ( i < BOOL_COUNT )
                mBoolData[ i ] = res[i].vDataValue.boolVal;
            else
                mFloatData[ i - BOOL_COUNT ] = res[i].vDataValue.fltVal;
        }
        server.OpcMassFree( mGroupID, res );
    }
    for ( size_t i = 0; i < BOOL_COUNT; ++i )
    {
        std::cout << i << " = " << mBoolData[i] << std::endl;
    }
    for ( size_t i = 0; i < FLOAT_COUNT; ++i )
    {
        std::cout << BOOL_COUNT + i << " = " << mFloatData[i] << std::endl;
    }

    return 0;
}

