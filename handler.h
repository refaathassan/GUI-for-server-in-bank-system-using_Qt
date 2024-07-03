#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include "databasehandler.h"
class Handler : public QObject
{
    Q_OBJECT
public:
    explicit Handler(QObject *parent = nullptr);
    virtual QJsonObject Handling(QJsonObject json)=0;
    virtual void SetNextHandler(Handler*)=0;
signals:
    //void SendToSocket(QJsonObject jsond);
protected:
    DataBaseHandler* base;
    static QString CurrentAcountNumber;
    static QString CurrentType;
};

#endif // HANDLER_H

