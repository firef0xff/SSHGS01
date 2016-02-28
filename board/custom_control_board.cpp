#include "custom_control_board.h"
#include "../settings/settings.h"
#include <sstream>

namespace control_board
{

CustomControlBoard::CustomControlBoard()
{
    Init();
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
    settings->fDtrControl = DTR_CONTROL_ENABLE;
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
    Receive( port );
}
int CustomControlBoard::Command::GetValue( COMPort& port ) const
{
    static const std::string Prefix = "*G ";
    static const std::string Postfix = "#\r\n";
    std::stringstream cmd;
    cmd << Prefix << mAddr << Postfix;    
    Send( cmd.str(), port );

    std::string answer = Receive( port );
    return 0;
}

void CustomControlBoard::Command::Send( std::string const& cmd, COMPort& port ) const
{
    port.Write( reinterpret_cast<const BYTE*>( cmd.c_str() ), cmd.size() );
}
std::string CustomControlBoard::Command::Receive( COMPort& port ) const
{
    typedef std::vector< std::pair< size_t, size_t > > Messages;
    char buff[4096] = {0};
    const int len = 4096;

    char* p_buff = &buff[0];
    size_t remain_len = len;

    const char header[] = "ASCII Table ~ Character Map\r\n";

    bool end = false;
    do
    {
        size_t readed = port.Read( reinterpret_cast<BYTE*>( p_buff ), remain_len );
        remain_len -= std::min( readed, remain_len );
        p_buff += readed;

        if ( buff[0] != '*' )
            throw std::runtime_error( "Ошибка при передаче данных: " + std::string( buff ) );

        char end_symbol = *(p_buff - 1);
        end = end_symbol == '!' || end_symbol == '#';
    }
    while( !end && remain_len );
    *p_buff = 0;
    return std::string( buff, len - remain_len );
}


void CustomControlBoard::test()
{
//     Запись в плату
//     *S A01 3#

//    * Префикс команды
//    S SET – задать значение
//    A01 Имя парамтера
//    3 Значение (signed int) -32767 to 32767
//    # Постфикс.

    CustomControlBoard cb;

    cb.RUN_STOP.SetValue( 2, *cb.mPort );
    cb.RUN_STOP.GetValue( *cb.mPort );
}

}//namespace control_board