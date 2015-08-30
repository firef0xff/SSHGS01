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
    explicit ServoTitleInfo(QWidget *parent = 0);
    ~ServoTitleInfo();

private:
    Ui::ServoTitleInfo *ui;
    std::unique_ptr< QWidget > mChildWindow;

    void closeEvent(QCloseEvent *);
    bool SaveInputParams();

signals:
    void closed();
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_RaspredControl_activated(int index);
};

#endif // SERVO_TITLE_INFO_H
