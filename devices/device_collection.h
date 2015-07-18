#pragma once
#include <QList>

namespace examinee
{
class Device;

class DeviceCollection
{
public:
    typedef QList< Device* > DataType;
    DeviceCollection();
    ~DeviceCollection();

    DataType const& Get();

private:
    friend class Device;

    DeviceCollection( DeviceCollection const& );
    void operator = ( DeviceCollection const& );

    void Register( Device* );
    void UnRegister( Device* );

    DataType mDevices;
};

}//namespace examinee

