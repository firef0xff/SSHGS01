#include "test_params_pumps.h"
#include <memory>
#include <mutex>
#include "../tests.h"
#include "../../cpu/cpu_memory.h"
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

namespace test
{
bool ParseValue ( SPIN& sig, QString const& val )
{
   if ( !val.compare( "Левое", Qt::CaseInsensitive ) )
   {
       sig = spLeft;
       return true;
   }
   else if ( !val.compare( "Правое", Qt::CaseInsensitive ) )
   {
       sig = spRight;
       return true;
   }
   return false;
}
QString ToString( SPIN const& v )
{
   switch (v)
   {
       case spUnknown:
           return "Не задано";
       case spLeft:
           return "Левое";
       case spRight:
           return "Правое";
   default:
       return "Неизвестное значение";
   }
}

namespace pump
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

Parameters::Parameters()
{
   Reset();
}

void Parameters::Reset()
{
   test::Parameters::Reset();
   //Перед началом испытания необходимо задать следующие параметры:
   mSerianNo = "";   //Серийный номер //Идентификационный номер
   mModel = ""; //Модель гидронасоса
   mSectionsCount = 0; //Количество секций в насосе
   mSpin = spUnknown;   //Направление вращения

   mElektrikControl = false; //Наличие электронного управления
   mVoltage = 0; //Напряжение питания, В
   mVoltageType = VT_UNKNOWN; //Тип тока

   mHydroControl = false; //Наличие гидравлического управления
   mMaxControlPressure = 0; //Максимальное давление в канале управления, бар

   mWorkVolume1 = 0.0; //Рабочий объем насоса №1

  //Давление в напорной полости насоса №1:
   mPressureMin1 = 0.0; //Минимальное
   mPressureNom1 = 0.0; //номинальное (рабочее)
   mPressureMax1 = 0.0; //максимальное

   mWorkVolume2 = 0.0; //Рабочий объем насоса №2

  //Давление в напорной полости насоса №2
   mPressureMin2 = 0.0; //Минимальное
   mPressureNom2 = 0.0; //номинальное (рабочее)
   mPressureMax2 = 0.0; //максимальное

  //Частота вращения насоса(ов):
   mFrequencyMin = 0; //Минимальная
   mFrequencyNom = 0; //Номинальная
   mFrequencyMax = 0; //Максимальная

   mVolumeKPD = 0.0; //Объемный кпд насоса
   mExpenditure = 0.0; //Дренаж насоса при номинальном давлении и номинальном расходе л/мин

  //Сервисные настройки для испытания насосов:
   mFuncTestTime = 0; //Продолжительность испытаний функционирования
   mStrongTestTime = 0; //Продолжительность испытаний на прочность
   mHermTestTime = 0; //Продолжительность испытаний наружной герметичности

   mX = 0;//Время набора требуемого давления, сек (X)
   mY = 0;//Время набора требуемой частоты вращения, сек (Y)
   mZ = 0;//Время стабилизации потока, сек (Z)
   mA = 0.0;//Коэффициент функционирования, % (А)
   mA1 = 0.0;//Коэффициент функционирования для дренажа, % (А1)
   mE = 0.0;//Точность для испытаний функциональные зависимости, сек (Е)
   mB = 0;//Продолжительность испытаний функциональные зависимости, сек (В)
}

QString Parameters::ToString() const
{
   QString res;
   return res;
}

test::TestCase& Parameters::TestCollection() const
{
   return Pumps;
}

QJsonObject Parameters::Serialise() const
{
    QJsonObject obj;
    obj.insert("mSerianNo", mSerianNo);
    obj.insert("mModel", mModel);
    obj.insert("mSectionsCount", mSectionsCount);
    obj.insert("mSpin", mSpin);

    obj.insert("mElektrikControl", mElektrikControl);
    obj.insert("mVoltage", mVoltage);
    obj.insert("mVoltageType", mVoltageType);

    obj.insert("mHydroControl", mHydroControl);
    obj.insert("mMaxControlPressure", mMaxControlPressure);

    obj.insert("mWorkVolume1", mWorkVolume1);
    obj.insert("mPressureMin1", mPressureMin1);
    obj.insert("mPressureNom1", mPressureNom1);
    obj.insert("mPressureMax1", mPressureMax1);

    obj.insert("mWorkVolume2", mWorkVolume2);
    obj.insert("mPressureMin2", mPressureMin2);
    obj.insert("mPressureNom2", mPressureNom2);
    obj.insert("mPressureMax2", mPressureMax2);

    obj.insert("mFrequencyMin", mFrequencyMin);
    obj.insert("mFrequencyNom", mFrequencyNom);
    obj.insert("mFrequencyMax", mFrequencyMax);

    obj.insert("mVolumeKPD", mVolumeKPD);
    obj.insert("mExpenditure", mExpenditure);

    //Сервисные настройки для испытания насосов:
    obj.insert("mFuncTestTime", mFuncTestTime);
    obj.insert("mStrongTestTime", mStrongTestTime);
    obj.insert("mHermTestTime", mHermTestTime);

    obj.insert("mX", mX);
    obj.insert("mY", mY);
    obj.insert("mZ", mZ);
    obj.insert("mA", mA);
    obj.insert("mA1", mA1);
    obj.insert("mE", mE);
    obj.insert("mB", mB);

    QJsonObject ret = test::Parameters::Serialise();
    ret.insert("pump", obj);

    return ret;
}
bool Parameters::Deserialize(const QJsonObject &obj )
{
    Reset();
    bool ret = test::Parameters::Deserialize( obj );
    auto val = obj.value("pump");
    if ( val.isObject() )
    {
        QJsonObject obj = val.toObject();
        mSerianNo = obj.value("mSerianNo").toString();
        mModel = obj.value("mModel").toString();
        mSectionsCount = obj.value("mSectionsCount").toInt();
        mSpin = static_cast<SPIN>( obj.value("mSpin").toInt() );

        mElektrikControl = obj.value("mElektrikControl").toBool();
        mVoltage = obj.value("mVoltage").toDouble();
        mVoltageType = static_cast<VOLTAGE_TYPE>( obj.value("mVoltageType").toInt() );

        mHydroControl = obj.value("mHydroControl").toBool();
        mMaxControlPressure = obj.value("mMaxControlPressure").toDouble();

        mWorkVolume1 = obj.value("mWorkVolume1").toDouble();
        mPressureMin1 = obj.value("mPressureMin1").toDouble();
        mPressureNom1 = obj.value("mPressureNom1").toDouble();
        mPressureMax1 = obj.value("mPressureMax1").toDouble();

        mWorkVolume2 = obj.value("mWorkVolume2").toDouble();
        mPressureMin2 = obj.value("mPressureMin2").toDouble();
        mPressureNom2 = obj.value("mPressureNom2").toDouble();
        mPressureMax2 = obj.value("mPressureMax2").toDouble();

        mFrequencyMin = obj.value("mFrequencyMin").toInt();
        mFrequencyNom = obj.value("mFrequencyNom").toInt();
        mFrequencyMax = obj.value("mFrequencyMax").toInt();

        mVolumeKPD = obj.value("mVolumeKPD").toDouble();
        mExpenditure = obj.value("mExpenditure").toDouble();

        //Сервисные настройки для испытания насосов:
        mFuncTestTime = obj.value("mFuncTestTime").toInt();
        mStrongTestTime = obj.value("mStrongTestTime").toInt();
        mHermTestTime = obj.value("mHermTestTime").toInt();

        mX = obj.value("mX").toInt();
        mY = obj.value("mY").toInt();
        mZ = obj.value("mZ").toInt();
        mA = obj.value("mA").toDouble();
        mA1 = obj.value("mA1").toDouble();
        mE = obj.value("mE").toDouble();
        mB = obj.value("mB").toInt();
    }
    else
        ret = false;

    return ret;
}

void Parameters::WriteToController() const
{
   //здесь реализовать логику ограничений параметров
   auto& mem = cpu::CpuMemory::Instance().DB71;

   mem.Number_of_sections = mSectionsCount == 2;
   mem.Direction_of_rotation = mSpin == spRight;
   mem.Electronic_control = mElektrikControl;
   mem.Hydraulic_control = mHydroControl;

   mem.Type_control;     //Тип управления (дискретный/аналоговый)
   mem.Type_current_discrete = mVoltageType == VT_DC;


   mem.Q_N1 = mWorkVolume1;
   mem.P_N1_min = mPressureMin1;       //ограничить правами пользователя
   mem.P_N1_nominal = mPressureNom1;   //ограничить правами пользователя
   mem.P_N1_max = mPressureMax1;       //ограничить правами пользователя

   mem.Q_N2 = mWorkVolume2;
   mem.P_N2_min = mPressureMin2;       //ограничить правами пользователя
   mem.P_N2_nominal = mPressureNom2;   //ограничить правами пользователя
   mem.P_N2_max = mPressureMax2;       //ограничить правами пользователя

   mem.Rotat_speed_pump_min = mFrequencyMin;    //ограничить правами пользователя
   mem.Rotat_speed_pump_nominal = mFrequencyNom;//ограничить правами пользователя
   mem.Rotat_speed_pump_max = mFrequencyMax;    //ограничить правами пользователя

   mem.KPD_pump = mVolumeKPD;
   mem.Drainage_pump = mExpenditure;
   mem.Voltage = mVoltage;
   mem.Max_P_control = mMaxControlPressure;

   mem.Time_test_function = mFuncTestTime;
   mem.Time_test_strength = mStrongTestTime * 60;
   mem.Time_test_tightness = mHermTestTime;

   mem.Time_set_P = mX;
   mem.Time_set_rotation = mY;
   mem.Time_stabil_flow = mZ;
   mem.Coeffic_function = mA;
   mem.Coeffic_function_drainag = mA1;
   mem.Precision_test_function = mE;
   mem.Time_function_dependence = mB;

   mem.Temperature_Work; //Рабочая температура масла в баке
   mem.Contur_A_Q1; //выбран контур А для секции 1
   mem.Contur_B_Q1; //выбран контур В для секции 1
   mem.Contur_C_Q1; //выбран контур С для секции 1
   mem.Contur_A_Q2; //выбран контур А для секции 2
   mem.Contur_B_Q2; //выбран контур В для секции 2
   mem.Contur_C_Q2; //выбран контур С для с2кции 2

   mem.Write();
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

   DrawHelper drw( painter, free_rect );
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
   DrawRowCenter( level_font, Qt::black, "Испытания гидронасоса", row_skale );
   DrawRowCenter( level_font, Qt::red, mModel, row_skale );

   DrawRowLeft( text_font, Qt::black, Qt::red, "Идентификационный номер: ", mSerianNo, "", row_skale);

   test::pump::Parameters old;
   bool compare = !compare_width.isEmpty();
   if ( compare )
   {
      QJsonObject f = test::ReadFromFile( compare_width ).value("Params").toObject();
      old.Deserialize( f );
   }

   auto MakeCompare = [compare]( QString base, QString old )
   {
      if ( !compare )
         return base;
      return base + " (" + old + ")";
   };

   bool use_second_section = mSectionsCount > 1;
   auto MakeSektion = [use_second_section]( double f, double s )
   {
      if ( !use_second_section )
         return test::ToString(f);
      return test::ToString(f) + ", " + test::ToString(s);
   };

   DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Направление вращения"),         MakeCompare( test::ToString(mSpin), test::ToString(old.mSpin) ), "", row_skale );

//   DrawRowLeft( text_font, Qt::black, Qt::black, "Секция №1:", "", "", row_skale );
   DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Рабочий объем, см³/об"),        MakeCompare( MakeSektion( mWorkVolume1, mWorkVolume2 ), MakeSektion( old.mWorkVolume1, old.mWorkVolume2 ) ), "", row_skale );
   DrawRowLeft( text_font, Qt::black, Qt::black, "Давление:", "", "", row_skale );
   DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("- минимальное, бар"),           MakeCompare( MakeSektion( mPressureMin1, mPressureMin2 ), MakeSektion( old.mPressureMin1, old.mPressureMin2 ) ), "", row_skale );
   DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("- номинальное (рабочее), бар"), MakeCompare( MakeSektion( mPressureNom1, mPressureNom2 ), MakeSektion( old.mPressureNom1, old.mPressureNom2 ) ), "", row_skale );
   DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("- максимальное, бар"),          MakeCompare( MakeSektion( mPressureMax1, mPressureMax2 ), MakeSektion( old.mPressureMax1, old.mPressureMax2 ) ), "", row_skale );

   DrawRowLeft( text_font, Qt::black, Qt::black, "Частота вращения приводного вала:", "", "", row_skale );
   DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("- минимальная, бар"),        MakeCompare( test::ToString( mFrequencyMin ), test::ToString( old.mFrequencyMin ) ), "", row_skale );
   DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("- номинальная, бар"),        MakeCompare( test::ToString( mFrequencyNom ), test::ToString( old.mFrequencyNom ) ), "", row_skale );
   DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("- максимальная, бар"),       MakeCompare( test::ToString( mFrequencyMax ), test::ToString( old.mFrequencyMax ) ), "", row_skale );

   DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Объемный КПД насоса, %"),    MakeCompare( test::ToString( mVolumeKPD ), test::ToString( old.mVolumeKPD ) ), "", row_skale );
   DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Дренаж насоса при номинальном режиме, л/мин"), MakeCompare( test::ToString( mExpenditure ), test::ToString( old.mExpenditure ) ), "", row_skale );



   DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Тонкость фильтрации рабочей жидкости, мкм"), "________", "", row_skale );
   DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Тип масла"), "Лукой Гейзер HLP32", "", row_skale );
   DrawRowLeft( text_font, Qt::black, Qt::black, FillToSize("Вязкость масла (при 40˚С), сСт"), test::ToString(32), "", row_skale );

