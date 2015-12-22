#pragma once
#include <QList>
#include <QJsonArray>

namespace examinee
{
class Device;

class DeviceCollection
{
public:
    typedef QList< Device* > DataType;
    DeviceCollection( QString const& f_name );
    ~DeviceCollection();

    DataType const& Get();
    void Set( DataType const& data );

    bool Load();
    void Save();

private:
    friend class Device;    

    DeviceCollection( DeviceCollection const& );
    void operator = ( DeviceCollection const& );

    QJsonArray Serialize();
    bool DeSerialize( QJsonArray const& arr );

    void Clear();

    DataType mDevices;
    QString mFileName;
};

}//namespace examinee

