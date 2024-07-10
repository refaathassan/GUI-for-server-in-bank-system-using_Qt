#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>
#include "databasehandler.h"
#include <QRandomGenerator>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcess>
class Handler : public QObject
{
    Q_OBJECT
public:
    explicit Handler(QObject *parent = nullptr);
    virtual QJsonObject Handling(QJsonObject json)=0;
    virtual void SetNextHandler(Handler*)=0;
    void sendEmail(const QString &to, const QString &subject, const QString &body);
signals:
    //void SendToSocket(QJsonObject jsond);
protected:
    DataBaseHandler* base;
    static QString CurrentAcountNumber;
    static QString CurrentType;
};

#endif // HANDLER_H

