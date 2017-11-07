#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>

class LineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit LineEdit(QWidget *parent = Q_NULLPTR);

protected:
    void focusInEvent(QFocusEvent *) override;
};

#endif // LINEEDIT_H
