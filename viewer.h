#pragma once
#include <QWidget>
#include <QPixmap>
#include <QVector>

namespace Ui {
class Viewer;
}

class Viewer : public QWidget
{
    Q_OBJECT

public:
    explicit Viewer(QWidget *parent = 0);
    ~Viewer();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_Save_clicked();

    void on_SavePDF_clicked();

signals:
    void closed();

private:
    void closeEvent(QCloseEvent *e);

    Ui::Viewer *ui;
    int mPageNo;

    QVector< QPixmap > mPages;

    void DrawPage();
};
