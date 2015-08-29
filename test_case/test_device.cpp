#include "test_device.h"

namespace examinee
{

namespace hydro
{

Device::Device( DeviceCollection* collection,
                QString const& name,
                QString const& manufacturer ):
    examinee::Device( collection, name, manufacturer )
{

}

ElectroDevice::ElectroDevice( DeviceCollection* collection,
                              QString const& name,
                              QString const& manufacturer ):
    Device( collection, name, manufacturer )
{
    mProperties.insert( "ТипУправления", "Электронное" );
}

ElectroHydroDevice::ElectroHydroDevice( DeviceCollection* collection,
                                        QString const& name,
                                        QString const& manufacturer,
                                        qint32 min_control_pressure,
                                        qint32 max_control_pressure ):
    Device( collection, name, manufacturer )
{
    mProperties.insert( "ТипУправления", "Электрогидравлическое" );
    mProperties.insert( "ДавлениеУправления.Максимум", max_control_pressure );
    mProperties.insert( "ДавлениеУправления.Минимум", min_control_pressure );
}

}//namespace hydro
}//namespace examinee

