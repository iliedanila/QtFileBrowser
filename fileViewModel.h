#ifndef FILEVIEWMODEL_H
#define FILEVIEWMODEL_H

#include "qabstractitemmodel.h"
#include <QList>
#include <QFileInfo>

class FileViewModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit FileViewModel(QObject *parent = Q_NULLPTR);

    void addFilesInFolder(const QString& folder);
    void removeFilesInFolder(const QString& folder);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    QFileInfoList files;
};

#endif // FILEVIEWMODEL_H
