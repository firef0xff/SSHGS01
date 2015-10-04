#include "functional_test.h"
#include <QJsonObject>

namespace test
{

namespace hydro
{

FunctionalTest::FunctionalTest():
    test::hydro::Test( "Испытание функционирования", 1 )
{}

bool FunctionalTest::Run()
{

    return false;
}

QJsonObject FunctionalTest::Serialise() const
{
    QJsonObject obj;
    obj.insert("ReelA", ReelA.Serialise() );
    obj.insert("ReelB", ReelB.Serialise() );

    return obj;
}
bool FunctionalTest::Deserialize( QJsonObject const& obj )
{
    bool res = ReelA.Deserialize( obj.value("ReelA").toObject() );
    res *= ReelB.Deserialize( obj.value("ReelB").toObject() );
    return res;
}

QJsonObject FunctionalTest::ReelResult::Serialise() const
{
    QJsonObject obj;

    obj.insert( "work_on_min_pressure", work_on_min_pressure );
    obj.insert( "work_on_max_pressure", work_on_max_pressure );

    obj.insert( "I", I );
    obj.insert( "U", U );
    obj.insert( "R", R );
    obj.insert( "P", P );

    return obj;
}
bool FunctionalTest::ReelResult::Deserialize( QJsonObject const& obj )
{
    work_on_min_pressure = obj.value( "work_on_min_pressure" ).toBool();
    work_on_max_pressure = obj.value( "work_on_max_pressure" ).toBool();

    I = obj.value( "I" ).toDouble();
    U = obj.value( "U" ).toDouble();
    R = obj.value( "R" ).toDouble();
    P = obj.value( "P" ).toDouble();

    return true;
}

}//namespace hydro

}//namespace test

