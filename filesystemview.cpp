#include "filesystemview.h"

#include <QKeyEvent>

FileSystemView::FileSystemView(QWidget *parent)
:
    QTableView(parent)
{

}

void FileSystemView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Tab)
    {
        emit switchMe();
    }
    else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        auto index = selectionModel()->currentIndex();
        emit doubleClicked(index);
    }
    else if (event->key() == Qt::Key_Backspace)
    {
        emit goToParent();
    }
    else
    {
        QTableView::keyPressEvent(event);
    }
}

void FileSystemView::focusInEvent(QFocusEvent *event)
{
    QTableView::focusInEvent(event);
    emit gotFocus();
}