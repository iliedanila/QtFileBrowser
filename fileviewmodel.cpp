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
    emit layoutAboutToBeChanged();
    QDir dir(folder);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    auto fileInfoList = dir.entryInfoList(QStringList());
    files.append(fileInfoList);
    emit layoutChanged();
}

void FileViewModel::removeFilesInFolder(const QString &folder)
{
    qDebug() << "removeFilesInFolder " << folder << '\n';
    emit layoutAboutToBeChanged();
    QDir dir(folder);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    auto fileInfoList = dir.entryInfoList(QStringList());

    for(auto& fileInfo : fileInfoList)
    {
        files.removeAt(files.indexOf(fileInfo));
    }
    emit layoutChanged();
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
    return files.at(index.row()).fileName();
}
