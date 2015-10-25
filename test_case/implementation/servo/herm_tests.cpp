#include "herm_tests.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QPushButton>
#include "../test_params_servo.h"

#include "../../../../mylib/Widgets/GraphBuilder/graph_builder.h"
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
        QString s = "Наружная течь (не)замечена";
        s += LeakFounded ? "" : "не";
        s += "замечена";
        painter.drawText( rect, s );
    });

    return res;
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
    Data d;
    d.Leak = 1;
    d.Signal = 0.25;

    Graph.push_back( d );
    d.Leak += 0.1;
    d.Signal += 0.01;

    Graph.push_back( d );
    d.Leak += 0.1;
    d.Signal += 0.01;

    Graph.push_back( d );
    d.Leak += 0.1;
    d.Signal += 0.01;

    Graph.push_back( d );
    d.Leak += 0.1;
    d.Signal += 0.01;

    Graph.push_back( d );
    d.Leak += 0.1;
    d.Signal += 0.01;

    Graph.push_back( d );
    d.Leak += 0.1;
    d.Signal += 0.01;

    Graph.push_back( d );
    d.Leak += 0.1;
    d.Signal += 0.01;

    Graph.push_back( d );
    d.Leak += 0.1;
    d.Signal += 0.01;

    Graph.push_back( d );
    d.Leak += 0.1;
    d.Signal += 0.01;

    Graph.push_back( d );
    d.Leak += 0.1;
    d.Signal += 0.01;

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

bool InsideHermTest::Draw( QPainter& painter, QRect &free_rect ) const
{
    test::servo::Parameters *params = static_cast< test::servo::Parameters * >( CURRENT_PARAMS );
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
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Графиг опорного сигнала и расхода в канале утечки ( Т )";
        painter.drawText( rect, s );
    });

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &text_font, params ]( QRect const& rect )
    {
        ff0x::GraphBuilder::LinePoints data;

        double max_signal = 0;
        double max_Leak = 0;

        foreach ( Data const& item, Graph )
        {
            double abs_sig = std::abs( item.Signal );
            double abs_leak = std::abs( item.Leak );

            if ( max_signal < abs_sig )
                max_signal = abs_sig;

            if ( max_Leak < abs_leak )
                max_Leak = abs_leak;

            data.push_back( QPointF( item.Signal, item.Leak ) );
        }

        ff0x::GraphBuilder builder ( 1024, 768, text_font );
        ff0x::GraphBuilder::GraphDataLine lines;
        lines.push_back( ff0x::GraphBuilder::Line( data, Qt::blue ) );
        painter.drawPixmap( rect, builder.Draw( lines, max_signal * 1.25, max_Leak * 1.25, 0.05, 0.5, "л/мин", params->ControlSignal() == ST_10_10_V ? "В" : "мА", true ) );
    }, free_rect.width()/4*3  );

    free_rect.setHeight( 0 );

    return res;
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

