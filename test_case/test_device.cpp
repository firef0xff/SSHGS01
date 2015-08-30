#include "test_device.h"
#include "tests.h"
namespace examinee
{

namespace hydro
{

Device::Device( QString const& name,
                QString const& manufacturer ):
    examinee::Device( &test::HydroTests.Devices(), name, manufacturer )
{

}

ElectroDevice::ElectroDevice( QString const& name,
                              QString const& manufacturer ):
    Device( name, manufacturer )
{
    mProperties.insert( "ТипУправления", "Электронное" );
}

ElectroHydroDevice::ElectroHydroDevice( QString const& name,
                                        QString const& manufacturer,
                                        qint32 min_control_pressure,
                                        qint32 max_control_pressure ):
    Device( name, manufacturer )
{
    mProperties.insert( "ТипУправления", "Электрогидравлическое" );
    mProperties.insert( "ДавлениеУправления.Максимум", max_control_pressure );
    mProperties.insert( "ДавлениеУправления.Минимум", min_control_pressure );
}

}//namespace hydro

namespace servo
{

Device::Device( QString const& name,
                QString const& manufacturer ):
    examinee::Device( &test::ServoTests.Devices(), name, manufacturer )
{
}

}//namespace servo

}//namespace examinee

