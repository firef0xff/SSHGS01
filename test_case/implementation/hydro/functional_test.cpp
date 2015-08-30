#include "functional_test.h"

namespace test
{

namespace hydro
{

FunctionalTest::FunctionalTest():
    test::hydro::Test( "Испытание функционирования" )
{}

bool FunctionalTest::Run()
{
   /* bool k1,k2 = true;
    if ( k1 || k2 )
    {
        //1 привести к виду
        SetSchema( shema::State( false, false, false ) );

        //2 создать минимально допустимое давление на датчике м3
        SetPressure( 1000 );
        if ( k1 )
        {
            //3 привести к виду
            SetSchema( shema::State( true, false, false ) );

            //4 подача требуемого напряжения на катушку
            SetVoltage( 200 );
        }
        if ( k2 )
        {
            //3 привести к виду
            SetSchema( shema::State( true, false, false ) );

            //4 подача требуемого напряжения на катушку
            SetVoltage( 200 );
        }
        //5 ожидание динамики 3 сек
        WaitPressure( 3 );

        //6 снять напряжение с катушки
        SetVoltage( 200 );

        //7 привести к виду
        SetSchema( shema::State( false, false, false ) );

        if ( k1 )
        {
            //8 создать максимально допустимое давление на датчике м3
            SetPressure( 1000 );

            //9 привести к виду
            SetSchema( shema::State( true, false, false ) );

            //10 подача требуемого напряжения на катушку
            SetVoltage( 200 );

            //11 произвести замер I U катушки (расчитать параметры)
            CalckParams();

            //12 ожидание динамики 3 сек
            WaitPressure( 3 );
        }
        if ( k2 )
        {
            //8 создать максимально допустимое давление на датчике м3
            SetPressure( 1000 );

            //9 привести к виду
            SetSchema( shema::State( true, false, false ) );

            //10 подача требуемого напряжения на катушку
            SetVoltage( 200 );

            //11 произвести замер I U катушки (расчитать параметры)
            CalckParams();

            //12 ожидание динамики 3 сек
            WaitPressure( 3 );
        }
    }*/
    return false;
}

bool SetSchema( shema::State const& state );
bool SetPressure( double pressure );
bool SetVoltage( double voltage );
bool WaitPressure( uint sec );
bool CalckParams( );

}//namespace hydro

}//namespace test

