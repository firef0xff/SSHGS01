#pragma once
#include "../test_base.h"
#include <QVector>
#include <map>

namespace test
{

namespace servo
{
class FrequencyCharacteristics :public test::servo::Test
{
public:
    struct ArrData
    {
        double position = 0;
        double signal = 0;
    };
    typedef std::vector< ArrData > DataSet;
    typedef std::pair< const double , DataSet > SourceItem;
    typedef std::map<const double , DataSet> Source;

    FrequencyCharacteristics();
    bool Run();

    bool Success() const;
    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );

    bool Draw(QPainter &painter, QRect &free_rect, const QString &compare_width ) const;

protected:
    void UpdateData();
private:

    Source mSource1;
    Source mSource2;
    Source mSource3;
};

}//namespace servo

}//namespace test
