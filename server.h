#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTextStream>
//#include <QDebug>
#include "serverhandler.h"  // Include server handler class

class Server : public QTcpServer
{
    Q_OBJECT  // Macro required for Qt's meta-object system
public:
    explicit Server(QObject *parent = nullptr);  // Constructor declaration
    void StartServer(void);  // Method declaration

signals:  // Qt signal declarations

protected:
    void incomingConnection(qintptr handle);  // Method declaration
private:
    QTextStream qin;  // Private member variable
    QTextStream qout;  // Private member variable
    qint32 port;  // Private member variable
};

#endif // SERVER_H
