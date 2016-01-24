#include "test_params_hydro.h"
#include <memory>
#include <mutex>
#include "../tests.h"
#include "../../cpu/cpu_memory.h"
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

namespace test
{

namespace hydro
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
    mVoltage( 0.0 ),
    mVoltageRange( 0.0 ),
    mLost( 0),
    mVoltageType( VT_UNKNOWN ),
    mMaxWorkPressure ( 0.0 ),
    mMinTestPressure ( 0.0 ),
    mHermPressure( 0.0 ),
    mTestControlPressure( 0.0 ),
    mHermSignal ( CS_UNKNOWN ),
    mPABTSignal ( CS_UNKNOWN ),
    mPBATSignal ( CS_UNKNOWN ),
    mActuationOnTime ( 0.0 ),
    mActuationOffTime ( 0.0 ),
    mOnDD1A( false ),
    mOnDD2A( false ),
    mOnDD3A( false ),
    mOffDD1A( false ),
    mOffDD2A( false ),
    mOffDD3A( false ),
    mOnDD1B( false ),
    mOnDD2B( false ),
    mOnDD3B( false ),
    mOffDD1B( false ),
    mOffDD2B( false ),
    mOffDD3B( false ),
    mOnDynamic_1 ( DN_UNKNOWN ),
    mOffDynamic_1 ( DN_UNKNOWN ),
    mOnDynamic_2 ( DN_UNKNOWN ),
    mOffDynamic_2 ( DN_UNKNOWN )
{
    Reset();
}

void Parameters::Reset()
{
    test::Parameters::Reset();
    mGsType = "";
    mVoltage =  0.0;
    mVoltageRange =  0.0;
    mLost = 0;
    mVoltageType = VT_UNKNOWN;
    mMaxWorkPressure  =  0.0;
    mMinTestPressure  =  0.0;
    mHermPressure =  0.0;
    mTestControlPressure = 0.0;
    mHermSignal  =  CS_UNKNOWN;
    mPABTSignal  =  CS_UNKNOWN;
    mPBATSignal  =  CS_UNKNOWN;
    mActuationOnTime  =  0.0;
    mActuationOffTime  =  0.0;

    mOnDD1A = false;
    mOnDD2A = false;
    mOnDD3A = false;
    mOffDD1A = false;
    mOffDD2A = false;
    mOffDD3A = false;
    mOnDD1B = false;
    mOnDD2B = false;
    mOnDD3B = false;
    mOffDD1B = false;
    mOffDD2B = false;
    mOffDD3B = false;

    mOnDynamic_1 = DN_UNKNOWN;
    mOffDynamic_1 = DN_UNKNOWN;
    mOnDynamic_2 = DN_UNKNOWN;
    mOffDynamic_2 = DN_UNKNOWN;
}

QString Parameters::ToString() const
{
    QString res;
    res+= "Параметры гидрораспределителя:\n";
    res+= "  Серийный номер: " + mSerNo +"\n" ;
    res+= "  Тип: " + mGsType +"\n";
    res+= "  Напряжение питания, В: " + test::ToString( mVoltage ) +"\n";
    res+= "  Тип питающего напряжения: " + test::ToString( mVoltageType ) + "\n";
    res+= "  Допустимое отклонение напрядения питания, %: " + test::ToString( mVoltageRange ) + "\n";
    res+= "  Допустимое значение утечки, %: " + test::ToString( mLost ) + "\n";
    res+= "  Количество катушек питания: " + test::ToString( mReelCount ) + "\n";
    res+= "  Тип управления: " + test::ToString( mControlType ) + "\n";
    res+= "  Максимальное давление управления, Бар: " + test::ToString( mMaxControlPressure ) + "\n";
    res+= "  Минимальное давление управления, Бар: " + test::ToString( mMinControlPressure ) + "\n";
    res+= "\n";
    res+= "Параметры стенда:\n";
    res+= "  Максимальный расход, л/мин: " + test::ToString( mMaxExpenditure ) + "\n";
    res+= "  Максимальное рабочее давление, Бар: " + test::ToString( mMaxWorkPressure ) + "\n";
    res+= "  Время срабатывания включения распределителя, мсек: " + test::ToString( mActuationOnTime ) + "\n";
    res+= "  Время срабатывания выключения распределителя, мсек: " + test::ToString( mActuationOffTime ) + "\n";
    res+= "\n";
    res+= "Параметры испытаний:\n";
    res+= "  Давление для испытания функционирования минимальным давлением, Бар: " + test::ToString( mMinTestPressure ) + "\n";
    res+= "  Давление для испытания функционирования минимальным давлением, Бар: " + test::ToString( mHermPressure ) + "\n";
    res+= "  Давление управления для испытания времени прерключения, Бар: " + test::ToString( mTestControlPressure ) + "\n";
    res+= "  Сигнал проверки внутренней герметичности: " + test::ToString( mHermSignal ) + "\n";
    res+= "  Сигнал PABT: " + test::ToString( mPABTSignal ) + "\n";
    res+= "  Сигнал PBAT: " + test::ToString( mPBATSignal ) + "\n";
    res+= "  Диагностика включения катушки:\n";
    res+= "    Катушка А:\n";
    res+= "      Датчики по которым определяется включение: \n";
    res+= "        ДД1:" + test::ToString( mOnDD1A ) + "\n";
    res+= "        ДД2:" + test::ToString( mOnDD2A ) + "\n";
    res+= "        ДД3:" + test::ToString( mOnDD3A ) + "\n";
    res+= "      Датчики по которым определяется выключение: \n";
    res+= "        ДД1:" + test::ToString( mOffDD1A ) + "\n";
    res+= "        ДД2:" + test::ToString( mOffDD2A ) + "\n";
    res+= "        ДД3:" + test::ToString( mOffDD3A ) + "\n";
    res+= "      Ожидаемая динамика после включения: " + test::ToString( mOnDynamic_1 ) + "\n";
    res+= "      Ожидаемая динамика после выключения: " + test::ToString( mOffDynamic_1 ) + "\n";
    res+= "    Катушка Б:\n";
    res+= "      Датчики по которым определяется включение: \n";
    res+= "        ДД1:" + test::ToString( mOnDD1B ) + "\n";
    res+= "        ДД2:" + test::ToString( mOnDD2B ) + "\n";
    res+= "        ДД3:" + test::ToString( mOnDD3B ) + "\n";
    res+= "      Датчики по которым определяется выключение: \n";
    res+= "        ДД1:" + test::ToString( mOffDD1B ) + "\n";
    res+= "        ДД2:" + test::ToString( mOffDD2B ) + "\n";
    res+= "        ДД3:" + test::ToString( mOffDD3B ) + "\n";
    res+= "      Ожидаемая динамика после включения: " + test::ToString( mOnDynamic_2 ) + "\n";
    res+= "      Ожидаемая динамика после выключения: " + test::ToString( mOffDynamic_2 ) + "\n";

    return res;
}

test::TestCase& Parameters::TestCollection() const
{
    return HydroTests;
}

QJsonObject Parameters::Serialise() const
{
    QJsonObject obj;
    obj.insert("GsType", mGsType);
    obj.insert("Voltage", mVoltage);
    obj.insert("VoltageRange", mVoltageRange);
    obj.insert("Lost", mLost);
    obj.insert("VoltageType", mVoltageType);
    obj.insert("MaxWorkPressure", mMaxWorkPressure);
    obj.insert("MinTestPressure", mMinTestPressure);
    obj.insert("HermPressure", mHermPressure);
    obj.insert("TestControlPressure", mTestControlPressure);
    obj.insert("HermSignal", mHermSignal);
    obj.insert("PABTSignal", mPABTSignal);
    obj.insert("PBATSignal", mPBATSignal);
    obj.insert("ActuationOnTime", mActuationOnTime);
    obj.insert("ActuationOffTime", mActuationOffTime);

    obj.insert("OnDD1A", mOnDD1A);
    obj.insert("OnDD2A", mOnDD2A);
    obj.insert("OnDD3A", mOnDD3A);
    obj.insert("OffDD1A", mOffDD1A);
    obj.insert("OffDD2A", mOffDD2A);
    obj.insert("OffDD3A", mOffDD3A);
    obj.insert("OnDD1B", mOnDD1B);
    obj.insert("OnDD2B", mOnDD2B);
    obj.insert("OnDD3B", mOnDD3B);
    obj.insert("OffDD1B", mOffDD1B);
    obj.insert("OffDD2B", mOffDD2B);
    obj.insert("OffDD3B", mOffDD3B);

    obj.insert("OnDynamic_1", mOnDynamic_1);
    obj.insert("OffDynamic_1", mOffDynamic_1);
    obj.insert("OnDynamic_2", mOnDynamic_2);
    obj.insert("OffDynamic_2", mOffDynamic_2);

    QJsonObject ret = test::CommonParameters::Serialise();
    ret.insert("hydro", obj);

    return ret;
}
bool Parameters::Deserialize(const QJsonObject &obj )
{
    Reset();
    bool ret = test::CommonParameters::Deserialize( obj );
    auto val = obj.value("hydro");
    if ( val.isObject() )
    {
        QJsonObject obj = val.toObject();
        mGsType = obj.value("GsType").toString();
        mVoltage =  obj.value("Voltage").toInt();
        mVoltageRange =  obj.value("VoltageRange").toDouble();
        mLost = obj.value("Lost").toDouble();
        mVoltageType = static_cast<VOLTAGE_TYPE>( obj.value("VoltageType").toInt() );
        mMaxWorkPressure  =  obj.value("MaxWorkPressure").toDouble();
        mMinTestPressure  =  obj.value("MinTestPressure").toDouble();
        mHermPressure =  obj.value("HermPressure").toDouble();
        mTestControlPressure =  obj.value("TestControlPressure").toDouble();
        mHermSignal  =  static_cast<CONTROL_SIGNAL>( obj.value("HermSignal").toDouble() );
        mPABTSignal  =  static_cast<CONTROL_SIGNAL>( obj.value("PABTSignal").toInt() );
        mPBATSignal  =  static_cast<CONTROL_SIGNAL>( obj.value("PBATSignal").toInt() );
        mActuationOnTime  =  obj.value("ActuationOnTime").toDouble();
        mActuationOffTime  =  obj.value("ActuationOffTime").toDouble();

        mOnDD1A     = obj.value("OnDD1A").toBool();
        mOnDD2A     = obj.value("OnDD2A").toBool();;
        mOnDD3A     = obj.value("OnDD3A").toBool();;
        mOffDD1A    = obj.value("OffDD1A").toBool();;
        mOffDD2A    = obj.value("OffDD2A").toBool();;
        mOffDD3A    = obj.value("OffDD3A").toBool();;
        mOnDD1B     = obj.value("OnDD1B").toBool();;
        mOnDD2B     = obj.value("OnDD2B").toBool();;
        mOnDD3B     = obj.value("OnDD3B").toBool();;
        mOffDD1B    = obj.value("OffDD1B").toBool();;
        mOffDD2B    = obj.value("OffDD2B").toBool();;
        mOffDD3B    = obj.value("OffDD3B").toBool();;

        mOnDynamic_1 = static_cast<DYNAMIC>( obj.value("OnDynamic_1").toInt() );
        mOffDynamic_1 = static_cast<DYNAMIC>( obj.value("OffDynamic_1").toInt() );
        mOnDynamic_2 = static_cast<DYNAMIC>( obj.value("OnDynamic_2").toInt() );
        mOffDynamic_2 = static_cast<DYNAMIC>( obj.value("OffDynamic_2").toInt() );
        ret = true;
    }
    else
        ret = false;

    return ret;
}

void Parameters::WriteToController() const
{
    auto& mem = cpu::CpuMemory::Instance().DB30;

    mem.Current = mVoltageType == VT_DC ? 0: 1;
    mem.Coil = mReelCount == 1 ? 0 : 1;
    mem.TypeControl = mControlType == CT_ELECTRIC ? 0: 1;

    mem.Voltage = mVoltage;
    mem.Delta_U = mVoltageRange;

    mem.Q_max = mMaxExpenditure;
    mem.Q_min = mLost;
    mem.P_max_rab = mMaxWorkPressure;
    mem.P_min = mMinTestPressure;
    mem.P_narug_germ = mHermPressure;

    mem.Signal_vnutr_germ = static_cast< int >(mHermSignal) >= 0 ? static_cast< int >(mHermSignal) : 0;
    mem.Signal_PA_BT = static_cast< int >(mPABTSignal) >= 0 ? static_cast< int >(mPABTSignal) : 0;
    mem.Signal_PB_AT = static_cast< int >(mPBATSignal) >= 0 ? static_cast< int >(mPBATSignal) : 0;

    mem.Time_ON = mActuationOnTime;
    mem.Time_OFF = mActuationOffTime;

    mem.Min_P = mMinControlPressure;
    mem.Max_P = mMaxControlPressure;

    mem.Time_P = mTestControlPressure;
    mem.DD1_open_a = mOnDD1A;
    mem.DD2_open_a = mOnDD2A;
    mem.DD3_open_a = mOnDD3A;
    mem.TypeD_open_a = static_cast< int >(mOnDynamic_1) >= 0 ? static_cast< int >(mOnDynamic_1) : 0;

    mem.DD1_open_b = mOnDD1B;
    mem.DD2_open_b = mOnDD2B;
    mem.DD3_open_b = mOnDD3B;
    mem.TypeD_open_b = static_cast< int >(mOnDynamic_2) >= 0 ? static_cast< int >(mOnDynamic_2) : 0;

    mem.DD1_close_a = mOffDD1A;
    mem.DD2_close_a = mOffDD2A;
    mem.DD3_close_a = mOffDD3A;
    mem.TypeD_close_a = static_cast< int >(mOffDynamic_1) >= 0 ? static_cast< int >(mOffDynamic_1) : 0;

    mem.DD1_close_b = mOffDD1B;
    mem.DD2_close_b = mOffDD2B;
    mem.DD3_close_b = mOffDD3B;
    mem.TypeD_close_b = static_cast< int >(mOffDynamic_2) >= 0 ? static_cast< int >(mOffDynamic_2) : 0;

    mem.Write();

    cpu::CpuMemory::Instance().DB31.Q_5_5ma = mDefaultExpenditure;
}

bool Parameters::Draw(QPainter &painter, QRect &free_rect, QString const& compare_width ) const
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

