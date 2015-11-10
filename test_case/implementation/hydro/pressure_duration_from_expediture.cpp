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
    Wait( mResults.OP4_Work, mResults.OP4_End );

    mData.clear();
    Data a1;
    a1.BP5 = mResults.OP4_D_P_03Qmax_A;
    a1.BP3 = mResults.OP4_D_A_03Qmax_A;
    a1.BP5_3 = mResults.OP4_BP5_BP3_03Qmax_A;
    a1.Expenditure = mResults.OP4_Q_Fakt_03Qmax_A;

    Data a2;
    a2.BP5 = mResults.OP4_D_P_06Qmax_A;
    a2.BP3 = mResults.OP4_D_A_06Qmax_A;
    a2.BP5_3 = mResults.OP4_BP5_BP3_06Qmax_A;
    a2.Expenditure = mResults.OP4_Q_Fakt_06Qmax_A;

    Data a3;
    a3.BP5 = mResults.OP4_D_P_Qmax_A;
    a3.BP3 = mResults.OP4_D_A_Qmax_A;
    a3.BP5_3 = mResults.OP4_BP5_BP3_Qmax_A;
    a3.Expenditure = mResults.OP4_Q_Fakt_Qmax_A;


    Data b1;
    b1.BP5 = mResults.OP4_D_P_03Qmax_B;
    b1.BP3 = mResults.OP4_D_A_03Qmax_B;
    b1.BP5_3 = mResults.OP4_BP5_BP3_03Qmax_B;
    b1.Expenditure = mResults.OP4_Q_Fakt_03Qmax_B;

    Data b2;
    b2.BP5 = mResults.OP4_D_P_06Qmax_B;
    b2.BP3 = mResults.OP4_D_A_06Qmax_B;
    b2.BP5_3 = mResults.OP4_BP5_BP3_06Qmax_B;
    b2.Expenditure = mResults.OP4_Q_Fakt_06Qmax_B;

    Data b3;
    b3.BP5 = mResults.OP4_D_P_Qmax_B;
    b3.BP3 = mResults.OP4_D_A_Qmax_B;
    b3.BP5_3 = mResults.OP4_BP5_BP3_Qmax_B;
    b3.Expenditure = mResults.OP4_Q_Fakt_Qmax_B;

    mData.push_back( Channels( a1, b1 ) );
    mData.push_back( Channels( a2, b2 ) );
    mData.push_back( Channels( a3, b3 ) );

    OilTemp = mResults.Temperatura_masla;

    return Success();
}

QJsonObject PressureDurationFromExpenditure::Serialise() const
{
    QJsonObject obj = Test::Serialise();

    QJsonArray a;

    foreach ( Channels const& d, mData )
    {
        QJsonObject o;
        o.insert( "ChannelA", d.first.Serialise() );
        o.insert( "ChannelB", d.second.Serialise() );

        a.insert( a.end(), o );
    }
    obj.insert("Data", a );

    return obj;
}
bool PressureDurationFromExpenditure::Deserialize( QJsonObject const& obj )
{
    Test::Deserialize( obj );
    QJsonArray a = obj.value("Data").toArray();
    foreach ( QJsonValue const& v, a )
    {
        QJsonObject o = v.toObject();
        Data d1;
        Data d2;
        if ( d1.Deserialize( o.value( "ChannelA" ).toObject() ) &&
             d2.Deserialize( o.value( "ChannelB" ).toObject() ) )
            mData.push_back( Channels( d1, d2 ) );
    }
    return true;
}

bool PressureDurationFromExpenditure::Draw( QPainter& painter, QRect &free_rect ) const
{
    test::hydro::Parameters *params = static_cast< test::hydro::Parameters * >( CURRENT_PARAMS );
    if ( !params )
        return true;

    QFont header_font = painter.font();
    header_font.setFamily("Arial");
    QFont result_font = header_font;
    result_font.setUnderline(true);
    QFont text_font = header_font;
    header_font.setPointSize( 14 );
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
        ff0x::GraphBuilder::LinePoints B35A;
        ff0x::GraphBuilder::LinePoints B35B;

        ff0x::GraphBuilder::LinePoints B35Ae;
        ff0x::GraphBuilder::LinePoints B35Be;


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
                    Data d1;
                    Data d2;
                    if ( d1.Deserialize( o.value( "ChannelA" ).toObject() ) &&
                         d2.Deserialize( o.value( "ChannelB" ).toObject() ) )
                    {
                        B35Ae.push_back( QPointF( d1.Expenditure, d1.BP5_3 ) );
                        B35Be.push_back( QPointF( d2.Expenditure, d2.BP5_3 ) );
                    }
                }
            }
        }

        foreach ( Channels const& item, mData )
        {
            B35A.push_back( QPointF( item.first.Expenditure, item.first.BP5_3 ) );
            B35B.push_back( QPointF( item.second.Expenditure, item.second.BP5_3 ) );
        }

        QFont f = text_font;
        f.setPointSize( 6 );
        int w = (rect.height() - metrix.height())*0.98;
        int h = (rect.height() - metrix.height())*0.98;
        QRect p1(rect.left(), rect.top(), w, h );
        QRect p2(rect.right() - w, rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
        QRect p2t(p2.left(), p2.bottom(), p2.width(), metrix.height());;
        ff0x::GraphBuilder builder ( w, h, ff0x::GraphBuilder::PlusPlus ,f );
        ff0x::GraphBuilder::GraphDataLine lines1;
        ff0x::GraphBuilder::GraphDataLine lines2;
        lines1.push_back( ff0x::GraphBuilder::Line(B35A, ff0x::GraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !B35Ae.empty() )
            lines1.push_back( ff0x::GraphBuilder::Line(B35Ae, ff0x::GraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );
        if (params->ReelCount() == 2 )
        {
            lines2.push_back( ff0x::GraphBuilder::Line(B35B, ff0x::GraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
            if ( !B35Be.empty() )
                lines2.push_back( ff0x::GraphBuilder::Line(B35Be, ff0x::GraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );
        }
        painter.drawPixmap( p1, builder.Draw( lines1, 690, 360, 50, 50, "", "", true ) );
        painter.drawPixmap( p2, builder.Draw( lines2, 690, 360, 50, 50, "", "", true ) );


        DrawRowCenter( p1t, text_font, Qt::black, "График: «перепад Р-->A»" );
        DrawRowCenter( p2t, text_font, Qt::black, "График: «перепад Р-->B»" );

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
