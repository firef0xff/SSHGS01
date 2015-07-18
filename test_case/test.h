#pragma once
#include <QString>

namespace test
{

class TestCase;
class Test
{
public:
    Test( TestCase* test_case, QString const& name, uint8_t number );
    virtual ~Test();
    virtual bool Run() = 0;

    QString const& Name();
    uint8_t const& Number();

private:
    friend class TestCase;
    Test( Test const& );
    void operator = ( Test const& );

    void Free();

    QString mName;
    uint8_t mNumber;
    TestCase *mCase;
};

}//namespace test
