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
    test::hydro::Test( "Проверка перепада давления и зависимость перепада давления от расхода", 4 ),
    mRowsPrinted( 0 )
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
    obj.insert("OilTemp", OilTemp );

    return obj;
}
bool PressureDurationFromExpenditure::Deserialize( QJsonObject const& obj )
{
    OilTemp = obj.value("OilTemp").toDouble();
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
    mTablePrinted = false;
}

bool PressureDurationFromExpenditure::Draw( QPainter& painter, QRect &free_rect ) const
{
    test::hydro::Parameters *params = static_cast< test::hydro::Parameters * >( CURRENT_PARAMS );
    if ( !params )
        return true;

    QFont header_font = painter.font();
    QFont text_font = painter.font();
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
    res = DrawLine( num, free_rect, text_font, [ this, &painter ]( QRect const& rect )
    {
        QString s = "Средняя температура масла во время испытания: " + QString::number( OilTemp );
        painter.drawText( rect, s );
    });

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
    if (!mTablePrinted)
    {
        doc.setHtml( table + footer );
        QRect r;
        AllocatePlace( r, doc.documentLayout()->documentSize().height(), free_rect );
        doc.drawContents( &painter, rect);
        if ( mRowsPrinted == mData.size() )
            mTablePrinted = true;
    }
    painter.restore();


    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &text_font, &params ]( QRect const& rect )
    {
        ff0x::GraphBuilder::LinePoints B5A;
        ff0x::GraphBuilder::LinePoints B3A;
        ff0x::GraphBuilder::LinePoints B35A;

        ff0x::GraphBuilder::LinePoints B5B;
        ff0x::GraphBuilder::LinePoints B3B;
        ff0x::GraphBuilder::LinePoints B35B;

        foreach ( Channels const& item, mData )
        {
            B5A.push_back( QPointF( item.first.Expenditure, item.first.BP5 ) );
            B3A.push_back( QPointF( item.first.Expenditure, item.first.BP3 ) );
            B35A.push_back( QPointF( item.first.Expenditure, item.first.BP5_3 ) );

            B5B.push_back( QPointF( item.second.Expenditure, item.second.BP5 ) );
            B3B.push_back( QPointF( item.second.Expenditure, item.second.BP3 ) );
            B35B.push_back( QPointF( item.second.Expenditure, item.second.BP5_3 ) );
        }

        QFont f = text_font;
        f.setPointSize( 6 );
        ff0x::GraphBuilder builder ( rect.width(), rect.height(), f );
        ff0x::GraphBuilder::GraphDataLine lines;
        lines.push_back( ff0x::GraphBuilder::Line(B5A, Qt::blue) );
        lines.push_back( ff0x::GraphBuilder::Line(B3A, Qt::darkBlue) );
        lines.push_back( ff0x::GraphBuilder::Line(B35A, Qt::gray) );
        if (params->ReelCount() == 2 )
        {
            lines.push_back( ff0x::GraphBuilder::Line(B5B, Qt::red) );
            lines.push_back( ff0x::GraphBuilder::Line(B3B, Qt::darkRed) );
            lines.push_back( ff0x::GraphBuilder::Line(B35B, Qt::magenta) );
        }
        painter.drawPixmap( rect, builder.Draw( lines, 690, 360, 50, 50, "x", "y", true ) );
    }, free_rect.width()/4*3  );



    return res;
//    QPixmap pix( r.width(), r.height() );
//    QRect pix_r( 0,0, r.width(), r.height() );
//    QPainter pix_painter( &pix );
//    pix_painter.fillRect( pix_r, Qt::white );
//    pix_painter.setFont( text_font );
//    doc.drawContents( &pix_painter, pix_r);
//    painter.drawPixmap( r, pix );
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
