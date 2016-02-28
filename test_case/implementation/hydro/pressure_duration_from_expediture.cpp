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


class PressureDurationFromExpenditure::GrapfData
{
public:
    GrapfData( PressureDurationFromExpenditure const* test, QString compare_width )
    {
        QPointF x_range_1;
        QPointF y_range_1;

        QPointF x_range_2;
        QPointF y_range_2;

        QPointF x_range_e;
        QPointF y_range_e;
        //поиск данных теста
        bool use_etalone = false;
        foreach (QJsonValue const& val, test::ReadFromFile(compare_width).value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == test->mId )
            {
                PressureDurationFromExpenditure::DataSet data = FromJson( obj.value("data").toObject().value("Data").toArray() );
                B35Ae2 = ProcessA( data, x_range_1, y_range_1 );
                B35Be2 = ProcessB( data, x_range_2, y_range_2 );
                x_range_e = ff0x::MergeRanges( x_range_1, x_range_2 );
                y_range_e = ff0x::MergeRanges( y_range_1, y_range_2 );
                use_etalone = true;
            }
        }

        B35A = ProcessA( test->mData, x_range_1, y_range_1 );
        B35B = ProcessB( test->mData, x_range_2, y_range_2 );
        x_range = ff0x::MergeRanges( x_range_1, x_range_2 );
        y_range = ff0x::MergeRanges( y_range_1, y_range_2 );

        x_range = ff0x::MergeRanges( x_range, x_range_e, use_etalone );
        y_range = ff0x::MergeRanges( y_range, y_range_e, use_etalone );
    }

    ff0x::NoAxisGraphBuilder::LinePoints B35A;
    ff0x::NoAxisGraphBuilder::LinePoints B35B;

    ff0x::NoAxisGraphBuilder::LinePoints B35Ae2;
    ff0x::NoAxisGraphBuilder::LinePoints B35Be2;

    QPointF x_range;
    QPointF y_range;
};

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

    OilTemp = round(mResults.T_oil*100)/100;

    return Success();
}

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

void PressureDurationFromExpenditure::ResetDrawLine()
{
    Test::ResetDrawLine();
    if ( mGrapfs )
    {
        delete mGrapfs;
        mGrapfs = nullptr;
    }
}
bool PressureDurationFromExpenditure::Draw(QPainter& painter, QRect &free_rect , const QString &compare_width) const
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

    if (!mGrapfs)
        mGrapfs = new GrapfData( this, compare_width );

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &text_font, &params, &DrawRowCenter, &metrix ]( QRect const& rect )
    {
        painter.save();

        QFont f = text_font;
        f.setPointSize( 12 );
        int w = (rect.width())*0.98;
        int h = (rect.height() - metrix.height())*0.98;
        QRect p1(rect.left(), rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
        ff0x::NoAxisGraphBuilder builder ( w, h, f );
        ff0x::NoAxisGraphBuilder::GraphDataLine lines;
        lines.push_back( ff0x::NoAxisGraphBuilder::Line(mGrapfs->B35A,
                          ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !mGrapfs->B35Ae2.empty() )
            lines.push_back( ff0x::NoAxisGraphBuilder::Line(mGrapfs->B35Ae2,
                              ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );


        DrawRowCenter( p1t, text_font, Qt::black, "График: «перепад Р-->A»"  );
        {
            QPointF x_range;
            QPointF y_range;
            double x_step = 0;
            double y_step = 0;

            ff0x::DataLength( mGrapfs->x_range, x_range, x_step );
            ff0x::DataLength( mGrapfs->y_range, y_range, y_step );

            painter.drawPixmap( p1, builder.Draw( lines, x_range, y_range, x_step, y_step, "Расход (л/мин)", "Δ Р (бар)", true ) );
        }


        painter.restore();
    }, 1, 480  );

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &text_font, &params, &DrawRowCenter, &metrix ]( QRect const& rect )
    {
        painter.save();

        QFont f = text_font;
        f.setPointSize( 12 );
        int w = (rect.width())*0.98;
        int h = (rect.height() - metrix.height())*0.98;
        QRect p1(rect.left(), rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
        ff0x::NoAxisGraphBuilder builder ( w, h, f );
        ff0x::NoAxisGraphBuilder::GraphDataLine lines;

        lines.push_back( ff0x::NoAxisGraphBuilder::Line( mGrapfs->B35B,
                          ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !mGrapfs->B35Be2.empty() )
            lines.push_back( ff0x::NoAxisGraphBuilder::Line(mGrapfs->B35Be2,
                              ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );


        DrawRowCenter( p1t, text_font, Qt::black, "График: «перепад Р-->B»" );
        {
            QPointF x_range;
            QPointF y_range;
            double x_step = 0;
            double y_step = 0;

            ff0x::DataLength( mGrapfs->x_range, x_range, x_step );
            ff0x::DataLength( mGrapfs->y_range, y_range, y_step );

            painter.drawPixmap( p1, builder.Draw( lines, x_range, y_range, x_step, y_step, "Расход (л/мин)", "Δ Р (бар)", true ) );
        }

        painter.restore();
    }, 1, 480  );


    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &result_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, result_font, Qt::black, "РЕЗУЛЬТАТ:" );
    }, 3 );

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "Испытание ", Qt::red, Success()? "пройдено" : "не пройдено" );
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

