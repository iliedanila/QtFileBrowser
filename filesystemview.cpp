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
    auto selection = selectionModel()->selection();
    if (selection.count() == 0)
    {
        selectionModel()->setCurrentIndex(model()->index(0, 0), QItemSelectionModel::SelectCurrent);
        selectionModel()->select(model()->index(0, 0), QItemSelectionModel::ClearAndSelect);
    }

    QTableView::focusInEvent(event);
    emit gotFocus();
}
