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
    Start();
    if ( ReelControl() )
        Wait( mControlReelBits.op23_ok, mControlReelBits.op23_end );
    else
        Wait( mControlBoardBits.op13_ok, mControlBoardBits.op13_end );
    if ( IsStopped() )
        return false;

    if ( ReelControl() )
    {
#warning нужен расход на питающем канале?
        for ( size_t i = 0; i < m23Results.CONSUMPTION_A_COUNT; ++i )
        {
            Data d;
            d.Expenditure = m23Results.consumption_a[i];
            d.ChA.BP3 = m23Results.bp3_a[i];
            d.ChB.BP3 = m23Results.bp4_b[i];
            d.ChA.BP5 = m23Results.bp5_a[i];
            d.ChA.BP5 = m23Results.bp5_b[i];
            mData.push_back( d );
        }
    }
    else
    {
#warning нужен расход на питающем канале?
        for ( size_t i = 0; i < m13Results.CONSUMPTION_A_COUNT; ++i )
        {
            Data d;
            d.Expenditure = m13Results.consumption_a[i];
            d.ChA.BP3 = m13Results.bp3_a[i];
            d.ChB.BP3 = m13Results.bp4_b[i];
            d.ChA.BP5 = m13Results.bp5_a[i];
            d.ChA.BP5 = m13Results.bp5_b[i];
            mData.push_back( d );
        }
    }

    OilTemp = mTemperature.T_oil;
    return Success();
}
bool ExpeditureFromPressureDuration::Success() const
{
    return true;
}
QJsonObject ExpeditureFromPressureDuration::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    QJsonArray a;
    foreach (Data const& d, mData)
    {
        a.insert( a.end(), d.Serialise() );
    }
    obj.insert("Data", a );

    return obj;
}
bool ExpeditureFromPressureDuration::Deserialize( QJsonObject const& obj )
{
    QJsonArray a = obj.value("Data").toArray();
    foreach (QJsonValue const& v, a)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            mData.insert( mData.end(), d );
    }

    Test::Deserialize( obj );
    return true;
}

void ExpeditureFromPressureDuration::ResetDrawLine()
{
    PrintedRows = 0;
    Test::ResetDrawLine();
}

