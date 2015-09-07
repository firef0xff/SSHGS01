#pragma once
#include <QString>
#include <QJsonDocument>


namespace app
{

class Settings
{
public:
    static Settings& Instance();

    QString ResultPath() const;   //куда складывать результаты тестов
    QString TestPath() const;     //куда складывать параметры тестов

    void Save();
    void Load();

    QtMsgType LogLevel() const;
private:
    Settings();
    Settings( Settings const& ) = delete;
    void operator = ( Settings const&) = delete;

    QJsonDocument mDocument;
    std::string mFileName;
};


}//namespace app

