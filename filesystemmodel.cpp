#include "filesystemmodel.h"

#include <QMimeData>

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

    return defaultFlags /*| Qt::ItemIsEditable*/ | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

Qt::DropActions FileSystemModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList FileSystemModel::mimeTypes() const
{
    QStringList types;
    types << "application/filePaths";
    return types;
}

QMimeData *FileSystemModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes)
    {
        if (index.isValid() && index.column() == 0)
        {
            auto fileinfo = fileInfo(index);
            stream << fileinfo.absoluteFilePath();
        }
    }

    mimeData->setData("application/filePaths", encodedData);
    return mimeData;
}

bool FileSystemModel::dropMimeData(const QMimeData *data,
                                   Qt::DropAction action,
                                   int row,
                                   int column,
                                   const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;
    if (!data->hasFormat("application/filePaths"))
        return false;
    if (column > 0)
        return false;

    int beginRow;

    if (row != -1)
        beginRow = row;
    else if (parent.isValid())
        beginRow = parent.row();
    else
        beginRow = rowCount(QModelIndex());

    QFileInfo parentFile = fileInfo(parent);
    QString parentPath = parentFile.absoluteFilePath();

    QByteArray encodedData = data->data("application/filePaths");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QList<QFileInfo> newItems;
    int rows = 0;

    while (!stream.atEnd())
    {
        QString filePath;
        stream >> filePath;
        newItems << QFileInfo(filePath);
        ++rows;
    }
    return true;
}

bool FileSystemModel::insertRows(int row, int count, const QModelIndex &parent)
{
    // TODO:
}

bool FileSystemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    // TODO:
}
