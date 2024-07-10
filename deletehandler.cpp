#include "deletehandler.h"

DeleteHandler::DeleteHandler(QObject *parent)
    : Handler{parent}
{
    pru=nullptr;
}


QJsonObject DeleteHandler::Handling(QJsonObject json)
{
    bool flag=false;
    QJsonObject news;
    if(json["Request"].toString()=="DeleteUser")
    {
        //qDebug()<<"log request"<<Qt::endl;
        base->SetPath(QCoreApplication::applicationDirPath()+"\\base.json");
        base->InitDatatBase();
        if(Handler::CurrentType=="admin")
        {
            qDebug()<<"Request from admin to Delete User "<<json["accountnumber"].toString()<<Qt::endl;
            for(auto& vv:base->GetjsonVec())
            {
                if(vv["accountnumber"].toString()==json["accountnumber"].toString())
                {
                    flag=true;
                    base->Delete(vv["accountnumber"].toString());
                    news["Request"]="DeleteUser";
                    news["Response"]="the user "+vv["fullname"].toString()+"  deleted";

                }
                else
                {

                }
            }
            base->SetPath(QCoreApplication::applicationDirPath()+"\\history.json");
            base->InitDatatBase();
            for(auto& vv:base->GetjsonVec())
            {
                if(vv["accountnumber"].toString()==json["accountnumber"].toString())
                {
                    base->Delete(vv["accountnumber"].toString());
                   // news["Request"]="DeleteUser";
                   // news["Response"]="the user "+vv["fullname"].toString()+"  deleted";

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
            news["Request"]="DeleteUser";
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

void DeleteHandler::SetNextHandler(Handler *pru)
{
    this->pru=pru;
}