    auto DrawRowCenter2 = [ &painter, &free_rect ]( QFont font, QColor color, QString text,  QColor color2, QString text2, double spase = 1 )
    {
        painter.save();
        QFontMetrics metrix( font );
        QRect place;
        AllocatePlace( place, metrix.height()*spase ,free_rect );
        QPoint start_point( place.center().x() - metrix.width( text + text2 ) / 2, place.center().y() +metrix.height()/2);
        painter.setFont( font );
        painter.setPen( color );
        painter.drawText( start_point, text );
        QPoint start_point2( place.center().x() - metrix.width( text + text2 ) / 2 + metrix.width( text ) , place.center().y() +metrix.height()/2);
        painter.setPen( color2 );
        painter.drawText( start_point2, text2 );
        painter.restore();
    };


    auto DrawRowLeft = [ &painter, &free_rect ]( QFont font,
            QColor color1, QColor color2,
            QString label, QString value,
            QString value2 = "", double spase = 1 )
    {
        painter.save();
        QFontMetrics metrix( font );
        QRect place;
        AllocatePlace( place, metrix.height()*spase, free_rect );
        QPoint start_point( place.left() , place.center().y()+metrix.height()/2 );
        QPoint start_point2( place.left() + metrix.width(label), place.center().y() +metrix.height()/2);
        QPoint start_point3( place.left() + metrix.width(label + value), place.center().y() +metrix.height()/2);
        painter.setFont( font );
        painter.setPen( color1 );
        painter.drawText( start_point, label );
        painter.setPen( color2 );
        painter.drawText( start_point2, value );
        painter.setPen( Qt::gray );
        painter.drawText( start_point3, value2 );
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

    DrawRowCenter2( title_font, Qt::black, "ОТЧЕТ", Qt::red, " ( " + mReportType + " ) ", row_skale);
    DrawRowCenter( level_font, Qt::black, "Испытания дискретного аппарата", row_skale );
    DrawRowCenter( level_font, Qt::red, mGsType, row_skale );

    DrawRowLeft( text_font, Qt::black, Qt::red, "Идентификационный номер: ", mSerNo, "", row_skale);

    test::hydro::Parameters old;
    QJsonObject f = test::ReadFromFile( compare_width ).value("Params").toObject();
    old.Deserialize( f );

    QString str_e_wp =          !compare_width.isEmpty() ? " (" +test::ToString(old.MaxWorkPressure()) + ")" : QString();
    QString str_e_exp =         !compare_width.isEmpty() ? " (" +test::ToString(old.MaxExpenditure()) + ")" : QString();
    QString str_e_mxcp =        !compare_width.isEmpty() ? ( old.ControlType() != CT_ELECTRIC ? " (" +test::ToString(old.MaxControlPressure()) + ")" : "( - )" ) : QString();
    QString str_e_mncp =        !compare_width.isEmpty() ? ( old.ControlType() != CT_ELECTRIC ? " (" +test::ToString(old.MinControlPressure()) + ")" : "( - )" ) : QString();
    QString str_e_vt =          !compare_width.isEmpty() ? " (" +test::ToString(old.Voltage()) + ")" : QString();

    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Максимальное давление, бар"), test::ToString( mMaxWorkPressure ),str_e_wp, row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Максимальный расход, л/мин"), test::ToString( mMaxExpenditure ), str_e_exp,row_skale );
    if ( mControlType == CT_ELECTRIC )
    {
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Максимальное давление управления*, бар"), "-", str_e_mxcp, row_skale );
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Минимальное давление управления*, бар"), "-", str_e_mncp, row_skale );
    }
    else
    {
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Максимальное давление управления*, бар"), test::ToString(mMaxControlPressure), str_e_mxcp,  row_skale );
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Минимальное давление управления*, бар"), test::ToString(mMinControlPressure), str_e_mncp, row_skale );
    }
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Напряжение пинания, В"), "="+test::ToString( mVoltage )+" (~220)",str_e_vt, row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Тонкость фильтрации рабочей жидкости, мкм"), test::ToString(3), "", row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Тип масла"), "Лукой Гейзер HLP32", "", row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Вязкость масла (при 40˚С), сСт"), test::ToString(32), "", row_skale );

    if ( !compare_width.isEmpty() )
    {        
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Аппарат для сравнения характеристик"), old.SerNo(), "", row_skale );
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Дата испытания сравниваемого аппарата"), old.Date().toString("dd MMMM yyyy г. hh:mm"), "", row_skale );
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Сравнение с эталоном"), old.ReportType().compare("Эталон", Qt::CaseInsensitive) == 0? "Да": "Нет", "", row_skale );
    }
    else
    {
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Аппарат для сравнения характеристик"), "-", "", row_skale );
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Дата испытания сравниваемого аппарата"), "-", "", row_skale );
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Сравнение с эталоном"), "-", "", row_skale );
    }

    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Класс чистоты жидкости (по ISO 4406)"), "17/15/12", "", row_skale );

    DrawRowCenter( text_font, Qt::black, "", row_skale );

    DrawRowLeft( text_font, Qt::black, Qt::red, "Испытания проводил: ", mUser, "", row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::red, "Дата проведения испытаний: ", mDate.toString("dd MMMM yyyy г. hh:mm"), "", row_skale );

    DrawLastRow( text_font, Qt::black, "*Для распределителей с электрогидравлическим управлением");
    return true;
}

