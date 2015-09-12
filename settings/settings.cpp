#include "settings.h"
#include <mutex>
#include <memory>
#include <QJsonObject>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QDateTime>
#include <QFile>

namespace app
{

namespace
{

std::recursive_mutex CREATE_MUTEX;

static std::unique_ptr< Settings > INSTANCE;

QtMsgType ToMsgType ( QString const& level )
{
    if ( level.compare( "Информация", Qt::CaseInsensitive ) )
        return QtInfoMsg;
    else if ( level.compare( "Предупреждения", Qt::CaseInsensitive ) )
        return QtWarningMsg;
    else if ( level.compare( "Ошибки", Qt::CaseInsensitive ) )
        return QtCriticalMsg;
    else if ( level.compare( "Все", Qt::CaseInsensitive ) )
        return QtDebugMsg;
    return static_cast< QtMsgType > (-1);
}

void myLogHandler(QtMsgType type, const QMessageLogContext &where, const QString &msg)
{
    if ( ToMsgType( Settings::Instance().LogLevel() ) < type )
        return;

    std::ofstream f( "log.txt", std::ios_base::out | std::ios_base::app );
    QByteArray localMsg = msg.toLocal8Bit();
    std::string date = QDateTime::currentDateTime().toString( Qt::RFC2822Date ).toStdString();

    switch (type)
    {
    case QtDebugMsg:
        f << date.c_str()
          << " Debug: [";
    case QtInfoMsg:
        f << date.c_str()
          << " Info: [";
        break;
    case QtWarningMsg:
        f << date.c_str()
          << " Warn: [";
        break;
    case QtCriticalMsg:
        f << date.c_str()
          << " Error: [";
        break;
    case QtFatalMsg:
        f << date.c_str()
          << " Error: [";
        break;
    }
    if (where.file)
        f<< where.file << ":" << where.line;
    if (where.function)
        f<< " - " << where.function;
    f<< "]" << localMsg.constData() << std::endl;
    f.flush();
    f.close();
}


}//namespace

Settings& Settings::Instance()
{
    if ( !INSTANCE )
    {
        std::lock_guard< std::recursive_mutex > lock( CREATE_MUTEX );
        if ( !INSTANCE )
        {
            INSTANCE.reset( new Settings() );
        }
    }

    return *INSTANCE.get();
}

QString Settings::ResultPath() const
{
    return mDocument.object().value("ResultPath").toString();
}
void Settings::ResultPath( QString const& val)
{
    auto obj = mDocument.object();
    obj.insert( "ResultPath", val );
    mDocument.setObject( obj );
}

QString Settings::TestPath() const
{
    return mDocument.object().value("TestPath").toString();
}
void Settings::TestPath( QString const& val)
{
    auto obj = mDocument.object();
    obj.insert( "TestPath", val );
    mDocument.setObject( obj );
}

QString Settings::LogLevel() const
{
    return mDocument.object().value("LogLevel").toString();
}
void Settings::LogLevel( QString const& val)
{
    auto obj = mDocument.object();
    obj.insert( "LogLevel", val );
    mDocument.setObject( obj );
}


void Settings::Save()
{

    QFile f( mFileName );
    if ( mDocument.isNull() )
    {
        QJsonObject obj;
        mDocument.setObject( obj );
        ResultPath( "results" );
        TestPath( "test_cases" );
        LogLevel( "Ошибки" );
    }
    f.open(QIODevice::WriteOnly);
    f.write( mDocument.toJson() );
    f.close();
}
void Settings::Load()
{
    QFile f( mFileName );
    if ( f.exists() )
    {        
        f.open(QIODevice::ReadOnly);
        QJsonParseError err;
        mDocument = QJsonDocument::fromJson( f.readAll() , &err );

        if ( err.error != QJsonParseError::NoError )
        {
            qWarning() << err.errorString()<<" позиция: " << QString::number(err.offset);
        }
        f.close();
    }
    else
        Save();
}

Settings::Settings():
    mFileName("settings.json")
{
    qInstallMessageHandler( myLogHandler );
    Load();
}




}//namespace app
