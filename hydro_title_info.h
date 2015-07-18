#ifndef TITLE_INFO_H
#define TITLE_INFO_H

#include <QWidget>

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

private:
    Ui::HydroTitleInfo *ui;
    void closeEvent(QCloseEvent *);

signals:
    void closed();
};

#endif // TITLE_INFO_H
