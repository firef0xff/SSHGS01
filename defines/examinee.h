#pragma once
#include <QString>
#include <QVector>

namespace examinee
{

class Device
{
public:
    Device();
    Device(QString const& name, QString const& manufacturer);
    Device( Device const& );
    Device( Device&& );

    Device& operator = ( Device const&);
    Device& operator = ( Device &&);

    QString const& Name() const;
    QString const& Manufacturer() const;
private:
    QString mName;
    QString mManufacturer;
};

class DeviceCollection
{
public:
    static QVector< Device > const& Devices();

private:
    DeviceCollection();
    DeviceCollection( DeviceCollection const& );
    void operator = ( DeviceCollection const& );
    QVector< Device > mDevices;
};

}//namespace examinee

