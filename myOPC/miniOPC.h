#pragma once
#include "impl/types.h"
#include <memory>
#include <mutex>
#include <thread>

#ifdef WINDOWS
#include "impl/win_ole_mode.h"
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
            while ( !ptr || !ptr->Connected() )
            {
                ptr.reset( new Server() );
                std::this_thread::sleep_for( std::chrono::milliseconds(100) );
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
typedef Server< WinOleMode, SimaticNET > miniOPC;
#else
typedef Server< DemoMode, SimaticNET > miniOPC;
#endif

}//namespace opc
