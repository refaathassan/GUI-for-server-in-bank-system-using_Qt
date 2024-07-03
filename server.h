#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTextStream>
#include <QDebug>
#include "serverhandler.h"
class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void StartServer(void);

signals:

    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle);
private:
    QTextStream qin;
    QTextStream qout;
    qint32 port;
};

#endif // SERVER_H
