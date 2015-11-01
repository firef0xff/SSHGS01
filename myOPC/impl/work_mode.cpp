#include "work_mode.h"
#include "../miniOPC.h"

#ifdef WINDOWS
namespace opc
{

#define LOCALE_ID    0x409	// Code 0x409 = ENGLISH
#define REQUESTED_UPDATE_RATE 500

WorkMode::WorkMode( wchar_t *ServerName ):
    TimeBias( 0 ),
    PercentDeadband( 0.0 ),
    Connected( false )
{
    //подключение к серверу
    result = CoInitialize( NULL ); //подготовка СОМ библиотек к работе
    // получение идентификатора класса по имени сервера
    result = CLSIDFromProgID( ServerName, &clsid );
    if ( result != S_OK )
    {
        return;
    }
    // получение адреса сервера
    result = CoCreateInstance( clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IOPCServer, (void**)& pIOPCServer );
    if ( result != S_OK )
    {
        return;
    }
    Connected = true;
    Groups.clear();
}

WorkMode::~WorkMode()
{
    Groups.clear();
    result = pIOPCServer->Release();
}

GROUP_ID  WorkMode::AddGroup( wchar_t const* pGroupName, wchar_t const* Addresses[]/*массив второго уровня*/,
                            size_t ItemsCount/*массив считается элементом*/)
{
    if (!Connected)
    {
        return 0;
    }

    boost::shared_ptr<GroupPTRs> tmp( new GroupPTRs() );
    tmp->ItemsCount = ItemsCount;
    result = pIOPCServer->AddGroup( pGroupName,
                                    true,
                                    REQUESTED_UPDATE_RATE,
                                    Groups.size() + 1,
                                    &TimeBias,
                                    &PercentDeadband,
                                    LOCALE_ID,
                                    &GrpSrvHandle,
                                    &RevisedUpdateRate,
                                    IID_IOPCItemMgt,
                                    ( LPUNKNOWN* )& tmp->pItemMgt );
    if( result != S_OK )
    {
        return 0;
    }

    tmp->pItems.resize( tmp->ItemsCount );
    for( int i = 0; i < tmp->ItemsCount; i++ )
    {
        tmp->pItems[i].szAccessPath        	=  L"";
        tmp->pItems[i].szItemID		     	=  Addresses[i];
        tmp->pItems[i].bActive             	=  TRUE;
        tmp->pItems[i].hClient             	=  1;
        tmp->pItems[i].dwBlobSize          	=  0;
        tmp->pItems[i].pBlob			    =  NULL;
        tmp->pItems[i].vtRequestedDataType 	=  0;
    }

    result = tmp->pItemMgt->AddItems( tmp->ItemsCount,
                                      &(*tmp->pItems.begin()),
                                      &tmp->pItemResult,
                                      &pErrors);
    if( result != S_OK && result != S_FALSE )
    {
        return 0;
    }

    result = tmp->pItemMgt->QueryInterface( IID_IOPCSyncIO, (void**)&tmp->pSyncIO );
    if( result < 0 )
    {
        return 0;
    }

    Groups.push_back( tmp );
    return Groups.size();
}

WorkMode::Item    WorkMode::GetGroup( GROUP_ID _id )
{
    int k = 0, pos = _id - 1;//счетчик и реальная позиция в списке
    for ( Item i = Groups.begin(); i != Groups.end(); i++,k++)
    {
        if ( k == pos )
        {
            return i;
        }
    }
    return Groups.end();
}

void 	WorkMode::OpcMassFree( GROUP_ID _id, OPCITEMSTATE* mass )
{
    if ( !Connected )
    {
        return;
    }
    Item _item = GetGroup( _id );
    if ( _item == Groups.end() )
    {
        return;
    }
    boost::shared_ptr<GroupPTRs> __item = *_item;
    for( int i=0; i<__item->ItemsCount; i++ )
    {
        VariantClear( &mass[i].vDataValue );
    }
    CoTaskMemFree( mass );
}

OPCITEMSTATE*	WorkMode::Read ( GROUP_ID _id )
{
    if ( !Connected )
    {
        return nullptr;
    }
    Item _item = GetGroup( _id );
    if ( _item == Groups.end() )
    {
        return nullptr;
    }

    boost::shared_ptr<GroupPTRs> __item = *_item;
    std::vector<OPCHANDLE> phServer;
    phServer.resize( __item->ItemsCount );//массив указателией на OPC
    OPCITEMSTATE *pItemsValues = nullptr; //указатель на состояния итемов в опс

    for( int i=0; i < __item->ItemsCount; i++ )
    {
        phServer[i] = __item->pItemResult[i].hServer;
    }

    result = __item->pSyncIO->Read( OPC_DS_CACHE, __item->ItemsCount, &(*phServer.begin()), &pItemsValues, &pRErrors );
    if( result == S_OK )
    {
        return pItemsValues;
    }
    else
    {
        return nullptr;
    }
}

HRESULT	WorkMode::WriteValue   ( GROUP_ID _id, size_t pos, void *item, types type )
{
    return  WriteMass( _id, pos, 1, item, type );
}
HRESULT	WorkMode::WriteMass ( GROUP_ID _id, size_t pos, size_t mass_len, void *item, types type )
{
    if ( !Connected )
    {
        return E_FAIL;
    }

    Item _item = GetGroup( _id );
    if ( _item==Groups.end() )
    {
        return E_FAIL;
    }

    boost::shared_ptr<GroupPTRs> __item = *_item;
    if ( __item->ItemsCount <= pos )
    {
        return E_FAIL;
    }

    std::vector<OPCHANDLE> phServer;
    phServer.resize( mass_len );
    std::vector<VARIANT> values;
    values.resize( mass_len );
    HRESULT *pWErrors;

    for ( size_t i = 0; i < mass_len; i++ )
    {
        phServer[i]	= __item->pItemResult[pos+i].hServer;
        switch ( type )
        {
            case tBOOL:
            {
                values[i].vt        = VT_BOOL;
                values[i].boolVal   = *( static_cast< bool* >( item ) + i );
                break;
            }
            case tINT:
            {
                values[i].vt    = VT_I4;
                values[i].lVal  = *( static_cast< int* >( item ) + i );
                break;
            }
            case tFLOAT:
            {
                values[i].vt		= VT_R4;
                values[i].fltVal 	= *( static_cast< float* >( item ) + i );
                break;
            }
            default:
            {
                return E_FAIL;
            }
        }
    }

    result = __item->pSyncIO->Write( mass_len, &(*phServer.begin()), &(*values.begin()), &pWErrors );
    HRESULT res = *pWErrors;

    if( result == S_OK || result == S_FALSE )
    {
        CoTaskMemFree( pWErrors );
        return res;
    }
    else
    {
        return res;
    }
}

}//namespace opc

#endif
