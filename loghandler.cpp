#include "loghandler.h"

LogHandler::LogHandler(QObject *parent)
    : Handler{parent}
{
    pru=nullptr;
}

QJsonObject LogHandler::Handling(QJsonObject json)
{


    bool flag=false;
    QJsonObject news;
    if(json["Request"].toString()=="Log")
    {
        //qDebug()<<"log request"<<Qt::endl;
        base->SetPath(QCoreApplication::applicationDirPath()+"\\base.json");
        base->InitDatatBase();
        for(auto& vv:base->GetjsonVec())
        {
            if((vv["username"].toString()==json["username"].toString())&&(vv["password"].toString()==json["password"].toString()))
            {
                flag=true;
                //qDebug()<<"refaat is here"<<Qt::endl;
                news["Request"]="Log";
                news["Response"]="1";
                Handler::CurrentType=vv["type"].toString();
                if(Handler::CurrentType=="admin")
                {
                    news["type"]="admin";
                    Handler::CurrentAcountNumber="";
                }
                else
                {
                    news["type"]="user";
                    Handler::CurrentAcountNumber=vv["accountnumber"].toString();
                }

            }
            else
            {

            }
        }
        if(flag==false)
        {
            news["Request"]="Log";
            news["Response"]="0";
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

void LogHandler::SetNextHandler(Handler *pru)
{
    this->pru=pru;
}
