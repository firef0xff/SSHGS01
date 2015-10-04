#include "cylinder_test.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QPushButton>

namespace test
{
namespace hydro_cylinder
{

FunctionalTest::FunctionalTest():
    test::hydro_cylinder::Test( "Испытание функционирования", 17 ),
    Result(false)
{}

bool FunctionalTest::Run()
{
    Question();
    return Result;
}

QJsonObject FunctionalTest::Serialise() const
{
    QJsonObject obj;
    obj.insert("Result", Result );

    return obj;
}
bool FunctionalTest::Deserialize( QJsonObject const& obj )
{
    Result = obj.value("Result").toBool();
    return true;
}

void FunctionalTest::Question()
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

}//namespace hydro_cylinder

}//namespace test
