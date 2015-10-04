#include "herm_tests.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QPushButton>
namespace test
{
namespace servo
{

OutsideHermTest::OutsideHermTest():
    test::servo::Test( "Проверка аппарата пробным давлением", 9 ),
    LeakFounded(false)
{}

bool OutsideHermTest::Run()
{
    Question();
    return !LeakFounded;
}

QJsonObject OutsideHermTest::Serialise() const
{
    QJsonObject obj;
    obj.insert("LeakFounded", LeakFounded );

    return obj;
}
bool OutsideHermTest::Deserialize( QJsonObject const& obj )
{
    LeakFounded = obj.value("LeakFounded").toBool();
    return true;
}

void OutsideHermTest::Question()
{
#warning найти как обойти падение
    /*QMessageBox msg;
    msg.setWindowTitle( "Визуальный контроль" );
    msg.setText( "Заметна ли течь по резьбам и стыкам,\nпотение наружных поверхностей гидрораспределителя" );
    QPushButton *no = msg.addButton("Течь не обнаружена", QMessageBox::NoRole );
    QPushButton *yes = msg.addButton("Течь обнаружена", QMessageBox::YesRole );
    msg.setModal( true );
    no->setDefault( false );
    yes->setDefault( false );
    msg.exec();
    LeakFounded = msg.clickedButton() == yes;*/
}


//----------------------------------------------
InsideHermTest::InsideHermTest():
    test::servo::Test( "Внутренняя утечка", 10 )
{}

bool InsideHermTest::Run()
{
    return true;
}
QJsonObject InsideHermTest::Serialise() const
{
    QJsonObject obj;
    QJsonArray a;
    foreach (Data const& d, Graph)
    {
        a.insert( a.end(), d.Serialise() );
    }
    obj.insert("Graph", a );

    return obj;
}
bool InsideHermTest::Deserialize( QJsonObject const& obj )
{
    QJsonArray a = obj.value("Graph").toArray();
    foreach (QJsonValue const& v, a)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            Graph.insert( Graph.end(), d );
    }
    return true;
}

QJsonObject InsideHermTest::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("Signal", Signal );
    obj.insert("Leak", Leak );

    return obj;
}
bool InsideHermTest::Data::Deserialize( QJsonObject const& obj )
{
    Signal = obj.value("Signal").toDouble();
    Leak = obj.value("Leak").toDouble();
    return true;
}

}//namespace servo

}//namespace test

