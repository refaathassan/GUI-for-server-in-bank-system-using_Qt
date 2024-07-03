#include "serverhandler.h"



ServerHandler::ServerHandler(qint32 ID, QObject *parent): QThread{parent},ID{ID}
{
    PRU1=new LogHandler();
    PRU2=new GetAccounHandler();
    PRU3=new ViewAccountBalanceHandler();
    PRU4=new ViewDataBase();
    PRU5=new DeleteHandler();
    PRU6=new AddUserHandler();
    PRU7=new MakeTransactionHandler();
    PRU8=new TransferAmountHandler();
    PRU1->SetNextHandler(PRU2);
    PRU2->SetNextHandler(PRU3);
    PRU3->SetNextHandler(PRU4);
    PRU4->SetNextHandler(PRU5);
    PRU5->SetNextHandler(PRU6);
    PRU6->SetNextHandler(PRU7);
    PRU7->SetNextHandler(PRU8);
    //connect(PRU1,&Handler::SendToSocket,this,&ServerHandler::SendMassage);
}

void ServerHandler::OnDisconnect()
{
    if(soc->isOpen())
    {
        soc->close();
        qDebug()<<"client =>"<<ID<<"  Disconnected  "<<Qt::endl;
    }
}

void ServerHandler::OnReadyRead()
{

    QByteArray B_arrary=soc->readAll();
    qDebug()<<"on ready read"<<B_arrary<<Qt::endl;
    //QString str=QString(B_arrary);
    QJsonDocument json=QJsonDocument::fromJson(B_arrary);
    QJsonObject jsonObj=json.object();
    Operation(jsonObj);
    //PRU1->Handling(jsonObj);
    // qint32 length=str.split("/")[0].split(':')[1].toInt();
    // QString data=str.right(length);
    // QJsonDocument json=QJsonDocument::fromJson(data.toUtf8());
    // QJsonObject jsonObj=json.object();
    // qint32 dataSize=jsonObj["size"].toInt();
    // QString realdata=jsonObj["FileData"].toString();
    // if(dataSize==realdata.size())
    // {
    //     Operation(realdata);
    // }

    //Operation(QString(B_arrary));
}

void ServerHandler::SendMassage(QJsonObject json)
{
    if(soc->isOpen())
    {
        //QJsonObject news;
        // news["file-type"]="massage";
        // news["size"]=massage.toUtf8().size();
        // news["FileData"]=massage;
        QByteArray byte=QJsonDocument(json).toJson(QJsonDocument::Compact);
        // QString str=QString("JsonSize:%1/").arg(byte.size());
        // byte.prepend(str.toUtf8());
        qDebug()<<"on ready send"<<byte<<Qt::endl;
        soc->write(byte);
    }
}

void ServerHandler::Operation(QJsonObject json)
{
    // if(massage=="hello")
    // {
    //     SendMassage("massage reply..............");
    // }
   QJsonObject js =PRU1->Handling(json);

   SendMassage(js);

}

void ServerHandler::run()
{
    qDebug()<<"client=>  "<<ID<<"   is running in thread=> "<<QThread::currentThreadId()<<Qt::endl;
    soc=new QTcpSocket;
    soc->setSocketDescriptor(ID);
    connect(soc,&QTcpSocket::readyRead,this,&ServerHandler::OnReadyRead,Qt::DirectConnection);
    connect(soc,&QTcpSocket::disconnected,this,&ServerHandler::OnDisconnect,Qt::DirectConnection);
    //SendMassage("Hello From My Server...........");
    exec();
}
