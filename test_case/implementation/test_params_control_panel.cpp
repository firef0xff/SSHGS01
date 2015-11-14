#include "test_params_control_panel.h"
#include <mutex>
#include <memory>
#include "../tests.h"

namespace test
{
namespace control_board
{
static std::shared_ptr< Parameters > PARAMS_INSTANCE;
static std::mutex PARAMS_INIT_MUTEX;


Parameters& Parameters::Instance()
{
    if ( !PARAMS_INSTANCE )
    {
        std::lock_guard< std::mutex > lock( PARAMS_INIT_MUTEX );
        if ( !PARAMS_INSTANCE )
        {
            PARAMS_INSTANCE.reset( new Parameters() );
        }
    }

    return *PARAMS_INSTANCE.get();
}

Parameters::Parameters():
    mGsType(""),
    mSignalType( ST_UNKNOWN ),
    mVoltage( 0 ),
    mMaxAmperage( 0 ),
    mReelResist( 0 )
{
    Reset();
}

void Parameters::Reset()
{
    test::Parameters::Reset();

    mGsType = "";
    mSignalType = ST_UNKNOWN;
    mVoltage = 0;
    mMaxAmperage = 0;
    mReelResist = 0;
}

QString Parameters::ToString() const
{
    QString res;
    res+= "Параметры платы:\n";
    res+= "  Тип: " + mGsType +"\n";
    res+= "  Тип входного сигнала: " + test::ToString( mVoltage ) +"\n";
    res+= "  Напряжение питания, В: " + test::ToString( mSignalType ) +"\n";
    res+= "  Максимальный выходной ток, А: " + test::ToString( mMaxAmperage ) +"\n";
    res+= "  Сопротивление катушки распределителя, Ом: " + test::ToString( mReelResist ) +"\n";
    res+= "\n";

    return res;
}

test::TestCase& Parameters::TestCollection() const
{
    return ControlBoardTests;
}

QJsonObject Parameters::Serialise() const
{
    QJsonObject obj;

    obj.insert("GsType", mGsType);
    obj.insert("Voltage", mVoltage);
    obj.insert("SignalType", mSignalType);
    obj.insert("MaxAmperage", mMaxAmperage);
    obj.insert("ReelResist", mReelResist);

    QJsonObject ret = test::Parameters::Serialise();
    ret.insert("control_board", obj);

    return ret;
}
bool Parameters::Deserialize(const QJsonObject &obj )
{
    Reset();
    bool ret = test::Parameters::Deserialize( obj );
    auto val = obj.value("control_board");
    if ( val.isObject() )
    {
        QJsonObject obj = val.toObject();

        mGsType  =  obj.value("GsType").toString();
        mVoltage  =  obj.value("Voltage").toDouble();
        mSignalType  =  static_cast<SIGNAL_TYPE>( obj.value("SignalType").toInt() );
        mMaxAmperage  =  obj.value("MaxAmperage").toDouble();
        mReelResist  =  obj.value("ReelResist").toDouble();

        ret = true;
    }
    else
        ret = false;

    return ret;
}

bool Parameters::Draw(QPainter &painter, QRect &free_rect ) const
{
    QFont title_font = painter.font();
    title_font.setFamily("Arial");
    title_font.setPointSize(18);

    QFont level_font = title_font;
    level_font.setPointSize( 14 );

    QFont text_font = title_font;
    text_font.setPointSize( 12 );

    auto DrawRowCenter = [ &painter, &free_rect ]( QFont font, QColor color, QString text, double spase = 1 )
    {
        painter.save();
        QFontMetrics metrix( font );
        QRect place;
        AllocatePlace( place, metrix.height()*spase ,free_rect );
        QPoint start_point( place.center().x() - metrix.width( text ) / 2, place.center().y() +metrix.height()/2);
        painter.setFont( font );
        painter.setPen( color );
        painter.drawText( start_point, text );
        painter.restore();
    };

    auto DrawRowLeft = [ &painter, &free_rect ]( QFont font, QColor color1, QColor color2,  QString label, QString value, double spase = 1 )
    {
        painter.save();
        QFontMetrics metrix( font );
        QRect place;
        AllocatePlace( place, metrix.height()*spase, free_rect );
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
    int width = m.width("12345678901234567890123456789012345678901234567890");
    char symbol = '.';
    auto FillToSize = [ width, &m, symbol ]( QString text )
    {
        while( m.width( text + symbol ) < width )
            text += symbol;
        return text + " ";
    };


    double row_skale = 2;

    DrawRowCenter( title_font, Qt::black, "ОТЧЕТ", row_skale );
    DrawRowCenter( level_font, Qt::black, "Испытания плат управления", row_skale );
    DrawRowCenter( level_font, Qt::red, mGsType, row_skale );

    DrawRowLeft( text_font, Qt::black, Qt::red, "Идентификационный номер: ", mGsType, row_skale);

    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Тип входного сигнала"), test::ToString(mSignalType), row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Напряжение питания платы, VDC"), test::ToString(mVoltage), row_skale );

    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Максимальный выходной ток платы, А"), test::ToString(mMaxAmperage), row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Сопротивление катушки распределителя, Ом"), test::ToString(mReelResist), row_skale );

    QString model_ser_no = test::ReadFromEtalone().value(ModelId()).toObject().value("Params").toObject().value("GsType").toString();
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Эталонный аппарат"), model_ser_no, row_skale );

    DrawRowLeft( text_font, Qt::black, Qt::red, "Испытания проводил: ", mUser, row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::red, "Дата проведения испытаний: ", mDate.toString("dd MMMM yyyy г. hh:mm"), row_skale );

    return true;
}

bool Parameters::GsType ( QString const& val )
{
    mGsType = val;
    return true;
}
QString const& Parameters::GsType () const
{
    return mGsType;
}

bool Parameters::SignalType ( QString const& val )
{
    return ParseValue( mSignalType, val );
}
SIGNAL_TYPE const& Parameters::SignalType () const
{
    return mSignalType;
}

bool Parameters::Voltage ( QString const& val )
{
    return ParseValue( mVoltage, val );
}
double const& Parameters::Voltage () const
{
    return mVoltage;
}

bool Parameters::MaxAmperage ( QString const& val )
{
    return ParseValue( mMaxAmperage, val );
}
double const& Parameters::MaxAmperage () const
{
    return mMaxAmperage;
}

bool Parameters::ReelResist ( QString const& val )
{
    return ParseValue( mReelResist, val );
}
double const& Parameters::ReelResist () const
{
    return mReelResist;
}

}//namespace control_board
}//namespace test