//   if ( compare )
//   {
//       DrawRowLeft( text_font, Qt::black, Qt::red, "Аппарат для сравнения характеристик: ", "", "", row_skale );
//       DrawRowLeft( text_font, Qt::black, Qt::red, "", old.SerianNo(), "", row_skale );
//       DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Дата испытания сравниваемого аппарата"), old.Date().toString("dd MMMM yyyy г. hh:mm"), "", row_skale );
//       DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Сравнение с эталоном"), old.ReportType().compare("Эталон", Qt::CaseInsensitive) == 0? "Да": "Нет", "", row_skale );
//   }
//   else
//   {
//       DrawRowLeft( text_font, Qt::black, Qt::red, "Аппарат для сравнения характеристик: ", "-", "", row_skale );
//       DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Дата испытания сравниваемого аппарата"), "-", "", row_skale );
//       DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Сравнение с эталоном"), "-", "", row_skale );
//   }

   DrawRowLeft( text_font, Qt::black, Qt::red, FillToSize("Класс чистоты жидкости (по ISO 4406)"), "________", "", row_skale );

   DrawRowCenter( text_font, Qt::black, "", row_skale );

   DrawRowLeft( text_font, Qt::black, Qt::red, "Испытания проводил: ", mUser, "", row_skale );
   DrawRowLeft( text_font, Qt::black, Qt::red, "Дата проведения испытаний: ", mDate.toString("dd MMMM yyyy г. hh:mm"), "", row_skale );

   free_rect.setHeight(0);
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

    DrawRowCenter( title_font, Qt::black, "Результаты испытаний", 1 );
    DrawRowCenter2( title_font, Qt::black, "насоса ", Qt::red, mModel, 2 );

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
            "<table width='100%' border='1.5' cellspacing='-0.5' cellpadding='10'>"
               "<tr>"
                   "<th> Номер </th>"
                   "<th> Испытания </th>"
                   "<th> Работоспособность </th>"
               "</tr>";

    QString footer = "</table>"
            "</body>"
            "</html>";

    QString row;
    for ( auto it =  mTestCase.begin(), end = mTestCase.end(); it != end; ++it )
    {
        Test* ptr = *it;
        if (ptr->RepSkeep())
           continue;

        int num = ptr->Number();
        QString test_str = ptr->RepName();
        QString res_str = ptr->RepRes();

        row +=  "<tr height='100'>"
                   "<td >"+test::ToString( num )+"</td>"
                   "<td style='text-align: left;'>"+ test_str.replace("\n","<br>") +"</td>"
                   "<td>"+ res_str.replace("\n","<br>") +"</td>"
                "</tr>";
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
    DrawRowLeft( text_font, Qt::black, "Гидронасос "+ mModel, 1.5 );
    DrawRowLeft( text_font, Qt::black, "Идентификационный номер: "+ mSerianNo, 1.5 );

    DrawRowLeft( text_font, Qt::black, "__________________________ к эксплуатации.", 5 );

    DrawRowLeft( text_font, Qt::black, "__________________________                /__________________________/", 5 );

    DrawRowLeft( text_font, Qt::black, mDate.toString("dd MMMM yyyy г.") );
    return true;
}


