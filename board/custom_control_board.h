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

//    void SetInput( int );
    int  GetInput();

    void SetOutA( int );
    int  GetOutA();

    void SetOutB( int );
    int  GetOutB();

    int  GetOverLoadA();
    int  GetOverLoadB();
    int  GetOverLoadC();

    void SetListenParam( int );

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

//    void SetA30( int );
    int  GetA30();

    void SetA31( int );
    int  GetA31();

    void Load( int );
    void Save( int );

    std::string TestRanges();
private:
    class Command
    {
    public:
        Command( std::string const& addr, int def_val, int min_val, int max_val);
        void SetValue( int val, COMPort& port );
        int GetValue( COMPort& port ) const;
        bool TestRange( std::string& addr, int& min, int&max, COMPort& port );

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

//    Максимальный ток. канала А и Б при 20мА на входе
    Command A02 = Command( "A02", 1000, 0, 5000 );

//    Минимальный ток. канала А и Б при 20мА на входе
    Command A03 = Command( "A03", -1000, -5000, 0 );

//    Амплитуда вибрационного сигнала мА
    Command A04 = Command( "A04", 20, 0, 500 );

//    Частота вибрационного сигнала, Гц
    Command A05 = Command( "A05", 5, 1, 200 );

//    Полярность сигнала А
//    0 – биполярный
//    1 – однополярный в положительной части
//    -1 – однополярный в отрицательной части
    Command A06 = Command( "A06", 1, -1, 1 );
//    Полярность сигнала B
//    0 – биполярный
//    1 – однополярный в положительной части
//    -1 – однополярный в отрицательной части
    Command A07 = Command( "A07", 1, -1, 1 );

//    Входной ток (управление) мкА
    Command A10 = Command( "A10", 12000, 4000, 20000 );//RO

//    Значение на выходе А мА
    Command A11 = Command( "A11", 0, -5500, 5500 );
//    Значение на выходе Б мА
    Command A12 = Command( "A12", 0, -5500, 5500 );

//    Флаг перегрузки канала А
    Command A13 = Command( "A13", 0, 0, 1 );//RO
//    Флаг перегрузки канала Б
    Command A14 = Command( "A14", 0, 0, 1 );//RO

//    Мониторинг параметра В эту ячейку задается адрес интересующего параметра
    Command A20 = Command( "A20", 0, 0, 40 );//RO

//    0 – Силовой выходы на 200 мА отключены
//    1 – Силовой выход включен
    Command A21 = Command( "A21", 0, 0, 1 );
//    Максимальный ток  при 20мА на входе
    Command A22 = Command( "A22", 2000, 0, 2000 );
//    Минимальный ток при 4мА на входе
    Command A23 = Command( "A23", -2000, -2000, 0 );
//    Амплитуда вибрационного сигнала мА, Гц
    Command A24 = Command( "A24",50, 0, 200 );
//    Частота вибрационного сигнала
    Command A25 = Command( "A25", 20, 1, 200 );
//    Полярность сигнала С
//    0 – биполярный
//    1 – однополярный в положительной части
//    -1 – однополярный в отрицательной части
    Command A26 = Command( "A26", 1, -1, 1 );
//    Входной ток (управление)
    Command A30 = Command( "A30", 12000, 4000, 20000 );
//    Значение на выходе
    Command A31 = Command( "A31", 0, -2000, 2000 );

//    Перегрузка канала С
    Command A32 = Command( "A32", 0, 0, 1 );

    Command A39 = Command( "A39", 0, 0, 1 ); //Чтение параметров из памяти (нужен ручной запуск после чтения)
    Command A40 = Command( "A40", 0, 0, 1 ); //Запись параметров в память (нужен ручной запуск после записи)
};


}//namespace control_board


