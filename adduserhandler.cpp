#include "adduserhandler.h"


AddUserHandler::AddUserHandler(QObject *parent)
    : Handler{parent}
{
    pru=nullptr;
}


QJsonObject AddUserHandler::Handling(QJsonObject json)
{
    bool flag=false;
    bool isvalid=false;
    QJsonObject news;
    QJsonObject user;
    if(json["Request"].toString()=="AddUser")
    {
        //qDebug()<<"log request"<<Qt::endl;
        base->SetPath(QCoreApplication::applicationDirPath()+"\\base.json");
        base->InitDatatBase();
        if(Handler::CurrentType=="admin")
        {
            qDebug()<<"Request from admin to AddUser  "<<json["username"].toString()<<Qt::endl;
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
                user["username"]=json["username"];
                if(json["type"]=="user")
                {
                    int randomNumber;
                    QString str;
                    do
                    {
                        isvalid=false;
                        randomNumber = QRandomGenerator::global()->bounded(1, 1000000);
                        str=QString::number(randomNumber);
                        for(auto& vv:base->GetjsonVec())
                        {
                            if(vv["accountnumber"].toString()==str)
                            {
                                isvalid==true;
                            }
                        }
                    }while(isvalid==true);
                    user["accountnumber"]=str;
                    user["email"]=json["email"].toString();
                    user["balance"]=0;
                }
                user["fullname"]=json["fullname"];
                user["password"]=json["password"];
                user["type"]=json["type"];


                news["Request"]="AddUser";
                news["Response"]="the user "+user["fullname"].toString()+" add";
                base->Add(user);
            }
        }
        else
        {
        }
        if(flag==false)
        {
            news["Request"]="AddUser";
            news["Response"]="please choose other user name";
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

void AddUserHandler::SetNextHandler(Handler *pru)
{
    this->pru=pru;
}
