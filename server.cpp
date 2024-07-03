#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer{parent},qin(stdin),qout(stdout)
{}


void Server::StartServer()
{
    // qout<<"enter server to listen.........."<<Qt::endl;
    // qout.flush();
    // port=qin.readLine().toInt();
    this->listen(QHostAddress::Any,1234);
    if(this->isListening())
    {
        qout<<" server is  listen to =>  "<<port<<Qt::endl;
    }
    else
    {
        qout<<"server can not listen......"<<Qt::endl;
    }
}

void Server::incomingConnection(qintptr handle)
{
    qDebug()<<"client  "<<handle<<"  connected "<<Qt::endl;
    ServerHandler* hand=new ServerHandler(handle,this);
    connect(hand,&QThread::finished,hand,&QThread::deleteLater);
    hand->start();
}
