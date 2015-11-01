#pragma once
#include "types.h"

#ifdef WINDOWS
#include "../opc.h"
#include <list>
#include <Classes.hpp>
#include <boost/thread.hpp>

namespace opc
{

class WorkMode
{
public:
    WorkMode( wchar_t *ServerName );//конструктор
    ~WorkMode();//деструктор

    GROUP_ID        AddGroup    ( wchar_t const* pGroupName, wchar_t const* Addresses[], size_t ItemsCount );
    OPCITEMSTATE*   Read        ( GROUP_ID _id ); //вернуть минимально возможный набор который нужно будет распределить в массив
    HRESULT         WriteValue  ( GROUP_ID _id, size_t pos, void *item, types type );
    HRESULT         WriteMass   ( GROUP_ID _id, size_t pos, size_t mass_len, void *item, types type );
    void            OpcMassFree ( GROUP_ID _id, OPCITEMSTATE* mass);

private:
    //типы
    class GroupPTRs                 //структура с данными по шруппе ОПС сервера
    {
    public:
        GroupPTRs():
            pItemMgt( nullptr ),
            pSyncIO ( nullptr ),
            pItems ( nullptr ),
            pItemResult( nullptr )
        {}
        ~GroupPTRs()
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
    typedef std::list< boost::shared_ptr< GroupPTRs > > GroupsList;
    typedef GroupsList::const_iterator Item;

    //функции
    Item GetGroup ( GROUP_ID _id );
    //данные
    IOPCServer  *pIOPCServer;	//указатель на OPC сервер

    GroupsList  Groups;//Данные по группам сервера
    OPCHANDLE   GrpSrvHandle;
    HRESULT     *pErrors, *pRErrors, result;

    _GUID       clsid;		//идентификатор класса сервера
    long        TimeBias; 	//смещение времени
    float       PercentDeadband;
    DWORD       RevisedUpdateRate;

    //состояние сервера
    bool Connected;
};

}//namespace opc
#endif




