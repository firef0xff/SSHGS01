#pragma once
#include <QString>
#include <QVariant>
#include <QMap>

namespace examinee
{

class DeviceCollection;
class Device
{
public:

    typedef QMap< QString, QVariant > PropList;
    Device( DeviceCollection* collection, QString const& name, QString const& manufacturer );
    virtual ~Device();

    QString const& Name() const;
    QString const& Manufacturer() const;
    PropList const& Properties() const;

protected:
    PropList mProperties;

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
