#pragma once

#include "../test_base.h"



namespace shema
{

class State
{
public:
    State( bool p, bool a, bool b ):
        A(a),B(b),P(p),T(true)
    {}
    bool IsOpen_A()
    {
        return A;
    }
    bool IsOpen_B()
    {
        return B;
    }
    bool IsOpen_P()
    {
        return P;
    }
    bool IsOpen_T()
    {
        return T;
    }
private:
    bool A;
    bool B;
    bool P;
    bool T;
};

}

namespace test
{

namespace hydro
{

class FunctionalTest :public test::hydro::Test
{
public:
    FunctionalTest();
    bool Run();

private:

    bool SetSchema( shema::State const& state );
    bool SetPressure( double pressure );
    bool SetVoltage( double voltage );
    bool WaitPressure( uint sec );
    bool CalckParams( );

};

}//namespace hydro

}//namespace test
