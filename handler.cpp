#include "handler.h"

Handler::Handler(QObject *parent)
    : QObject{parent}
{
    base = new DataBaseHandler;  // Initialize the database handler
}

QString Handler::CurrentAcountNumber = "";  // Initialize static member variable
QString Handler::CurrentType = "";  // Initialize static member variable

void Handler::sendEmail(const QString &to, const QString &subject, const QString &body)
{
    QProcess pro;
    // Path to your batch script
    QString batchFilePath = QCoreApplication::applicationDirPath() + "//send_email.bat";

    // Prepare arguments
    QStringList arguments;
    arguments << to << subject << body;

    // Start the batch script with arguments
    pro.startDetached(batchFilePath, arguments);

}
