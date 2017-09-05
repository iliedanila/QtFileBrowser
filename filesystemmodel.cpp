#include "filesystemmodel.h"

FileSystemModel::FileSystemModel(QObject *parent)
:
    QFileSystemModel(parent)
{

}

Qt::ItemFlags FileSystemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QFileSystemModel::flags(index);

    if (!index.isValid())
        return defaultFlags | Qt::ItemIsDropEnabled;

    return defaultFlags | Qt::ItemIsEditable | Qt::ItemIsDragEnabled /*| Qt::ItemIsDropEnabled*/;
}
