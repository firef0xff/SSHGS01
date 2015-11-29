#include <iostream>
#include "miniOPC.h"

class Test
{
public:
    Test()
    {
        std::cout << "connecting to opc" << std::endl;
        opc::miniOPC& server = opc::miniOPC::Instance();
        std::cout << "connecting done" << std::endl;

        std::cout << "add group" << std::endl;
        mGroupID = server.AddGroup( L"DB31", mAdresses, BOOL_COUNT + FLOAT_COUNT );
        std::cout << "add group done" << std::endl;
    }

    void Run()
    {
        opc::miniOPC& server = opc::miniOPC::Instance();
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

        server.WriteValue( mGroupID, 0, static_cast<void*>( &t ), opc::tBOOL );
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

        int i = 0;
        while ( i >= 0 )
        {
            //++i;
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

            bool mWriteBoolDataZero[ BOOL_COUNT ] = {false, false, false};
            float mWriteFloatDataZero[ FLOAT_COUNT ] = { 0.0 };
            server.WriteMass( mGroupID, 0, BOOL_COUNT, static_cast<void*>( mWriteBoolDataZero ), opc::tBOOL );
            server.WriteMass( mGroupID, BOOL_COUNT, FLOAT_COUNT, static_cast<void*>( mWriteFloatDataZero ), opc::tFLOAT );

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

            //std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    enum
    {
        BOOL_COUNT = 3,
        FLOAT_COUNT = 1
    };

    wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
        //bool
        L"CPU/DB31.Start_Oper",
        L"CPU/DB31.Stop_Oper",
        L"CPU/DB31.Nasos_M2",

        L"CPU/DB31.N_Operation"
    };

    uint64_t mGroupID = 0;


    bool mWriteBoolData[ BOOL_COUNT ] = {false, true, true};
    float mWriteFloatData[ FLOAT_COUNT ] = { 100.0 };
    bool t = true;

    bool mBoolData[ BOOL_COUNT ] = {false};
    float mFloatData[ FLOAT_COUNT ] = { 0.0 };
};

int main()
{
    Test t;
    new std::thread( &Test::Run, &t );

    std::this_thread::sleep_for(std::chrono::seconds(3600));

    return 0;
}

