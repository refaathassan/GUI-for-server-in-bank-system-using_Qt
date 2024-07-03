#include "transferamounthandler.h"

TransferAmountHandler::TransferAmountHandler(QObject *parent)
    : Handler{parent}
{
    pru=nullptr;
}



QJsonObject TransferAmountHandler::Handling(QJsonObject json)
{
    bool flag=false;
    QJsonObject news;
    if(json["Request"].toString()=="TransferAmount")
    {
        //qDebug()<<"log request"<<Qt::endl;
        base->InitDatatBase();
        if(Handler::CurrentType=="user")
        {

            for(auto& ss:base->GetjsonVec())
            {
                if(ss["accountnumber"].toString()==CurrentAcountNumber)
                {
                    //qDebug()<<json["amount"]<<"  "<<json["amount"].toInt()<<"i am in processing  "<<"  "<<isDigit<<Qt::endl;
                    if(((ss["balance"].toInt()>=json["amount"].toInt())))
                    {
                        for(auto& rr:base->GetjsonVec())
                        {
                            if(rr["accountnumber"].toString()==json["accountnumber"].toString())
                            {
                                flag=true;
                                ss["balance"]=ss["balance"].toInt()-json["amount"].toInt();
                                rr["balance"]=rr["balance"].toInt()+json["amount"].toInt();
                                base->UpDate(ss);
                                base->UpDate(rr);
                                news["Request"]="TransferAmount";
                                news["Response"]=" the operation successed ......... ";
                            }
                        }

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
            news["Request"]="TransferAmount";
            news["Response"]="there is some thng wrong in the operation";
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

void TransferAmountHandler::SetNextHandler(Handler *pru)
{
    this->pru=pru;
}

