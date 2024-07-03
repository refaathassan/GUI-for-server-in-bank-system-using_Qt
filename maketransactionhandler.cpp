#include "maketransactionhandler.h"

MakeTransactionHandler::MakeTransactionHandler(QObject *parent)
    : Handler{parent}
{
    pru=nullptr;
}


QJsonObject MakeTransactionHandler::Handling(QJsonObject json)
{
    bool flag=false;
    bool isDigit=false;
    QJsonObject news;
    if(json["Request"].toString()=="MakeTransaction")
    {
        //qDebug()<<"log request"<<Qt::endl;
        base->InitDatatBase();
        if(Handler::CurrentType=="user")
        {

            for(auto& vv:base->GetjsonVec())
            {
                if(vv["accountnumber"].toString()==CurrentAcountNumber)
                {
                    qDebug()<<json["amount"]<<"  "<<json["amount"].toInt()<<"i am in processing  "<<"  "<<isDigit<<Qt::endl;
                    if(((vv["balance"].toInt()+json["amount"].toInt())>=0))
                    {
                        flag=true;
                        vv["balance"]=vv["balance"].toInt()+json["amount"].toInt();
                        base->UpDate(vv);
                        news["Request"]="MakeTransaction";
                        news["Response"]="the user "+vv["fullname"].toString()+"  your balance updated";
                    }

                }
                else
                {

                }
            }
        }
        else
        {
        }
        if(flag==false)
        {
            news["Request"]="MakeTransaction";
            news["Response"]="your balance not updated";
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
    qDebug()<<Handler::CurrentType<<"   "<<Handler::CurrentAcountNumber<<Qt::endl;
    return news;
}

void MakeTransactionHandler::SetNextHandler(Handler *pru)
{
    this->pru=pru;
}
