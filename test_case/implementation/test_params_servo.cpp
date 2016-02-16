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
    mControlReelResist(0),
    mVoltage(0),
    mTestChannelA( false ),
    mTestChannelB( false ),
    mSignalOnChannelA( CS_UNKNOWN ),
    mSignalOnChannelB( CS_UNKNOWN ),
    mStartFrequency( 0.0 ),
    mAmplInc( 0.0 )
{
    mAmplitudes[0] = 0;
    mAmplitudes[1] = 0;
    mAmplitudes[2] = 0;
}

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

    mControlReelResist = 0;

    mAmplitudes[0] = 0;
    mAmplitudes[1] = 0;
    mAmplitudes[2] = 0;

    mVoltage = 0;

    mTestChannelA = false;
    mTestChannelB = false;
    mSignalOnChannelA = CS_UNKNOWN;
    mSignalOnChannelB = CS_UNKNOWN;

    mStartFrequency = 0.0;
    mAmplInc = 0.0;
}
QString Parameters::ToString() const
{
    QString res;
    res+= "Параметры аппаратуры управления направлением расхода:\n";
    res+= "  Тип гидрораспределителя: " + mGsType +"\n" ;
    res+= "  Серийный номер: " + mSerNo +"\n" ;
    res+= "  Тип управления: " + test::ToString( mReelControl ) + "\n";
    res+= "  Количество позиций: " + test::ToString( mPosCount ) + "\n";
    res+= "  Тип управления распределителем: " + test::ToString( mControlType ) + "\n";
    res+= "  Максимальное давление управления, Бар: " + test::ToString( mMaxControlPressure ) + "\n";
    res+= "  Минимальное давление управления, Бар: " + test::ToString( mMinControlPressure ) + "\n";
    res+= "  Уровень управляющих сигналов: " + test::ToString( mControlSignal ) + "\n";
    res+= "  Амплитуды: " + test::ToString( mAmplitudes[0] ) + " " + test::ToString( mAmplitudes[1] ) + " "  + test::ToString( mAmplitudes[2] ) + "\n";
    res+= "  Инктемент амплитуды: " + test::ToString( mAmplInc ) + "\n";
    res+= "Парамеры для аппаратуры с типом управления: " + test::ToString( RC_CONTROL_BOX );
    res+= "  Напряжение на блоке управления: " + test::ToString( mVoltage ) + "\n";
    res+= "  Сигнал, соответствующий полному переключению в состояние А: " + test::ToString( mSignalStateA ) + "\n";
    res+= "  Сигнал, соответствующий полному переключению в состояние Б: " + test::ToString( mSignalStateB ) + "\n";
    res+= "  Сигнал, соответствующий нулевому положению: " + test::ToString( mSignalState0 ) + "\n";
    res+= "Парамеры для аппаратуры с типом управления: " + test::ToString( RC_REEL );
    res+= "  Сопротивление катушки управления, Ом: " + test::ToString( mControlReelResist ) + "\n";
    res+= "\n";
    res+= "Параметры стенда:\n";
    res+= "  Максимальный расход, л/мин: " + test::ToString( mMaxExpenditure ) + "\n";
    res+= "  Номинальное давление, Бар: " + test::ToString( mPressureNominal ) + "\n";
    res+= "\n";
    res+= "Параметры испытаний:\n";
    res+= "  Давления при испытании аппарата пробным давлением, Бар: " + test::ToString( mPressureTesting ) + "\n";
    res+= "  Максимальный расход в канале А, при следующем опорном сигнале, X_max_A, л/мин: " + test::ToString( mMaxExpenditureA ) + "\n";
    res+= "  Максимальный расход в канале Б, при следующем опорном сигнале, X_max_Б, л/мин: " + test::ToString( mMaxExpenditureB ) + "\n";
    res+= "  Стартовая частота для построения частотных характеристик, Гц: " + test::ToString( mStartFrequency ) + "\n";
    res+= "  Инкремент частоты при построении частотных характеристик, Гц: " + test::ToString( mFrequencyInc ) + "\n";
    res+= "  Тестирование канала А: " + test::ToString( mTestChannelA ) + "\n";
    res+= "  Катушка для тестирования канала А: " + test::ToString( mSignalOnChannelA ) + "\n";
    res+= "  Тестирование канала Б " + test::ToString( mTestChannelB ) + "\n";
    res+= "  Катушка для тестирования канала Б: " + test::ToString( mSignalOnChannelB ) + "\n";

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

    servo.insert("ControlReelResist", mControlReelResist);

    servo.insert("Amplitudes0", mAmplitudes[0]);
    servo.insert("Amplitudes1", mAmplitudes[1]);
    servo.insert("Amplitudes2", mAmplitudes[2]);

    servo.insert("Voltage", mVoltage);

    servo.insert("TestChannelA", mTestChannelA);
    servo.insert("TestChannelB", mTestChannelB);
    servo.insert("SignalOnChannelA", mSignalOnChannelA);
    servo.insert("SignalOnChannelB", mSignalOnChannelB);

    servo.insert( "StartFrequency", mStartFrequency );
    servo.insert( "AmplInc", mAmplInc );

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
        mPressureNominal = obj.value("PressureNominal").toDouble();
        mPressureTesting = obj.value("PressureTesting").toDouble();
        mMaxExpenditureA = obj.value("MaxExpenditureA").toDouble();
        mMaxExpenditureB = obj.value("MaxExpenditureB").toDouble();
        mFrequencyInc = obj.value("FrequencyInc").toDouble();

        mControlSignal = static_cast<SIGNAL_TYPE>(obj.value("ControlSignal").toInt());
        mSignalStateA = obj.value("SignalStateA").toDouble();
        mSignalStateB = obj.value("SignalStateB").toDouble();
        mSignalState0 = obj.value("SignalState0").toDouble();

        mControlReelResist = obj.value("ControlReelResist").toDouble();

        mAmplitudes[0] = obj.value("Amplitudes0").toDouble();
        mAmplitudes[1] = obj.value("Amplitudes1").toDouble();
        mAmplitudes[2] = obj.value("Amplitudes2").toDouble();

        mVoltage = obj.value("Voltage").toDouble();

        mTestChannelA = obj.value("TestChannelA").toBool();
        mTestChannelB = obj.value("TestChannelB").toBool();
        mSignalOnChannelA = static_cast<CONTROL_SIGNAL>( obj.value("SignalOnChannelA").toInt() );
        mSignalOnChannelB = static_cast<CONTROL_SIGNAL>( obj.value("SignalOnChannelB").toInt() );

        mStartFrequency = obj.value("StartFrequency").toDouble();
        mAmplInc = obj.value("AmplInc").toDouble();
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
        auto& mem2 = cpu::CpuMemory::Instance().DB33;
        mem.channel_a = mTestChannelA; //62.0 испытание канал А
        mem.channel_b = mTestChannelB; //62.1 испытание канала В
        mem.channel_k_a = mSignalOnChannelA == CS_REEL_B ? 1: 0;
        mem.channel_k_b = mSignalOnChannelB == CS_REEL_B ? 1: 0;
        switch (mControlSignal)                   //2 управляющий сигнал
        {
            case ST_100_mA:
                mem.s860ma = 100;
                break;
            case ST_300_mA:
                mem.s860ma = 300;
                break;
            case ST_600_mA:
                mem.s860ma = 600;
                break;
            case ST_860_mA:
                mem.s860ma = 860;
                break;
            case test::ST_1600_mA:
                mem.s860ma = 1600;
                break;
            case test::ST_2500_mA:
                mem.s860ma = 2500;
                break;
            case test::ST_3750_mA:
                mem.s860ma = 3750;
                break;
            case test::ST_5000_mA:
                mem.s860ma = 5000;
                break;
            default:
                mem.s860ma = 0;
                break;
        }


        mem2.x_max_a = mSignalStateA;             //4 сигнал переключение в А
        mem2.x_pos_0 = mSignalState0;             //12 сигнал переключение в 0
        if ( mPosCount == 3 )
        {
            mem2.x_max_b = mSignalStateB;             //8 сигнал переключение в В
        }

        mem.test_press = mPressureTesting;          //18 испытание пробным давлением
        mem.nominal_press = mPressureNominal;       //22 номинальное давление
        mem.q_max_a = mMaxExpenditureA;             //26 макс расход в А    ПОКАЗАТЬ НА ФОРМЕ
        mem.q_max_b = mMaxExpenditureB;             //30 макс расход в В
        mem.q_max = mMaxExpenditure;               //34 максимальный расход
        mem.amp_1 = mAmplitudes[0];               //38 амплитуда 1
        mem.amp_2 = mAmplitudes[1];               //42 амплитуда 2
        mem.amp_3 = mAmplitudes[2];               //46 амплитуда 3
        mem.increment = mFrequencyInc;           //50 инкремент частоты
        mem.press_control_min = mMinControlPressure;   //54 мин давдение управления
        mem.press_control_max = mMaxControlPressure;   //58 макс давление управления
        mem2.start_frequency = mStartFrequency;
        mem2.ampl_inc = mAmplInc;

        mem2.Write();
        mem.Write();
    }
    else
    {
        auto& mem = cpu::CpuMemory::Instance().DB33;       
        mem.s4_20ma = mControlSignal == ST_4_20_mA; //2.0 сигнал 4-20 мА
        mem.s0_20ma = mControlSignal == ST_0_20_mA; //2.1 сигнал 0-20 мА
        mem.s10v = mControlSignal == ST_10_10_V; //2.2 сигнал +/- 10В
        mem.s0_10v = mControlSignal == ST_0_10_V; //2.2 сигнал 0 - 10В
        mem.s10ma = mControlSignal == ST_10_10_mA; //2.3 сигнал +/- 10 мА
        mem.s15ma = mControlSignal == ST_15_15_mA; //2.4 сигнал +/- 15мА
        mem.s20ma = mControlSignal == ST_20_20_mA; //2.5 сиганл +/- 20 мА
        mem.s40ma = mControlSignal == ST_40_40_mA; //2.6 сигнал +/- 40 мА

        mem.channel_a = mTestChannelA; //62.0 испытание канал А
        mem.channel_b = mTestChannelB; //62.1 испытание канала В
        mem.channel_k_a = mSignalOnChannelA == CS_REEL_B ? 1: 0;
        mem.channel_k_b = mSignalOnChannelB == CS_REEL_B ? 1: 0;

        mem.x_max_a = mSignalStateA;             //4 сигнал переключение в А
        mem.x_pos_0 = mSignalState0;             //12 сигнал переключение в 0
        if ( mPosCount == 3 )
        {
            mem.x_max_b = mSignalStateB;             //8 сигнал переключение в В
        }

        mem.test_press = mPressureTesting;          //16 пробное давление
        mem.nominal_press = mPressureNominal;       //20 номинальное давление
        mem.q_max_a = mMaxExpenditureA;             //24 макс. расход в А
        mem.q_max_b = mMaxExpenditureB;             //28 макс. расход в В
        mem.q_max = mMaxExpenditure;               //32 макс. допустимый расход
        mem.amp_1 = mAmplitudes[0];               //36 амплитуда 1
        mem.amp_2 = mAmplitudes[1];               //40 амплитуда 2
        mem.amp_3 = mAmplitudes[2];               //44 амплитуда 3
        mem.increment = mFrequencyInc;           //48 инкремент частоты
        mem.press_control_min = mMinControlPressure;   //52 мин. давление управления
        mem.press_control_max = mMaxControlPressure;   //56 макс. давление управления
        mem.U_Plat = mVoltage;
        mem.start_frequency = mStartFrequency;
        mem.ampl_inc = mAmplInc;

        mem.Write();
    }
    auto& mem1 = cpu::CpuMemory::Instance().DB30;
    mem1.Clear();
    mem1.Coil = mPosCount == 2 ? 0 : 1;
    mem1.TypeControl = mControlType == CT_ELECTRIC ? 0: 1;
    mem1.Write();
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
    int width = m.width("123456789012345678901234567890123456789012345");
    char symbol = '.';
    auto FillToSize = [ width, &m, symbol ]( QString text )
    {
        while( m.width( text + symbol ) < width )
            text += symbol;
        return text + " ";
    };


    double row_skale = 2;

    DrawRowCenter2( title_font, Qt::black, "ОТЧЕТ", Qt::red, " ( " + mReportType + " ) ", row_skale);
    DrawRowCenter( level_font, Qt::black, "Испытания пропорционального аппарата", row_skale );
    DrawRowCenter( level_font, Qt::red, mGsType, row_skale );

    DrawRowLeft( text_font, Qt::black, Qt::red, "Идентификационный номер: ", mSerNo, "", row_skale);

    test::servo::Parameters old;
    QJsonObject f = test::ReadFromFile( compare_width ).value("Params").toObject();
    old.Deserialize( f );

    QString str_e_pn =          !compare_width.isEmpty() ? " (" +test::ToString(old.PressureNominal()) + ")" : QString();
    QString str_e_me =          !compare_width.isEmpty() ? " (" +test::ToString(old.MaxExpenditure()) + ")" : QString();
    QString str_e_mxcp =        !compare_width.isEmpty() ? ( old.ControlType() != CT_ELECTRIC ? " (" +test::ToString(old.MaxControlPressure()) + ")" : "( - )" ) : QString();
    QString str_e_mncp =        !compare_width.isEmpty() ? ( old.ControlType() != CT_ELECTRIC ? " (" +test::ToString(old.MinControlPressure()) + ")" : "( - )" ) : QString();

    QString str_e_rc =          !compare_width.isEmpty() ? " (" +test::ToShortString(old.ReelControl()) + ")" : QString();
    QString str_e_vt =          !compare_width.isEmpty() ? " (" +test::ToString(old.Voltage()) + ")" : QString();
    QString str_e_sa =          !compare_width.isEmpty() ? " (" +test::ToString(old.SignalStateA()) + ")" : QString();
    QString str_e_sb =          !compare_width.isEmpty() ? " (" +test::ToString(old.SignalStateB()) + ")" : QString();
    QString str_e_s0 =          !compare_width.isEmpty() ? " (" +test::ToString(old.SignalState0()) + ")" : QString();

    QString str_e_mea =          !compare_width.isEmpty() ? " (" +test::ToString(old.MaxExpenditureA()) + ")" : QString();
    QString str_e_meb =          !compare_width.isEmpty() ? " (" +test::ToString(old.MaxExpenditureB()) + ")" : QString();
    QString str_e_amp =          !compare_width.isEmpty() ? " (" +test::ToString(old.Amplitudes()[0]) + ", " +
                                                                  test::ToString(old.Amplitudes()[1]) + ", " +
                                                                  test::ToString(old.Amplitudes()[2]) + ")" : QString();
    QString str_e_fi =          !compare_width.isEmpty() ? " (" +test::ToString(old.FrequencyInc()) + ")" : QString();


    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Номинальное давление, бар"), test::ToString( mPressureNominal ),str_e_pn, row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Максимальный расход цилиндра, л/мин"), test::ToString( mMaxExpenditure ),str_e_me, row_skale );
    if ( mControlType == CT_ELECTRIC )
    {
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Максимальное давление управления*, бар"), "-", str_e_mxcp, row_skale );
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Минимальное давление управления*, бар"), "-", str_e_mncp, row_skale );
    }
    else
    {
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Максимальное давление управления*, бар"), test::ToString(mMaxControlPressure), str_e_mxcp, row_skale );
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Минимальное давление управления*, бар"), test::ToString(mMinControlPressure),str_e_mncp, row_skale );
    }

    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Тип управления"), test::ToShortString(mReelControl),str_e_rc, row_skale );
    if ( mReelControl == RC_CONTROL_BOX )
    {
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Напряжение питания, В"), test::ToString(mVoltage),str_e_vt, row_skale );
    }
    DrawRowLeft( text_font, Qt::black, Qt::red, "Сигнал, соответствующий:", "" );
    QString ed_izm = (mControlSignal == ST_10_10_V || mControlSignal == ST_0_10_V) ? " В": " мА";

    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("- нулевому положению"), test::ToString(mSignalState0) + ed_izm, str_e_s0, row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("- полному переключению в положение А"), test::ToString(mSignalStateA) + ed_izm, str_e_sa, row_skale );
    if ( mPosCount == 3 )
    {
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("- полному переключению в положение В"), test::ToString(mSignalStateB) + ed_izm, str_e_sb, row_skale );
    }
    else
    {
        DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("- полному переключению в положение В"), test::ToString(mSignalStateA) + ed_izm,str_e_sa, row_skale );
    }
    DrawRowLeft( text_font, Qt::black, Qt::red, "Максимальный задаваемый расход насоса:", "" );
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("- в канале А при максимально опорном сигнале, л/мин"), test::ToString(mMaxExpenditureA),str_e_mea, row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("- в канале В при максимально опорном сигнале, л/мин"), test::ToString(mMaxExpenditureB),str_e_meb, row_skale );

    DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Амплитуды управляющих сигналов, %"), test::ToString(mAmplitudes[0]) + ", " +
                                                                                                   test::ToString(mAmplitudes[1]) + ", " +
                                                                                                   test::ToString(mAmplitudes[2]),str_e_amp, row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Инкремент частоты, Гц"), test::ToString(mFrequencyInc),str_e_fi, row_skale );
    DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Тип масла"), "Лукойл Гейзер HLP32", "", row_skale );
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
double const& Parameters::PressureNominal () const
{
    return mPressureNominal;
}

