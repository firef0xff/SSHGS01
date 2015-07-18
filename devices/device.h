#pragma once
#include <QString>
#include <QVariant>

namespace examinee
{

class DeviceCollection;
class Device
{
public:
    Device(DeviceCollection* collection, QString const& name, QString const& manufacturer);
    ~Device();


    QString const& Name() const;
    QString const& Manufacturer() const;
private:
    friend class DeviceCollection;

    Device( Device const& );
    Device& operator = ( Device const&);

    void Free();
    QString mName;
    QString mManufacturer;
    DeviceCollection* mCollection;
};

}//namespace examinee

Q_DECLARE_METATYPE( const examinee::Device* )
