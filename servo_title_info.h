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

signals:
    void closed();
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_RaspredControl_activated(int index);
};

#endif // SERVO_TITLE_INFO_H
