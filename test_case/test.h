#pragma once
#include <QString>
#include <QVariant>
#include <functional>

namespace test
{

class TestCase;
class Test
{
public:
    typedef std::function< void( QString const& ) > LogFunction;
    Test( TestCase* test_case, QString const& name, uint8_t number );
    virtual ~Test();
    virtual bool Run( LogFunction, bool const& ) = 0;

    QString const& Name() const;
    uint8_t const& Number() const;

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

Q_DECLARE_METATYPE( test::Test* )
