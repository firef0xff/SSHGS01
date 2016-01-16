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

    void on_Compare_clicked();

signals:
    void closed();

private:
    void Init();
    void closeEvent(QCloseEvent *e);

    Ui::Viewer *ui;
    int mPageNo;

    QVector< QPixmap > mPages;
    QPixmap header_logo;
    QPixmap footer_logo;

    QString CompareWidth;

    void DrawPage();

    QRect PreparePage( QPainter& painter, QRect const& page_rect );
};
