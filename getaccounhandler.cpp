#include "getaccounhandler.h"

GetAccounHandler::GetAccounHandler(QObject *parent)
    : Handler{parent}
{
    pru=nullptr;
}


QJsonObject GetAccounHandler::Handling(QJsonObject json)
{
    bool flag=false;
    QJsonObject news;
    if(json["Request"].toString()=="GetAccountNumber")
    {
            //qDebug()<<"log request"<<Qt::endl;
            base->InitDatatBase();
            if(Handler::CurrentType=="admin")
            {
                for(auto& vv:base->GetjsonVec())
                {
                    if(vv["username"].toString()==json["username"].toString())
                    {
                        flag=true;
                        //qDebug()<<"refaat is here"<<Qt::endl;
                        news["Request"]="GetAccountNumber";
                        news["Response"]=vv["accountnumber"];

                    }
                    else
                    {

                    }
                }
            }
            else
            {
                flag=true;
                //qDebug()<<"refaat is here"<<Qt::endl;
                news["Request"]="GetAccountNumber";
                news["Response"]=Handler::CurrentAcountNumber;

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
    qDebug()<<Handler::CurrentType<<"   "<<Handler::CurrentAcountNumber<<Qt::endl;
    return news;
}

void GetAccounHandler::SetNextHandler(Handler *pru)
{
    this->pru=pru;
}