#include "herm_tests.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QPushButton>
#include <mutex>
#include <functional>
#include "../test_params_servo.h"

#include "../../../../mylib/Widgets/GraphBuilder/graph_builder.h"
namespace test
{
namespace servo
{

OutsideHermTest::OutsideHermTest():
    test::servo::Test( "Проверка аппарата пробным давлением", 10, 20 ),
    LeakFounded(false)
{}

bool OutsideHermTest::Run()
{
    Start();
    if ( ReelControl() )
        Wait( mControlReelBits.op20_ok, mControlReelBits.op20_end );
    else
        Wait( mControlBoardBits.op10_ok, mControlBoardBits.op10_end );
    if ( IsStopped() )
        return false;
    Question();

    OilTemp = mTemperature.T_oil;

    std::mutex mutex;
    std::unique_lock< std::mutex > lock( mutex );
    Launcher( std::bind( &OutsideHermTest::Question, this ) );

    mCondVar.wait( lock );

    return Success();
}
bool OutsideHermTest::Success() const
{
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
    header_font.setFamily("Arial");
    header_font.setPointSize( 14 );
    QFont result_font = header_font;
    result_font.setUnderline(true);
    QFont text_font = header_font;
    text_font.setPointSize( 12 );


    auto DrawRowCenter = [ &painter, &free_rect ](QRect const& place, QFont const& font, QColor const& color, QString const& text )
    {
        painter.save();
        QFontMetrics metrix( font );
        QPoint start_point( place.center().x() - metrix.width( text ) / 2, place.center().y() +metrix.height()/2);
        painter.setFont( font );
        painter.setPen( color );
        painter.drawText( start_point, text );
        painter.restore();
    };
    auto DrawRowLeft = [ &painter, &free_rect ](    QRect const& place,
                                                    QFont const& font,
                                                    QColor const& color1,
                                                    QString const& label,
                                                    QColor const& color2 = Qt::black,
                                                    QString const& value = "")
    {
        painter.save();
        QFontMetrics metrix( font );
        QPoint start_point( place.left() , place.center().y()+metrix.height()/2 );
        QPoint start_point2( place.left() + metrix.width(label), place.center().y() +metrix.height()/2);
        painter.setFont( font );
        painter.setPen( color1 );
        painter.drawText( start_point, label );
        painter.setPen( color2 );
        painter.drawText( start_point2, value );
        painter.restore();
    };

    QFontMetrics m(text_font);
    int width = m.width("123456789012345678901234567890123456789012345");
    char symbol = '.';
    auto FillToSize = [ width, &m, symbol ]( QString text )
    {
        while( m.width( text + symbol ) < width )
            text += symbol;
        return text + " ";
    };


    uint32_t num = 0;
    bool res = DrawLine( num, free_rect, header_font,
    [ this, &painter, &DrawRowCenter, &header_font ]( QRect const& rect )
    {
        DrawRowCenter( rect, header_font, Qt::black, "1."+mName );
    }, 1.5 );

    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
        DrawRowLeft( r, text_font, Qt::black, "Испытание давлением проводится для определения герметичности и" );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "прочности испытываемого аппарата перед проведением последующих испытаний." );
    }, 1.5 );


    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Давление при проведении испытаний, бар"), Qt::red, "что писать?" );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Температура масла во время испытаний, ˚С"), Qt::red, test::ToString(OilTemp) );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Длительность испытания, сек"), Qt::red, test::ToString( TestingTime ) );
    }, 2 );


    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &result_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, result_font, Qt::black, "РЕЗУЛЬТАТ:" );
    }, 1.5 );

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "Наружная течь при испытании ", Qt::red, Success()? "не обнаружена" : "обнаружена" );
    }, 3 );
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
    test::servo::Test( "Проверка внутренней герметичности", 11, 21 )
{}

