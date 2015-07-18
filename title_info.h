#ifndef TITLE_INFO_H
#define TITLE_INFO_H

#include <QWidget>

namespace Ui {
class TitleInfo;
}

class TitleInfo : public QWidget
{
    Q_OBJECT

public:
    explicit TitleInfo(QWidget *parent = 0);
    ~TitleInfo();

private:
    Ui::TitleInfo *ui;
};

#endif // TITLE_INFO_H
