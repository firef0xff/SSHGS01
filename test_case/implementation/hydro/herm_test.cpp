#include "herm_test.h"
#include <QJsonObject>
#include <QMessageBox>
#include <QPushButton>
namespace test
{
namespace hydro
{

OutsideHermTest::OutsideHermTest():
    test::hydro::Test( "Проверка наружной герметичности", 2 ),
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
    test::hydro::Test( "Проверка внутренней герметичности", 3 )
{}

bool InsideHermTest::Run()
{
    return true;
}
QJsonObject InsideHermTest::Serialise() const
{
    QJsonObject obj;
    obj.insert("Seconds", Seconds );
    obj.insert("Leak", Leak );

    return obj;
}
bool InsideHermTest::Deserialize( QJsonObject const& obj )
{
    Seconds = obj.value("Seconds").toInt();
    Leak = obj.value("Leak").toDouble();
    return true;
}

}//namespace hydro

}//namespace test

