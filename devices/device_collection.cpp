#include "device_collection.h"
#include "device.h"
#include <QFile>
#include <QJsonDocument>
#include <memory>


namespace examinee
{

DeviceCollection::DeviceCollection(const QString &f_name):
    mFileName( f_name )
{
}
DeviceCollection::~DeviceCollection()
{
    Clear();
}
DeviceCollection::DataType const& DeviceCollection::Get()
{
    return mDevices;
}
void DeviceCollection::Set( DataType const& data )
{
    //сравнить списки
    foreach (Device* ptr, mDevices)
    {
        if ( std::find(data.begin(), data.end(), ptr) == data.end() )
        {
            //удалить элементы которых нет во входящем
            delete ptr;
        }
    }
    //полностью скопировать входящий
    mDevices = data;
}



void DeviceCollection::Save()
{
    QJsonDocument doc;
    doc.setArray( Serialize() );

    QFile f( mFileName );
    f.open( QFile::WriteOnly );
    f.write( doc.toJson() );
    f.close();
}
bool DeviceCollection::Load()
{
    QFile f( mFileName );
    f.open( QFile::ReadOnly );

    QJsonDocument doc = QJsonDocument::fromJson( f.readAll() );
    f.close();

    return DeSerialize( doc.array() );
}

QJsonArray DeviceCollection::Serialize()
{
    QJsonArray arr;
    foreach (auto ptr, mDevices)
    {
        arr.push_back( ptr->Serailize() );
    }
    return arr;
}
bool DeviceCollection::DeSerialize( QJsonArray const& arr )
{
    Clear();
    bool res = true;
    foreach (QJsonValue const& val, arr)
    {
        std::unique_ptr< Device > ptr ( new Device() );
        bool r = ptr->DeSerailize( val.toObject() );
        if ( r )
        {
            mDevices.push_back( ptr.release() );
        }
        res *= r;
    }
    return res;
}

void DeviceCollection::Clear()
{
    foreach (auto device, mDevices)
    {
        delete device;
    }
    mDevices.clear();
}
}//namespace examinee
