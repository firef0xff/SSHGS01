#ifndef CHILD_WIDGET_H
#define CHILD_WIDGET_H

#include <QWidget>

class ChildWidget : public QWidget
{
   Q_OBJECT
public:
   explicit ChildWidget(QWidget *parent = nullptr);

    void closeEvent(QCloseEvent *e);
public slots:
    void on_login();

signals:
    void login();
    void closed();

private:
    virtual void OnLogin();
};

#endif // CHILD_WIDGET_H