bool Parameters::DrawResults(QPainter &painter, QRect &free_rect ) const
{
    QFont title_font = painter.font();
    title_font.setFamily("Arial");
    title_font.setPointSize(14);

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
    auto DrawRowCenter2 = [ &painter, &free_rect ]( QFont font, QColor color, QString text, QColor color2, QString text2, double spase = 1 )
    {
        painter.save();
        QFontMetrics metrix( font );
        QRect place;
        AllocatePlace( place, metrix.height()*spase ,free_rect );
        QPoint start_point( place.center().x() - ( metrix.width( text ) + metrix.width( text2 ) ) / 2, place.center().y() +metrix.height()/2);
        QPoint start_point2( start_point.x() + metrix.width(text), start_point.y() );
        painter.setFont( font );
        painter.setPen( color );
        painter.drawText( start_point, text );
        painter.setPen( color2 );
        painter.drawText( start_point2, text2 );
        painter.restore();
    };

    auto DrawRowLeft = [ &painter, &free_rect ]( QFont font, QColor color1, QString label, double spase = 1 )
    {
        painter.save();
        QFontMetrics metrix( font );
        QRect place;
        AllocatePlace( place, metrix.height()*spase, free_rect );
        QPoint start_point( place.left() , place.center().y()+metrix.height()/2 );
        painter.setFont( font );
        painter.setPen( color1 );
        painter.drawText( start_point, label );
        painter.restore();
    };

    auto DrawRowLeft3 = [ &painter, &free_rect ](    QFont const& font,
                                                    QColor const& color1,
                                                    QString const& label,
                                                    QColor const& color2 = Qt::black,
                                                    QString const& value = "",
                                                    QColor const& color3 = Qt::black,
                                                    QString const& value2 = "",
                                                    double spase = 1)
    {
        painter.save();
        QFontMetrics metrix( font );
        QRect place;
        AllocatePlace( place, metrix.height()*spase, free_rect );
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

    DrawRowCenter( title_font, Qt::black, "", 7 );
    DrawRowCenter( title_font, Qt::black, "Результаты испытаний", 1 );
    DrawRowCenter2( title_font, Qt::black, "дискретного аппарата ", Qt::red, mGsType, 2 );

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

    bool sucsess = true;

    QString row;
    for ( auto it =  mTestCase.begin(), end = mTestCase.end(); it != end; ++it )
    {
        Test* ptr = *it;
        row +=  "<tr>"
                   "<td>"+test::ToString( ptr->Number() )+"</td>"
                   "<td>"+ QString(ptr->Name()).replace("\n","<br>") +"</td>"
                   "<td style='font-size:28pt; color: \"red\"; font-weight:bold;'>"+ (ptr->Success() ? QString("+"):QString("-")) +"</td>"
                "</tr>";
        sucsess &= ptr->Success();
    }

    QTextDocument doc;
    doc.setUndoRedoEnabled( false );
    doc.setTextWidth( free_rect.width() );
    doc.setUseDesignMetrics( true );
    doc.setDefaultTextOption ( QTextOption (Qt::AlignHCenter )  );
    doc.setHtml( header + row + footer );
    auto h = doc.documentLayout()->documentSize().height();

    QRect place;
    AllocatePlace( place, h ,free_rect );
    QRectF r( 0, 0, place.width(), place.height() );
    painter.save();
    painter.translate( place.topLeft() );
    doc.drawContents( &painter, r);
    painter.restore();

    DrawRowLeft( text_font, Qt::black, "ИТОГ:", 3 );
    DrawRowLeft3( text_font, Qt::black, "Гидроаппарат ",
                            Qt::red, mGsType + (sucsess? " годен": " не годен"),
                            Qt::black, " к эксплуатации", 1 );
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

bool Parameters::Voltage ( QString const& val )
{
    return ParseValue( mVoltage, val );
}
double const& Parameters::Voltage () const
{
    return mVoltage;
}

bool Parameters::VoltageRange ( QString const& val )
{
    return ParseValue( mVoltageRange, val );
}
double const& Parameters::VoltageRange () const
{
    return mVoltageRange;
}

bool Parameters::Lost ( QString const& val )
{
    return ParseValue( mLost, val );
}
double const& Parameters::Lost () const
{
    return mLost;
}

bool Parameters::VoltageType ( QString const& val )
{
    return ParseValue( mVoltageType, val );
}
VOLTAGE_TYPE const& Parameters::VoltageType () const
{
    return mVoltageType;
}


bool Parameters::MaxWorkPressure ( QString const& val )
{
    return ParseValue( mMaxWorkPressure, val );
}
double const& Parameters::MaxWorkPressure () const
{
    return mMaxWorkPressure;
}

bool Parameters::MinTestPressure ( QString const& val )
{
    return ParseValue( mMinTestPressure, val );
}
double const& Parameters::MinTestPressure () const
{
    return mMinTestPressure;
}

bool Parameters::HermPressure ( QString const& val )
{
    return ParseValue( mHermPressure, val );
}
double const& Parameters::HermPressure () const
{
    return mHermPressure;
}

bool Parameters::TestControlPressure ( QString const& val )
{
    return ParseValue( mTestControlPressure, val );
}
const double &Parameters::TestControlPressure() const
{
    return mTestControlPressure;
}

bool Parameters::HermSignal ( QString const& val )
{
    return ParseValue( mHermSignal, val );
}
CONTROL_SIGNAL const& Parameters::HermSignal () const
{
    return mHermSignal;
}

bool Parameters::PABTSignal ( QString const& val )
{
    return ParseValue( mPABTSignal, val );
}
CONTROL_SIGNAL const& Parameters::PABTSignal () const
{
    return mPABTSignal;
}

bool Parameters::PBATSignal ( QString const& val )
{
    return ParseValue( mPBATSignal, val );
}
CONTROL_SIGNAL const& Parameters::PBATSignal () const
{
    return mPBATSignal;
}

bool Parameters::ActuationOnTime ( QString const& val )
{
    return ParseValue( mActuationOnTime, val );
}
double const& Parameters::ActuationOnTime () const
{
    return mActuationOnTime;
}

bool Parameters::ActuationOffTime ( QString const& val )
{
    return ParseValue( mActuationOffTime, val );
}
const double &Parameters::ActuationOffTime() const
{
    return mActuationOffTime;
}

bool Parameters::OnDD1A ( Qt::CheckState const& val )
{
    mOnDD1A = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OnDD1A () const
{
    return mOnDD1A;
}
bool Parameters::OnDD2A ( Qt::CheckState const& val )
{
    mOnDD2A = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OnDD2A () const
{
    return mOnDD2A;
}
bool Parameters::OnDD3A ( Qt::CheckState const& val )
{
    mOnDD3A = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OnDD3A () const
{
    return mOnDD3A;
}

bool Parameters::OffDD1A ( Qt::CheckState const& val )
{
    mOffDD1A = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OffDD1A () const
{
    return mOffDD1A;
}
bool Parameters::OffDD2A ( Qt::CheckState const& val )
{
    mOffDD2A = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OffDD2A () const
{
    return mOffDD2A;
}
bool Parameters::OffDD3A ( Qt::CheckState const& val )
{
    mOffDD3A = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OffDD3A () const
{
    return mOffDD3A;
}

bool Parameters::OnDD1B ( Qt::CheckState const& val )
{
    mOnDD1B = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OnDD1B () const
{
    return mOnDD1B;
}
bool Parameters::OnDD2B ( Qt::CheckState const& val )
{
    mOnDD2B = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OnDD2B () const
{
    return mOnDD2B;
}
bool Parameters::OnDD3B ( Qt::CheckState const& val )
{
    mOnDD3B = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OnDD3B () const
{
    return mOnDD3B;
}

bool Parameters::OffDD1B ( Qt::CheckState const& val )
{
    mOffDD1B = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OffDD1B () const
{
    return mOffDD1B;
}
bool Parameters::OffDD2B ( Qt::CheckState const& val )
{
    mOffDD2B = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OffDD2B () const
{
    return mOffDD2B;
}
bool Parameters::OffDD3B ( Qt::CheckState const& val )
{
    mOffDD3B = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OffDD3B () const
{
    return mOffDD3B;
}

bool Parameters::OnDynamic_1 ( QString const& val )
{
    return ParseValue( mOnDynamic_1, val );
}
DYNAMIC const& Parameters::OnDynamic_1 () const
{
    return mOnDynamic_1;
}

bool Parameters::OffDynamic_1 ( QString const& val )
{
    return ParseValue( mOffDynamic_1, val );
}
DYNAMIC const& Parameters::OffDynamic_1 () const
{
    return mOffDynamic_1;
}

bool Parameters::OnDynamic_2 ( QString const& val )
{
    return ParseValue( mOnDynamic_2, val );
}
DYNAMIC const& Parameters::OnDynamic_2 () const
{
    return mOnDynamic_2;
}

bool Parameters::OffDynamic_2 ( QString const& val )
{
    return ParseValue( mOffDynamic_2, val );
}
DYNAMIC const& Parameters::OffDynamic_2 () const
{
    return mOffDynamic_2;
}

}//namespace hydro

}//namespace test
