#pragma once
#include <QMap>
#include "../devices/device_collection.h"

namespace test
{
class Test;
class TestCase
{
public:
    friend class Test;
    typedef QMap< int, Test* > DataType;

    TestCase( const QString &name, QString const& devices, QString const& descr = QString() );
    ~TestCase();
    DataType const& Tests() const;
    QString const& Name() const;
    QString const& Descr() const;
    examinee::DeviceCollection& Devices();
private:

    TestCase( TestCase const& );
    void operator = ( TestCase const& );

    static TestCase& Instance();

    void Register( Test* );
    void UnRegister( Test* );

    DataType mData;
    QString mName;
    QString mDescr;

    examinee::DeviceCollection mCollection;
};

}//namespace test
