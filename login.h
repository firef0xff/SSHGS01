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

signals:
    void on_login();

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

    void on_card_in( QString const& str );
    void on_card_out();
private:
    void closeEvent(QCloseEvent *e);

    bool accept = false;
    int repats = 3;
    Ui::Login *ui;
    QMainWindow *mMainWindow;

};
