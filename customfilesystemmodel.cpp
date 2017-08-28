#include <QFileSystemModel>
#include <QDebug>

#include "customfilesystemmodel.h"

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
