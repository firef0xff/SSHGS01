#include "settings.h"
#include <mutex>
#include <memory>
#include <QJsonObject>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QDateTime>

namespace app
{

namespace
{

std::mutex CREATE_MUTEX;

static std::unique_ptr< Settings > INSTANCE;


void myLogHandler(QtMsgType type, const QMessageLogContext &where, const QString &msg)
{
    if ( Settings::Instance().LogLevel() < type )
        return;

    std::ofstream f( "log.txt", std::ios_base::out | std::ios_base::app );
    QByteArray localMsg = msg.toLocal8Bit();
    std::string date = QDateTime::currentDateTime().toString( Qt::RFC2822Date ).toStdString();

    switch (type)
    {
    case QtDebugMsg:
        f << date.c_str()
          << " Debug: [" << where.file << ":" << where.line << " - " << where.function << "]" << localMsg.constData() << std::endl;
        break;
    case QtInfoMsg:
        f << date.c_str()
          << " Info: [" << where.file << ":" << where.line << " - " << where.function  <<  "]" << localMsg.constData() << std::endl;
        break;
    case QtWarningMsg:
        f << date.c_str()
          << " Warn: [" << where.file << ":" << where.line << " - " << where.function  <<  "]" << localMsg.constData() << std::endl;
        break;
    case QtCriticalMsg:
        f << date.c_str()
          << " Error: [" << where.file << ":" << where.line << " - " << where.function  <<  "]" << localMsg.constData() << std::endl;
        break;
    case QtFatalMsg:
        f << date.c_str()
          << " Fatal: [" << where.file << ":" << where.line << " - " << where.function  <<  "]" << localMsg.constData() << std::endl;
        abort();
    }
}


}//namespace

Settings& Settings::Instance()
{
    if ( !INSTANCE )
    {
        std::lock_guard< std::mutex > lock( CREATE_MUTEX );
        if ( !INSTANCE )
        {
            INSTANCE.reset( new Settings() );
        }
    }

    return *INSTANCE.get();
}

QString Settings::ResultPath() const
{
    QJsonObject const& obj = mDocument.object();
    QJsonValue const& val = obj.value("ResultPath");
    return val.toString();
}
QString Settings::TestPath() const
{
    QJsonObject const& obj = mDocument.object();
    QJsonValue const& val = obj.value("TestPath");
    return val.toString();
}
QtMsgType Settings::LogLevel() const
{
    QJsonObject const& obj = mDocument.object();
    QJsonValue const& val = obj.value("LogLevel");

    if ( val.toString().compare( "Info", Qt::CaseInsensitive ) )
        return QtInfoMsg;
    else if ( val.toString().compare( "Warning", Qt::CaseInsensitive ) )
        return QtWarningMsg;
    else if ( val.toString().compare( "Error", Qt::CaseInsensitive ) )
        return QtCriticalMsg;
    else if ( val.toString().compare( "Fatal", Qt::CaseInsensitive ) )
        return QtFatalMsg;
    else if ( val.toString().compare( "Debug", Qt::CaseInsensitive ) )
        return QtDebugMsg;
    return QtWarningMsg;
}
void Settings::Save()
{

    std::ofstream f( mFileName.c_str(), std::ios_base::binary| std::ios_base::out );

    if ( mDocument.isNull() )
    {
        QJsonObject obj;
        obj.insert("ResultPath", "results");
        obj.insert("TestPath", "test_cases");
        obj.insert("LogLevel", "Error");
        mDocument.setObject( obj );
    }

    auto data = mDocument.toJson();
    f.write( data.constData(), data.size() );

    f.close();
}
void Settings::Load()
{
    std::ifstream f( mFileName.c_str(), std::ios_base::binary| std::ios_base::in );
    if ( f )
    {
        f.seekg( 0, f.end );
        auto lenght = f.tellg();
        f.seekg( 0, f.beg );

        char* buf = new char[lenght]();

        f.read ( buf, lenght );

        QByteArray json( buf, lenght );
        QJsonParseError err;
        mDocument = QJsonDocument::fromJson( json , &err );

        if ( err.error != QJsonParseError::NoError )
        {
            qWarning() << err.errorString()<<" позиция: " << QString::number(err.offset);
        }
        delete[] buf;
        f.close();
    }
    else
        Save();
}

Settings::Settings():
    mFileName("settings.json")
{
    qInstallMessageHandler( myLogHandler );
}




}//namespace app
