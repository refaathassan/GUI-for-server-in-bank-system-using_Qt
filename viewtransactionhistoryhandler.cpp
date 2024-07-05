#include "viewtransactionhistoryhandler.h"


ViewTransactionHistoryHandler::ViewTransactionHistoryHandler(QObject *parent)
    : Handler{parent}
{
    pru=nullptr;
}

QJsonObject ViewTransactionHistoryHandler::Handling(QJsonObject json)
{
    bool flag=false;
    QJsonObject news;
    if(json["Request"].toString()=="ViewTransactionHistory")
    {
        //qDebug()<<"log request"<<Qt::endl;
        base->SetPath(QCoreApplication::applicationDirPath()+"\\history.json");
        base->InitDatatBase();
        if(Handler::CurrentType=="admin")
        {
            qDebug()<<"Request from admin to View Transaction History"<<Qt::endl;
            for(auto& vv:base->GetjsonVec())
            {

                if(vv["accountnumber"].toString()==json["accountnumber"].toString())
                {
                    flag=true;
                    QJsonArray arr=vv["transactions"].toArray();
                    QJsonArray arr1;
                    if(arr.size()<=json["count"].toInt())
                    {
                        arr1=arr;

                    }
                    else
                    {
                        for(int i=0;i<json["count"].toInt();i++)
                        {
                        QJsonObject jsons=arr[i].toObject();
                          arr1.push_back(jsons);
                        }
                    }
                    news["Request"]="ViewTransactionHistory";
                    news["Response"]=arr1;
                }
                else
                {

                }

            }

        }
        else
        {
            qDebug()<<"Request from user  "<<CurrentAcountNumber <<" to View Transaction History"<<Qt::endl;
            for(auto& vv:base->GetjsonVec())
            {

                if(vv["accountnumber"].toString()==CurrentAcountNumber)
                {
                   // qDebug()<<"i am in array"<<Qt::endl;
                    flag=true;
                    QJsonArray arr=vv["transactions"].toArray();
                    QJsonArray arr1;
                    if(arr.size()<=json["count"].toInt())
                    {
                        arr1=arr;

                    }
                    else
                    {
                        for(int i=0;i<json["count"].toInt();i++)
                        {
                            QJsonObject jsons=arr[i].toObject();
                            arr1.push_back(jsons);
                        }
                    }
                    news["Request"]="ViewTransactionHistory";
                    news["Response"]=arr1;
                }
                else
                {

                }

            }

        }
        if(flag==false)
        {
            news["Request"]="ViewTransactionHistory";
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
    //qDebug()<<Handler::CurrentType<<"   "<<Handler::CurrentAcountNumber<<"  "<<json["count"].toInt()<<Qt::endl;
    return news;
}

void ViewTransactionHistoryHandler::SetNextHandler(Handler *pru)
{
    this->pru=pru;
}
