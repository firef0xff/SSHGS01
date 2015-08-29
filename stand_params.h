#pragma once
#include <QWidget>
#include <memory>

namespace Ui {
class StandParams;
}

class StandParams : public QWidget
{
    Q_OBJECT

public:
    explicit StandParams(QWidget *parent = 0);
    ~StandParams();

private:
    Ui::StandParams *ui;
    std::unique_ptr< QWidget > mChildWindow;

    void closeEvent(QCloseEvent *);
    bool SaveInputParams();

signals:
    void closed();
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};