QString Parameters::ModelId() const
{
    return mModel;
}

//------------------------------------------------------------------------------------------------
bool Parameters::SerianNo( QString const& val )
{
   mSerianNo = val;
   return true;
}
QString const& Parameters::SerianNo()
{
   return mSerianNo;
}
bool Parameters::Model( QString const& val )
{
   mModel = val;
   return !mModel.isEmpty();
}
QString const& Parameters::Model()
{
   return mModel;
}
//Ответственный User()
bool Parameters::SectionsCount( qint32 val )
{
   mSectionsCount = val;
   return true;
}
qint32 Parameters::SectionsCount()
{
   return mSectionsCount;
}
bool Parameters::Spin( QString const& val )
{
   return ParseValue( mSpin, val );
}
SPIN Parameters::Spin()
{
   return mSpin;
}

bool Parameters::ElektrikControl( bool val )
{
   mElektrikControl = val;
   return true;
}
bool Parameters::ElektrikControl()
{
   return mElektrikControl;
}
bool Parameters::Voltage( QString const& val )
{
   return ParseValue( mVoltage, val );
}
double Parameters::Voltage()
{
   return mVoltage;
}
bool Parameters::VoltageType( QString const& val )
{
   return ParseValue( mVoltageType, val );
}
VOLTAGE_TYPE Parameters::VoltageType()
{
   return mVoltageType;
}

