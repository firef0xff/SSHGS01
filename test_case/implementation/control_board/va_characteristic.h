#pragma once
#include "../test_base.h"
#include <QVector>
#include <memory>

namespace test
{

namespace control_board
{
class VACharacteristic :public test::control_board::Test
{
public:
    class GrapfData;
    struct Data
    {
        Data():
            signal(0),
            current(0)
        {}

        QJsonObject Serialise() const;
        bool Deserialize( QJsonObject const& obj );
        double signal;
        double current;
    };
    typedef QVector<Data> DataSet;
    VACharacteristic();
    ~VACharacteristic();
    bool Run();
    bool Success() const;
    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );

    bool Draw(QPainter &painter, QRect &free_rect, const QString &compare_width ) const;
    void ResetDrawLine();
protected:
    void UpdateData();
private:
    friend class GrapfData;
    DataSet Graph;
    mutable std::unique_ptr<GrapfData> mGrapfs;
    /// вероятно тоже график
    /// (хочу просто массива с контроллера для рисования )
};

}//namespace servo

}//namespace test
