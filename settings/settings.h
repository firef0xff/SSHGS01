#pragma once
#include <QString>
#include <QJsonDocument>


namespace app
{

enum UserLevel
{
    Uncknown = -1,
    User = 0,
    Master = 1,
    Admin = 2
};

class UserInfo
{
public:
    QString Login;
    QString pass_hash;
    QString card_id;
    UserLevel level;
    void SetCardId( QString p );
    void SetPass( QString p );
};
typedef QVector< UserInfo > UserData;


class Settings
{
public:
    static Settings& Instance();

    QString ResultPath() const;   //куда складывать результаты тестов
    void ResultPath( QString const& val);

    QString TestPath() const;     //куда складывать параметры тестов
    void TestPath(QString const& val);

    QString LogLevel() const;     //текущий уровень логирования
    void LogLevel( QString const& val);

    void MainPupm( QString const& val);
    QString MainPupm() const;     //используемый основной насос

    void Save();
    void Load();

    void User( QString u );
    QString User() const;

    void Users( UserData const& u );
    UserData Users() const;

    bool CheckUser( QString const& user, QString const& pass );
    bool CheckUser( QString const& card_id );

    UserLevel UserAccess() const;

    void ComAddr( QString const& v);
    QString ComAddr() const;

    void LockChecking( bool v );
private:
    Settings();
    Settings( Settings const& ) = delete;
    void operator = ( Settings const&) = delete;

    QJsonDocument mDocument;
    QString mFileName;
    UserLevel mCurrentLevel;

    bool mLockUserChecking = false;
};


}//namespace app

