#pragma once
#include "impl/types.h"
#include <memory>
#include <mutex>
#include <thread>

#ifdef DEMO
#include "impl/demo_mode.h"
#else
#ifdef WINDOWS
#include "impl/win_ole_mode.h"
#endif
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
struct VipaOpc7
{
    static wchar_t const* Name()
    {
        return L"VIPA.OPCServer";
    }
};

#ifdef DEMO
typedef Server< DemoMode, VipaOpc7 > miniOPC;
#else
#ifdef WINDOWS
typedef Server< WinOleMode, VipaOpc7 > miniOPC;
#endif
#endif

template < class T >
void    ReadToArray (  VARIANT& variant, T* array, uint32_t size )
{
    T* values = nullptr;
    miniOPC::Instance().GetArrayData( variant, reinterpret_cast<void**>(&values) );
    if ( values )
    {
        for (size_t i = 0; i < size; i++)
        {
            array[ i ] = values[i];
        }
    }
    miniOPC::Instance().FreeArrayData( variant );
}

}//namespace opc
