#include "pressure_duration_from_expediture.h"
#include <QJsonObject>
#include <QJsonArray>
#include "../test_params_hydro.h"
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

namespace test
{
namespace hydro
{

PressureDurationFromExpenditure::PressureDurationFromExpenditure():
    test::hydro::Test( "Проверка перепада давления и зависимость перепада давления от расхода", 4 ),
    mRowsPrinted( 0 )
{}

bool PressureDurationFromExpenditure::Run()
{
    mData.clear();
    Data d1;
    d1.BP3 = 0;
    Data d2;    
    d2.BP3 = 0;

    for ( size_t i = 0; i < 4; ++i )
    {
        mData.push_back( Channels( d1, d2 ) );
        ++d1.BP3; ++d2.BP3;
    }
    return true;
}

QJsonObject PressureDurationFromExpenditure::Serialise() const
{
    QJsonObject obj;

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

void PressureDurationFromExpenditure::ResetDrawLine()
{
    Test::ResetDrawLine();
    mRowsPrinted = 0;
}

bool PressureDurationFromExpenditure::Draw( QPainter& painter, QRect &free_rect ) const
{
    test::hydro::Parameters *params = static_cast< test::hydro::Parameters * >( CURRENT_PARAMS );
    if ( !params )
        return true;

    QFont header_font = painter.font();
    QFont text_font = painter.font();
    text_font.setFamily("  ");
    header_font.setPointSize( 14 );
    text_font.setPointSize( 12 );

    QFontMetrics head_metrix( header_font );

    uint32_t num = 0;
    bool res = DrawLine( num, free_rect, header_font,
    [ this, &painter, &head_metrix, &header_font ]( QRect const& rect )
    {
        QString n = "Проверка перепада давления и зависимость";
        QPoint start_point( rect.center().x() - head_metrix.width( n ) / 2, rect.bottom() );
        painter.setFont( header_font );
        painter.drawText( start_point, n );
    });
    res = DrawLine( num, free_rect, header_font,
    [ this, &painter, &head_metrix, &header_font ]( QRect const& rect )
    {
        QString n = "перепада давления от расхода";
        QPoint start_point( rect.center().x() - head_metrix.width( n ) / 2, rect.bottom() );
        painter.setFont( header_font );
        painter.drawText( start_point, n );
    });

    painter.setFont( text_font );
    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});

    painter.save();
    QRectF rect( 0, 0, free_rect.width(), free_rect.height() );
    painter.translate( free_rect.topLeft() );

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
            "<table width='100%' border='1' cellspacing='0' cellpadding='0'>"
               "<tr>"
                   "<th colspan='4'>Канал А</th>"
                   "<th colspan='4'>Канал Б</th>"
               "</tr>"
               "<tr>"
                   "<th>Давление<br>БР5, бар<br></th>"
                   "<th>Давление<br>БР3, бар<br></th>"
                   "<th>Перепад<br>БР5 - бр3, бар<br></th>"
                   "<th>Расход,<br>л/мин<br></th>"
                   "<th>Давление<br>БР5, бар</th>"
                   "<th>Давление<br>БР3, бар</th>"
                   "<th>Перепад<br>БР5 - бр3, бар <br></th>"
                   "<th>Расход,<br>л/мин</th>"
               "</tr>";

    QString footer = "</table>"
            "</body>"
            "</html>";

    QString table = header;
    auto MakeRow = [ &table, this, &params ]( int i ) -> QString
    {
        QString row =   "<tr>"
                            "<td>" + QString::number( mData[i].first.BP5 ) + "</td>"
                            "<td>" + QString::number( mData[i].first.BP3 ) + "</td>"
                            "<td>" + QString::number( mData[i].first.BP5_3 ) + "</td>"
                            "<td>" + QString::number( mData[i].first.Expenditure ) + "</td>";
        if ( params->ReelCount() == 2 )
        {
                row +=      "<td>" + QString::number( mData[i].second.BP5 ) + "</td>"
                            "<td>" + QString::number( mData[i].second.BP3 ) + "</td>"
                            "<td>" + QString::number( mData[i].second.BP5_3 ) + "</td>"
                            "<td>" + QString::number( mData[i].second.Expenditure ) + "</td>";
        }
        else
        {
            row +=          "<td></td>"
                            "<td></td>"
                            "<td></td>"
                            "<td></td>";
        }
          row +=        "</tr>";
        return row;
    };

    QTextDocument doc;
    doc.setUndoRedoEnabled( false );
    doc.setTextWidth( rect.width() );
    doc.setUseDesignMetrics( true );
    doc.setDefaultTextOption ( QTextOption (Qt::AlignHCenter )  );

    for ( auto i = mRowsPrinted; i < mData.size(); ++i )
    {
        QString tmp = table + MakeRow( i );
        doc.setHtml( tmp + footer );

        auto h = doc.documentLayout()->documentSize().height();

        if ( h < rect.height() )
        {
            ++mRowsPrinted;
            table = tmp;
        }
        else
        {
            res = false;
            break;
        }
    }
    doc.setHtml( table + footer );
    QRect r;
    AllocatePlace( r, doc.documentLayout()->documentSize().height(), free_rect );
    doc.drawContents( &painter, rect);
    painter.restore();
    return res;
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
