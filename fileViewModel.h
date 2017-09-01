#ifndef FILEVIEWMODEL_H
#define FILEVIEWMODEL_H

#include "qabstractitemmodel.h"
#include <QList>
#include <QFileInfo>
#include <QFileIconProvider>

class FileViewModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit FileViewModel(QObject *parent = Q_NULLPTR);

    void addFilesInFolder(const QString& folder);
    void removeFilesInFolder(const QString& folder);

    virtual QModelIndex index(int row, int column, const QModelIndex &) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual Qt::ItemFlags flags(const QModelIndex & index) const override;

public slots:
    void changeLayout();

private:
    QFileInfoList files;
    QFileIconProvider fileIconProvider;
    qint32 columnsCount;
};

#endif // FILEVIEWMODEL_H
