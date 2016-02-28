#ifndef SERVO_TITLE_INFO_H
#define SERVO_TITLE_INFO_H

#include <QWidget>
#include <memory>

namespace Ui {
class ServoTitleInfo;
}

class ServoTitleInfo : public QWidget
{
    Q_OBJECT

public:
    explicit ServoTitleInfo(bool new_mode, QWidget *parent = 0);
    ~ServoTitleInfo();

private:
    Ui::ServoTitleInfo *ui;
    std::unique_ptr< QWidget > mChildWindow;
    bool mNewMode;

    void closeEvent(QCloseEvent *);
    bool SaveInputParams();
    void FromParams();
    void CheckRights();

signals:
    void closed();
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_RaspredControl_activated(int index);
    void on_ControlSignal_activated(int index);
    void on_ControlType_activated(int index);
    void on_TestChA_clicked();
    void on_TestChB_clicked();
    void on_ControlReelChA_activated(const QString &arg1);
    void on_ControlReelChB_activated(const QString &arg1);
    void on_PosCount_valueChanged(int arg1);
};

#endif // SERVO_TITLE_INFO_H
