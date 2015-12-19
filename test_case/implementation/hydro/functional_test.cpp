#include "functional_test.h"
#include <QJsonObject>
#include "../test_params_hydro.h"
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

namespace test
{

namespace hydro
{

FunctionalTest::FunctionalTest():
    test::hydro::Test( "Испытание функционирования", 1 )
{}

bool FunctionalTest::Run()
{
    Start();
    Wait( mResults.op1_ok, mResults.op1_end );
    if ( IsStopped() )
        return false;

    ReelA.work_on_max_pressure = mResults.op1_max_ok_a && !mResults.op1_max_o_a;
    ReelA.work_on_min_pressure = mResults.op1_min_ok_a && !mResults.op1_min_no_a;
    ReelA.I = mResults.op1_current_a;
    ReelA.P = mResults.op1_power_a;
    ReelA.R = mResults.op1_resist_a;
    ReelA.U = mResults.op1_volt_a;


    ReelB.work_on_max_pressure = mResults.op1_max_ok_b && !mResults.op1_max_no_b;
    ReelB.work_on_min_pressure = mResults.op1_min_ok_b && !mResults.op1_min_no_b;
    ReelB.I = mResults.op1_current_b;
    ReelB.P = mResults.op1_power_b;
    ReelB.R = mResults.op1_resist_b;
    ReelB.U = mResults.op1_volt_b;

    OilTemp = mResults.T_oil;

    return Success();
}

QJsonObject FunctionalTest::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    obj.insert("ReelA", ReelA.Serialise() );
    obj.insert("ReelB", ReelB.Serialise() );

    return obj;
}
bool FunctionalTest::Deserialize( QJsonObject const& obj )
{
    bool res = ReelA.Deserialize( obj.value("ReelA").toObject() );
    res *= ReelB.Deserialize( obj.value("ReelB").toObject() );
    Test::Deserialize( obj );
    return res;
}

bool FunctionalTest::Draw( QPainter& painter, QRect &free_rect ) const
{
    test::hydro::Parameters *params = static_cast< test::hydro::Parameters * >( CURRENT_PARAMS );
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
        DrawRowLeft( r, text_font, Qt::black, "В данном испытании происходит проверка прохода рабочей жидкости в" );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "линиях, предусмотренных схемой гидроаппаратуры и характеристики катушек" );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "управления." );
    }, 1.5 );


    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font, params ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Давление при проведении испытаний, бар"), Qt::red, test::ToString(params->MinTestPressure()) );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Расход при проведении испытаний, л/мин" ), Qt::black, "что писать?" );
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


    QString header = "<html>"
            "<head>"
              "<meta charset='utf-8'>"
              "<style type='text/css'>"
                   "td { text-align: center;}"
                   "th { font-weight: normal;}"
                   "table {border-collapse: collapse; border-style: solid;}"
             "</style>"
            "</head>"
            "<body>"
            "<table width='100%' border='1.5' cellspacing='-0.5' cellpadding='-0.5'>"
               "<tr>"
                   "<th> </th>"
                   "<th>Сила тока, А</th>"
                   "<th>Напряжение, В</th>"
                   "<th>Сопротивление, Ом</th>"
                   "<th>Мощность, Вт</th>"
               "</tr>";

    QString footer = "</table>"
            "</body>"
            "</html>";

    QString row =   "<tr>"
                        "<td>Катушка a</td>"
                        "<td>" + test::ToString(ReelA.I) + "</td>"
                        "<td>" + test::ToString(ReelA.U) + "</td>"
                        "<td>" + test::ToString(ReelA.R) + "</td>"
                        "<td>" + test::ToString(ReelA.P) + "</td>"
                    "</tr>";
    if (params->ReelCount() == 2)
    {
        row +=  "<tr>"
                    "<td>Катушка b</td>"
                    "<td>" + test::ToString(ReelB.I) + "</td>"
                    "<td>" + test::ToString(ReelB.U) + "</td>"
                    "<td>" + test::ToString(ReelB.R) + "</td>"
                    "<td>" + test::ToString(ReelB.P) + "</td>"
                "</tr>";
    }

    QTextDocument doc;
    doc.setUndoRedoEnabled( false );
    doc.setTextWidth( free_rect.width() );
    doc.setUseDesignMetrics( true );
    doc.setDefaultTextOption ( QTextOption (Qt::AlignHCenter )  );
    doc.setHtml( header + row + footer );
    auto h = doc.documentLayout()->documentSize().height();

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &doc, &text_font ]( QRect const& rect )
    {
        painter.save();
        QRectF r( 0, 0, rect.width(), rect.height() );
        painter.translate( rect.topLeft() );
        doc.drawContents( &painter, r);
        painter.restore();
    }, 1, h );


    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "Функционирование клапана ", Qt::red, Success()? "верно" : "не верно" );
    }, 3 );

    return res;
}

bool FunctionalTest::Success() const
{
    test::hydro::Parameters *params = static_cast< test::hydro::Parameters * >( CURRENT_PARAMS );
    if ( !params )
        return false;

    bool res = ReelA.work_on_max_pressure && ReelA.work_on_min_pressure;
    if (params->ReelCount() == 2 )
    {
        res &= ReelB.work_on_max_pressure && ReelB.work_on_min_pressure;
    }
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

