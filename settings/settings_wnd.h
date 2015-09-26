#pragma once
#include <QWidget>

namespace Ui {
class settings_wnd;
}

class settings_wnd : public QWidget
{
    Q_OBJECT

public:
    explicit settings_wnd(QWidget *parent = 0);
    ~settings_wnd();

private:
    void closeEvent(QCloseEvent *e);

    QString ChooseDir( QString const& place = QString() );

    Ui::settings_wnd *ui;

signals:
    void closed();
private slots:
    void on_TestParamsButton_clicked();
    void on_toolButton_2_clicked();
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
};