bool Parameters::PressureTesting ( QString const& val )
{
    return ParseValue( mPressureTesting, val );
}
double const& Parameters::PressureTesting () const
{
    return mPressureTesting;
}

bool Parameters::MaxExpenditureA ( QString const& val )
{
    return ParseValue( mMaxExpenditureA, val );
}
double const& Parameters::MaxExpenditureA () const
{
    return mMaxExpenditureA;
}

bool Parameters::MaxExpenditureB ( QString const& val )
{
    return ParseValue( mMaxExpenditureB, val );
}
double const& Parameters::MaxExpenditureB () const
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

bool Parameters::ControlReelResist ( QString const& val )
{
    return ParseValue( mControlReelResist, val );
}
const double &Parameters::ControlReelResist() const
{
    return mControlReelResist;
}

bool Parameters::Amplitudes( QString const& val1, QString const& val2, QString const& val3 )
{
    bool res  = ParseValue( mAmplitudes[0], val1 );
         res &= ParseValue( mAmplitudes[1], val2 );
         res &= ParseValue( mAmplitudes[2], val3 );
    return res;
}
const std::array< double, 3 > &Parameters::Amplitudes() const
{
    return mAmplitudes;
}

bool Parameters::Voltage ( QString const& val )
{
    return ParseValue( mVoltage, val );
}
const double &Parameters::Voltage() const
{
    return mVoltage;
}

