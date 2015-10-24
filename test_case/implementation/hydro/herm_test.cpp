#include "herm_test.h"
#include <QJsonObject>
#include <QMessageBox>
#include <QPushButton>
#include <mutex>
#include <functional>
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
    std::mutex mutex;
    std::unique_lock< std::mutex > lock( mutex );
    Launcher( std::bind( &OutsideHermTest::Question, this ) );

    mCondVar.wait( lock );

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

bool OutsideHermTest::Draw( QPainter& painter, QRect &free_rect ) const
{
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
    [ this, &painter ]( QRect const& rect )
    {
        QString s;
        s += LeakFounded ? "Испытание внешней герметичности не пройдено" : "Течь при испытании наружной герметичности не обнаружена";
        painter.drawText( rect, s );
    });

    return res;
}

void OutsideHermTest::Question()
{
    QMessageBox msg;
    msg.setWindowTitle( "Визуальный контроль" );
    msg.setText( "Заметна ли течь по резьбам и стыкам,\nпотение наружных поверхностей гидрораспределителя" );
    QPushButton *no = msg.addButton("Течь не обнаружена", QMessageBox::NoRole );
    QPushButton *yes = msg.addButton("Течь обнаружена", QMessageBox::YesRole );
    msg.setModal( true );
    no->setDefault( false );
    yes->setDefault( false );
    msg.exec();
    LeakFounded = msg.clickedButton() == yes;
    mCondVar.notify_one();
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

bool InsideHermTest::Draw( QPainter& painter, QRect &free_rect ) const
{
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
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Средний расчет расход утечки за " + QString::number( Seconds ) + " сек, " + QString::number( Leak ) + " л/мин";
        painter.drawText( rect, s );
    });

    return res;
}

}//namespace hydro

}//namespace test

