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
    else
    {
        QTableView::keyPressEvent(event);
    }
}