bool Parameters::TestChannelA ( bool val )
{
    mTestChannelA = val;
    return true;
}
const bool &Parameters::TestChannelA() const
{
    return mTestChannelA;
}

bool Parameters::TestChannelB ( bool val )
{
    mTestChannelB = val;
    return true;
}
const bool &Parameters::TestChannelB() const
{
    return mTestChannelB;
}

bool Parameters::SignalOnChannelA ( QString const& val )
{
    return ParseValue( mSignalOnChannelA, val );
}
const CONTROL_SIGNAL &Parameters::SignalOnChannelA() const
{
    return mSignalOnChannelA;
}

bool Parameters::SignalOnChannelB ( QString const& val )
{
    return ParseValue( mSignalOnChannelB, val );
}

const CONTROL_SIGNAL &Parameters::SignalOnChannelB() const
{
    return mSignalOnChannelB;
}

bool Parameters::StartFrequency ( QString const& val )
{
    return ParseValue( mStartFrequency, val );
}
const double &Parameters::StartFrequency() const
{
    return mStartFrequency;
}

bool Parameters::AmplInc ( QString const& val )
{
    return ParseValue( mAmplInc, val );
}
const double &Parameters::AmplInc() const
{
    return mAmplInc;
}

}//namespace servo

}//namespace test
