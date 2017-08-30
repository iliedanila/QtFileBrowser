#include "fileViewModel.h"
#include <QDebug>
#include <QDirIterator>
#include <QFileIconProvider>

FileViewModel::FileViewModel(QObject* parent)
:
       QAbstractTableModel(parent)
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

QModelIndex FileViewModel::index(int row, int column, const QModelIndex &/*parent*/) const
{
    return createIndex(row, column);
}

QModelIndex FileViewModel::parent(const QModelIndex &/*child*/) const
{
    return QModelIndex();
}

int FileViewModel::rowCount(const QModelIndex &/*parent*/) const
{
    return files.count();
}

int FileViewModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

QVariant FileViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole || role == Qt::DecorationRole)
    {
        switch (index.column())
        {
        case 0:
            {
                return fileIconProvider.icon(files.at(index.row()));
                break;
            }
        case 1:
            {
                return files.at(index.row()).fileName();
                break;
            }
        }
    }
    else if (role == Qt::TextAlignmentRole)
    {
        switch (index.column())
        {
        case 0:
            {
                return Qt::AlignCenter;
            }
        case 1:
            {
                return Qt::AlignLeft;
            }
        }
    }
    return QVariant();
}

QVariant FileViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::TextAlignmentRole)
        {
            return Qt::AlignHCenter;
        }
        else if (role == Qt::DisplayRole)
        {
            if (section == 0)
            {
                return "Icon";
            }
            else if (section == 1)
            {
                return "File Name";
            }
        }
    }
    return QVariant();
}

Qt::ItemFlags FileViewModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return QAbstractTableModel::flags(index) | Qt::ItemIsDropEnabled;

    return  QAbstractTableModel::flags(index) | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

}
