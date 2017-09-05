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
};

#endif // FILESYSTEMMODEL_H
