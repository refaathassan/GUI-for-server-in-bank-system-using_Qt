#include <QCoreApplication>
#include "server.h"
#include <QTextStream>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QDateTime>
void customMassage(QtMsgType Type,const QMessageLogContext &cont,const QString &msg)
{
    QString log;
    switch(Type)
    {
    case QtDebugMsg:
        log="DEBUGE";
        break;
    case QtWarningMsg:
        log="WARNING";
        break;
    case QtInfoMsg:
        log="INFORMATION";
        break;
    case QtFatalMsg:
        log="FATAL";
        break;
    default:
        break;
    }
    QString loggMassage=QString(" [%1]  %2")
                              .arg(QDateTime::currentDateTime().toString(Qt::ISODate))
                              .arg(msg);
    QTextStream err(stderr);
    err<<loggMassage<<"\n";
    fflush(stderr);
    QFile file("application.log");
    if( file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        out<<loggMassage<<"\n";
        file.close();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qInstallMessageHandler(customMassage);
    // qInfo()<<"hello info"<<Qt::endl;
    // qDebug()<<"hello Debuge"<<Qt::endl;
    Server server;
    server.StartServer();
    return a.exec();
}
