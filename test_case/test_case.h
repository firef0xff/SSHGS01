#pragma once
#include <QMap>

namespace test
{
class Test;
class TestCase
{
public:
    friend class Test;
    typedef QMap< int, Test* > DataType;

    TestCase( const QString &name, QString const& descr = QString() );
    ~TestCase();
    DataType const& Tests() const;
    QString const& Name() const;
    QString const& Descr() const;

private:

    TestCase( TestCase const& );
    void operator = ( TestCase const& );

    static TestCase& Instance();

    void Subscribe( Test* );
    void Unsubscribe( Test* );

    DataType mData;
    QString mName;
    QString mDescr;
};

}//namespace test
