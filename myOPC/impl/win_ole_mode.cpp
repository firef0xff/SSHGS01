#include "win_ole_mode.h"
#include "../miniOPC.h"
#include <iostream>


#ifdef WINDOWS
namespace opc
{
#define EN_LOCALE_ID    0x0409	// Code 0x0409 = ENGLISH
#define RU_LOCALE_ID    0x0419	// Code 0x0419 = ENGLISH
#define LOCALE_ID    RU_LOCALE_ID	// Code 0x409 = ENGLISH
#define REQUESTED_UPDATE_RATE 500


SyncThread::SyncThread():
    interrupt(false)
{}
SyncThread::~SyncThread()
{
    interrupt = true;
    mRunWaiter.notify_all();
    mExecWaiter.notify_all();
    if ( mThread.joinable() )
        mThread.join();
}
void SyncThread::Start()
{
    std::unique_lock< std::mutex > lock( Mutex );
    mThread = std::thread( &SyncThread::Run, this );
    mRunWaiter.wait(lock);
}
void SyncThread::Exec( Function func )
{
    std::unique_lock< std::mutex > lock( Mutex );
    mFunc = func;
    mRunWaiter.notify_one();
    mExecWaiter.wait( lock );
}
void SyncThread::Run()
{
    for(;;)
    {
        try
        {
            mRunWaiter.notify_all();
            std::unique_lock< std::mutex > lock( Mutex );
            mRunWaiter.wait(lock);
            if ( interrupt )
                return;
            if (mFunc)
                mFunc();
            mExecWaiter.notify_one();
        }
        catch(...)
        {

        }
    }
}


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
    mThread.Start();
    mThread.Exec( std::bind( &WinOleMode::Init, this, ServerName ) );
}


void WinOleMode::Init( wchar_t const* ServerName )
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
    GROUP_ID id = 0;
    mThread.Exec( std::bind( &WinOleMode::AddGroupImpl, this, std::ref(id), pGroupName, Addresses, ItemsCount ) );
    return id;
}

void WinOleMode::AddGroupImpl ( GROUP_ID& id, wchar_t const* pGroupName, wchar_t const* Addresses[], size_t ItemsCount )
{
    if (!mConnected)
    {
        id = 0;
        return;
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
        id = 0;
        return;
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
        id = 0;
        return;
    }
    CoTaskMemFree( pErrors );

    result = tmp->pItemMgt->QueryInterface( IID_IOPCSyncIO, (void**)&tmp->pSyncIO );
    LogErrStrong( result );
    if( result < 0 )
    {
        id = 0;
        return;
    }

    Groups.push_back( std::move( tmp ) );
    id = Groups.size();
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
    mThread.Exec( std::bind( &WinOleMode::OpcMassFreeImpl, this, id, mass ) );
}
void    WinOleMode::OpcMassFreeImpl( GROUP_ID id, OPCITEMSTATE* mass)
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
    OPCITEMSTATE *res = nullptr;
    mThread.Exec( std::bind( &WinOleMode::ReadImpl, this, &res, id ) );
    return res;
}
void            WinOleMode::ReadImpl ( OPCITEMSTATE **res, GROUP_ID id )
{
    if ( !mConnected )
    {
        *res = nullptr;
        return;
    }
    Item it = GetGroup( id );
    if ( it == Groups.end() )
    {
        *res = nullptr;
        return;
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
    if ( pRErrors )
    {
        LogErrStrong( *pRErrors );
        CoTaskMemFree( pRErrors );
    }
    if( result == S_OK )
    {
        *res = pItemsValues;
        return;
    }
    else
    {
        *res = nullptr;
        return;
    }
}


HRESULT	WinOleMode::WriteValue  ( GROUP_ID id, size_t pos, void *item, types type )
{
    return  WriteMass( id, pos, 1, item, type );
}
HRESULT	WinOleMode::WriteMass   ( GROUP_ID id, size_t pos, size_t mass_len, void *item, types type )
{
    HRESULT res = E_FAIL;
    mThread.Exec( std::bind( &WinOleMode::WriteMassImpl, this, std::ref(res), id, pos, mass_len, item, type ) );
    return res;
}
void    WinOleMode::WriteMassImpl   ( HRESULT &res, GROUP_ID id, size_t pos, size_t mass_len, void *item, types type )
{
    if ( !mConnected )
    {
        res = E_FAIL;
        return;
    }

    Item it = GetGroup( id );
    if ( it == Groups.end() )
    {
        res = E_FAIL;
        return;
    }

    Group* ptr = it->get();
    if ( ptr->ItemsCount <= pos )
    {
        res = E_FAIL;
        return;
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
                res = E_FAIL;
                return;
            }
        }
    }

    result = ptr->pSyncIO->Write( mass_len, &(*phServer.begin()), &(*values.begin()), &pWErrors );

    LogErrStrong(result);

    res = result;
    if (pWErrors)
    {
        LogErrStrong(*pWErrors);
        res = *pWErrors;
        CoTaskMemFree( pWErrors );
    }
}
void    WinOleMode::GetArrayData( VARIANT& variant, void **values )
{
    SafeArrayAccessData( variant.parray, values );
}
void    WinOleMode::FreeArrayData( VARIANT& variant )
{
    SafeArrayUnaccessData( variant );
}
bool    WinOleMode::Connected   ()
{
    return mConnected;
}

void WinOleMode::LogErrStrong( HRESULT err )
{    
    if (err)
        std::cout << std::hex << err << std::endl;
#ifdef DEBUG
    LPWSTR  ErrorStr = L"";    //текст ошибки
    pIOPCServer->GetErrorString( err, LOCALE_ID, &ErrorStr);
    CoTaskMemFree(ErrorStr);
#endif
}



}//namespace opc

#endif
