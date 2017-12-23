#ifndef TEST_FORM_H
#define TEST_FORM_H

#include "child_widget.h"
#include <QList>
#include <memory>

namespace Ui {
class TestForm;
}

namespace test
{
class TestCase;
}

class QLayoutItem;
class QCheckBox;

class TestForm : public ChildWidget
{
    Q_OBJECT

public:
    explicit TestForm( bool new_mode,  QWidget *parent = 0);
    ~TestForm();

private:
    typedef std::shared_ptr< QCheckBox > ControlPtr;
    typedef QLayoutItem* SpaserPtr;
    Ui::TestForm *ui;
    QList< ControlPtr > mControls;
    SpaserPtr mHorisontal;
    SpaserPtr mVertical;
    std::unique_ptr< QWidget > mChildWindow;

    void CheckRights();


private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
};

#endif // TEST_FORM_H
