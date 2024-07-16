#include <QCoreApplication>  // Include the core application class for Qt
#include "server.h"  // Include the custom server class header
#include <QTextStream>  // Include QTextStream for console output
#include <QString>  // Include QString for string manipulation
#include <QFile>  // Include QFile for file operations
#include <QDebug>  // Include qDebug() for debugging output
#include <QDateTime>  // Include QDateTime for date/time handling

// Custom message handler function
void customMassage(QtMsgType Type, const QMessageLogContext &cont, const QString &msg)
{
    QString log;
    // Determine log type based on QtMsgType
    switch(Type)
    {
    case QtDebugMsg:
        log = "DEBUGE";  // Debug message
        break;
    case QtWarningMsg:
        log = "WARNING";  // Warning message
        break;
    case QtInfoMsg:
        log = "INFORMATION";  // Informational message
        break;
    case QtFatalMsg:
        log = "FATAL";  // Fatal error message
        break;
    default:
        break;
    }
    // Format log message with current date/time and message content
    QString loggMassage = QString(" [%1]  %2")
                              .arg(QDateTime::currentDateTime().toString(Qt::ISODate))
                              .arg(msg);
    QTextStream err(stderr);  // QTextStream for standard error output
    err << loggMassage << "\n";  // Print log message to stderr
    fflush(stderr);  // Flush standard error stream

    QFile file("application.log");  // Open log file "application.log"
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);  // QTextStream for file output
        out << loggMassage << "\n";  // Write log message to file
        file.close();  // Close the log file
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);  // Create the application object
    qInstallMessageHandler(customMassage);  // Install custom message handler

    // qInfo()<<"hello info"<<Qt::endl;  // Example of Qt info message
    // qDebug()<<"hello Debuge"<<Qt::endl;  // Example of Qt debug message

    Server server;  // Create an instance of the Server class
    server.StartServer();  // Start the server

    return a.exec();  // Execute the application event loop
}
