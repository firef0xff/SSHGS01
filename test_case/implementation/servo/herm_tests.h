#pragma once
#include "../test_base.h"
#include <QVector>
#include <condition_variable>
#include <memory>

namespace test
{

namespace servo
{

class OutsideHermTest :public test::servo::Test
{
public:
    OutsideHermTest();
    ~OutsideHermTest();
    bool Run();

    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );

    bool Draw(QPainter &painter, QRect &free_rect, QString  const& ) const;

    bool Success() const;

private:
    void Question();
    bool LeakFounded; //наружная течь (не)замечена -- диалоговое окно
    std::condition_variable mCondVar;
};

class InsideHermTest :public test::servo::Test
{
public:
    class GrapfData;
    struct Data
    {
        Data():
            Signal(0),
            Leak(0)
        {}

        QJsonObject Serialise() const;
        bool Deserialize( QJsonObject const& obj );
        double Signal;
        double Leak;
    };

    typedef std::vector<Data> DataSet;

    InsideHermTest();
    ~InsideHermTest();
    bool Run();
    bool Success() const;
    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );

    void ResetDrawLine();
    bool Draw(QPainter &painter, QRect &free_rect, QString  const& compare_width ) const;

protected:
    void UpdateData();
private:
    friend class GrapfData;

    // графиг опорного сигнала и расхода в канале утечки ( Т )

    DataSet GraphA;
    DataSet GraphB;
    mutable std::unique_ptr<GrapfData> mGrapfs;
};

}//namespace servo

}//namespace test
