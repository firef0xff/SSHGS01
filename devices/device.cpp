#include "device.h"
#include "device_collection.h"

namespace examinee
{
Device::Device()
{
}
Device::~Device()
{
}

QString const& Device::Name() const
{
    return mName;
}
QString const& Device::Manufacturer() const
{
    return mManufacturer;
}

Device::PropList const& Device::Properties() const
{
    return mProperties;
}

void Device::Name( QString const& v )
{
    mName = v;
}
void Device::Manufacturer( QString const& v )
{
    mManufacturer = v;
}
void Device::Properties( PropList const& v )
{
    mProperties = v;
}


QJsonObject Device::Serailize()
{
    QJsonObject obj;
    obj.insert("name", mName);
    obj.insert("manufacturer", mManufacturer);
    QJsonObject prop;
    for (auto it = mProperties.begin(), end = mProperties.end(); it !=end; ++it)
    {
        switch (it.value().type())
        {
        case QVariant::Bool:
            prop.insert( it.key(), it.value().toBool() );
            break;
        case QVariant::Double:
            prop.insert( it.key(), it.value().toDouble() );
            break;
        case QVariant::String:
            prop.insert( it.key(), it.value().toString() );
            break;
        default:
            break;
        }
    }
    obj.insert("properties", prop);

    return obj;
}
bool Device::DeSerailize( QJsonObject const& obj )
{
    mProperties.clear();
    bool res = true;

    mName = obj.value("name").toString();
    mManufacturer = obj.value("manufacturer").toString();
    QJsonObject prop = obj.value("properties").toObject();

    for (auto it = prop.begin(), end = prop.end(); it !=end; ++it )
    {
        if ( it.value().isBool() )
            mProperties.insert( it.key(), it.value().toBool());
        else if ( it.value().isDouble() )
            mProperties.insert( it.key(), it.value().toDouble());
        else if ( it.value().isString() )
            mProperties.insert( it.key(), it.value().toString());
        else
            res = false;
    }
    return res;
}

}//namespace examinee

