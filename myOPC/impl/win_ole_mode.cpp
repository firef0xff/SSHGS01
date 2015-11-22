#include "win_ole_mode.h"
#include "../miniOPC.h"

#ifdef WINDOWS
namespace opc
{
#define EN_LOCALE_ID    0x0409	// Code 0x0409 = ENGLISH
#define RU_LOCALE_ID    0x0419	// Code 0x0419 = ENGLISH
#define LOCALE_ID    RU_LOCALE_ID	// Code 0x409 = ENGLISH
#define REQUESTED_UPDATE_RATE 500

WinOleMode::WinOleMode( wchar_t const* ServerName ):
    pIOPCServer( nullptr ),
    GrpSrvHandle( 0 ),
    result( E_FAIL ),
    clsid(),
    TimeBias( 0 ),
    PercentDeadband( 0.0 ),
    RevisedUpdateRate( 0 ),
    mConnected( false )
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
    result = CoCreateInstance( clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IOPCServer, (void**) &pIOPCServer );
    LogErrStrong( result );
    if ( result != S_OK )
    {
        return;
    }
    mConnected = true;
    Groups.clear();
}

WinOleMode::~WinOleMode()
{
    Groups.clear();
    if ( pIOPCServer )
        result = pIOPCServer->Release();
}

GROUP_ID  WinOleMode::AddGroup( wchar_t const* pGroupName, wchar_t const* Addresses[]/*массив второго уровня*/,
                            size_t ItemsCount/*массив считается элементом*/)
{
    if (!mConnected)
    {
        return 0;
    }

    GroupPtr tmp( new Group() );
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
    LogErrStrong( result );

    if( result != S_OK )
    {
        return 0;
    }

    tmp->pItems.resize( tmp->ItemsCount );
    for( size_t i = 0; i < tmp->ItemsCount; i++ )
    {
        tmp->pItems[i].szAccessPath        	=  const_cast< wchar_t* >( L"" );           //очень очень дикий костыль
        tmp->pItems[i].szItemID		     	=  const_cast< wchar_t* >( Addresses[i] );  //очень очень дикий костыль
        tmp->pItems[i].bActive             	=  TRUE;
        tmp->pItems[i].hClient             	=  1;
        tmp->pItems[i].dwBlobSize          	=  0;
        tmp->pItems[i].pBlob			    =  NULL;
        tmp->pItems[i].vtRequestedDataType 	=  VT_EMPTY;
        tmp->pItems[i].wReserved            =  0;
    }

    HRESULT* pErrors = nullptr;
    result = tmp->pItemMgt->AddItems( tmp->ItemsCount,
                                      &(*tmp->pItems.begin()),
                                      &tmp->pItemResult,
                                      &pErrors);
    LogErrStrong( result );
    LogErrStrong( *pErrors );

    if( result != S_OK && result != S_FALSE )
    {
        return 0;
    }
    CoTaskMemFree( pErrors );

    result = tmp->pItemMgt->QueryInterface( IID_IOPCSyncIO, (void**)&tmp->pSyncIO );
    LogErrStrong( result );
    if( result < 0 )
    {
        return 0;
    }

    Groups.push_back( std::move( tmp ) );
    return Groups.size();
}

WinOleMode::Item    WinOleMode::GetGroup( GROUP_ID id )
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

void 	WinOleMode::OpcMassFree( GROUP_ID id, OPCITEMSTATE* mass )
{
    Item it = GetGroup( id );
    if ( it == Groups.end() )
    {
        return;
    }
    Group* ptr = it->get();
    for( size_t i=0; i<ptr->ItemsCount; i++ )
    {
        VariantClear( &mass[i].vDataValue );
    }
    CoTaskMemFree( mass );
}

OPCITEMSTATE*	WinOleMode::Read ( GROUP_ID id )
{
    if ( !mConnected )
    {
        return nullptr;
    }
    Item it = GetGroup( id );
    if ( it == Groups.end() )
    {
        return nullptr;
    }

    Group* ptr = it->get();
    std::vector<OPCHANDLE> phServer;
    phServer.resize( ptr->ItemsCount );//массив указателией на OPC
    OPCITEMSTATE *pItemsValues = nullptr; //указатель на состояния итемов в опс

    for( size_t i=0; i < ptr->ItemsCount; i++ )
    {
        phServer[i] = ptr->pItemResult[i].hServer;
    }

    HRESULT* pRErrors = nullptr;
    result = ptr->pSyncIO->Read( OPC_DS_CACHE, ptr->ItemsCount, &(*phServer.begin()), &pItemsValues, &pRErrors );
    LogErrStrong( result );
    CoTaskMemFree( pRErrors );
    if( result == S_OK )
    {
        return pItemsValues;
    }
    else
    {
        return nullptr;
    }
}

HRESULT	WinOleMode::WriteValue   ( GROUP_ID id, size_t pos, void *item, types type )
{
    return  WriteMass( id, pos, 1, item, type );
}
HRESULT	WinOleMode::WriteMass ( GROUP_ID id, size_t pos, size_t mass_len, void *item, types type )
{
    if ( !mConnected )
    {
        return E_FAIL;
    }

    Item it = GetGroup( id );
    if ( it == Groups.end() )
    {
        return E_FAIL;
    }

    Group* ptr = it->get();
    if ( ptr->ItemsCount <= pos )
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
        phServer[i]	= ptr->pItemResult[pos + i].hServer;
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

    result = ptr->pSyncIO->Write( mass_len, &(*phServer.begin()), &(*values.begin()), &pWErrors );

#ifdef DEBUG
    LPWSTR  ErrorStr = L"";    //текст ошибки
    pIOPCServer->GetErrorString( result, LOCALE_ID, &ErrorStr);
    CoTaskMemFree(ErrorStr);

    pIOPCServer->GetErrorString( *pWErrors, LOCALE_ID, &ErrorStr);
    CoTaskMemFree(ErrorStr);
#endif

    HRESULT res = *pWErrors;
    CoTaskMemFree( pWErrors );
    return res;
}
bool    WinOleMode::Connected   ()
{
    return mConnected;
}

void WinOleMode::LogErrStrong( HRESULT err )
{
#ifdef DEBUG
    LPWSTR  ErrorStr = L"";    //текст ошибки
    pIOPCServer->GetErrorString( err, LOCALE_ID, &ErrorStr);
    CoTaskMemFree(ErrorStr);
#else
    (void) err;
#endif
}

}//namespace opc

#endif
