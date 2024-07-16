#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>
#include "databasehandler.h"  // Include required header file
#include <QRandomGenerator>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcess>
#include <QVector>
class Handler : public QObject
{
    Q_OBJECT  // Macro required for Qt's meta-object system
public:
    explicit Handler(QObject *parent = nullptr);  // Constructor declaration
    virtual QJsonObject Handling(QJsonObject json) = 0;  // Pure virtual method declaration
    virtual void SetNextHandler(Handler*) = 0;  // Pure virtual method declaration
    void sendEmail(const QString &to, const QString &subject, const QString &body);  // Method declaration
signals:
         //void SendToSocket(QJsonObject jsond);  // Signal declaration (commented out)
protected:
    DataBaseHandler* base;  // Protected member variable
    static QString CurrentAcountNumber;  // Static member variable
    static QString CurrentType;  // Static member variable
};

#endif // HANDLER_H
