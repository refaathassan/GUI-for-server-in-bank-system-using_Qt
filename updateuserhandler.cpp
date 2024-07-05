#include "updateuserhandler.h"

UpdateUserHandler::UpdateUserHandler(QObject *parent)
    : Handler{parent}
{
    pru=nullptr;
}


QJsonObject UpdateUserHandler::Handling(QJsonObject json)
{
    bool flag=false;
    QJsonObject news;
    if(json["Request"].toString()=="UpdateUser")
    {
        //qDebug()<<"log request"<<Qt::endl;
        base->SetPath(QCoreApplication::applicationDirPath()+"\\base.json");
        base->InitDatatBase();
        if(Handler::CurrentType=="admin")
        {
            qDebug()<<"Request from admin to UpdateUser"<<Qt::endl;
            flag=true;
            for(auto& vv:base->GetjsonVec())
            {
                if(vv["username"].toString()==json["username"].toString())
                {
                    flag=false;
                }
            }
            if(flag==true)
            {
                flag=false;
                for(auto& vv:base->GetjsonVec())
                {
                    if(vv["accountnumber"].toString()==json["accountnumber"].toString())
                    {
                        flag=true;
                            if(json["username"]!="")
                                vv["username"]=json["username"];

                            if(json["fullname"].toString()!="")
                                vv["fullname"]=json["fullname"];

                            if(json["password"].toString()!="")
                                vv["password"]=json["password"];

                            vv["type"]=json["type"];

                            if(json["balance"].toString()!="")
                                vv["balance"]=json["balance"].toString().toInt();
                            news["Request"]="UpdateUser";
                            news["Response"]="the user "+vv["fullname"].toString()+" Updated";
                            base->UpDate(vv);
                    }

                 }
                if(flag==false)
                 {
                     news["Request"]="UpdateUser";
                     news["Response"]="the user not updated";
                 }
            }
            else
            {
            news["Request"]="UpdateUser";
            news["Response"]="the user not updated";
            }
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

void UpdateUserHandler::SetNextHandler(Handler *pru)
{
    this->pru=pru;
}
