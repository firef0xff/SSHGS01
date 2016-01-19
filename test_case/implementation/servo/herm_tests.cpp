#include "herm_tests.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QPushButton>
#include <mutex>
#include <functional>
#include "../test_params_servo.h"
#include "thread"

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

    OilTemp = round( mTemperature.T_oil *100)/100;

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
    QJsonObject obj = Test::Serialise();
    obj.insert("LeakFounded", LeakFounded );
    return obj;
}
bool OutsideHermTest::Deserialize( QJsonObject const& obj )
{
    LeakFounded = obj.value("LeakFounded").toBool();
    Test::Deserialize(obj);
    return true;
}

bool OutsideHermTest::Draw(QPainter& painter, QRect &free_rect , const QString &) const
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
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font, &params ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Давление при проведении испытаний, бар"), Qt::red, test::ToString( params->PressureTesting()) );
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
    GraphA.clear();
    GraphB.clear();
    Start();
    if ( ReelControl() )
        Wait( mControlReelBits.op21_ok, mControlReelBits.op21_end );
    else
        Wait( mControlBoardBits.op11_ok, mControlBoardBits.op11_end );
    if ( IsStopped() )
        return false;

    std::this_thread::sleep_for( std::chrono::seconds(2));
    UpdateData();
    for ( size_t i = 0; i < m11Results.CONSUMPTION_A_COUNT; ++i )
    {
        Data d;
        d.Leak = m11Results.consumption_a[i];
        if ( ReelControl() )
            d.Signal = fabs( m11Results.ref_a[i] );
        else
            d.Signal = fabs( m11Results.ref_a[i] );
        GraphA.push_back(d);

        d.Leak = m11Results.consumption_b[i];
        if ( ReelControl() )
            d.Signal = fabs( m11Results.ref_b[i] );
        else
            d.Signal = fabs( m11Results.ref_b[i] );
        GraphB.push_back(d);
    }


    OilTemp = round( mTemperature.T_oil *100)/100;

    return Success();
}
void InsideHermTest::UpdateData()
{
    Test::UpdateData();
    m11Results.Read();
}
bool InsideHermTest::Success() const
{
    return true;
}

namespace
{

QJsonArray  ToJson ( InsideHermTest::DataSet const& data )
{
    QJsonArray arr;

    foreach ( InsideHermTest::Data const& d, data )
    {
        arr.insert( arr.end(), d.Serialise() );
    }

    return std::move( arr );
}
InsideHermTest::DataSet FromJson( QJsonArray const& arr )
{
    InsideHermTest::DataSet data;

    foreach (QJsonValue const& v, arr)
    {
        InsideHermTest::Data d;
        if ( d.Deserialize( v.toObject() ) )
            data.insert( data.end(), d );
    }

    return std::move( data );
}

ff0x::NoAxisGraphBuilder::LinePoints Process ( InsideHermTest::DataSet const& src, QPointF& x_range, QPointF& y_range )
{
    ff0x::NoAxisGraphBuilder::LinePoints result;

    for ( int i = 0; i < src.size(); ++i )
    {
        double const& x = src[i].Signal;
        double const& y = src[i].Leak;

        if ( !i )
        {
            x_range.setX( x );
            x_range.setY( x );
            y_range.setX( y );
            y_range.setY( y );
        }
        else
        {
            if ( x > x_range.x() )
                x_range.setX( x );
            if ( x < x_range.y() )
                x_range.setY( x );

            if ( y > y_range.x() )
                y_range.setX( y );
            if ( y < y_range.y() )
                y_range.setY( y );
        }

        result.push_back( QPointF( x, y ) );
    }
    return std::move( result );
}

}//namespace

QJsonObject InsideHermTest::Serialise() const
{
    QJsonObject obj = Test::Serialise();

    obj.insert("GraphA", ToJson( GraphA ) );
    obj.insert("GraphB", ToJson( GraphB ) );

    return obj;
}
bool InsideHermTest::Deserialize( QJsonObject const& obj )
{
    GraphA = FromJson( obj.value("GraphA").toArray() );
    GraphB = FromJson( obj.value("GraphB").toArray() );
    Test::Deserialize(obj);
    return true;
}


