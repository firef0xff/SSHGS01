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
    return true;
}
QJsonObject ExpeditureFromPressureDuration::Serialise() const
{
    QJsonObject obj;
    QJsonArray a;
    foreach (Data const& d, BP5_3)
    {
        a.insert( a.end(), d.Serialise() );
    }
    obj.insert("BP5_3", a );

    QJsonArray b;
    foreach (Data const& d, BP3_V)
    {
        b.insert( b.end(), d.Serialise() );
    }
    obj.insert("BP3_V", a );

    return obj;
}
bool ExpeditureFromPressureDuration::Deserialize( QJsonObject const& obj )
{
    QJsonArray a = obj.value("BP5_3").toArray();
    foreach (QJsonValue const& v, a)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            BP5_3.insert( BP5_3.end(), d );
    }

    QJsonArray b = obj.value("BP3_V").toArray();
    foreach (QJsonValue const& v, b)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            BP3_V.insert( BP3_V.end(), d );
    }


    return true;
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
   /* auto MakeRow = [ &table, this, &params ]( int i ) -> QString
    {
        QString row =   "<tr>"
                            "<td>" + QString::number( mData[i].first.Expenditure ) + "</td>"
                            "<td>" + QString::number( mData[i].first.BP5 ) + "</td>"
                            "<td>" + QString::number( mData[i].first.BP3 ) + "</td>"
                            "<td>" + QString::number( mData[i].first.BP5_3 ) + "</td>";
        if ( params->ReelCount() == 2 )
        {
                row +=      "<td>" + QString::number( mData[i].second.BP5 ) + "</td>"
                            "<td>" + QString::number( mData[i].second.BP3 ) + "</td>"
                            "<td>" + QString::number( mData[i].second.BP5_3 ) + "</td>";
        }
        else
        {
            row +=          "<td></td>"
                            "<td></td>"
                            "<td></td>";
        }
          row +=        "</tr>";
        return row;
    };

    for ( auto i = 0; i < mData.size(); ++i )
    {
        table += MakeRow( i );
    }*/
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

        ff0x::GraphBuilder::LinePoints data;
        ff0x::GraphBuilder::LinePoints data_e;

        ff0x::GraphBuilder::LinePoints data2;
        ff0x::GraphBuilder::LinePoints data2_e;

        double max_signal = 0;
        double max_Leak = 0;

        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromEtalone().value( test::CURRENT_PARAMS->ModelId()).toObject().value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                QJsonArray a = obj.value("data").toObject().value("BP5_3").toArray();
                foreach ( QJsonValue const& v, a )
                {
                    QJsonObject o = v.toObject();
                    data_e.push_back( QPointF( o.value("Signal").toDouble(), o.value("Expenditure").toDouble() ) );
                }
                a = obj.value("data").toObject().value("BP3_V").toArray();
                foreach ( QJsonValue const& v, a )
                {
                    QJsonObject o = v.toObject();
                    data2_e.push_back( QPointF( o.value("Signal").toDouble(), o.value("Expenditure").toDouble() ) );
                }
            }
        }


        foreach ( Data const& item, BP5_3 )
        {
            double abs_sig = std::abs( item.Duration );
            double abs_leak = std::abs( item.Expenditure );

            if ( max_signal < abs_sig )
                max_signal = abs_sig;

            if ( max_Leak < abs_leak )
                max_Leak = abs_leak;

            data.push_back( QPointF( item.Duration, item.Expenditure ) );
        }

        foreach ( Data const& item, BP3_V )
        {
            double abs_sig = std::abs( item.Duration );
            double abs_leak = std::abs( item.Expenditure );

            if ( max_signal < abs_sig )
                max_signal = abs_sig;

            if ( max_Leak < abs_leak )
                max_Leak = abs_leak;

            data2.push_back( QPointF( item.Duration, item.Expenditure ) );
        }

        QFont f = text_font;
        f.setPointSize( 6 );
        int w = (rect.height() - metrix.height())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::GraphBuilder builder ( w, h, ff0x::GraphBuilder::PlusPlus, f );
        ff0x::GraphBuilder::GraphDataLine lines1;
        lines1.push_back( ff0x::GraphBuilder::Line(data, ff0x::GraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !data_e.empty() )
            lines1.push_back( ff0x::GraphBuilder::Line(data_e, ff0x::GraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );

        ff0x::GraphBuilder::GraphDataLine lines2;
        lines2.push_back( ff0x::GraphBuilder::Line(data2, ff0x::GraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !data2_e.empty() )
            lines2.push_back( ff0x::GraphBuilder::Line(data2_e, ff0x::GraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );


        QRect p1(rect.left(), rect.top(), w, h );
        QRect p2(rect.right() - w, rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
        QRect p2t(p2.left(), p2.bottom(), p2.width(), metrix.height());
        DrawRowCenter( p1t, text_font, Qt::black, "P->A" );
        DrawRowCenter( p2t, text_font, Qt::black, "P->B" );
        painter.drawPixmap( p1, builder.Draw( lines1, max_signal * 1.25, max_Leak * 1.25, 0.05, 0.5, "Δ Р (бар)", "Расход (л/мин)", true ) );
        painter.drawPixmap( p2, builder.Draw( lines2, max_signal * 1.25, max_Leak * 1.25, 0.05, 0.5, "Δ Р (бар)", "Расход (л/мин)", true ) );

        painter.restore();
    }, 1, free_rect.width()/2 + metrix.height()  );


//    free_rect.setHeight( 0 );
    return res;
}

QJsonObject ExpeditureFromPressureDuration::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("Duration", Duration );
    obj.insert("Expenditure", Expenditure );

    return obj;
}
bool ExpeditureFromPressureDuration::Data::Deserialize( QJsonObject const& obj )
{
    Duration = obj.value("Duration").toDouble();
    Expenditure = obj.value("Expenditure").toDouble();
    return true;
}

}//namespace servo

}//namespace test
