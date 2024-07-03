#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
//#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include "loghandler.h"
#include "getaccounhandler.h"
#include "viewaccountbalancehandler.h"
#include "viewdatabase.h"
#include "deletehandler.h"
#include "adduserhandler.h"
#include "maketransactionhandler.h"
#include "transferamounthandler.h"
class ServerHandler : public QThread
{
    Q_OBJECT
public:
    explicit ServerHandler(qint32 ID=0,QObject *parent = nullptr);
    void OnDisconnect(void);
    void OnReadyRead(void);
    void SendMassage(QJsonObject json);
    void Operation(QJsonObject json);
signals:
private:
    qint32 ID;
    QTcpSocket * soc;

    Handler *PRU1;
    Handler *PRU2;
    Handler *PRU3;
    Handler *PRU4;
    Handler *PRU5;
    Handler *PRU6;
    Handler *PRU7;
    Handler *PRU8;
    // QThread interface
protected:
    void run();
};

#endif // SERVERHANDLER_H
