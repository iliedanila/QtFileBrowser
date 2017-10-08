#include "filesystemmodel.h"

#include <QMimeData>
#include <QVariant>

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
    Q_UNUSED(row);
    Q_UNUSED(parent);

    if (action == Qt::IgnoreAction)
        return true;
    if (!data->hasFormat("application/filePaths"))
        return false;
    if (column > 0)
        return false;

    QByteArray encodedData = data->data("application/filePaths");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QStringList filePaths;
    while (!stream.atEnd())
    {
        QString filePath;
        stream >> filePath;
        filePaths << filePath;
    }

    QString destinationPath;
    if (parent.row() == -1)
    {
        destinationPath = rootPath();
    }
    else
    {
        auto destination = fileInfo(parent);
        destinationPath = destination.isDir() ?
            destination.absoluteFilePath() :
            destination.dir().absolutePath();
    }

    emit dropFiles(filePaths, destinationPath);

    return true;
}

QVariant FileSystemModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::TextAlignmentRole)
    {
        int flags;
        if (index.column() == 0)
        {
            flags = Qt::AlignLeft | Qt::AlignVCenter;
        }
        else
        {
            flags = Qt::AlignCenter;
        }
        return flags;
    }
    else if (role == Qt::ToolTipRole && index.column() == 0)
    {
        return fileInfo(index).absoluteFilePath();
    }
    else
    {
        return QFileSystemModel::data(index, role);
    }
}

