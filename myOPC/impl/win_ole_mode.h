#pragma once
#include "types.h"

#ifdef WINDOWS
#include "../opc.h"
#include <vector>
#include <memory>

namespace opc
{

class WinOleMode
{
public:
    WinOleMode( wchar_t const* ServerName );//конструктор
    ~WinOleMode();//деструктор

    GROUP_ID        AddGroup    ( wchar_t const* pGroupName, wchar_t const* Addresses[], size_t ItemsCount );
    OPCITEMSTATE*   Read        ( GROUP_ID id ); //вернуть минимально возможный набор который нужно будет распределить в массив
    HRESULT         WriteValue  ( GROUP_ID id, size_t pos, void *item, types type );
    HRESULT         WriteMass   ( GROUP_ID id, size_t pos, size_t mass_len, void *item, types type );
    void            OpcMassFree ( GROUP_ID id, OPCITEMSTATE* mass);
    bool            Connected   ();

private:
    //типы
    class Group                 //структура с данными по шруппе ОПС сервера
    {
    public:
        Group():
            pItemMgt( nullptr ),
            pSyncIO ( nullptr ),
            pItemResult( nullptr )
        {}
        ~Group()
        {
            if ( pItemResult )
            {
                CoTaskMemFree( pItemResult );
            }
        }

        IOPCItemMgt		*pItemMgt;		//указатель на Группу адресов с данными
        IOPCSyncIO  	*pSyncIO;		//Указатель на ентерфейс для синхронизации данных
        std::vector<OPCITEMDEF> pItems;	//массив с элементами группы
        OPCITEMRESULT   *pItemResult;	//указатель на элемент
        size_t			ItemsCount;		//количество элементов в группе
    };    
    typedef std::unique_ptr< Group > GroupPtr;
    typedef std::vector< GroupPtr > GroupsList;
    typedef GroupsList::const_iterator Item;

    //функции
    Item GetGroup ( GROUP_ID id );
    //данные
    IOPCServer  *pIOPCServer;	//указатель на OPC сервер

    GroupsList  Groups;//Данные по группам сервера
    OPCHANDLE   GrpSrvHandle;
    HRESULT     result;

    _GUID       clsid;		//идентификатор класса сервера
    long        TimeBias; 	//смещение времени
    float       PercentDeadband;
    DWORD       RevisedUpdateRate;

    //состояние сервера
    bool mConnected;
};

}//namespace opc
#endif




