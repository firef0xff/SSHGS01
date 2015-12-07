#include "test_params_servo.h"
#include <mutex>
#include <memory>
#include "../tests.h"
#include "../../cpu/cpu_memory.h"

namespace test
{

namespace servo
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
    mReelControl( RC_UNKNOWN ),
    mPressureNominal( 0 ),
    mPressureTesting( 0 ),
    mMaxExpenditureA( 0 ),
    mMaxExpenditureB( 0 ),
    mFrequencyInc( 0 ),
    mControlSignal(ST_UNKNOWN),
    mSignalStateA(0),
    mSignalStateB(0),
    mSignalState0(0),
    mEndSgnal(0),
    mControlReelResist(0)
{}

void Parameters::Reset()
{
    test::Parameters::Reset();

    mGsType = "";
    mReelControl = RC_UNKNOWN;
    mPressureNominal = 0;
    mPressureTesting = 0;
    mMaxExpenditureA = 0;
    mMaxExpenditureB = 0;
    mFrequencyInc = 0;

    mControlSignal = ST_UNKNOWN;
    mSignalStateA = 0;
    mSignalStateB = 0;
    mSignalState0 = 0;

    mEndSgnal = 0;
    mControlReelResist = 0;
}
QString Parameters::ToString() const
{
    QString res;
    res+= "Параметры аппаратуры управления направлением расхода:\n";
    res+= "  Тип гидрораспределителя: " + mGsType +"\n" ;
    res+= "  Серийный номер: " + mSerNo +"\n" ;
    res+= "  Тип управления: " + test::ToString( mReelControl ) + "\n";
    res+= "  Количество катушек питания: " + test::ToString( mReelCount ) + "\n";
    res+= "  Тип управления распределителем: " + test::ToString( mControlType ) + "\n";
    res+= "  Максимальное давление управления, Бар: " + test::ToString( mMaxControlPressure ) + "\n";
    res+= "  Минимальное давление управления, Бар: " + test::ToString( mMinControlPressure ) + "\n";
    res+= "  Уровень управляющих сигналов: " + test::ToString( mControlSignal ) + "\n";
    res+= "Парамеры для аппаратуры с типом управления: " + test::ToString( RC_CONTROL_BOX );
    res+= "  Сигнал, соответствующий полному переключению в состояние А: " + test::ToString( mSignalStateA ) + "\n";
    res+= "  Сигнал, соответствующий полному переключению в состояние Б: " + test::ToString( mSignalStateB ) + "\n";
    res+= "  Сигнал, соответствующий нулевому положению: " + test::ToString( mSignalState0 ) + "\n";
    res+= "Парамеры для аппаратуры с типом управления: " + test::ToString( RC_REEL );
    res+= "  Сигнал, соответствующий переключению аппарата в конечное состоние: " + test::ToString( mEndSgnal ) + "\n";
    res+= "  Сопротивление катушки управления, Ом: " + test::ToString( mSignalState0 ) + "\n";
    res+= "\n";
    res+= "Параметры стенда:\n";
    res+= "  Максимальный расход, л/мин: " + test::ToString( mMaxExpenditure ) + "\n";
    res+= "  Номинальное давление, Бар: " + test::ToString( mPressureNominal ) + "\n";
    res+= "\n";
    res+= "Параметры испытаний:\n";
    res+= "  Давления при испытании аппарата пробным давлением, Бар: " + test::ToString( mPressureTesting ) + "\n";
    res+= "  Максимальный расход в канале А, при следующем опорном сигнале, X_max_A, л/мин: " + test::ToString( mMaxExpenditureA ) + "\n";
    res+= "  Максимальный расход в канале Б, при следующем опорном сигнале, X_max_Б, л/мин: " + test::ToString( mMaxExpenditureB ) + "\n";
    res+= "  Инкремент частоты при построении частотных характеристик, Гц: " + test::ToString( mFrequencyInc ) + "\n";

    return res;
}

test::TestCase& Parameters::TestCollection() const
{
    return ServoTests;
}

QJsonObject Parameters::Serialise() const
{
    QJsonObject res = test::CommonParameters::Serialise();
    QJsonObject servo;
    servo.insert("GsType", mGsType);
    servo.insert("ReelControl", mReelControl);
    servo.insert("PressureNominal", mPressureNominal);
    servo.insert("PressureTesting", mPressureTesting);
    servo.insert("MaxExpenditureA", mMaxExpenditureA);
    servo.insert("MaxExpenditureB", mMaxExpenditureB);
    servo.insert("FrequencyInc", mFrequencyInc);

    servo.insert("ControlSignal", mControlSignal);
    servo.insert("SignalStateA", mSignalStateA);
    servo.insert("SignalStateB", mSignalStateB);
    servo.insert("SignalState0", mSignalState0);

    servo.insert("EndSgnal", mEndSgnal);
    servo.insert("ControlReelResist", mControlReelResist);

    res.insert("servo", servo);
    return res;
}
bool Parameters::Deserialize( QJsonObject const& obj )
{
    Reset();
    bool res = test::CommonParameters::Deserialize( obj );

    auto val = obj.value("servo");
    if ( val.isObject() )
    {
        QJsonObject obj = val.toObject();

        mGsType = obj.value("GsType").toString();
        mReelControl = static_cast<RELL_CONTROL>(obj.value("ReelControl").toInt());
        mPressureNominal = obj.value("PressureNominal").toInt();
        mPressureTesting = obj.value("PressureTesting").toInt();
        mMaxExpenditureA = obj.value("MaxExpenditureA").toInt();
        mMaxExpenditureB = obj.value("MaxExpenditureB").toInt();
        mFrequencyInc = obj.value("FrequencyInc").toDouble();

        mControlSignal = static_cast<SIGNAL_TYPE>(obj.value("ControlSignal").toInt());
        mSignalStateA = obj.value("SignalStateA").toDouble();
        mSignalStateB = obj.value("SignalStateB").toDouble();
        mSignalState0 = obj.value("SignalState0").toDouble();

        mEndSgnal = obj.value("EndSgnal").toDouble();
        mControlReelResist = obj.value("ControlReelResist").toDouble();
        res = true;
    }
    else
        res = false;

    return res;
}

void Parameters::WriteToController() const
{
    if ( test::servo::Parameters::Instance().ReelControl() == RC_REEL )
    {
        auto& mem = cpu::CpuMemory::Instance().DB35;
    }
    else
    {
        auto& mem = cpu::CpuMemory::Instance().DB33;
    }
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

    auto DrawLastRow = [ &painter, &free_rect ]( QFont font, QColor color, QString text, double spase = 1 )
    {
        painter.save();
        QFontMetrics metrix( font );
        QRect place;
        QRect draw_place;
        while ( AllocatePlace( place, metrix.height()*spase ,free_rect ) )
        {
            draw_place = place;
        }
        QPoint start_point( place.left() , place.center().y()+metrix.height()/2 );
        painter.setFont( font );
        painter.setPen( color );
        painter.drawText( start_point, text );
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
    DrawRowCenter( level_font, Qt::black, "Испытания пропорционального аппарата", row_skale );
    DrawRowCenter( level_font, Qt::red, mGsType, row_skale );

    DrawRowLeft( text_font, Qt::black, Qt::red, "Идентификационный номер: ", mSerNo, row_skale);
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Номинальное давление, бар"), test::ToString( mPressureNominal ), row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Максимальный расход, л/мин"), test::ToString( mMaxExpenditure ), row_skale );
    if ( mControlType == CT_ELECTRIC )
    {
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Максимальное давление управления*, бар"), "-", row_skale );
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Минимальное давление управления*, бар"), "-", row_skale );
    }
    else
    {
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Максимальное давление управления*, бар"), test::ToString(mMaxControlPressure), row_skale );
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Минимальное давление управления*, бар"), test::ToString(mMinControlPressure), row_skale );
    }

    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Тип управления"), test::ToString(mReelControl), row_skale );

