#include "transferamounthandler.h"

TransferAmountHandler::TransferAmountHandler(QObject *parent)
    : Handler{parent}
{
    pru=nullptr;
}



QJsonObject TransferAmountHandler::Handling(QJsonObject json)
{
    bool flag=false;
     bool tran_flag1=false;
     bool tran_flag2=false;
    QJsonObject tran_obj1;
    QJsonObject tran_obj2;

    QJsonObject news;
    if(json["Request"].toString()=="TransferAmount")
    {
        qDebug()<<"Request from user  "<<Handler::CurrentAcountNumber<<" to Transfer Amount "<<json["amount"].toInt()<<" to "<<json["accountnumber"].toString()<<Qt::endl;
        base->SetPath(QCoreApplication::applicationDirPath()+"\\base.json");
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
                                sendEmail(ss["email"].toString(),"bank notification",
                                          "you transfred amount of "+QString::number(json["amount"].toInt())+" $ to "
                                          +rr["accountnumber"].toString());
                                sendEmail(rr["email"].toString(),"bank notification",
                                          "you received   "+QString::number(json["amount"].toInt())+" $ from "
                                              +ss["accountnumber"].toString());

                                base->UpDate(ss);
                                base->UpDate(rr);
                                news["Request"]="TransferAmount";
                                news["Response"]=" the operation successed ......... ";
                                base->SetPath(QCoreApplication::applicationDirPath()+"\\history.json");
                                base->InitDatatBase();
                                for(auto& sen:base->GetjsonVec())
                                {
                                    if(sen["accountnumber"].toString()==ss["accountnumber"].toString())
                                    {
                                        tran_flag1=true;
                                        QDateTime Date = QDateTime::currentDateTime();
                                        QString TimeString = Date.toString("yyyy-MM-dd HH:mm:ss");
                                        tran_obj1["date"]=TimeString;
                                        tran_obj1["amount"]=json["amount"].toInt();
                                        tran_obj1["descraption"]="send to "+json["accountnumber"].toString();
                                        QJsonArray arr=sen["transactions"].toArray();
                                        arr.push_front(tran_obj1);
                                        sen["transactions"]=arr;
                                        base->UpDate(sen);
                                    }
                                }
                                if(tran_flag1==false)
                                {
                                    QJsonArray arr;
                                    QJsonObject new_tran;
                                    new_tran["accountnumber"]=CurrentAcountNumber;
                                    QDateTime Date = QDateTime::currentDateTime();
                                    QString TimeString = Date.toString("yyyy-MM-dd HH:mm:ss");
                                    tran_obj1["date"]=TimeString;
                                    tran_obj1["amount"]=json["amount"].toInt();
                                    if(json["amount"].toInt()>0)
                                    tran_obj1["descraption"]="send to "+json["accountnumber"].toString();
                                    arr.push_front(tran_obj1);
                                    new_tran["transactions"]=arr;
                                    base->Add(new_tran);

                                }
                                for(auto& rec:base->GetjsonVec())
                                {
                                    if(rec["accountnumber"].toString()==rr["accountnumber"].toString())
                                    {
                                        tran_flag2=true;
                                        QDateTime Date = QDateTime::currentDateTime();
                                        QString TimeString = Date.toString("yyyy-MM-dd HH:mm:ss");
                                        tran_obj2["date"]=TimeString;
                                        tran_obj2["amount"]=json["amount"].toInt();
                                        tran_obj2["descraption"]="recieved from "+CurrentAcountNumber;
                                        QJsonArray arr=rec["transactions"].toArray();
                                        arr.push_front(tran_obj2);
                                        rec["transactions"]=arr;
                                        base->UpDate(rec);
                                    }
                                }
                                if(tran_flag2==false)
                                {
                                    QJsonArray arr;
                                    QJsonObject new_tran;
                                    new_tran["accountnumber"]=json["accountnumber"].toString();
                                    QDateTime Date = QDateTime::currentDateTime();
                                    QString TimeString = Date.toString("yyyy-MM-dd HH:mm:ss");
                                    tran_obj2["date"]=TimeString;
                                    tran_obj2["amount"]=json["amount"].toInt();
                                    tran_obj2["descraption"]="recieved from "+CurrentAcountNumber;
                                    arr.push_front(tran_obj2);
                                    new_tran["transactions"]=arr;
                                    base->Add(new_tran);

                                }
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