bool InsideHermTest::Draw(QPainter& painter, QRect &free_rect , const QString &compare_width) const
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
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font, &params ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Давление при проведении испытаний, бар"), Qt::red, test::ToString( params->PressureTesting()) );
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
    [ this, &painter, &text_font, &DrawRowCenter, &metrix, &compare_width ]( QRect const& rect )
    {
        painter.save();

        ff0x::NoAxisGraphBuilder::LinePoints dataA;
        ff0x::NoAxisGraphBuilder::LinePoints dataA_e;
        ff0x::NoAxisGraphBuilder::LinePoints dataA_e2;

        ff0x::NoAxisGraphBuilder::LinePoints dataB;
        ff0x::NoAxisGraphBuilder::LinePoints dataB_e;
        ff0x::NoAxisGraphBuilder::LinePoints dataB_e2;

        QPointF x_range_1;
        QPointF y_range_1;

        QPointF x_range_1e;
        QPointF y_range_1e;

        QPointF x_range_1e2;
        QPointF y_range_1e2;

        QPointF x_range_2;
        QPointF y_range_2;

        QPointF x_range_2e;
        QPointF y_range_2e;

        QPointF x_range_2e2;
        QPointF y_range_2e2;

//        //поиск данных теста
//        foreach (QJsonValue const& val, test::ReadFromEtalone().value( test::CURRENT_PARAMS->ModelId()).toObject().value("Results").toArray())
//        {
//            auto obj = val.toObject();
//            if ( obj.value("id").toInt() == mId )
//            {
//                dataA_e = Process( FromJson( obj.value("data").toObject().value("GraphA").toArray()), x_range_1e, y_range_1e );
//                dataB_e = Process( FromJson( obj.value("data").toObject().value("GraphB").toArray()), x_range_2e, y_range_2e );
//            }
//        }
        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromFile(compare_width).value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                dataA_e2 = Process( FromJson( obj.value("data").toObject().value("GraphA").toArray()), x_range_1e2, y_range_1e2 );
                dataB_e2 = Process( FromJson( obj.value("data").toObject().value("GraphB").toArray()), x_range_2e2, y_range_2e2 );
            }
        }

        dataA = Process( GraphA, x_range_1, y_range_1);
        dataB = Process( GraphB, x_range_2, y_range_2);

        QFont f = text_font;
        f.setPointSize( 6 );
        int w = (rect.height() - metrix.height())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::NoAxisGraphBuilder builder ( w, h, f );
        ff0x::NoAxisGraphBuilder::GraphDataLine lines_a;
        lines_a.push_back( ff0x::NoAxisGraphBuilder::Line(dataA,
                           ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !dataA_e.empty() )
            lines_a.push_back( ff0x::NoAxisGraphBuilder::Line(dataA_e,
                               ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );
        if ( !dataA_e2.empty() )
            lines_a.push_back( ff0x::NoAxisGraphBuilder::Line(dataA_e2,
                               ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );

        ff0x::NoAxisGraphBuilder::GraphDataLine lines_b;
        lines_b.push_back( ff0x::NoAxisGraphBuilder::Line(dataB,
                           ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !dataB_e.empty() )
            lines_b.push_back( ff0x::NoAxisGraphBuilder::Line(dataB_e,
                               ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );
        if ( !dataB_e2.empty() )
            lines_b.push_back( ff0x::NoAxisGraphBuilder::Line(dataB_e2,
                               ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );


        QRect p1(rect.left(), rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
        QRect p2(rect.right() - w, rect.top(), w, h );
        QRect p2t(p2.left(), p2.bottom(), p2.width(), metrix.height());

        DrawRowCenter( p1t, text_font, Qt::black, "График утечек (канал А)" );
        {
            QPointF x_range;
            QPointF y_range;
            double x_step = 0;
            double y_step = 0;

            ff0x::DataLength( x_range_1,
                              x_range_1e, !dataA_e.empty(),
                              x_range_1e2, !dataA_e2.empty(),
                              x_range, x_step );
            ff0x::DataLength( y_range_1,
                              y_range_1e, !dataA_e.empty(),
                              y_range_1e2, !dataA_e2.empty(),
                              y_range, y_step );

            painter.drawPixmap( p1, builder.Draw( lines_a, x_range, y_range, x_step, y_step, "Опорный сигнал", "Расход (л/мин)", true ) );
        }

        DrawRowCenter( p2t, text_font, Qt::black, "График утечек (канал B)" );
        {
            QPointF x_range;
            QPointF y_range;
            double x_step = 0;
            double y_step = 0;
            ff0x::DataLength( x_range_2,
                              x_range_2e, !dataB_e.empty(),
                              x_range_2e2, !dataB_e2.empty(),
                              x_range, x_step );
            ff0x::DataLength( y_range_2,
                              y_range_2e, !dataB_e.empty(),
                              y_range_2e2, !dataB_e2.empty(),
                              y_range, y_step );
            painter.drawPixmap( p2, builder.Draw( lines_b, x_range, y_range, x_step, y_step, "Опорный сигнал", "Расход (л/мин)", true ) );
        }

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

