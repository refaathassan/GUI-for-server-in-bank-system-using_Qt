#include "handler.h"

Handler::Handler(QObject *parent)
    : QObject{parent}
{
    base=new DataBaseHandler;
}
QString Handler::CurrentAcountNumber="";
QString Handler::CurrentType="";
void Handler::sendEmail(const QString &to, const QString &subject, const QString &body)
{
    QProcess process;
    // Path to your batch script
    QString batchFilePath =QCoreApplication::applicationDirPath()+"//send_email.bat";

    // Prepare arguments
    QStringList arguments;
    arguments << to << subject << body;

    // Start the batch script with arguments
    // qDebug() << "Starting batch script with arguments:" << arguments;
    process.start(batchFilePath, arguments);
    if (!process.waitForFinished())
    {
        qDebug() << "Failed to run batch script:" << process.errorString();
        return;
    }

    // // Read and display the output or error
    // QString output = process.readAllStandardOutput();
    // QString error = process.readAllStandardError();
    // if (!error.isEmpty())
    // {
    //     qDebug() << "Error sending email:" << error;
    // }
    // else
    // {
    //     qDebug() << "Output:" << output;
    // }
}
