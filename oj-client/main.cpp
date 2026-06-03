#include "src/ui/mainwindow.h"

#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFont>
#include <QTextStream>

namespace
{
void writeStartupLog(const QString &message)
{
    QDir dir = QDir::current();
    dir.mkpath("data");

    QFile file(dir.filePath("data/startup.log"));
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        return;
    }

    QTextStream stream(&file);
    stream << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz")
           << " | " << message << '\n';
}
}

int main(int argc, char *argv[])
{
    writeStartupLog("main: entering");
    QApplication a(argc, argv);
    writeStartupLog("main: QApplication created");
    a.setFont(QFont("Microsoft YaHei UI", 10));
    writeStartupLog("main: application font set");
    QApplication::setQuitOnLastWindowClosed(false);
    writeStartupLog("main: quitOnLastWindowClosed set");
    MainWindow w;
    writeStartupLog("main: MainWindow constructed");
    w.show();
    writeStartupLog("main: MainWindow show called");
    return QCoreApplication::exec();
}
