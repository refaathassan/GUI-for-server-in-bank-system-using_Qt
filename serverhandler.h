#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
//#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCryptographicHash>
#include <QByteArray>
#include <QCryptographicHash>
#include <QByteArray>
#include <QFile>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>


#include "qaesencryption.h"

#include "loghandler.h"
#include "getaccounhandler.h"
#include "viewaccountbalancehandler.h"
#include "viewdatabase.h"
#include "deletehandler.h"
#include "adduserhandler.h"
#include "maketransactionhandler.h"
#include "transferamounthandler.h"
#include "viewtransactionhistoryhandler.h"
#include "updateuserhandler.h"

class ServerHandler : public QThread
{
    Q_OBJECT
public:
    explicit ServerHandler(qint32 ID=0,QObject *parent = nullptr);
    void OnDisconnect(void);
    void OnReadyRead(void);
    void SendMassage(QJsonObject json);
    void Operation(QJsonObject json);

    QByteArray decryptAndVerify(const QByteArray &encryptedData, const QByteArray &key, const QByteArray &iv, const QByteArray &rsaPublicKeyFile);
    bool verifySignature(const QByteArray &data, const QByteArray &signature, const QByteArray &rsaPublicKeyFile);

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
    Handler *PRU9;
    Handler *PRU10;
    QByteArray key;
    QByteArray iv;
    QString publick;
    // QThread interface
protected:
    void run();
};

#endif // SERVERHANDLER_H
