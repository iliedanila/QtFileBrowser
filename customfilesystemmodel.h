#ifndef CUSTOMFILESYSTEMMODEL_H
#define CUSTOMFILESYSTEMMODEL_H

#include <QFileSystemModel>

class CustomFileSystemModel : public QFileSystemModel
{
    Q_OBJECT

public:
    explicit CustomFileSystemModel(QObject *parent = Q_NULLPTR);

signals:
    void folderChanged(const QString& newFolder);

public slots:
    void enterFolder(const QModelIndex& index);
};

#endif // CUSTOMFILESYSTEMMODEL_H
