#include "maketransactionhandler.h"

MakeTransactionHandler::MakeTransactionHandler(QObject *parent)
    : Handler{parent}
{
    pru=nullptr;
}


QJsonObject MakeTransactionHandler::Handling(QJsonObject json)
{
    bool flag=false;
    bool tran_flag=false;
    QJsonObject news;
    QJsonObject tran_obj;
    if(json["Request"].toString()=="MakeTransaction")
    {
        qDebug()<<"Request from user  "<<Handler::CurrentAcountNumber<<" to Make Transaction amount "<<json["amount"].toInt()<<Qt::endl;
        base->SetPath(QCoreApplication::applicationDirPath()+"\\base.json");
        base->InitDatatBase();
        if(Handler::CurrentType=="user")
        {

            for(auto& vv:base->GetjsonVec())
            {
                if(vv["accountnumber"].toString()==CurrentAcountNumber)
                {
                    //qDebug()<<json["amount"]<<"  "<<json["amount"].toInt()<<"i am in processing  "<<"  "<<isDigit<<Qt::endl;
                    if(((vv["balance"].toInt()+json["amount"].toInt())>=0))
                    {
                        flag=true;
                        vv["balance"]=vv["balance"].toInt()+json["amount"].toInt();
                        QString str;
                        if(json["amount"].toInt()>0)
                            str="deposite";
                        else
                            str="withdraw";
                        sendEmail(vv["email"].toString(),"bank notification",
                                  "you made "+str+" by "+QString::number(json["amount"].toInt())+" $");
                        base->UpDate(vv);
                        news["Request"]="MakeTransaction";
                        news["Response"]="the user "+vv["fullname"].toString()+"  your balance updated";
                        base->SetPath(QCoreApplication::applicationDirPath()+"\\history.json");
                        base->InitDatatBase();
                        for(auto& tt:base->GetjsonVec())
                        {
                            if(tt["accountnumber"].toString()==vv["accountnumber"].toString())
                            {
                                tran_flag=true;
                                QDateTime Date = QDateTime::currentDateTime();
                                QString TimeString = Date.toString("yyyy-MM-dd HH:mm:ss");
                                tran_obj["date"]=TimeString;
                                tran_obj["amount"]=json["amount"].toInt();
                                if(json["amount"].toInt()>0)
                                    tran_obj["descraption"]="deposit";
                                else
                                    tran_obj["descraption"]="withdraw";
                                QJsonArray arr=tt["transactions"].toArray();
                                arr.push_front(tran_obj);
                                tt["transactions"]=arr;
                                base->UpDate(tt);
                            }
                        }
                        if(tran_flag==false)
                        {
                            QJsonArray arr;
                            QJsonObject new_tran;
                            new_tran["accountnumber"]=CurrentAcountNumber;
                            QDateTime Date = QDateTime::currentDateTime();
                            QString TimeString = Date.toString("yyyy-MM-dd HH:mm:ss");
                            tran_obj["date"]=TimeString;
                            tran_obj["amount"]=json["amount"].toInt();
                            if(json["amount"].toInt()>0)
                                tran_obj["descraption"]="deposit";
                            else
                                tran_obj["descraption"]="withdraw";
                            arr.push_front(tran_obj);
                            new_tran["transactions"]=arr;
                            base->Add(new_tran);

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
    //qDebug()<<Handler::CurrentType<<"   "<<Handler::CurrentAcountNumber<<Qt::endl;
    return news;
}

void MakeTransactionHandler::SetNextHandler(Handler *pru)
{
    this->pru=pru;
}
