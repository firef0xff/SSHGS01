#pragma once
#include "child_widget.h"

namespace Ui {
class settings_wnd;
}

class settings_wnd : public ChildWidget
{
    Q_OBJECT

public:
    explicit settings_wnd(QWidget *parent = 0);
    ~settings_wnd();

private:
    QString ChooseDir( QString const& place = QString() );

    Ui::settings_wnd *ui;

private slots:
    void on_TestParamsButton_clicked();
    void on_toolButton_2_clicked();
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
    void on_toolButton_3_clicked();
    void on_Motoclock_clicked();
};

