#pragma once

#include "../../test.h"

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
namespace hydro
{

class FunctionalTest :public test::Test
{
public:
    FunctionalTest( TestCase* test_case, QString const& name, uint8_t number );
    bool Run();

private:

    bool SetSchema( shema::State const& state );
    bool SetPressure( double pressure );
    bool SetVoltage( double voltage );
    bool WaitPressure( uint sec );
    bool CalckParams( );

};

}

#endif // FUNCTIONALTEST_H
