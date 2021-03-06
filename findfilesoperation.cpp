#include "findfilesoperation.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QRegExp>

FindFilesOperation::FindFilesOperation(QObject *parent)
:
    QThread(parent),
    canceled(0)
{
}

void FindFilesOperation::setSearchFolder(const QString aFolder)
{
    QMutexLocker lock(&searchFolderMutex);
    searchFolder = aFolder;
}

void FindFilesOperation::setFileName(const QString aFileName)
{
    QMutexLocker lock(&fileNameMutex);
    fileName = aFileName;
}

void FindFilesOperation::setTextToFind(const QString aTextToFind)
{
    QMutexLocker lock(&textToFindMutex);
    textToFind = aTextToFind;
}

void FindFilesOperation::cancel()
{
    canceled = 1;
}

void FindFilesOperation::run()
{
    canceled = 0;

    QMutexLocker lockFolder(&searchFolderMutex);
    QMutexLocker lockText(&textToFindMutex);
    QMutexLocker lockFileName(&fileNameMutex);

    executeSearch();
}

void FindFilesOperation::executeSearch()
{
    int processedEntries = 0;
    QDir dir(searchFolder);
    QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files, QDir::DirsLast);
    int totalEntries = entries.count();
    emit entriesCountChanged(totalEntries);

    while (!entries.empty() && !canceled)
    {
        auto fileInfo = entries.front();
        entries.pop_front();
        processedEntries++;

        QRegExp regExp(fileName);
        regExp.setPatternSyntax(QRegExp::Wildcard);
        auto currentFileName = fileInfo.fileName();
        auto matchReg = regExp.exactMatch(currentFileName);

        if (matchReg || currentFileName.startsWith(fileName))
        {
            emit foundMatch(fileInfo.absoluteFilePath());
        }

        emit processedEntriesCount(processedEntries);

        if (fileInfo.isDir())
        {
            QDir folder(fileInfo.absoluteFilePath());
            auto newItems = folder.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files, QDir::DirsLast);
            entries.append(newItems);
            totalEntries += newItems.count();
            emit entriesCountChanged(totalEntries);
        }
    }
}
