#include "functions.h"
#include <QValidator>

bool ParamChecker ( QWidget* control, bool r )
{
    QPalette palette = control->palette();

    if ( !r )
    {
        palette.setColor( control->backgroundRole(), Qt::red );
        control->setAutoFillBackground( true );
    }
    else
    {
        control->setAutoFillBackground( false );
    }
    control->setPalette( palette );
    return r;
}
bool ValidateRange ( QLineEdit* control, bool r )
{
    QString text = control->text();
    int pos = control->cursorPosition();
    return std::min( control->validator()->validate( text, pos ) == QValidator::Acceptable , r );
}
