
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
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>

#include "qaesencryption.h"  // Include AES encryption class

// Include all handler classes
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
    Q_OBJECT  // Macro required for Qt's meta-object system
public:
    explicit ServerHandler(qint32 ID = 0, QObject *parent = nullptr);  // Constructor declaration
    void OnDisconnect(void);  // Method declaration
    void OnReadyRead(void);  // Method declaration
    void SendMassage(QJsonObject json);  // Method declaration
    void Operation(QJsonObject json);  // Method declaration

    QByteArray decryptAndVerify(const QByteArray &encryptedData, const QByteArray &key, const QByteArray &iv, const QByteArray &rsaPublicKeyFile);  // Method declaration
    bool verifySignature(const QByteArray &data, const QByteArray &signature, const QByteArray &rsaPublicKeyFile);  // Method declaration

signals:  // Qt signal declarations

private:
    qint32 ID;  // Private member variable
    QTcpSocket *soc;  // Private member variable

    // Handler pointers
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
    bool flag;
    QByteArray key;  // Private member variable
    QByteArray iv;  // Private member variable
    QString publick;  // Private member variable

protected:
    void run();  // Method declaration (from QThread)
};

#endif // SERVERHANDLER_H
