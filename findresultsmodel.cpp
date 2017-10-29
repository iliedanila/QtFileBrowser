#include "findresultsmodel.h"

#include <QFileInfo>

FindResultsModel::FindResultsModel(QObject *parent)
:
    QAbstractListModel(parent)
{
}

int FindResultsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return resultList.count();
}

int FindResultsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant FindResultsModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        if (role == Qt::DisplayRole)
        {
            return resultList.at(index.row());
        }
        else if (role == Qt::DecorationRole)
        {
            QFileInfo fileInfo(resultList.at(index.row()));
            return iconProvider.icon(fileInfo);
        }
        else if (role == Qt::TextAlignmentRole)
        {
            return Qt::AlignLeft;
        }
    }
    return QVariant();
}

Qt::ItemFlags FindResultsModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);

    if (!index.isValid())
        return defaultFlags | Qt::ItemIsDropEnabled;

    return defaultFlags | Qt::ItemIsDragEnabled /*| Qt::ItemIsDropEnabled*/;
}

QVariant FindResultsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section == 0 && orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return "File Name";
    }
    return QVariant();
}

void FindResultsModel::foundMatch(QString filePath)
{
    emit layoutAboutToBeChanged();
    resultList.push_back(filePath);
    emit layoutChanged();
}