    DrawRowLeft( text_font, Qt::black, Qt::red, "Сигнал, соответствующий:", "" );
    QString ed_izm = mControlSignal == ST_10_10_V ? " В": " мА";
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("- полному переключению в положение А"), test::ToString(mSignalStateA) + ed_izm, row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("- полному переключению в положение В"), test::ToString(mSignalStateB) + ed_izm, row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("- нулевому положению"), test::ToString(mSignalState0) + ed_izm, row_skale );

    DrawRowLeft( text_font, Qt::black, Qt::red, "Максимальный расход:", "" );
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("- в канале А при максимально опорном сигнале, л/мин"), test::ToString(mMaxExpenditureA) + ed_izm, row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("- в канале В при максимально опорном сигнале, л/мин"), test::ToString(mMaxExpenditureB) + ed_izm, row_skale );

    DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Амплитуды управляющих сигналов, %"), "не реализовано", row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Инкремент частоты, Гц"), test::ToString(mFrequencyInc), row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Тип масла"), "Лукойл Гейзер HLP32", row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Вязкость масла (при 40˚С), сСт"), test::ToString(32), row_skale );

    QString model_ser_no = test::ReadFromEtalone().value(ModelId()).toObject().value("Params").toObject().value("SerNo").toString();

    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Эталонный аппарат"), model_ser_no, row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Класс чистоты жидкости (по ISO 4406)"), "17/15/12", row_skale );

    DrawRowCenter( text_font, Qt::black, "", row_skale );

    DrawRowLeft( text_font, Qt::black, Qt::red, "Испытания проводил: ", mUser, row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::red, "Дата проведения испытаний: ", mDate.toString("dd MMMM yyyy г. hh:mm"), row_skale );

    DrawLastRow( text_font, Qt::black, "*Для распределителей с электрогидравлическим управлением");
    return true;
}

