#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QFileSystemModel>

class FileSystemModel : public QFileSystemModel
{
    Q_OBJECT

public:
    enum Columns
    {
        eIcon = 0,
        eName,
        eSize,
        eType,
        eDate,
        eColumnCount
    };

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
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

signals:
    void dropFiles(QStringList, QString);
};

#endif // FILESYSTEMMODEL_H
