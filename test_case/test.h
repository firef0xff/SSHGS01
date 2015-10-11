#pragma once
#include <QString>
#include <QVariant>
#include <functional>
#include <stdint.h>
#include <QPainter>
#include <functional>

namespace test
{

class TestCase;
class Test
{
public:
    typedef std::function< void( QString const& ) > LogFunction;
    Test( TestCase* test_case, QString const& name, uint8_t number, uint8_t id );
    virtual ~Test();
    bool Run( LogFunction, bool& );
    virtual bool Run() = 0;

    QString const& Name() const;
    uint8_t const& Number() const;
    uint8_t const& ID() const;

    virtual QJsonObject Serialise() const = 0;
    virtual bool Deserialize( QJsonObject const& obj ) = 0;

    // true если больше нечего выводить ecли false то предоставленного куска мало
    virtual bool Draw( QPainter& painter, QRect &free_rect ) const = 0;

    virtual void ResetDrawLine();

protected:
    bool IsStopped();
    LogFunction Log;
    QString mName;

    bool AllocatePlace(QRect& place, int height, QRect& source ) const;

    typedef std::function< void ( QRect const& ) > DrawLineHandler;
    bool DrawLine ( uint32_t &num, QRect& source, QFont font, DrawLineHandler do_draw, int custom_height = 0 ) const;

private:
    friend class TestCase;
    Test( Test const& );
    void operator = ( Test const& );

    void Free();

    uint8_t mNumber;
    uint8_t mId;
    TestCase *mCase;

    bool* mStopMarker;
    mutable uint32_t mDrawLine; //отметка на которой остановилась печать
};

}//namespace test

Q_DECLARE_METATYPE( test::Test* )
