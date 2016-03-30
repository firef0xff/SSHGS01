#pragma once
#include "../../mylib/COMPort/ComPort.h"
#include <stdexcept>
#include <memory>

namespace control_board
{

class COMError : public std::runtime_error
{
public:
    COMError ( std::string const& err ):
        std::runtime_error( err )
    {}
};
class CustomControlBoard
{
public:
    CustomControlBoard();
    ~CustomControlBoard();

    void SetRUN_STOP( int );
    int  GetRUN_STOP();

    void SetMIN_CUR( int );
    int  GetMIN_CUR();

    void SetMAX_CUR( int );
    int  GetMAX_CUR();

    void SetV_AMP( int );
    int  GetV_AMP();

    void SetV_FREQ( int );
    int  GetV_FREQ();

    void SetPOLARITY_A( int );
    int  GetPOLARITY_A();

    void SetPOLARITY_B( int );
    int  GetPOLARITY_B();

    void SetInput( int );
    int  GetInput();

    void SetOutA( int );
    int  GetOutA();

    void SetOutB( int );
    int  GetOutB();

    void SetA21( int );
    int  GetA21();

    void SetA22( int );
    int  GetA22();

    void SetA23( int );
    int  GetA23();

    void SetA24( int );
    int  GetA24();

    void SetA25( int );
    int  GetA25();

    void SetA26( int );
    int  GetA26();

    void SetA30( int );
    int  GetA30();

    void SetA31( int );
    int  GetA31();
private:
    class Command
    {
    public:
        Command( std::string const& addr, int def_val, int min_val, int max_val);
        void SetValue( int val, COMPort& port );
        int GetValue( COMPort& port ) const;

    private:
        void Send( std::string const& cmd, COMPort& port ) const;
        std::string Receive( COMPort& port ) const;

        std::string mAddr;
        int mDefVal;
        int mMinVal;
        int mMaxVal;
    };
    void Init();
    std::unique_ptr< COMPort > mPort;

//    0 – Силовые выходы на 5 А отключены
//    1 – Силовой выход А включен
//    2 – Силовой выход В включен
//    3 – оба выхода включены
    Command A01 = Command( "A01", 0, 0, 3 );

//    Минимальный ток. Аналог параметра o1: I Min мА
    Command A02 = Command( "A02", 0, 0, 5000 );

//    Максимальный ток. Аналог параметра G1: “I Max” мА
    Command A03 = Command( "A03", 5000, 0, 5000 );

//    Амплитуда вибрационного сигнала мА
    Command A04 = Command( "A04", 0, 0, 200 );

//    Частота вибрационного сигнала, Гц
    Command A05 = Command( "A05", 5, 1, 200 );

//    Полярность сигнала А
//    0 – биполярный
//    1 – однополярный в положительной части
//    2 – однополярный в отрицательной части
    Command A06 = Command( "A06", 0, 0, 2 );
//    Полярность сигнала B
//    0 – биполярный
//    1 – однополярный в положительной части
//    2 – однополярный в отрицательной части
    Command A07 = Command( "A07", 0, 0, 2 );

//    Входной ток (управление) мкА
    Command A10 = Command( "A10", 12000, 4000, 20000 );

//    Значение на выходе А мА
    Command A11 = Command( "A11", 0, -5000, 5000 );
//    Значение на выходе Б мА
    Command A12 = Command( "A12", 0, -5000, 5000 );

//    0 – Силовой выходы на 200 мА отключены
//    1 – Силовой выход включен
    Command A21 = Command( "A21", 0, 0, 1 );
//    Минимальный ток. Аналог параметра o1: I Min мА
    Command A22 = Command( "A22", 0, 0, 200 );
//    Максимальный ток. Аналог параметра G1: “I Max”
    Command A23 = Command( "A23", 200, 0, 200 );
//    Амплитуда вибрационного сигнала мА, Гц
    Command A24 = Command( "A24", 0, 0, 20 );
//    Частота вибрационного сигнала
    Command A25 = Command( "A25", 5, 1, 200 );
//    Полярность сигнала А
//    0 – биполярный
//    1 – однополярный в положительной части
//    2 – однополярный в отрицательной части
    Command A26 = Command( "A26", 0, 0, 2 );
//    Входной ток (управление)
    Command A30 = Command( "A30", 12000, 4000, 20000 );
//    Значение на выходе
    Command A31 = Command( "A31", 0, -200, 200 );
};


}//namespace control_board


