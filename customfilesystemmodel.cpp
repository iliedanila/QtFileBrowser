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
    qDebug() << "EnterFolder called.\n";
    QFileInfo tempFileInfo = fileInfo(index);
    QString absolutePath = tempFileInfo.absoluteFilePath();

    if (tempFileInfo.isDir())
    {
        emit folderChanged(absolutePath);
    }
}