QString Parameters::ModelId() const
{
    return mGsType;
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


bool Parameters::ReelControl ( QString const& val )
{
    return ParseValue( mReelControl, val );
}
RELL_CONTROL const& Parameters::ReelControl () const
{
    return mReelControl;
}

bool Parameters::PressureNominal ( QString const& val )
{
    return ParseValue( mPressureNominal, val );
}
qint32 const& Parameters::PressureNominal () const
{
    return mPressureNominal;
}

bool Parameters::PressureTesting ( QString const& val )
{
    return ParseValue( mPressureTesting, val );
}
qint32 const& Parameters::PressureTesting () const
{
    return mPressureTesting;
}

bool Parameters::MaxExpenditureA ( QString const& val )
{
    return ParseValue( mMaxExpenditureA, val );
}
qint32 const& Parameters::MaxExpenditureA () const
{
    return mMaxExpenditureA;
}

bool Parameters::MaxExpenditureB ( QString const& val )
{
    return ParseValue( mMaxExpenditureB, val );
}
qint32 const& Parameters::MaxExpenditureB () const
{
    return mMaxExpenditureB;
}

bool Parameters::FrequencyInc ( QString const& val )
{
    return ParseValue( mFrequencyInc, val );
}
const double &Parameters::FrequencyInc() const
{
    return mFrequencyInc;
}

bool Parameters::ControlSignal ( QString const& val )
{
    return ParseValue( mControlSignal, val );
}
const SIGNAL_TYPE &Parameters::ControlSignal() const
{
    return mControlSignal;
}

bool Parameters::SignalStateA ( QString const& val )
{
    return ParseValue( mSignalStateA, val );
}
const double &Parameters::SignalStateA() const
{
    return mSignalStateA;
}

bool Parameters::SignalStateB ( QString const& val )
{
    return ParseValue( mSignalStateB, val );
}
const double &Parameters::SignalStateB() const
{
    return mSignalStateB;
}

bool Parameters::SignalState0 ( QString const& val )
{
    return ParseValue( mSignalState0, val );
}
const double &Parameters::SignalState0() const
{
    return mSignalState0;
}

bool Parameters::EndSgnal ( QString const& val )
{
    return ParseValue( mEndSgnal, val );
}
const double &Parameters::EndSgnal() const
{
    return mEndSgnal;
}

bool Parameters::ControlReelResist ( QString const& val )
{
    return ParseValue( mControlReelResist, val );
}
const double &Parameters::ControlReelResist() const
{
    return mControlReelResist;
}

}//namespace servo

}//namespace test
