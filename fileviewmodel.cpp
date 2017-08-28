#include "fileviewmodel.h"
#include <QDebug>
#include <QDirIterator>

FileViewModel::FileViewModel(QObject* parent)
:
       QAbstractItemModel(parent)
{

}

void FileViewModel::addFilesInFolder(const QString &folder)
{
    qDebug() << "addFilesInFolder " << folder << '\n';
    QDirIterator dirIterator(folder, QDir::Files, QDirIterator::Subdirectories);

    while (dirIterator.hasNext())
    {
        QFileInfo fileInfo(dirIterator.next());
        files.push_back(fileInfo.fileName());
    }
}

void FileViewModel::removeFilesInFolder(const QString &folder)
{
    qDebug() << "removeFilesInFolder " << folder << '\n';
    QDirIterator dirIterator(folder, QDir::Files, QDirIterator::Subdirectories);

    while (dirIterator.hasNext())
    {
        QFileInfo fileInfo(dirIterator.next());
        files.removeAt(files.indexOf(fileInfo.fileName()));
    }
}

QModelIndex FileViewModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

QModelIndex FileViewModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int FileViewModel::rowCount(const QModelIndex &parent) const
{
    return files.count();
}

int FileViewModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant FileViewModel::data(const QModelIndex &index, int role) const
{
    return files.at(index.row());
}
