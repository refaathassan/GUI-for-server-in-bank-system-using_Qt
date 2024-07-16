#include "server.h"  // Include the header file for Server class

// Constructor for Server class, inheriting QTcpServer
Server::Server(QObject *parent)
    : QTcpServer{parent},  // Call base class constructor with parent object
    qin(stdin),  // Initialize qin as standard input stream
    qout(stdout)  // Initialize qout as standard output stream
{
    port=30000;  // Set default port number
}

// Method to start the server
void Server::StartServer()
{

    this->listen(QHostAddress::Any,port);  // Start listening on any available network interface and specified port

    if(this->isListening())  // Check if server is successfully listening
    {
        qout<<" server is  listen to =>  "<<port<<Qt::endl;  // Output confirmation message with port number
    }
    else
    {
        qout<<"server can not listen......"<<Qt::endl;  // Output error message if server failed to start listening
    }
}

// Method called when there is an incoming connection
void Server::incomingConnection(qintptr handle)
{
    //qDebug()<<"client  "<<handle<<"  connected "<<Qt::endl;  // Debug output to console indicating client connection

    ServerHandler* hand=new ServerHandler(handle,this);  // Create a new ServerHandler object for the incoming connection
    connect(hand,&QThread::finished,hand,&QThread::deleteLater);  // Connect finished signal of handler thread to deleteLater slot
    hand->start();  // Start the handler thread to manage the incoming connection
}
