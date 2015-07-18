#include "device.h"
#include "device_collection.h"

namespace examinee
{
Device::Device(DeviceCollection *collection, QString const& name, QString const& manufacturer):
    mName( name ),
    mManufacturer( manufacturer ),
    mCollection( collection )
{
    if ( mCollection != nullptr )
        mCollection->Register( this );
}
Device::~Device()
{
    if ( mCollection != nullptr )
        mCollection->UnRegister( this );
}

void Device::Free()
{
    mCollection = nullptr;
}
QString const& Device::Name() const
{
    return mName;
}
QString const& Device::Manufacturer() const
{
    return mManufacturer;
}

}//namespace examinee

