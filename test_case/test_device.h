#pragma once
#include "../devices/device.h"

namespace examinee
{

namespace hydro
{

class Device : public examinee::Device
{
public:
    Device( DeviceCollection* collection,
            QString const& name,
            QString const& manufacturer );
};

class ElectroDevice : public Device
{
public:
    ElectroDevice( DeviceCollection* collection,
                   QString const& name,
                   QString const& manufacturer );
};

class ElectroHydroDevice : public Device
{
public:
    ElectroHydroDevice( DeviceCollection* collection,
                        QString const& name,
                        QString const& manufacturer,
                        qint32 min_control_pressure,
                        qint32 max_control_pressure  );
};

}//namespace hydro

}//examinee


