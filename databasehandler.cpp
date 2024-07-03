#include "databasehandler.h"
#include <QCoreApplication>
DataBaseHandler::DataBaseHandler(QObject *parent)
    : QObject{parent},path{QCoreApplication::applicationDirPath()+"\\base.json"}
{
  InitDatatBase();
}

void DataBaseHandler::Add(QJsonObject jsons)
{
    InitDatatBase();
    //jsonVec.push_back(newobj);
    QJsonArray jsonarr;
    for(auto& vv:jsonVec)
    {
        jsonarr.append(vv);
    }
    jsonarr.append(jsons);
    QFile file(path);
    if(file.open(QIODevice::ReadWrite|QIODevice::Text))
    {
        file.write(QJsonDocument(jsonarr).toJson());
        file.close();
    }
}

void DataBaseHandler::Delete(QString AccountNumber)
{

    QJsonArray jsonarr;
    for(auto& vv:jsonVec)
    {
        if(vv["accountnumber"].toString()!=AccountNumber)
            jsonarr.append(vv);
    }
    QFile file(path);
    if(file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Truncate))
    {
        file.write(QJsonDocument(jsonarr).toJson());
        file.close();
    }

}
void DataBaseHandler::UpDate(QJsonObject jso)
{
    InitDatatBase();
    QJsonArray jsonarr;
    for(auto& vv:jsonVec)
    {
        if(vv["accountnumber"].toString()==jso["accountnumber"].toString())
            jsonarr.append(jso);
        else
           jsonarr.append(vv);
    }
    QFile file(path);
    if(file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Truncate))
    {
        file.write(QJsonDocument(jsonarr).toJson());
        file.close();
    }

}
void DataBaseHandler::InitDatatBase(void)
{
    jsonVec.clear();
    QFile file(path);
    if(file.open(QIODevice::ReadWrite|QIODevice::Text))
    {
        QByteArray Barray=file.readAll();
        QJsonDocument Jdocument=QJsonDocument::fromJson(Barray);
        QJsonArray jsonar=Jdocument.array();
        for(auto ele:jsonar)
        {
            QJsonObject js=ele.toObject();
            jsonVec<<js;
        }
        //qDebug()<<" readed file"<<Qt::endl;
        file.close();
    }
    else{
       // qDebug()<<" i can not oppen this file"<<Qt::endl;
    }


}

QVector<QJsonObject> DataBaseHandler::GetjsonVec()
{
    return jsonVec;
}

