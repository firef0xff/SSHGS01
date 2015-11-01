#include "demo_mode.h"
#include "cache/memory.h"

namespace opc
{
static memory::Memory& MEM()
{
    static memory::Memory m;
    return m;
}

DemoMode::DemoMode( wchar_t const* )
{}
DemoMode::~DemoMode()
{}

GROUP_ID        DemoMode::AddGroup    ( wchar_t const* pGroupName, wchar_t const* Addresses[], size_t ItemsCount )
{
    GroupPtr ptr( new Group );
    Group& lnk = *ptr;
    lnk.Name = pGroupName;
    for ( size_t i = 0; i < ItemsCount; ++i )
        lnk.Addresses.push_back( Addresses[i] );
    Groups.push_back( std::move( ptr ) );

    return Groups.size();
}
DemoMode::Item  DemoMode::GetGroup    ( GROUP_ID id )
{
    if ( id > 0)
    {
        --id;
        if ( id < Groups.size() )
        {
            return Groups.begin() + id;
        }
    }
    return Groups.end();
}

OPCITEMSTATE*   DemoMode::Read        ( GROUP_ID id )
{
    Item item = GetGroup( id );
    if ( item == Groups.end() )
        return nullptr;

    Group* ptr = item->get();
    OPCITEMSTATE* res = new OPCITEMSTATE[ ptr->Addresses.size() ]();

    for ( size_t i = 0; i < ptr->Addresses.size(); ++i )
    {
        res[i].vDataValue = MEM().Page< VARIANT >().Read( ptr->Addresses[i] );
    }
    return res;
}

HRESULT         DemoMode::WriteValue  ( GROUP_ID id, size_t pos, void *item, types type )
{
    return WriteMass( id, pos, 1, item, type );
}
HRESULT         DemoMode::WriteMass   ( GROUP_ID id, size_t pos, size_t mass_len, void *item, types type )
{
    Item group = GetGroup( id );
    if ( group == Groups.end() )
        return E_FAIL;

    Group* ptr = group->get();
    if ( ptr->Addresses.size() <= pos )
        return E_FAIL;

    for ( size_t i = 0; i < mass_len; i++ )
    {
        VARIANT value;
        switch ( type )
        {
            case tBOOL:
            {
                value.boolVal   = *( static_cast< bool* >( item ) + i );
                break;
            }
            case tINT:
            {
                value.lVal  = *( static_cast< int* >( item ) + i );
                break;
            }
            case tFLOAT:
            {
                value.fltVal 	= *( static_cast< float* >( item ) + i );
                break;
            }
            default:
            {
                return E_FAIL;
            }
        }
        MEM().Page<VARIANT>().Write( ptr->Addresses.at( pos + i), value );
    }

    return S_OK;
}
void            DemoMode::OpcMassFree ( GROUP_ID /*id*/, OPCITEMSTATE* mass )
{
    delete[] mass;
}
bool            DemoMode::Connected   ()
{
    return true;
}

}//namespace opc