bool InsideHermTest::Run()
{
    Start();
    if ( ReelControl() )
        Wait( mControlReelBits.op21_ok, mControlReelBits.op21_end );
    else
        Wait( mControlBoardBits.op11_ok, mControlBoardBits.op11_end );
    if ( IsStopped() )
        return false;

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
    header_font.setFamily("Arial");
    header_font.setPointSize( 14 );
    QFont result_font = header_font;
    result_font.setUnderline(true);
    QFont text_font = header_font;
    text_font.setPointSize( 12 );


    auto DrawRowCenter = [ &painter, &free_rect ](QRect const& place, QFont const& font, QColor const& color, QString const& text )
    {
        painter.save();
        QFontMetrics metrix( font );
        QPoint start_point( place.center().x() - metrix.width( text ) / 2, place.center().y() +metrix.height()/2);
        painter.setFont( font );
        painter.setPen( color );
        painter.drawText( start_point, text );
        painter.restore();
    };
    auto DrawRowLeft = [ &painter, &free_rect ](    QRect const& place,
                                                    QFont const& font,
                                                    QColor const& color1,
                                                    QString const& label,
                                                    QColor const& color2 = Qt::black,
                                                    QString const& value = "")
    {
        painter.save();
        QFontMetrics metrix( font );
        QPoint start_point( place.left() , place.center().y()+metrix.height()/2 );
        QPoint start_point2( place.left() + metrix.width(label), place.center().y() +metrix.height()/2);
        painter.setFont( font );
        painter.setPen( color1 );
        painter.drawText( start_point, label );
        painter.setPen( color2 );
        painter.drawText( start_point2, value );
        painter.restore();
    };

    QFontMetrics m(text_font);
    int width = m.width("123456789012345678901234567890123456789012345");
    char symbol = '.';
    auto FillToSize = [ width, &m, symbol ]( QString text )
    {
        while( m.width( text + symbol ) < width )
            text += symbol;
        return text + " ";
    };


    uint32_t num = 0;
    bool res = DrawLine( num, free_rect, header_font,
    [ this, &painter, &DrawRowCenter, &header_font ]( QRect const& rect )
    {
        DrawRowCenter( rect, header_font, Qt::black, "2."+mName );
    }, 1.5 );

    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
        DrawRowLeft( r, text_font, Qt::black, "Данный метод испытания направлен на выявление зависимости внутренних" );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "утечек от управляющего воздействия." );
    }, 1.5 );


    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Давление при проведении испытаний, бар"), Qt::red, "что писать?" );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Температура масла во время испытаний, ˚С"), Qt::red, test::ToString(OilTemp) );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Длительность испытания, сек"), Qt::red, test::ToString( TestingTime ) );
    }, 2 );


    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &result_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, result_font, Qt::black, "РЕЗУЛЬТАТ:" );
    }, 3 );


    QFontMetrics metrix( text_font );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &text_font, &DrawRowCenter, &metrix ]( QRect const& rect )
    {
        painter.save();

        ff0x::GraphBuilder::LinePoints data;
        ff0x::GraphBuilder::LinePoints data_e;

        double max_signal = 0;
        double max_Leak = 0;

        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromEtalone().value( test::CURRENT_PARAMS->ModelId()).toObject().value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                QJsonArray a = obj.value("data").toObject().value("Graph").toArray();
                foreach ( QJsonValue const& v, a )
                {
                    QJsonObject o = v.toObject();
                    data_e.push_back( QPointF( o.value("Signal").toDouble(), o.value("Leak").toDouble() ) );
                }
            }
        }

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
        QFont f = text_font;
        f.setPointSize( 6 );
        int w = (rect.height() - metrix.height())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::GraphBuilder builder ( w, h, ff0x::GraphBuilder::PlusPlus, f );
        ff0x::GraphBuilder::GraphDataLine lines;
        lines.push_back( ff0x::GraphBuilder::Line(data, ff0x::GraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !data_e.empty() )
            lines.push_back( ff0x::GraphBuilder::Line(data_e, ff0x::GraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );

        QRect p1(rect.left(), rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
        DrawRowCenter( p1t, text_font, Qt::black, "График утечек" );
        painter.drawPixmap( p1, builder.Draw( lines, max_signal * 1.25, max_Leak * 1.25, 0.05, 0.5, "Опорный сигнал", "Расход (л/мин)", true ) );

        painter.restore();
    }, 1, free_rect.width()/2 + metrix.height()  );

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

