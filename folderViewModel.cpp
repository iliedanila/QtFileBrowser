#include <QFileSystemModel>
#include <QDebug>

#include "folderViewModel.h"

CustomFileSystemModel::CustomFileSystemModel(QObject *parent)
:
    QFileSystemModel(parent)
{

}

void CustomFileSystemModel::enterFolder(const QModelIndex &index)
{
    QFileInfo tempFileInfo = fileInfo(index);
    QString absolutePath = tempFileInfo.absoluteFilePath();
    qDebug() << "EnterFolder called." << absolutePath << '\n';

    if (tempFileInfo.isDir())
    {
        emit folderDoubleClicked(absolutePath);
    }
}
