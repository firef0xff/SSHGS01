#include "test_params_hydro_cilinder.h"
#include "../tests.h"
#include <memory>
#include <mutex>

namespace test
{
namespace hydro_cylinder
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
    mSerNo(""),
    mMaxPressure(0),
    mMoveTime(0),
    mTestPressure(0),
    mHermTestTime(0),
    mExpenditure(0)
{
    Reset();
}

void Parameters::Reset()
{
    test::Parameters::Reset();
    mSerNo = "";
    mMaxPressure = 0;
    mMoveTime = 0;
    mTestPressure = 0;
    mHermTestTime = 0;
    mExpenditure = 0;
}

QString Parameters::ToString() const
{
    QString res;
    res+= "Параметры гидроцилиндра:\n";
    res+= "  Серийниый номер: " + mSerNo +"\n";
    res+= "  Максимальное давление нагрузки, Бар: " + test::ToString( mMaxPressure ) +"\n";
    res+= "  Номинальный расход, л/мин: " + test::ToString( mExpenditure ) +"\n";
    res+= "Параметры испытаний:\n";
    res+= "  Давление при испытании функционирования, Бар: " + test::ToString( mTestPressure ) +"\n";
    res+= "  Время перемещения в каждую сторону при испытании функционирования, с: " + test::ToString( mMoveTime ) +"\n";
    res+= "  Время испытания наружной герметичности, с: " + test::ToString( mHermTestTime ) +"\n";
    res+= "\n";

    return res;
}

test::TestCase& Parameters::TestCollection() const
{
    return HydroCylinder;
}

QJsonObject Parameters::Serialise() const
{
    QJsonObject obj;

    QJsonObject ret = test::Parameters::Serialise();
    obj.insert("SerNo", mSerNo);
    obj.insert("MaxPressure", mMaxPressure);
    obj.insert("MoveTime", mMoveTime);
    obj.insert("TestPressure",mTestPressure);
    obj.insert("HermTestTime",mHermTestTime);
    obj.insert("Expenditure",mExpenditure);

    ret.insert("hydro_cylinder", obj);

    return ret;
}
bool Parameters::Deserialize(const QJsonObject &obj )
{
    Reset();
    bool ret = test::Parameters::Deserialize( obj );
    auto val = obj.value("hydro_cylinder");
    if ( val.isObject() )
    {
        QJsonObject obj = val.toObject();

        mSerNo = obj.value("SerNo").toString();
        mMaxPressure = obj.value("MaxPressure").toInt();
        mMoveTime = obj.value("MoveTime").toDouble();
        mTestPressure = obj.value("TestPressure").toInt();
        mHermTestTime = obj.value("HermTestTime").toInt();
        mExpenditure = obj.value("Expenditure").toInt();

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
    DrawRowCenter( level_font, Qt::black, "Испытания гидроцилиндра", row_skale );
    DrawRowCenter( level_font, Qt::red, "тип цилидра? откуда брать?", row_skale );

    DrawRowLeft( text_font, Qt::black, Qt::red, "Идентификационный номер: ", mSerNo, row_skale);
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Максимальное давление, бар"), test::ToString( mMaxPressure ), row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Рабочее давление, бар"), test::ToString( mTestPressure ), row_skale );

    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Номинальный расход, л/мин"), test::ToString( mExpenditure ), row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Время перемещения в одну сторону, сек"), test::ToString( mMoveTime ), row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Время испытания наружной герметичности, сек"), test::ToString( mHermTestTime ), row_skale );


    DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Тонкость фильтрации рабочей жидкости, мкм"), test::ToString(3), row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Тип масла"), "Лукой Гейзер HLP32", row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Вязкость масла (при 40˚С), сСт"), test::ToString(32), row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Класс чистоты жидкости (по ISO 4406)"), "17/15/12", row_skale );

    DrawRowLeft( text_font, Qt::black, Qt::red, "Испытания проводил: ", mUser, row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::red, "Дата проведения испытаний: ", mDate.toString("dd MMMM yyyy г. hh:mm"), row_skale );

    return true;
}

bool Parameters::SerNo ( QString const& val )
{
    mSerNo = val;
    return true;
}
QString const& Parameters::SerNo () const
{
    return mSerNo;
}

bool Parameters::MaxPressure ( QString const& val )
{
    return ParseValue( mMaxPressure, val );
}
double const& Parameters::MaxPressure () const
{
    return mMaxPressure;
}

bool Parameters::MoveTime ( QString const& val )
{
    return ParseValue( mMoveTime, val );
}
double const& Parameters::MoveTime () const
{
    return mMoveTime;
}

bool Parameters::TestPressure ( QString const& val )
{
    return ParseValue( mTestPressure, val );
}
double const& Parameters::TestPressure () const
{
    return mTestPressure;
}

bool Parameters::HermTestTime ( QString const& val )
{
    return ParseValue( mHermTestTime, val );
}
double const& Parameters::HermTestTime () const
{
    return mHermTestTime;
}

bool Parameters::Expenditure ( QString const& val )
{
    return ParseValue( mExpenditure, val );
}
double const& Parameters::Expenditure () const
{
    return mExpenditure;
}

}//namespace hydro_cylinder

}//namespace test
