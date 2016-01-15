#include "cylinder_test.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QPushButton>
#include "../test_params_hydro_cilinder.h"
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <mutex>

namespace test
{
namespace hydro_cylinder
{

FunctionalTest::FunctionalTest():
    test::hydro_cylinder::Test( "Испытание функционирования", 32 ),
    HermResult(false),
    MaxPressureResult(false),
    WorkPressureResult(false),
    ExpenditureResult(false),
    MoveTimeResult(false)
{}

bool FunctionalTest::Run()
{
    Start();
    Wait( mBits.op32_ok, mBits.op32_end );
    if ( IsStopped() )
        return false;

    OilTemp = round( mTemperature.T_oil *100)/100;

    std::mutex mutex;
    std::unique_lock< std::mutex > lock( mutex );
    Launcher( std::bind( &FunctionalTest::Question, this ) );

    mCondVar.wait( lock );

    return Success();
}

QJsonObject FunctionalTest::Serialise() const
{
    QJsonObject obj;
    obj.insert("HermResult",            HermResult );
    obj.insert("MaxPressureResult",     MaxPressureResult );
    obj.insert("WorkPressureResult",    WorkPressureResult );
    obj.insert("ExpenditureResult",     ExpenditureResult );
    obj.insert("MoveTimeResult",        MoveTimeResult );

    return obj;
}
bool FunctionalTest::Deserialize( QJsonObject const& obj )
{
    HermResult = obj.value("HermResult").toBool();
    MaxPressureResult = obj.value("MaxPressureResult").toBool();
    WorkPressureResult = obj.value("WorkPressureResult").toBool();
    ExpenditureResult = obj.value("ExpenditureResult").toBool();
    MoveTimeResult = obj.value("MoveTimeResult").toBool();
    return true;
}

bool FunctionalTest::Success() const
{
    return HermResult && MaxPressureResult && WorkPressureResult && ExpenditureResult && MoveTimeResult;
}
bool FunctionalTest::Draw( QPainter& painter, QRect &free_rect ) const
{
    test::hydro_cylinder::Parameters *params = static_cast< test::hydro_cylinder::Parameters * >( CURRENT_PARAMS );
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
                                                    QString const& value = "",
                                                    QColor const& color3 = Qt::black,
                                                    QString const& value2 = "")
    {
        painter.save();
        QFontMetrics metrix( font );
        QPoint start_point( place.left() , place.center().y()+metrix.height()/2 );
        QPoint start_point2( start_point.x() + metrix.width(label), place.center().y() +metrix.height()/2);
        QPoint start_point3( start_point2.x() + metrix.width(value), place.center().y() +metrix.height()/2);
        painter.setFont( font );
        painter.setPen( color1 );
        painter.drawText( start_point, label );
        painter.setPen( color2 );
        painter.drawText( start_point2, value );
        painter.setPen( color3 );
        painter.drawText( start_point3, value2 );
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
    bool res = DrawLine( num, free_rect, result_font,
    [ this, &painter, &DrawRowCenter, &result_font ]( QRect const& rect )
    {
        DrawRowCenter( rect, result_font, Qt::black, "Результаты испытаний" );
    }, 2 );

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Температура масла во время испытаний, ˚С"), Qt::red, test::ToString(OilTemp) );
    }, 2 );


    QString header = "<html>"
            "<head>"
              "<meta charset='utf-8'>"
              "<style type='text/css'>"
                   "td { text-align: center;}"
                   "th { font-weight: normal; padding: 2px;}"
                   "table {border-collapse: collapse; border-style: solid; vertical-align:middle;}"
             "</style>"
            "</head>"
            "<body>"
            "<table width='100%' border='1.5' cellspacing='-0.5' cellpadding='-0.5'>"
               "<tr>"
                   "<th> Номер </th>"
                   "<th></th>"
                   "<th> Работоспособность </th>"
               "</tr>";

    QString footer = "</table>"
            "</body>"
            "</html>";

    typedef std::pair<QString, bool> Item;
    std::vector< Item > tests;
    tests.push_back( Item( "Наружная герметичность ", HermResult ) );
    tests.push_back( Item( "Максимальное давление", MaxPressureResult ) );
    tests.push_back( Item( "Рабочее давление", WorkPressureResult ) );
    tests.push_back( Item( "Номинальный расход", ExpenditureResult ) );
    tests.push_back( Item( "Время перемещения в одну сторону", MoveTimeResult ) );

    QString rows;
    for ( size_t i =  1, end = tests.size(); i <= end; ++i )
    {
        Item& data = tests[i-1];
        rows +=  "<tr>"
                   "<td>"+test::ToString( static_cast<int>(i) )+"</td>"
                   "<td>"+ data.first +"</td>"
                   "<td style='font-size:28pt; color: \"red\"; font-weight:bold;'>"+ (data.second ? QString("+"):QString("-")) +"</td>"
                "</tr>";
    }

    QString table = header + rows + footer;


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

    res = DrawLine( num, free_rect, header_font,
    [ this, &painter, &DrawRowLeft, &header_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, header_font, Qt::black, "ИТОГ:" );
    }, 1.5 );

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font, params ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font,   Qt::black, "Гидроцилиндр ",
                                        Qt::red, params->SerNo() + (Success()? QString(" годен ") : QString(" не годен ")),
                                        Qt::black, " к эксплуатации");
    }, 3 );
    return res;
}
void FunctionalTest::Question()
{
    QMessageBox msg;
    msg.setWindowTitle( "Визуальный контроль" );
    msg.setText( "Обнаружена ли течь или потение наружных\nповерхностей на испытуемом гидроцилиндре" );
    QPushButton *no = msg.addButton("Нет", QMessageBox::NoRole );
    QPushButton *yes = msg.addButton("Да", QMessageBox::YesRole );
    msg.setModal( true );
    no->setDefault( false );
    yes->setDefault( false );
    msg.exec();
    HermResult = msg.clickedButton() == no;
    mCondVar.notify_all();
}

}//namespace hydro_cylinder

}//namespace test
