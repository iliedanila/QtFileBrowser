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
    else if (event->key() == Qt::Key_Return)
    {
        auto index = selectionModel()->currentIndex();
        emit doubleClicked(index);
    }
    else if (event->key() == Qt::Key_Backspace)
    {
        emit goToParent();
    }
    else if (event->key() == Qt::Key_F5)
    {
        emit copy();
    }
    else if (event->key() == Qt::Key_F6)
    {
        emit move();
    }
    else if (event->key() == Qt::Key_F8 || event->key() == Qt::Key_Delete)
    {
        emit del();
    }
    else if (event->key() == Qt::Key_F7)
    {
        emit newFolder();
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
