#include "settings.h"
#include <mutex>
#include <memory>
#include <QJsonObject>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QCryptographicHash>
#include <QJsonArray>

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
    (void)type; (void)where; (void)msg;
#ifdef DEBUG
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
#endif
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

void Settings::MainPupm( QString const& val)
{
    auto obj = mDocument.object();
    obj.insert( "MainPupm", val );
    mDocument.setObject( obj );
}
QString Settings::MainPupm() const
{
    return mDocument.object().value("MainPupm").toString();
}

void Settings::User( QString u )
{
    auto obj = mDocument.object();
    obj.insert( "User", u );
    mDocument.setObject( obj );
}
QString Settings::User() const
{
    return mDocument.object().value("User").toString();
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
        MainPupm( "M1" );
        ComAddr( "COM1" );
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

void Settings::Users( UserData const& u )
{
    auto obj = mDocument.object();
    QJsonArray users;

    foreach ( UserInfo const& val, u)
    {
        QJsonObject usr;
        usr.insert( "login", val.Login );
        usr.insert( "pass", val.pass_hash );
        usr.insert( "level", val.level );
        users.push_back( usr );
    }
    obj.insert( "Users", users );
    mDocument.setObject( obj );
}
UserData Settings::Users() const
{
    UserData res;
    QJsonArray users = mDocument.object().value("Users").toArray();
    foreach ( QJsonValue const& val, users)
    {
        QJsonObject usr = val.toObject();
        UserInfo user;
        user.Login = usr.value("login").toString();
        user.pass_hash = usr.value("pass").toString();
        user.level = static_cast< UserLevel >( usr.value("level").toInt() );
        res.push_back( user );
    }
    return std::move( res );
}

void Settings::ComAddr( QString const& v)
{
    auto obj = mDocument.object();
    obj.insert( "ComAddr", v );
    mDocument.setObject( obj );
}
QString Settings::ComAddr() const
{
    return mDocument.object().value("ComAddr").toString();
}

bool Settings::CheckUser( QString const& user, QString const& pass )
{
    if ( user.isEmpty() || pass.isEmpty() )
        return false;
    auto users = Users();
    if ( pass == "admin"
#ifndef DEMO
         && users.empty()
#endif
         )
    {
        mCurrentLevel = Admin;
        return true;
    }

    QString hex ( QCryptographicHash::hash( QByteArray( pass.toStdString().c_str() ), QCryptographicHash::Algorithm::Md5 ) );

    foreach ( UserInfo const& val, users)
    {
        if ( val.Login == user && val.pass_hash == hex)
        {
            mCurrentLevel = val.level;
            return true;
        }
    }
    return false;
}

UserLevel Settings::UserAccess() const
{
    return mCurrentLevel;
}

Settings::Settings():
    mFileName("settings.json")
{
    qInstallMessageHandler( myLogHandler );
    Load();
}

void UserInfo::SetPass( QString p )
{
    pass_hash = QString( QCryptographicHash::hash( QByteArray( p.toStdString().c_str() ), QCryptographicHash::Algorithm::Md5 ) );
}



}//namespace app