bool Parameters::HydroControl( bool val )
{
   mHydroControl = val;
   return true;
}
bool Parameters::HydroControl()
{
   return mHydroControl;
}
bool Parameters::MaxControlPressure( QString const& val )
{
   return ParseValue( mMaxControlPressure, val );
}
double Parameters::MaxControlPressure()
{
   return mMaxControlPressure;
}

bool     Parameters::WorkVolume1( QString const& val )
{
   return ParseValue( mWorkVolume1, val );
}
double   Parameters::WorkVolume1()
{
   return mWorkVolume1;
}
bool     Parameters::PressureMin1( QString const& val )
{
   return ParseValue( mPressureMin1, val );
}
double   Parameters::PressureMin1()
{
   return mPressureMin1;
}
bool     Parameters::PressureNom1( QString const& val )
{
   return ParseValue( mPressureNom1, val );
}
double   Parameters::PressureNom1()
{
   return mPressureNom1;
}
bool     Parameters::PressureMax1( QString const& val )
{
   return ParseValue( mPressureMax1, val );
}
double   Parameters::PressureMax1()
{
   return mPressureMax1;
}

bool     Parameters::WorkVolume2( QString const& val )
{
   return ParseValue( mWorkVolume2, val );
}
double   Parameters::WorkVolume2()
{
   return mWorkVolume2;
}
bool     Parameters::PressureMin2( QString const& val )
{
   return ParseValue( mPressureMin2, val );
}
double   Parameters::PressureMin2()
{
   return mPressureMin2;
}
bool     Parameters::PressureNom2( QString const& val )
{
   return ParseValue( mPressureNom2, val );
}
double   Parameters::PressureNom2()
{
   return mPressureNom2;
}
bool     Parameters::PressureMax2( QString const& val )
{
   return ParseValue( mPressureMax2, val );
}
double   Parameters::PressureMax2()
{
   return mPressureMax2;
}


