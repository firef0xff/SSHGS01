#pragma once
#include "../test.h"
#include <QTime>
#include "cpu/cpu_memory.h"


namespace test
{

class TestCommonData : public test::Test
{
public:
    TestCommonData( TestCase* test_case, QString const& name, uint8_t number, uint8_t id );

protected:
    double OilTemp;
    int TestingTime;
    QTime StartTime;

    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );

    void Start();
    void Wait( bool& work, bool& done);
    virtual uint8_t CommandID();
    virtual void UpdateData(){}
    cpu::data::DB31& mCommand;
//    cpu::data::DB40& Exceptions;
};

namespace hydro
{

class Test :public test::TestCommonData
{
public:
    Test( QString const& name, uint8_t id );
protected:

    void UpdateData();
    cpu::data::DB32& mResults;

private:
    static uint8_t mTestsCount;
};

}//namespace hydro

namespace servo
{

class Test :public test::TestCommonData
{
public:
    Test( QString const& name, uint8_t id_board, uint8_t id_reel );
protected:
    bool ReelControl() const;
    cpu::data::DB32& mTemperature;
    cpu::data::DB34& mControlBoardBits;    //биты завершения испытаний серво оборудования с блоком управления
    cpu::data::DB36& mControlReelBits;    //биты завершения испытаний серво оборудования с катушками управления

    cpu::data::DB2&  m11Results;
    cpu::data::DB12& m21Results;

    cpu::data::DB3&  m12Results;
    cpu::data::DB13& m22Results;

    cpu::data::DB9&  m13Results;
    cpu::data::DB19& m23Results;


    cpu::data::DB10&  m14Result1;   //результаты пр14.1
    cpu::data::DB23&  m14Result2;   //результаты пр14.2
    cpu::data::DB20&  m24Result1;   //результаты пр24.1
    cpu::data::DB24&  m24Result2;   //результаты пр24.2

    cpu::data::DB11&  m15Result;
    cpu::data::DB21&  m25Result;
    cpu::data::DB60&  m1525Counts;
protected:
    void UpdateData();
private:
    static uint8_t mTestsCount;
    uint8_t CommandID();    
    uint8_t mIdReel;
};

}//namespace servo

namespace control_board
{
class Test :public test::TestCommonData
{
public:
    Test( QString const& name, uint8_t id );
protected:
    void UpdateData();
#warning partial implemented
    cpu::data::DB39& mBits;
    cpu::data::DB32& mTemperature;
    cpu::data::DB22& m31Results;
private:
    static uint8_t mTestsCount;
};
}//namespace control_board

namespace hydro_cylinder
{
class Test :public test::TestCommonData
{
public:
    Test( QString const& name, uint8_t id );
protected:
    void UpdateData();
#warning partial implemented
    cpu::data::DB39& mBits;
    cpu::data::DB32& mTemperature;

private:
    static uint8_t mTestsCount;
};
}//namespace hydro_cylinder


}//namespace test
