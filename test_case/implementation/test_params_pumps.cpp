#include "test_params_pumps.h"
#include <memory>
#include <mutex>
#include "../tests.h"
#include "../../cpu/cpu_memory.h"

namespace test
{
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

   mSerianNo = "";   //Идентификационный номер
   mSpin = spUnknown;   //Направление вращения
   mWorkVolume = 0.0; //рабочий объем
   //давление, бар
   mPressureMin = 0.0; //Минимальное
   mPressureNom = 0.0; //номинальное (рабочее)
   mPressureMax = 0.0; //максимальное

   //Частота вращения об/мин
   mSpinSpeedMin = 0; //Минимальная
   mSpinSpeedNom = 0; //Номинальная
   mSpinSpeedMax = 0; //Максимальная

   mVolumeKPD = 0.0; //Объемный кпд насоса
   mExpenditure = 0.0; //Дренаж насоса при номинальном режиме л/мин
}

QString Parameters::ToString() const
{
   QString res;
   res += "Параметры насоса:\n";
   res += "  Идентификационный номер:" + mSerianNo + "\n";
   res += "  Направление вращения:" + test::ToString(mSpin) + "\n";
   res += "  Рабочий объем:" + test::ToString(mWorkVolume) + "\n";

   res += "  Давление:\n";
   res += "  -минимальное, бар:" + test::ToString(mPressureMin) + "\n";
   res += "  -номинальное, бар:" + test::ToString(mPressureNom) + "\n";
   res += "  -максимальное, бар:" + test::ToString(mPressureMax) + "\n";

   res += "  Частота вращения приводного вала:\n";
   res += "  -минимальная, об/мин:" + test::ToString(mSpinSpeedMin) + "\n";
   res += "  -номинальная, об/мин:" + test::ToString(mSpinSpeedNom) + "\n";
   res += "  -максимальная, об/мин:" + test::ToString(mSpinSpeedMax) + "\n";

   res += "  Объемный КПД насоса, %:" + test::ToString(mVolumeKPD) + "\n";
   res += "  Дренаж насоса при номинальном режиме, л/мин:" + test::ToString(mSpinSpeedMax) + "\n";
}

test::TestCase& Parameters::TestCollection() const
{
   return Pumps;
}

QJsonObject Parameters::Serialise() const
{
    QJsonObject obj;
    obj.insert("SerianNo", mSerianNo);
    obj.insert("Spin", mSpin);
    obj.insert("WorkVolume", mWorkVolume);
    obj.insert("PressureMin", mPressureMin);
    obj.insert("PressureNom", mPressureNom);
    obj.insert("PressureMax", mPressureMax);
    obj.insert("SpinSpeedMin", mSpinSpeedMin);
    obj.insert("SpinSpeedNom", mSpinSpeedNom);
    obj.insert("SpinSpeedMax", mSpinSpeedMax);
    obj.insert("VolumeKPD", mVolumeKPD);
    obj.insert("Expenditure", mExpenditure);

    QJsonObject ret = test::Parameters::Serialise();
    ret.insert("pump", obj);

    return ret;
}
bool Parameters::Deserialize(const QJsonObject &obj )
{
    Reset();
    bool ret = test::Parameters::Deserialize( obj );
    auto val = obj.value("pupm");
    if ( val.isObject() )
    {
        QJsonObject obj = val.toObject();
        mSerianNo = obj.value("SerianNo").toString();
        mSpin =  static_cast<SPIN>( obj.value("Spin").toInt() );
        mWorkVolume =  obj.value("WorkVolume").toDouble();

        mPressureMin = obj.value("PressureMin").toDouble();
        mPressureNom = obj.value("PressureNom").toDouble();
        mPressureMax = obj.value("PressureMax").toDouble();

        mSpinSpeedMin = obj.value("SpinSpeedMin").toInt();
        mSpinSpeedNom = obj.value("SpinSpeedNom").toInt();
        mSpinSpeedMax = obj.value("SpinSpeedMax").toInt();

        mVolumeKPD = obj.value("VolumeKPD").toDouble();
        mExpenditure = obj.value("Expenditure").toDouble();
    }
    else
        ret = false;

    return ret;
}

void Parameters::WriteToController() const
{
//TODO
}

bool Parameters::Draw(QPainter &painter, QRect &free_rect, QString const& compare_width ) const
{
   //TODO
    return true;
}

bool Parameters::DrawResults(QPainter &painter, QRect &free_rect ) const
{
   //TODO
    return true;
}

QString Parameters::ModelId() const
{
    return mSerianNo;
}


}//namespace pump
}//namespace test
