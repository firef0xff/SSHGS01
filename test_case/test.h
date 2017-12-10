#pragma once
#include <QString>
#include <QVariant>
#include <functional>
#include <stdint.h>
#include <QPainter>
#include <functional>

namespace test
{

bool AllocatePlace(QRect& place, int height, QRect& source );

class TestCase;
class Test
{
public:
    typedef std::function< void( QString const& ) > LogFunction;
    typedef std::function< void() > Handler;
    typedef std::function< void( Handler ) > LaunchFunction;
    Test( TestCase* test_case, QString const& name, uint8_t number, uint8_t id );
    virtual ~Test();
    bool Run( LaunchFunction, LogFunction, bool& );
    virtual bool Run() = 0;

    QString const& Name() const;
    uint8_t const& Number() const;
    virtual uint8_t const& ID() const;

    virtual QJsonObject Serialise() const = 0;
    virtual bool Deserialize( QJsonObject const& obj ) = 0;

    // true если больше нечего выводить ecли false то предоставленного куска мало
    virtual bool Draw( QPainter& painter, QRect &free_rect, QString  const& compare_width ) const = 0;

    virtual void ResetDrawLine();  

    virtual bool Success() const;

    virtual QString RepName();
    virtual QString RepRes();

    virtual bool RepSkeep();

    bool Disabled() const;
    void Disabled( bool );

    void CompareWidth( QString const& f_name );

protected:
    bool IsStopped();
    LogFunction Log;
    LaunchFunction Launcher;
    QString mName;    

    typedef std::function< void ( QRect const& ) > DrawLineHandler;
    bool DrawLine ( uint32_t &num, QRect& source, QFont font, DrawLineHandler do_draw, double space = 1, int custom_height = 0 ) const;

protected:
    uint8_t mId;
    bool* mStopMarker;
private:
    friend class TestCase;
    Test( Test const& );
    void operator = ( Test const& );

    void Free();

    uint8_t mNumber;
    TestCase *mCase;

    mutable uint32_t mDrawLine; //отметка на которой остановилась печать
    bool mDisabled;
};

class DrawHelper
{
public:
    DrawHelper( QPainter& , QRect& );
    void DrawRowCenter( QRect const& place, QFont const& font, QColor const& color, QString const& text );

    void DrawRowLeft( QRect const& place,
                      QFont const& font,
                      QColor const& color1,
                      QString const& label);

    void DrawRowLeft( QRect const& place,
                      QFont const& font,
                      QColor const& color1,
                      QString const& label,
                      QColor const& color2,
                      QString const& value);

    void DrawRowLeft( QRect const& place,
                      QFont const& font,
                      QColor const& color1,
                      QString const& label,
                      QColor const& color2,
                      QString const& value,
                      QColor const& color3,
                      QString const& value3);
private:
    QPainter& painter;
    QRect &free_rect;
};

}//namespace test

Q_DECLARE_METATYPE( test::Test* )
