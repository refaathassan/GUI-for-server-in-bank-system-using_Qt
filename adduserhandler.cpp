#include "adduserhandler.h"


AddUserHandler::AddUserHandler(QObject *parent)
    : Handler{parent}
{
    pru=nullptr;
}


QJsonObject AddUserHandler::Handling(QJsonObject json)
{
    bool flag=false;
    bool isDigits=true;
    bool isnull=true;
    QJsonObject news;
    QJsonObject user;
    if(json["Request"].toString()=="AddUser")
    {
        //qDebug()<<"log request"<<Qt::endl;
        base->InitDatatBase();
        if(Handler::CurrentType=="admin")
        {
            for (const QString &key : json.keys()) {
                if (json.value(key)=="") {
                    isnull=false;
                }
            }
            if(isDigits==true&&isnull==true)
            {
                flag=true;
                user["username"]=json["username"];
                user["accountnumber"]=json["accountnumber"];
                user["fullname"]=json["fullname"];
                user["password"]=json["password"];
                user["type"]=json["type"];
                user["balance"]=0;

                news["Request"]="AddUser";
                news["Response"]="the user "+user["fullname"].toString()+" add";
                base->Add(user);
            }
            else
            {
                flag=true;
                news["Request"]="AddUser";
                news["Response"]="the user is not add becuase error in data";
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

void AddUserHandler::SetNextHandler(Handler *pru)
{
    this->pru=pru;
}
