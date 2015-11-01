#pragma once
#include "impl/types.h"
#include <memory>
#include <mutex>

#ifdef WINDOWS
#include "impl/work_mode.h"
#else
#include "impl/demo_mode.h"
#endif

namespace opc
{

template < class Impl, class NameRegistrator >
class Server : public Impl
{
public:
    static Server& Instance()
    {
        static std::unique_ptr< Server > ptr;
        if ( !ptr || !ptr->Connected() )
        {
            static std::mutex m;
            std::lock_guard< std::mutex > lock( m );
            if ( !ptr || !ptr->Connected() )
            {
                ptr.reset( new Server() );
            }
        }

        return *ptr;
    }

private:
    Server():Impl( NameRegistrator::Name() ){}
    Server( Server const& );
    void operator = ( Server const& );
};

struct SimaticNET
{
    static wchar_t const* Name()
    {
        return L"OPC.SimaticNET";
    }
};

#ifdef WINDOWS
typedef Server< WorkMode, SimaticNET > miniOPC;
#else
typedef Server< DemoMode, SimaticNET > miniOPC;
#endif

}//namespace opc
