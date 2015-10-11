#include "cylinder_test.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QPushButton>
#include "../test_params_hydro_cilinder.h"
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

bool FunctionalTest::Draw( QPainter& painter, QRect &free_rect ) const
{
    test::hydro_cylinder::Parameters *params = static_cast< test::hydro_cylinder::Parameters * >( CURRENT_PARAMS );
    if ( !params )
        return true;

    QFont header_font = painter.font();
    QFont text_font = painter.font();
    header_font.setPointSize( 14 );
    text_font.setPointSize( 12 );

    QFontMetrics head_metrix( header_font );

    uint32_t num = 0;

    bool res = DrawLine( num, free_rect, header_font,
    [ this, &painter, &head_metrix, &header_font ]( QRect const& rect )
    {
        QPoint start_point( rect.center().x() - head_metrix.width( mName ) / 2, rect.bottom() );
        painter.setFont( header_font );
        painter.drawText( start_point, mName );
    });

    painter.setFont( text_font );
    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, params ]( QRect const& rect )
    {
        QString s = "Серийный номер: " + params->SerNo();
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, params ]( QRect const& rect )
    {
        QString s = "Максимальное давление нагрузки гидроцилиндра, Бар: " + test::ToString( params->MaxPressure() );
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, params ]( QRect const& rect )
    {
        QString s = "Номинальный расход, л/мин: " + test::ToString( params->Expenditure() );
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, params ]( QRect const& rect )
    {
        QString s = "Давление при испытании функционирования, Бар: " + test::ToString( params->TestPressure() );
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, params ]( QRect const& rect )
    {
        QString s = "Время перемещения в каждую сторону при испытании функционирования, с: " + test::ToString( params->MoveTime() );
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, params ]( QRect const& rect )
    {
        QString s = "Время испытания наружной герметичности, с: " + test::ToString( params->HermTestTime() );
        painter.drawText( rect, s );
    });

    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Гидроцилиндр ";
        s += Result ? "" : "не";
        s += "пригоден для дальнейшего использования";
        painter.drawText( rect, s );
    });

    return res;
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
