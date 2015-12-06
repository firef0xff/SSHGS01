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
    virtual void UpdateData(){}
    cpu::data::DB31& mCommand;
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
    Test( QString const& name, uint8_t id );
private:
    static uint8_t mTestsCount;
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
    cpu::data::DB39& mBits;
    cpu::data::DB32& mTemperature;
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
    cpu::data::DB39& mBits;
    cpu::data::DB32& mTemperature;

private:
    static uint8_t mTestsCount;
};
}//namespace hydro_cylinder


}//namespace test
