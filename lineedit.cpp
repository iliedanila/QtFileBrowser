#include "lineedit.h"

LineEdit::LineEdit(QWidget *parent)
:
    QLineEdit(parent)
{
}

void LineEdit::focusInEvent(QFocusEvent* event)
{
    QLineEdit::focusInEvent(event);
    setCursorPosition(text().size());
}
