#include "custom_control_board.h"
#include "../settings/settings.h"
#include <sstream>
#include <chrono>

namespace control_board
{

CustomControlBoard::CustomControlBoard()
{
    Init();
}
CustomControlBoard::~CustomControlBoard()
{
    try
    {
        SetRUN_STOP( 0 );
        SetA21( 0 );
    }
    catch(...)
    {}
}

void CustomControlBoard::Init()
{
    //подготовка  таймаутов
    std::unique_ptr< COMMTIMEOUTS > TimeOuts ( new COMMTIMEOUTS() );
    TimeOuts->ReadIntervalTimeout = 0xFFFFFFFF;
    TimeOuts->ReadTotalTimeoutMultiplier = 0;
    TimeOuts->ReadTotalTimeoutConstant = 0;
    TimeOuts->WriteTotalTimeoutMultiplier = 0;
    TimeOuts->WriteTotalTimeoutConstant =0;

    QString Addr = app::Settings::Instance().ComAddr();
    mPort.reset(new COMPort( Addr.toStdString(), std::move( TimeOuts ), COMPort::CommMasks::ev_rxchar, 4096, 4096 )  );

    //корректировка настроек порта DCB

    std::unique_ptr< DCB> settings = mPort->Get_port_Settings();
    settings->BaudRate =COMPort::BaudRates::BR_115200;
    settings->ByteSize = 8;
    settings->Parity = COMPort::Paritys::NOparity;
    settings->StopBits = COMPort::StopBits::ONE;
    settings->fAbortOnError = FALSE;
    settings->fDtrControl = DTR_CONTROL_DISABLE;
    settings->fRtsControl = RTS_CONTROL_DISABLE;
    settings->fBinary = TRUE;
    settings->fParity = COMPort::fParitys::OFF;
    settings->fInX = FALSE;
    settings->fOutX = FALSE;
    settings->XonChar = (BYTE)0x11;
    settings->XoffChar = (BYTE)0x13;
    settings->fErrorChar = FALSE;
    settings->fNull = FALSE;
    settings->fOutxCtsFlow = FALSE;
    settings->fOutxDsrFlow = FALSE;
    settings->XonLim = 1024;
    settings->XoffLim = 1024;
    settings->fDsrSensitivity=FALSE;
    mPort->Set_DCB_Settings( std::move( settings ) );
}


void CustomControlBoard::SetRUN_STOP( int v )
{
    A01.SetValue( v, *mPort );
}
int  CustomControlBoard::GetRUN_STOP()
{
    return A01.GetValue( *mPort );
}

void CustomControlBoard::SetMIN_CUR( int v )
{
    A02.SetValue( v, *mPort );
}
int  CustomControlBoard::GetMIN_CUR()
{
    return A02.GetValue( *mPort );
}

void CustomControlBoard::SetMAX_CUR( int  v)
{
    A03.SetValue( v, *mPort );
}
int  CustomControlBoard::GetMAX_CUR()
{
    return A03.GetValue( *mPort );
}

void CustomControlBoard::SetV_AMP( int v)
{
    A04.SetValue( v, *mPort );
}
int  CustomControlBoard::GetV_AMP()
{
    return A04.GetValue( *mPort );
}

void CustomControlBoard::SetV_FREQ( int v)
{
    A05.SetValue( v, *mPort );
}
int  CustomControlBoard::GetV_FREQ()
{
    return A05.GetValue( *mPort );
}

void CustomControlBoard::SetPOLARITY_A( int v)
{
    A06.SetValue( v, *mPort );
}
int  CustomControlBoard::GetPOLARITY_A()
{
    return A06.GetValue( *mPort );
}

void CustomControlBoard::SetPOLARITY_B( int v)
{
    A07.SetValue( v, *mPort );
}
int  CustomControlBoard::GetPOLARITY_B()
{
    return A07.GetValue( *mPort );
}

void CustomControlBoard::SetInput( int v)
{
    A10.SetValue( v, *mPort );
}
int  CustomControlBoard::GetInput()
{
    return A10.GetValue( *mPort );
}

void CustomControlBoard::SetOutA( int v)
{
    A11.SetValue( v, *mPort );
}
int  CustomControlBoard::GetOutA()
{
    return A11.GetValue( *mPort );
}

void CustomControlBoard::SetOutB( int v)
{
    A12.SetValue( v, *mPort );
}
int  CustomControlBoard::GetOutB()
{
    return A12.GetValue( *mPort );
}

void CustomControlBoard::SetA21( int v)
{
    A21.SetValue( v, *mPort );
}
int  CustomControlBoard::GetA21()
{
    return A21.GetValue( *mPort );
}

void CustomControlBoard::SetA22( int v )
{
    A22.SetValue( v, *mPort );
}
int  CustomControlBoard::GetA22()
{
    return A22.GetValue( *mPort );
}

void CustomControlBoard::SetA23( int v )
{
    A23.SetValue( v, *mPort );
}
int  CustomControlBoard::GetA23()
{
    return A23.GetValue( *mPort );
}

void CustomControlBoard::SetA24( int v )
{
    A24.SetValue( v, *mPort );
}
int  CustomControlBoard::GetA24()
{
    return A24.GetValue( *mPort );
}

void CustomControlBoard::SetA25( int v )
{
    A25.SetValue( v, *mPort );
}
int  CustomControlBoard::GetA25()
{
    return A25.GetValue( *mPort );
}


void CustomControlBoard::SetA26( int v )
{
    A26.SetValue( v, *mPort );
}
int  CustomControlBoard::GetA26()
{
    return A26.GetValue( *mPort );
}

void CustomControlBoard::SetA30( int v )
{
    A30.SetValue( v, *mPort );
}
int  CustomControlBoard::GetA30()
{
    return A30.GetValue( *mPort );
}

void CustomControlBoard::SetA31( int v )
{
    A31.SetValue( v, *mPort );
}
int  CustomControlBoard::GetA31()
{
    return A31.GetValue( *mPort );
}


CustomControlBoard::Command::Command( std::string const& addr, int def_val, int min_val, int max_val):
    mAddr( addr ),
    mDefVal( def_val ),
    mMinVal( min_val ),
    mMaxVal( max_val )
{}
void CustomControlBoard::Command::SetValue( int val, COMPort& port )
{
    if ( val < mMinVal || val > mMaxVal )
        val = mDefVal;
    static const std::string Prefix = "*S ";
    static const std::string Postfix = "#\r\n";
    std::stringstream cmd;
    cmd << Prefix << mAddr << " " << val << Postfix;
    Send( cmd.str(), port );
    auto answ = Receive( port );
    if ( answ != "*DONE!\r\n" && answ != "*DONE#\n\r" )
        throw COMError( mAddr + " set: " + answ );

}
int CustomControlBoard::Command::GetValue( COMPort& port ) const
{
    static const std::string Prefix = "*G ";
    static const std::string Postfix = "#\r\n";
    std::stringstream cmd;
    cmd << Prefix << mAddr << Postfix;    
    Send( cmd.str(), port );

    auto answer = Receive( port );
    auto addtr = answer.substr( 1, 3);
    auto mark = answer.substr( 5, 1);
    auto ending = answer.substr( answer.length() - 3, 3);
    auto value = answer.substr( 7, answer.length() - 3 - 7 );
    if ( answer[0] != '*' ||
         addtr != mAddr ||
         mark != "=" ||
         (ending != Postfix && ending != "#\n\r") )
    {
        addtr = answer.substr( 1, 2);
        mark = answer.substr( 4, 1);
        ending = answer.substr( answer.length() - 3, 3);
        value = answer.substr( 6, answer.length() - 3 - 6 );
        if (mark != "=" ||
            ending != "#\n\r")
            throw COMError( mAddr + " get: " + answer );
    }

    return stoi( value );
}

void CustomControlBoard::Command::Send( std::string const& cmd, COMPort& port ) const
{
    port.Write( reinterpret_cast<const BYTE*>( cmd.c_str() ), cmd.size() );
}
std::string CustomControlBoard::Command::Receive( COMPort& port ) const
{
    char buff[4096] = {0};
    const int len = 4096;

    char* p_buff = &buff[0];
    size_t remain_len = len;

    bool end = false;

    auto start = std::chrono::system_clock::now();
    do
    {
        size_t readed = port.Read( reinterpret_cast<BYTE*>( p_buff ), remain_len );
        remain_len -= std::min( readed, remain_len );
        p_buff += readed;
        char end_symbol = *(p_buff - 1);
        char prev_symbol = *(p_buff - 2);
        end = ( ( prev_symbol =='\r' && end_symbol == '\n' ) ||
                ( prev_symbol =='\n' && end_symbol == '\r') );

        if ( (std::chrono::system_clock::now() - start) > std::chrono::seconds(10) )
            throw COMError("Read timeout");
    }
    while( !end && remain_len );
    *p_buff = 0;
    return std::string( buff, len - remain_len );
}

}//namespace control_board
