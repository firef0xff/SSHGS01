#pragma once
#include "../../mylib/COMPort/ComPort.h"

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
    static void test();
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
    Command RUN_STOP = Command( "A01", 0, 0, 3 );

//    Минимальный ток. Аналог параметра o1: I Min мА
    Command MIN_CUR = Command( "A02", 0, 0, 5000 );

//    Максимальный ток. Аналог параметра G1: “I Max” мА
    Command MAX_CUR = Command( "A03", 5000, 0, 5000 );

//    Амплитуда вибрационного сигнала мА
    Command V_AMP = Command( "A04", 0, 0, 200 );

//    Частота вибрационного сигнала, Гц
    Command V_FREQ = Command( "A05", 5, 1, 200 );

//    Полярность сигнала А
//    0 – биполярный
//    1 – однополярный в положительной части
//    2 – однополярный в отрицательной части
    Command POLARITY_A = Command( "A06", 0, 0, 2 );
//    Полярность сигнала B
//    0 – биполярный
//    1 – однополярный в положительной части
//    2 – однополярный в отрицательной части
    Command POLARITY_B = Command( "A07", 0, 0, 2 );

//    Входной ток (управление) мкА
    Command Input = Command( "A10", 12000, 4000, 20000 );

//    Значение на выходе А мА
    Command OutA = Command( "A11", 0, -5000, 5000 );
//    Значение на выходе Б мА
    Command OutB = Command( "A12", 0, -5000, 5000 );

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


