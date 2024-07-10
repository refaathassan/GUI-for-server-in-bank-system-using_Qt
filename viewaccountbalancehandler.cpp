#include "viewaccountbalancehandler.h"

ViewAccountBalanceHandler::ViewAccountBalanceHandler(QObject *parent)
    : Handler{parent}
{
pru=nullptr;
}


QJsonObject ViewAccountBalanceHandler::Handling(QJsonObject json)
{
    bool flag=false;
    QJsonObject news;
    if(json["Request"].toString()=="ViewAccountBalance")
    {

        base->SetPath(QCoreApplication::applicationDirPath()+"\\base.json");
        base->InitDatatBase();
        if(Handler::CurrentType=="admin")
        {
            qDebug()<<"Request from admin to View Account Balance  "<<json["accountnumber"].toString()<<Qt::endl;
            for(auto& vv:base->GetjsonVec())
            {
                if(vv["accountnumber"].toString()==json["accountnumber"].toString())
                {
                    flag=true;
                    //qDebug()<<"refaat is here"<<Qt::endl;
                    news["Request"]="ViewAccountBalance";
                    news["Response"]=vv["balance"];

                }
                else
                {

                }
            }
        }
        else
        {
            qDebug()<<"Request from user  "<<CurrentAcountNumber<<" to View Account Balance "<<Qt::endl;
            for(auto& vv:base->GetjsonVec())
            {
                if(vv["accountnumber"].toString()==CurrentAcountNumber)
                {
                    flag=true;
                    //qDebug()<<"refaat is here"<<Qt::endl;
                    news["Request"]="ViewAccountBalance";
                    news["Response"]=vv["balance"];

                }
                else
                {

                }
            }

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
    //qDebug()<<Handler::CurrentType<<"   "<<Handler::CurrentAcountNumber<<Qt::endl;
    return news;
}

void ViewAccountBalanceHandler::SetNextHandler(Handler *pru)
{
    this->pru=pru;
}
