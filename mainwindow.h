#pragma once
#include <QMainWindow>
#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    std::unique_ptr< QWidget > mChildWindow;
private slots:

    void on_TestCase1_clicked();
    void on_TestCase2_clicked();
    void on_ManualControl_clicked();
};
