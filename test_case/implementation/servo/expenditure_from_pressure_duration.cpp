#include "expenditure_from_pressure_duration.h"
#include <QJsonObject>
#include <QJsonArray>
#include "../../../../mylib/Widgets/GraphBuilder/graph_builder.h"
#include "test_case/test_params.h"
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

namespace test
{
namespace servo
{

ExpeditureFromPressureDuration::ExpeditureFromPressureDuration():
    test::servo::Test( "Проверка зависимости расхода «к потребителю» от перепада давлений нагрузки", 13, 23 )
{}

bool ExpeditureFromPressureDuration::Run()
{
    mData.clear();
    Start();
    if ( ReelControl() )
        Wait( mControlReelBits.op23_ok, mControlReelBits.op23_end );
    else
        Wait( mControlBoardBits.op13_ok, mControlBoardBits.op13_end );
    if ( IsStopped() )
        return false;

    for ( size_t i = 0; i < m13Results.CONSUMPTION_A_COUNT; ++i )
    {
        Data d;
        d.ChA.Expenditure = m13Results.consumption_a[i];
        d.ChB.Expenditure = m13Results.consumption_b[i];
        d.ChA.BP3 = m13Results.bp3_a[i];
        d.ChB.BP3 = m13Results.bp4_b[i];
        d.ChA.BP5 = m13Results.bp5_a[i];
        d.ChB.BP5 = m13Results.bp5_b[i];

        if ( d == Data() )
            break;

        mData.push_back( d );
    }

    OilTemp = round( mTemperature.T_oil *100)/100;
    return Success();
}
void ExpeditureFromPressureDuration::UpdateData()
{
    Test::UpdateData();
    m13Results.Read();
}
bool ExpeditureFromPressureDuration::Success() const
{
    return true;
}

namespace
{

QJsonArray ToJson( ExpeditureFromPressureDuration::DataSet const& data )
{
    QJsonArray arr;
    foreach ( ExpeditureFromPressureDuration::Data const& d, data )
    {
        arr.insert( arr.end(), d.Serialise() );
    }
    return std::move( arr );
}
ExpeditureFromPressureDuration::DataSet FromJson( QJsonArray const& arr )
{
    ExpeditureFromPressureDuration::DataSet data;

    foreach (QJsonValue const& v, arr)
    {
        ExpeditureFromPressureDuration::Data d;
        if ( d.Deserialize( v.toObject() ) )
            data.insert( data.end(), d );
    }
    return std::move( data );
}

ff0x::NoAxisGraphBuilder::LinePoints ProcessA( ExpeditureFromPressureDuration::DataSet const& src, QPointF& x_range, QPointF& y_range )
{
    ff0x::NoAxisGraphBuilder::LinePoints result;

    for ( int i = 0; i < src.size(); ++i )
    {
        double const& y = src[i].ChA.Expenditure;
        double const& x = src[i].ChA.BP5_3();
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
    result.push_back( QPointF( 0, 0 ) );
    if ( 0 > x_range.x() )
        x_range.setX( 0 );
    if ( 0 < x_range.y() )
        x_range.setY( 0 );

    if ( 0 > y_range.x() )
        y_range.setX( 0 );
    if ( 0 < y_range.y() )
        y_range.setY( 0 );


    return std::move( result );
}
ff0x::NoAxisGraphBuilder::LinePoints ProcessB( ExpeditureFromPressureDuration::DataSet const& src, QPointF& x_range, QPointF& y_range )
{
    ff0x::NoAxisGraphBuilder::LinePoints result;

    for ( int i = 0; i < src.size(); ++i )
    {
        double const& y = src[i].ChB.Expenditure;
        double const& x = src[i].ChB.BP5_3();
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
    result.push_back( QPointF( 0, 0 ) );
    if ( 0 > x_range.x() )
        x_range.setX( 0 );
    if ( 0 < x_range.y() )
        x_range.setY( 0 );

    if ( 0 > y_range.x() )
        y_range.setX( 0 );
    if ( 0 < y_range.y() )
        y_range.setY( 0 );
    return std::move( result );
}
}//namespace

QJsonObject ExpeditureFromPressureDuration::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    obj.insert("Data", ToJson( mData ) );

    return obj;
}
bool ExpeditureFromPressureDuration::Deserialize( QJsonObject const& obj )
{
    mData = FromJson( obj.value("Data").toArray() );
    Test::Deserialize( obj );
    return true;
}

void ExpeditureFromPressureDuration::ResetDrawLine()
{
    PrintedRows = 0;
    Test::ResetDrawLine();
}

bool ExpeditureFromPressureDuration::Draw(QPainter& painter, QRect &free_rect , const QString &compare_width) const
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
        DrawRowCenter( rect, header_font, Qt::black, "4.Проверка зависимости расхода «к потребителю»" );
    }, 1.5 );
    res = DrawLine( num, free_rect, header_font,
    [ this, &painter, &DrawRowCenter, &header_font ]( QRect const& rect )
    {
        DrawRowCenter( rect, header_font, Qt::black, "от перепада давлений нагрузки" );
    }, 1.5 );

    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
        DrawRowLeft( r, text_font, Qt::black, "Данные испытания, проводятся для определения характера изменения" );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "расхода «к потребителю» в зависимости от перепада давлений нагрузки." );
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


    //----------------
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
    auto MakeRow = [ &table, this ]( int i ) -> QString
    {
        QString row =   "<tr>";
        if ( mData[i].ChA != Data::Channel() )
        {
            row += "<td>" + QString::number( mData[i].ChA.Expenditure ) + "</td>"
                   "<td>" + QString::number( mData[i].ChA.BP5 ) + "</td>"
                   "<td>" + QString::number( mData[i].ChA.BP3 ) + "</td>"
                   "<td>" + QString::number( mData[i].ChA.BP5_3() ) + "</td>";
        }
        else
        {
            row += "<td></td>"
                   "<td></td>"
                   "<td></td>"
                   "<td></td>";
        }
        if ( mData[i].ChB != Data::Channel() )
        {
            row += "<td>" + QString::number( mData[i].ChB.Expenditure ) + "</td>"
                  "<td>" + QString::number( mData[i].ChB.BP5 ) + "</td>"
                  "<td>" + QString::number( mData[i].ChB.BP3 ) + "</td>"
                  "<td>" + QString::number( mData[i].ChB.BP5_3() ) + "</td>";
        }
        else
        {
            row += "<td></td>"
                   "<td></td>"
                   "<td></td>"
                   "<td></td>";
        }
        row +=        "</tr>";
        return row;
    };

    QTextDocument doc;
    doc.setUndoRedoEnabled( false );
    doc.setTextWidth( free_rect.width() );
    doc.setUseDesignMetrics( true );
    doc.setDefaultTextOption ( QTextOption (Qt::AlignHCenter )  );

    int rows_prapared = 0;
    for ( auto i = PrintedRows; i < mData.size(); ++i ) //PrintedRows
    {
        QString row = MakeRow( i );
        QString tmp = table + row + footer;
        doc.setHtml( tmp );
        auto h = doc.documentLayout()->documentSize().height();
        if ( h > free_rect.height() )
            break;
        ++rows_prapared;
        table += row;
    }
    if ( PrintedRows && rows_prapared )
        num += PrintedPage;
    PrintedRows += rows_prapared;
    table += footer;

    doc.setHtml( table );
    auto h = doc.documentLayout()->documentSize().height();

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &doc, &text_font, &rows_prapared ]( QRect const& rect )
    {
        if (!rows_prapared)
            return;
        painter.save();
        QRectF r( 0, 0, rect.width(), rect.height() );
        painter.translate( rect.topLeft() );
        doc.drawContents( &painter, r);
        painter.restore();
        ++PrintedPage;
    }, 1, h );
    ///----------------

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

        ff0x::GraphBuilder::LinePoints dataA;
        ff0x::GraphBuilder::LinePoints dataA_e;
        ff0x::GraphBuilder::LinePoints dataA_e2;

        ff0x::GraphBuilder::LinePoints dataB;
        ff0x::GraphBuilder::LinePoints dataB_e;
        ff0x::GraphBuilder::LinePoints dataB_e2;

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
//                DataSet data = FromJson( obj.value("data").toObject().value("Data").toArray() );
//                dataA_e = ProcessA( data, x_range_1e, y_range_1e );
//                dataB_e = ProcessB( data, x_range_2e, y_range_2e );
//            }
//        }
        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromFile(compare_width).value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                DataSet data = FromJson( obj.value("data").toObject().value("Data").toArray() );
                dataA_e2 = ProcessA( data, x_range_1e2, y_range_1e2 );
                dataB_e2 = ProcessB( data, x_range_2e2, y_range_2e2 );
            }
        }


        dataA = ProcessA( mData, x_range_1, y_range_1 );
        dataB = ProcessB( mData, x_range_2, y_range_2 );


        QFont f = text_font;
        f.setPointSize( 6 );
        int w = (rect.height() - metrix.height())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::NoAxisGraphBuilder builder ( w, h, f );
        ff0x::NoAxisGraphBuilder::GraphDataLine lines_a;
        lines_a.push_back( ff0x::NoAxisGraphBuilder::Line(dataA, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !dataA_e.empty() )
            lines_a.push_back( ff0x::NoAxisGraphBuilder::Line(dataA_e, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );
        if ( !dataA_e2.empty() )
            lines_a.push_back( ff0x::NoAxisGraphBuilder::Line(dataA_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );

        ff0x::NoAxisGraphBuilder::GraphDataLine lines_b;
        lines_b.push_back( ff0x::NoAxisGraphBuilder::Line(dataB, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !dataB_e.empty() )
            lines_b.push_back( ff0x::NoAxisGraphBuilder::Line(dataB_e, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );
        if ( !dataB_e2.empty() )
            lines_b.push_back( ff0x::NoAxisGraphBuilder::Line(dataB_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );

        QRect p1(rect.left(), rect.top(), w, h );
        QRect p2(rect.right() - w, rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
        QRect p2t(p2.left(), p2.bottom(), p2.width(), metrix.height());


        DrawRowCenter( p1t, text_font, Qt::black, "P->A" );
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

            painter.drawPixmap( p1, builder.Draw( lines_a, x_range, y_range, x_step, y_step, "Δ Р (бар)", "Расход (л/мин)", true ) );
        }
        DrawRowCenter( p2t, text_font, Qt::black, "P->B" );
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
            painter.drawPixmap( p2, builder.Draw( lines_b, x_range, y_range, x_step, y_step, "Δ Р (бар)", "Расход (л/мин)", true ) );

        }

        painter.restore();
    }, 1, free_rect.width()/2 + metrix.height()  );


    free_rect.setHeight( 0 );
    return res;
}

QJsonObject ExpeditureFromPressureDuration::Data::Channel::Serialise() const
{
    QJsonObject obj;
    obj.insert("Expenditure", Expenditure );
    obj.insert("BP3", BP3 );
    obj.insert("BP5", BP5 );
    return obj;
}
bool ExpeditureFromPressureDuration::Data::Channel::Deserialize( QJsonObject const& obj )
{
    Expenditure = obj.value("Expenditure").toDouble();
    BP3 = obj.value("BP3").toDouble();
    BP5 = obj.value("BP5").toDouble();
    return true;
}

QJsonObject ExpeditureFromPressureDuration::Data::Serialise() const
{
    QJsonObject obj;    
    obj.insert("ChA", ChA.Serialise() );
    obj.insert("ChB", ChB.Serialise() );

    return obj;
}
bool ExpeditureFromPressureDuration::Data::Deserialize( QJsonObject const& obj )
{   
    ChA.Deserialize( obj.value("ChA").toObject() );
    ChB.Deserialize( obj.value("ChB").toObject() );
    return true;
}

}//namespace servo

}//namespace test
