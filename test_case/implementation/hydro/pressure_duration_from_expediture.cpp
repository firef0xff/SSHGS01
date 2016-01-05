#include "pressure_duration_from_expediture.h"
#include <QJsonObject>
#include <QJsonArray>
#include "../test_params_hydro.h"
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include "../../../../mylib/Widgets/GraphBuilder/graph_builder.h"

namespace test
{
namespace hydro
{

PressureDurationFromExpenditure::PressureDurationFromExpenditure():
    test::hydro::Test( "Проверка перепада давления и зависимость\nперепада давления от расхода", 4 )
{}

bool PressureDurationFromExpenditure::Run()
{
    Start();
    Wait( mResults.op4_ok, mResults.op4_end );
    if ( IsStopped() )
        return false;

    mData.clear();
    Data a1;
    a1.BP5 = mResults.op4_p_p_03_a;
    a1.BP3 = mResults.op4_p_a_03_a;
    a1.BP5_3 = mResults.op4_bp5_bp3_03_a;
    a1.Expenditure = mResults.op4_q_03_a;

    Data a2;
    a2.BP5 = mResults.op4_p_p_06_a;
    a2.BP3 = mResults.op4_p_a_06_a;
    a2.BP5_3 = mResults.op4_bp5_bp3_06_a;
    a2.Expenditure = mResults.op4_q_06_a;

    Data a3;
    a3.BP5 = mResults.op4_p_p_max_a;
    a3.BP3 = mResults.op4_p_a_max_a;
    a3.BP5_3 = mResults.op4_bp5_bp3_max_a;
    a3.Expenditure = mResults.op4_q_max_a;


    Data b1;
    b1.BP5 = mResults.op4_p_p_03_b;
    b1.BP3 = mResults.op4_p_a_03_b;
    b1.BP5_3 = mResults.op4_bp5_bp3_03_b;
    b1.Expenditure = mResults.op4_q_03_b;

    Data b2;
    b2.BP5 = mResults.op4_p_p_06_b;
    b2.BP3 = mResults.op4_p_a_06_b;
    b2.BP5_3 = mResults.op4_bp5_bp3_06_b;
    b2.Expenditure = mResults.op4_q_06_b;

    Data b3;
    b3.BP5 = mResults.op4_p_p_max_b;
    b3.BP3 = mResults.op4_p_a_max_b;
    b3.BP5_3 = mResults.op4_bp5_bp3_max_b;
    b3.Expenditure = mResults.op4_q_max_b;

    mData.push_back( Channels( a1, b1 ) );
    mData.push_back( Channels( a2, b2 ) );
    mData.push_back( Channels( a3, b3 ) );

    OilTemp = mResults.T_oil;

    return Success();
}

namespace
{

QJsonArray ToJson( PressureDurationFromExpenditure::DataSet const& data )
{
    QJsonArray arr;
    foreach ( PressureDurationFromExpenditure::Channels const& d, data )
    {
        QJsonObject o;
        o.insert( "ChannelA", d.first.Serialise() );
        o.insert( "ChannelB", d.second.Serialise() );

        arr.insert( arr.end(), o );
    }

    return std::move( arr );
}
PressureDurationFromExpenditure::DataSet FromJson( QJsonArray arr )
{
    PressureDurationFromExpenditure::DataSet data;

    foreach ( QJsonValue const& v, arr )
    {
        QJsonObject o = v.toObject();
        PressureDurationFromExpenditure::Data d1;
        PressureDurationFromExpenditure::Data d2;
        if ( d1.Deserialize( o.value( "ChannelA" ).toObject() ) &&
             d2.Deserialize( o.value( "ChannelB" ).toObject() ) )
            data.push_back( PressureDurationFromExpenditure::Channels( d1, d2 ) );
    }

    return std::move ( data );
}

ff0x::NoAxisGraphBuilder::LinePoints ProcessA( PressureDurationFromExpenditure::DataSet const& src, QPointF& x_range, QPointF& y_range )
{
    ff0x::NoAxisGraphBuilder::LinePoints result;

    for ( int i = 0; i < src.size(); ++i )
    {
        double const& x = src[i].first.Expenditure;
        double const& y = src[i].first.BP5_3;
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
ff0x::NoAxisGraphBuilder::LinePoints ProcessB( PressureDurationFromExpenditure::DataSet const& src, QPointF& x_range, QPointF& y_range )
{
    ff0x::NoAxisGraphBuilder::LinePoints result;

    for ( int i = 0; i < src.size(); ++i )
    {
        double const& x = src[i].second.Expenditure;
        double const& y = src[i].second.BP5_3;
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

QJsonObject PressureDurationFromExpenditure::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    obj.insert("Data", ToJson( mData ) );
    return obj;
}
bool PressureDurationFromExpenditure::Deserialize( QJsonObject const& obj )
{
    Test::Deserialize( obj );
    mData = FromJson( obj.value("Data").toArray() );

    return true;
}

bool PressureDurationFromExpenditure::Draw( QPainter& painter, QRect &free_rect ) const
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
        QPoint start_point( place.center().x() - metrix.width( text ) / 2, place.center().y()+metrix.height()/2);
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
        DrawRowCenter( rect, header_font, Qt::black, "5.Проверка перепада давления и зависимость" );
    }, 2 );
    res = DrawLine( num, free_rect, header_font,
    [ this, &painter, &DrawRowCenter, &header_font ]( QRect const& rect )
    {
        DrawRowCenter( rect, header_font, Qt::black, "перепада давления от расхода" );
    }, 2 );

    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
        DrawRowLeft( r, text_font, Qt::black, "Данное испытание предназначено для построения зависимости перепада" );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "давления от расхода. Для построения такого графика необходимо произвести" );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "измерение давления на входе и выходе гидроаппарата, как минимум при трех" );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "разных значениях расхода. " );
    }, 1.5 );

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
                   "<th rowspan='2'>Расход<br>л/мин</th>"
                   "<th colspan='3'>Перепад P-->А</th>"
                   "<th rowspan='2'>Расход<br>л/мин</th>"
                   "<th colspan='3'>Перепад P-->B</th>"
               "</tr>"
               "<tr>"
                   "<th>Давление<br>в канале<br>P, бар</th>"
                   "<th>Давление<br>в канале<br>A, бар</th>"
                   "<th>Перепад,<br>бар</th>"
                   "<th>Давление<br>в канале<br>P, бар</th>"
                   "<th>Давление<br>в канале<br>B, бар</th>"
                   "<th>Перепад,<br>бар</th>"
               "</tr>";

    QString footer = "</table>"
            "</body>"
            "</html>";

    QString table = header;
    auto MakeRow = [ &table, this, &params ]( int i ) -> QString
    {
        QString row =   "<tr>"
                            "<td>" + QString::number( mData[i].first.Expenditure ) + "</td>"
                            "<td>" + QString::number( mData[i].first.BP5 ) + "</td>"
                            "<td>" + QString::number( mData[i].first.BP3 ) + "</td>"
                            "<td>" + QString::number( mData[i].first.BP5_3 ) + "</td>";
                row +=      "<td>" + QString::number( mData[i].second.Expenditure ) + "</td>"
                            "<td>" + QString::number( mData[i].second.BP5 ) + "</td>"
                            "<td>" + QString::number( mData[i].second.BP3 ) + "</td>"
                            "<td>" + QString::number( mData[i].second.BP5_3 ) + "</td>";

          row +=        "</tr>";
        return row;
    };

    for ( auto i = 0; i < mData.size(); ++i )
    {
        table += MakeRow( i );
    }
    table += footer;

    QTextDocument doc;
    doc.setUndoRedoEnabled( false );
    doc.setTextWidth( free_rect.width() );
    doc.setUseDesignMetrics( true );
    doc.setDefaultTextOption ( QTextOption (Qt::AlignHCenter )  );
    doc.setHtml( table );
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

    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    QFontMetrics metrix( text_font );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &text_font, &params, &DrawRowCenter, &metrix ]( QRect const& rect )
    {
        painter.save();
        ff0x::NoAxisGraphBuilder::LinePoints B35A;
        ff0x::NoAxisGraphBuilder::LinePoints B35B;

        ff0x::NoAxisGraphBuilder::LinePoints B35Ae;
        ff0x::NoAxisGraphBuilder::LinePoints B35Be;

        QPointF x_range_1;
        QPointF y_range_1;

        QPointF x_range_1e;
        QPointF y_range_1e;

        QPointF x_range_2;
        QPointF y_range_2;

        QPointF x_range_2e;
        QPointF y_range_2e;

        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromEtalone().value( test::CURRENT_PARAMS->ModelId()).toObject().value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                DataSet data = FromJson( obj.value("data").toObject().value("Data").toArray() );
                B35Ae = ProcessA( data, x_range_1e, y_range_1e );
                B35Be = ProcessB( data, x_range_2e, y_range_2e );
            }
        }

        B35A = ProcessA( mData, x_range_1, y_range_1 );
        B35B = ProcessB( mData, x_range_2, y_range_2 );



        QFont f = text_font;
        f.setPointSize( 6 );
        int w = (rect.height() - metrix.height())*0.98;
        int h = (rect.height() - metrix.height())*0.98;
        QRect p1(rect.left(), rect.top(), w, h );
        QRect p2(rect.right() - w, rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
        QRect p2t(p2.left(), p2.bottom(), p2.width(), metrix.height());
        ff0x::NoAxisGraphBuilder builder ( w, h, f );
        ff0x::NoAxisGraphBuilder::GraphDataLine lines1;
        ff0x::NoAxisGraphBuilder::GraphDataLine lines2;
        lines1.push_back( ff0x::NoAxisGraphBuilder::Line(B35A,
                          ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !B35Ae.empty() )
            lines1.push_back( ff0x::NoAxisGraphBuilder::Line(B35Ae,
                              ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );

        lines2.push_back( ff0x::NoAxisGraphBuilder::Line(B35B,
                          ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !B35Be.empty() )
            lines2.push_back( ff0x::NoAxisGraphBuilder::Line(B35Be,
                              ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );

        DrawRowCenter( p1t, text_font, Qt::black, "График: «перепад Р-->A»"  );
        {
            QPointF x_range;
            QPointF y_range;
            if ( !B35Ae.empty() )
            {
                x_range = QPointF( ceil ( std::max( x_range_1.x(), x_range_1e.x() )/10 )*10,
                                   floor( std::min( x_range_1.y(), x_range_1e.y() )/10 )*10 );

                y_range = QPointF ( ceil ( std::max( y_range_1.x(), y_range_1e.x() )/10 )*10,
                                    floor( std::min( y_range_1.y(), y_range_1e.y() )/10 )*10 );
            }
            else
            {
                x_range = QPointF( ceil ( x_range_1.x()/10 )*10,
                                   floor( x_range_1.y()/10 )*10 );

                y_range = QPointF ( ceil ( y_range_1.x()/10 )*10,
                                    floor( y_range_1.y()/10 )*10 );
            }

            painter.drawPixmap( p1, builder.Draw( lines1, x_range, y_range, ceil( x_range.x() - x_range.y() )/10, ceil(y_range.x() - y_range.y())/10, "Расход (л/мин)", "Δ Р (бар)", true ) );
        }
        DrawRowCenter( p2t, text_font, Qt::black, "График: «перепад Р-->B»" );
        {
            QPointF x_range;
            QPointF y_range;
            if ( !B35Ae.empty() )
            {
                x_range = QPointF( ceil ( std::max( x_range_2.x(), x_range_2e.x() )/10 )*10,
                                   floor( std::min( x_range_2.y(), x_range_2e.y() )/10 )*10 );

                y_range = QPointF ( ceil ( std::max( y_range_2.x(), y_range_2e.x() )/10 )*10,
                                    floor( std::min( y_range_2.y(), y_range_2e.y() )/10 )*10 );
            }
            else
            {
                x_range = QPointF( ceil ( x_range_2.x()/10 )*10,
                                   floor( x_range_2.y()/10 )*10 );

                y_range = QPointF ( ceil ( y_range_2.x()/10 )*10,
                                    floor( y_range_2.y()/10 )*10 );
            }
            painter.drawPixmap( p2, builder.Draw( lines2, x_range, y_range, ceil( x_range.x() - x_range.y() )/10, ceil(y_range.x() - y_range.y())/10, "Расход (л/мин)", "Δ Р (бар)", true ) );
        }


        painter.restore();
    }, 1, free_rect.width()/2 + metrix.height()  );


    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &result_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, result_font, Qt::black, "РЕЗУЛЬТАТ:" );
    }, 3 );

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "Испытание ", Qt::red, QString("нет критерия успеха теста!!!") + (Success()? "пройдено" : "не пройдено") );
    }, 2 );
    return res;
}

bool PressureDurationFromExpenditure::Success() const
{
    return true;
}

QJsonObject PressureDurationFromExpenditure::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("BP5", BP5 );
    obj.insert("BP3", BP3 );
    obj.insert("BP5_3", BP5_3 );
    obj.insert("Expenditure", Expenditure );

    return obj;
}
bool PressureDurationFromExpenditure::Data::Deserialize( QJsonObject const& obj )
{
    BP5 = obj.value("BP5").toDouble();
    BP3 = obj.value("BP3").toDouble();
    BP5_3 = obj.value("BP5_3").toDouble();
    Expenditure = obj.value("Expenditure").toDouble();
    return true;
}


}//namespace hydro

}//namespace test
