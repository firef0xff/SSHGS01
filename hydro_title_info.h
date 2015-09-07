#ifndef TITLE_INFO_H
#define TITLE_INFO_H

#include <QWidget>
#include <memory>

namespace Ui {
class HydroTitleInfo;
}

class HydroTitleInfo : public QWidget
{
    Q_OBJECT

public:
    explicit HydroTitleInfo(QWidget *parent = 0);
    ~HydroTitleInfo();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_ControlType_activated(int index);

    void on_GsType_activated(int index);

    void on_ReelCount_valueChanged(int arg1);

private:
    Ui::HydroTitleInfo *ui;
    void closeEvent(QCloseEvent *);
    std::unique_ptr< QWidget > mChildWindow;

    bool SaveInputParams();

signals:
    void closed();
};

#endif // TITLE_INFO_H
