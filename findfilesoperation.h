#ifndef FINDFILESOPERATION_H
#define FINDFILESOPERATION_H

#include <QThread>
#include <QMutex>

class FindFilesOperation : public QThread
{
    Q_OBJECT

public:
    explicit FindFilesOperation(QObject *parent = Q_NULLPTR);

signals:
    void processedEntriesCount(int);
    void entriesCountChanged(int);
    void foundMatch(QString);

public slots:
    void cancel();
    void setSearchFolder(const QString aFolder);
    void setFileName(const QString aFileName);
    void setTextToFind(const QString aTextToFind);

protected:
    void run() override;

private:
    void executeSearch();

    QAtomicInt canceled;

    QString searchFolder;
    QMutex searchFolderMutex;

    QString textToFind;
    QMutex textToFindMutex;

    QString fileName;
    QMutex fileNameMutex;
};

#endif // FINDFILESOPERATION_H
