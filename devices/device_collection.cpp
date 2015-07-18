#include "device_collection.h"
#include "device.h"

namespace examinee
{

DeviceCollection::DeviceCollection()
{
}
DeviceCollection::~DeviceCollection()
{
    foreach (auto device, mDevices)
    {
        device->Free();
    }
}
const DeviceCollection::DataType &DeviceCollection::Get()
{
    return mDevices;
}

void DeviceCollection::Register( Device* d )
{
    mDevices.append( d );
}
void DeviceCollection::UnRegister( Device* d )
{
    auto it = std::find(mDevices.begin(), mDevices.end(), d);
    if ( it != mDevices.end() )
        mDevices.erase( it );
}

}//namespace examinee
