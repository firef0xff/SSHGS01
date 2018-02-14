#include "child_widget.h"

ChildWidget::ChildWidget(QWidget *parent) : QWidget(parent)
{

}

void ChildWidget::on_login()
{
    OnLogin();
    emit login();
}

void ChildWidget::OnLogin()
{}

void ChildWidget::closeEvent(QCloseEvent *e)
{
   QWidget::closeEvent( e );
   emit closed();
}

