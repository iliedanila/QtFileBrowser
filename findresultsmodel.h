#ifndef FINDRESULTSMODEL_H
#define FINDRESULTSMODEL_H

#include <QAbstractListModel>
#include <QFileIconProvider>

class FindResultsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit FindResultsModel(QObject *parent = Q_NULLPTR);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QString fileName(const int row) const { return resultList.at(row); }

public slots:
    void foundMatch(QString);
    void clear();

private:
    QStringList resultList;
    QFileIconProvider iconProvider;
};

#endif // FINDRESULTSMODEL_H
