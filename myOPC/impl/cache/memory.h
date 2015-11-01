#pragma once
#include "cache.h"
#include "def.h"

namespace memory
{

class Memory
{
public:    
    Memory();
    ~Memory();

    template <typename ValueType>
    Cache<addr_type, ValueType>& Page()
    {
        static uint id = ++type_id;

        Pages::iterator it = memory.find( id );
        if ( it != memory.end() )
        {
            return *static_cast< Cache< addr_type, ValueType >* > ( it->second );
        }
        else
        {
            Cache< addr_type, ValueType >* ptr = new Cache<addr_type, ValueType>();
            memory.insert( PagesItem( id, ptr ) );
            return *ptr;
        }
    }

    void Reset();

private:
    typedef unsigned int uint;
    typedef CacheBase* CachePtr;
    typedef std::pair< const uint, CachePtr > PagesItem;
    typedef std::map< const uint, CachePtr > Pages;

    Pages memory;
    static uint type_id;
};

}
