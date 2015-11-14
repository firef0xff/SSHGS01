#pragma once
#include "../test.h"
#include <QTime>

#include "../../cpu/db31.h"
#include "../../cpu/db32.h"

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
};

namespace hydro
{

class Test :public test::TestCommonData
{
public:
    Test( QString const& name, uint8_t id );
protected:
    void Start();
    void Wait( bool& work, bool& done);
    cpu::data::DB31 mCommand;
    cpu::data::DB32 mResults;

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
private:
    static uint8_t mTestsCount;
};
}//namespace hydro_cylinder


}//namespace test
