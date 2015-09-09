#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QLOG::QLog::initLog(QLOG::QLog::File,QLOG::DebugMsg)->enbleQtMessage(true);
    QLogDebug << "this is a debug";
    QLogTest << "this is a test";
    QLogWarning << "this is a QLogWarning";
    MainWindow w;
    w.show();
    qDebug() << "this qDebug();";
    qWarning() << "this is QWarnning";
    return a.exec();
}
