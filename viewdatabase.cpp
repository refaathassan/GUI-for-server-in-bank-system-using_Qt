#include "viewdatabase.h"

ViewDataBase::ViewDataBase(QObject *parent)
    : Handler{parent}
{
    pru=nullptr;
}

QJsonObject ViewDataBase::Handling(QJsonObject json)
{
    bool flag=false;
    QJsonObject news;
    QJsonArray jsonarr;
    if(json["Request"].toString()=="ViewDataBase")
    {
        //qDebug()<<"log request"<<Qt::endl;
        base->InitDatatBase();
        if(Handler::CurrentType=="admin")
        {
            for(auto& vv:base->GetjsonVec())
            {
                jsonarr.append(vv);
            }
            flag=true;
            //qDebug()<<"refaat is here"<<Qt::endl;
            news["Request"]="ViewDataBase";
            news["Response"]=jsonarr;
        }
        else
        {

        }
        if(flag==false)
        {
            news["Request"]="GetAccountNumber";
            news["Response"]="no account number like that";
        }
        else
        {

        }
    }
    else
    {
        if(pru!=nullptr)
        {
            news= pru->Handling(json);
        }
        else{}
    }
   // qDebug()<<Handler::CurrentType<<"   "<<Handler::CurrentAcountNumber<<Qt::endl;
    return news;
}

void ViewDataBase::SetNextHandler(Handler *pru)
{
    this->pru=pru;
}
