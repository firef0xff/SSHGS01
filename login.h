#pragma once
#include <QDialog>
#include <QMainWindow>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QMainWindow *parent = 0);
    ~Login();

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    void closeEvent(QCloseEvent *e);

    QMainWindow *mMainWindow;

    Ui::Login *ui;
};
