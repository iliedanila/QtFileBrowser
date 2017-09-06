#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QFileSystemModel>

class FileSystemModel : public QFileSystemModel
{
    Q_OBJECT

public:
    FileSystemModel(QObject *parent = Q_NULLPTR);

    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
    virtual Qt::DropActions supportedDropActions() const override;
    virtual QStringList mimeTypes() const override;
    virtual QMimeData* mimeData(const QModelIndexList &indexes) const override;
    virtual bool dropMimeData(const QMimeData *data,
                              Qt::DropAction action,
                              int row,
                              int column,
                              const QModelIndex &parent) override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
};

#endif // FILESYSTEMMODEL_H
