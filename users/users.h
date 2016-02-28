#pragma once

#include <QWidget>
#include "../settings/settings.h"

namespace Ui {
class Users;
}
class QTableWidget;
class QTableWidgetItem;

class Users : public QWidget
{
    Q_OBJECT

public:
    explicit Users(QWidget *parent = 0);
    ~Users();
private:
    void closeEvent(QCloseEvent *e);
    void AutoWidth(QTableWidget *sg );
    void ShowData();

    Ui::Users *ui;
    bool mUpdateGreed;
    app::UserData mUsers;
    QTableWidgetItem* mCurrentItem = nullptr;
signals:
    void closed();
private slots:
    void on_UsersGreed_itemSelectionChanged();
    void on_Add_clicked();
    void on_Remove_clicked();
    void on_Edit_clicked();
};

Q_DECLARE_METATYPE( app::UserInfo* )
