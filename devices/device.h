#pragma once
#include <QString>
#include <QVariant>
#include <QMap>
#include <QJsonObject>

namespace examinee
{

class DeviceCollection;
class Device
{
public:

    typedef QMap< QString, QVariant > PropList;
    Device();
    virtual ~Device();

    QString const& Name() const;
    QString const& Manufacturer() const;
    PropList const& Properties() const;

    void Name( QString const& v );
    void Manufacturer( QString const& v );
    void Properties( PropList const& v );

    virtual QJsonObject Serailize();
    virtual bool DeSerailize( QJsonObject const& obj );

protected:
    PropList mProperties;

private:
    Device( Device const& );
    Device& operator = ( Device const&);

    QString mName;
    QString mManufacturer;
};

}//namespace examinee

Q_DECLARE_METATYPE( const examinee::Device* )
Q_DECLARE_METATYPE( examinee::Device* )
