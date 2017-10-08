#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QFile style(":qdarkstyle/style.qss");
    if (style.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream textStream(&style);
        app.setStyleSheet(textStream.readAll());
    }

    MainWindow w;
    w.showMaximized();

    return app.exec();
}