bool     Parameters::FrequencyMin( QString const& val )
{
   return ParseValue( mFrequencyMin, val );
}
qint32   Parameters::FrequencyMin()
{
   return mFrequencyMin;
}
bool     Parameters::FrequencyNom( QString const& val )
{
   return ParseValue( mFrequencyNom, val );
}
qint32   Parameters::FrequencyNom()
{
   return mFrequencyNom;
}
bool     Parameters::FrequencyMax( QString const& val )
{
   return ParseValue( mFrequencyMax, val );
}
qint32   Parameters::FrequencyMax()
{
   return mFrequencyMax;
}

bool     Parameters::VolumeKPD( QString const& val )
{
   return ParseValue( mVolumeKPD, val );
}
double   Parameters::VolumeKPD()
{
   return mVolumeKPD;
}
bool     Parameters::Expenditure( QString const& val )
{
   return ParseValue( mExpenditure, val );
}
double   Parameters::Expenditure()
{
   return mExpenditure;
}

//параметры испытания
bool Parameters::FuncTestTime( QString const& val )
{
   return ParseValue( mFuncTestTime, val );
}
qint32 Parameters::FuncTestTime()
{
   return mFuncTestTime;
}
bool Parameters::StrongTestTime( QString const& val )
{
   return ParseValue( mStrongTestTime, val );
}
qint32 Parameters::StrongTestTime()
{
   return mStrongTestTime;
}
bool Parameters::HermTestTime( QString const& val )
{
   return ParseValue( mHermTestTime, val );
}
qint32 Parameters::HermTestTime()
{
   return mHermTestTime;
}

bool Parameters::X( QString const& val )
{
   return ParseValue( mX, val );
}
qint32 Parameters::X()
{
   return mX;
}
bool Parameters::Y( QString const& val )
{
   return ParseValue( mY, val );
}
qint32 Parameters::Y()
{
   return mY;
}
bool Parameters::Z( QString const& val )
{
   return ParseValue( mZ, val );
}
qint32 Parameters::Z()
{
   return mZ;
}
bool Parameters::A( QString const& val )
{
   return ParseValue( mA, val );
}
double Parameters::A()
{
   return mA;
}
bool Parameters::A1( QString const& val )
{
   return ParseValue( mA1, val );
}
double Parameters::A1()
{
   return mA1;
}
bool Parameters::E( QString const& val )
{
   return ParseValue( mE, val );
}
double Parameters::E()
{
   return mE;
}
bool Parameters::B( QString const& val )
{
   return ParseValue( mB, val );
}
qint32 Parameters::B()
{
   return mB;
}

}//namespace pump
}//namespace test
