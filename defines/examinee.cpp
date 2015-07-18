#include "examinee.h"

namespace examinee
{
Device::Device(){}
Device::Device(QString const& name, QString const& manufacturer):
    mName( name ),
    mManufacturer( manufacturer )
{}
Device::Device( Device&& r):
    mName( std::move( r.mName ) ),
    mManufacturer( std::move( r.Manufacturer() ) )
{}
Device::Device( Device const& r ):
    mName( r.mName ),
    mManufacturer( r.Manufacturer() )
{}

Device& Device::operator = ( Device const& r)
{
    if ( &r != this )
    {
        mName = r.mName;
        mManufacturer = r.mManufacturer;
    }
    return *this;
}
Device& Device::operator = ( Device && r)
{
    if ( &r != this )
    {
        mName = std::move( r.mName );
        mManufacturer = std::move( r.mManufacturer );
    }
    return *this;
}

QString const& Device::Name() const
{
    return mName;
}
QString const& Device::Manufacturer() const
{
    return mManufacturer;
}


DeviceCollection::DeviceCollection()
{
    mDevices.push_back( Device("DS3",      "DUPLOMATIC") );
    mDevices.push_back( Device("DL3",      "DUPLOMATIC") );
    mDevices.push_back( Device("DL3B",     "DUPLOMATIC") );
    mDevices.push_back( Device("MDS3",     "DUPLOMATIC") );
    mDevices.push_back( Device("MDF3",     "DUPLOMATIC") );
    mDevices.push_back( Device("DS5",      "DUPLOMATIC") );
    mDevices.push_back( Device("DL5",      "DUPLOMATIC") );
    mDevices.push_back( Device("DL5B",     "DUPLOMATIC") );
    mDevices.push_back( Device("DD44",     "DUPLOMATIC") );
    mDevices.push_back( Device("E*P4",     "DUPLOMATIC") );
    mDevices.push_back( Device("DSP7",     "DUPLOMATIC") );
    mDevices.push_back( Device("DSP10",    "DUPLOMATIC") );
    mDevices.push_back( Device("DS(P)*M",  "DUPLOMATIC") );
    mDevices.push_back( Device("П6",       "РОССИЯ") );
    mDevices.push_back( Device("В6",       "РОССИЯ") );
    mDevices.push_back( Device("1Р6",      "РОССИЯ") );
    mDevices.push_back( Device("В10",      "РОССИЯ") );
    mDevices.push_back( Device("1Р10",     "РОССИЯ") );
    mDevices.push_back( Device("В16",      "РОССИЯ") );
    mDevices.push_back( Device("1Р203",    "REXROTH") );
    mDevices.push_back( Device("1Р323",    "REXROTH") );
    mDevices.push_back( Device("4WE6",     "REXROTH") );
    mDevices.push_back( Device("4WE10",    "REXROTH") );
    mDevices.push_back( Device("4WEH10…32","REXROTH") );
}
QVector< Device > const& DeviceCollection::Devices()
{
    static DeviceCollection collection;
    return collection.mDevices;
}

}//namespace examinee