bool ExpeditureFromPressureDuration::Draw( QPainter& painter, QRect &free_rect ) const
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
        QString row =   "<tr>"
                            "<td>" + QString::number( mData[i].Expenditure ) + "</td>"
                            "<td>" + QString::number( mData[i].ChA.BP5 ) + "</td>"
                            "<td>" + QString::number( mData[i].ChA.BP3 ) + "</td>"
                            "<td>" + QString::number( mData[i].ChA.BP5_3() ) + "</td>";
                row +=      "<td>" + QString::number( mData[i].ChB.BP5 ) + "</td>"
                            "<td>" + QString::number( mData[i].ChB.BP3 ) + "</td>"
                            "<td>" + QString::number( mData[i].ChB.BP5_3() ) + "</td>";
          row +=        "</tr>";
        return row;
    };

    QTextDocument doc;
    doc.setUndoRedoEnabled( false );
    doc.setTextWidth( free_rect.width() );
    doc.setUseDesignMetrics( true );
    doc.setDefaultTextOption ( QTextOption (Qt::AlignHCenter )  );

    int rows_prapared = 0;
    for ( auto i = PrintedRows; i < mData.size(); ++i )
    {
        table += MakeRow( i );
        QString tmp = table + footer;
        doc.setHtml( tmp );
        auto h = doc.documentLayout()->documentSize().height();
        if ( h > free_rect.height() )
            break;
        ++rows_prapared;
    }
    PrintedRows += rows_prapared;
    table += footer;

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
    ///----------------

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

        ff0x::GraphBuilder::LinePoints dataA;
        ff0x::GraphBuilder::LinePoints dataA_e;

        ff0x::GraphBuilder::LinePoints dataB;
        ff0x::GraphBuilder::LinePoints dataB_e;

        double max_signal_a = 0;
        double max_signal_b = 0;
        double max_Leak = 0;

        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromEtalone().value( test::CURRENT_PARAMS->ModelId()).toObject().value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                QJsonArray a = obj.value("data").toObject().value("Data").toArray();

                foreach ( QJsonValue const& v, a )
                {
                    QJsonObject o = v.toObject();
                    double expenditure =  o.value("Expenditure").toDouble();
                    double bp3_a = o.value("ChA").toObject().value("BP3").toDouble();
                    double bp5_a = o.value("ChA").toObject().value("BP5").toDouble();
                    double bp3_b = o.value("ChB").toObject().value("BP3").toDouble();
                    double bp5_b = o.value("ChB").toObject().value("BP5").toDouble();
                    dataA_e.push_back( QPointF( bp5_a - bp3_a, expenditure ) );
                    dataB_e.push_back( QPointF( bp5_b - bp3_b, expenditure ) );
                }
            }
        }


        foreach ( Data const& item, mData )
        {
            double abs_sig_a = std::abs( item.ChA.BP5_3() );
            double abs_sig_b = std::abs( item.ChB.BP5_3() );
            double abs_leak = std::abs( item.Expenditure );

            if ( max_signal_a < abs_sig_a )
                max_signal_a = abs_sig_a;

            if ( max_signal_b < abs_sig_b )
                max_signal_b = abs_sig_b;

            if ( max_Leak < abs_leak )
                max_Leak = abs_leak;

            dataA.push_back( QPointF( item.ChA.BP5_3(), item.Expenditure ) );
            dataB.push_back( QPointF( item.ChB.BP5_3(), item.Expenditure ) );
        }

        QFont f = text_font;
        f.setPointSize( 6 );
        int w = (rect.height() - metrix.height())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::GraphBuilder builder ( w, h, ff0x::GraphBuilder::PlusPlus, f );
        ff0x::GraphBuilder::GraphDataLine lines_a;
        lines_a.push_back( ff0x::GraphBuilder::Line(dataA, ff0x::GraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !dataA_e.empty() )
            lines_a.push_back( ff0x::GraphBuilder::Line(dataA_e, ff0x::GraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );

        ff0x::GraphBuilder::GraphDataLine lines_b;
        lines_b.push_back( ff0x::GraphBuilder::Line(dataB, ff0x::GraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !dataB_e.empty() )
            lines_b.push_back( ff0x::GraphBuilder::Line(dataB_e, ff0x::GraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );


        QRect p1(rect.left(), rect.top(), w, h );
        QRect p2(rect.right() - w, rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
        QRect p2t(p2.left(), p2.bottom(), p2.width(), metrix.height());
        DrawRowCenter( p1t, text_font, Qt::black, "P->A" );
        DrawRowCenter( p2t, text_font, Qt::black, "P->B" );
        painter.drawPixmap( p1, builder.Draw( lines_a, max_signal_a * 1.25, max_Leak * 1.25, ceil( max_signal_a )/10, ceil(max_Leak)/10, "Δ Р (бар)", "Расход (л/мин)", true ) );
        painter.drawPixmap( p2, builder.Draw( lines_b, max_signal_b * 1.25, max_Leak * 1.25, ceil( max_signal_b )/10, ceil(max_Leak)/10, "Δ Р (бар)", "Расход (л/мин)", true ) );

        painter.restore();
    }, 1, free_rect.width()/2 + metrix.height()  );


//    free_rect.setHeight( 0 );
    return res;
}

QJsonObject ExpeditureFromPressureDuration::Data::Channel::Serialise() const
{
    QJsonObject obj;
    obj.insert("BP3", BP3 );
    obj.insert("BP5", BP5 );
}
bool ExpeditureFromPressureDuration::Data::Channel::Deserialize( QJsonObject const& obj )
{
    BP3 = obj.value("BP3").toDouble();
    BP5 = obj.value("BP5").toDouble();
}

QJsonObject ExpeditureFromPressureDuration::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("Expenditure", Expenditure );
    obj.insert("ChA", ChA.Serialise() );
    obj.insert("ChB", ChB.Serialise() );

    return obj;
}
bool ExpeditureFromPressureDuration::Data::Deserialize( QJsonObject const& obj )
{   
    Expenditure = obj.value("Expenditure").toDouble();
    ChA.Deserialize( obj.value("ChA").toObject() );
    ChB.Deserialize( obj.value("ChB").toObject() );
    return true;
}

}//namespace servo

}//namespace test
