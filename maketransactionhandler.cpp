#include "maketransactionhandler.h"  // Include the header file for MakeTransactionHandler class

// Constructor for MakeTransactionHandler class
MakeTransactionHandler::MakeTransactionHandler(QObject *parent)
    : Handler{parent}  // Call the base class QObject constructor
{
    pru=nullptr;  // Initialize pru pointer to nullptr
}

// Handling function implementation
QJsonObject MakeTransactionHandler::Handling(QJsonObject json)
{
    bool flag=false;  // Flag to indicate if transaction was successful
    bool tran_flag=false;  // Flag to indicate if transaction history update was successful
    QJsonObject news;  // Response object to be returned
    QJsonObject tran_obj;  // Transaction object to store transaction details

    if(json["Request"].toString()=="MakeTransaction")  // Check if request is MakeTransaction
    {
        qDebug()<<"Request from user  "<<Handler::CurrentAcountNumber<<" to Make Transaction amount "<<json["amount"].toInt()<<Qt::endl;
        base->SetPath(QCoreApplication::applicationDirPath()+"\\base.json");  // Set path for base JSON database
        base->InitDatatBase();  // Initialize or reload database from file

        if(Handler::CurrentType=="user")  // If current user type is user
        {
            for(auto& vv:base->GetjsonVec())  // Iterate through user database entries
            {
                if(vv["accountnumber"].toString()==CurrentAcountNumber)  // Find current user by account number
                {
                    if(((vv["balance"].toInt()+json["amount"].toInt())>=0))  // Check if transaction is valid
                    {
                        flag=true;  // Set flag indicating successful transaction
                        vv["balance"]=vv["balance"].toInt()+json["amount"].toInt();  // Update user's balance

                        // Determine transaction type (deposit or withdrawal) and send email notification
                        QString str;
                        if(json["amount"].toInt()>0)
                            str="deposit";
                        else
                            str="withdraw";
                        sendEmail(vv["email"].toString(),"bank notification",
                                  "you made "+str+" by "+QString::number(json["amount"].toInt())+" $");

                        base->UpDate(vv);  // Update user's information in database

                        news["Request"]="MakeTransaction";
                        news["Response"]="the user "+vv["fullname"].toString()+"  your balance updated";

                        base->SetPath(QCoreApplication::applicationDirPath()+"\\history.json");  // Switch to history database
                        base->InitDatatBase();  // Initialize or reload history database

                        for(auto& tt:base->GetjsonVec())  // Iterate through history database entries
                        {
                            if(tt["accountnumber"].toString()==vv["accountnumber"].toString())  // Find user in history database
                            {
                                tran_flag=true;  // Set flag indicating successful history update
                                QDateTime Date = QDateTime::currentDateTime();  // Get current date/time
                                QString TimeString = Date.toString("yyyy-MM-dd HH:mm:ss");  // Format date/time string
                                tran_obj["date"]=TimeString;  // Set transaction date/time
                                tran_obj["amount"]=json["amount"].toInt();  // Set transaction amount

                                // Determine transaction description (deposit or withdrawal)
                                if(json["amount"].toInt()>0)
                                    tran_obj["descraption"]="deposit";
                                else
                                    tran_obj["descraption"]="withdraw";

                                QJsonArray arr=tt["transactions"].toArray();  // Get current transactions array
                                arr.push_front(tran_obj);  // Insert new transaction at the beginning
                                tt["transactions"]=arr;  // Update transactions array
                                base->UpDate(tt);  // Update user's history in history database
                            }
                        }

                        if(tran_flag==false)  // If transaction history update failed
                        {
                            QJsonArray arr;
                            QJsonObject new_tran;
                            new_tran["accountnumber"]=CurrentAcountNumber;  // Set account number in new transaction
                            QDateTime Date = QDateTime::currentDateTime();  // Get current date/time
                            QString TimeString = Date.toString("yyyy-MM-dd HH:mm:ss");  // Format date/time string
                            tran_obj["date"]=TimeString;  // Set transaction date/time
                            tran_obj["amount"]=json["amount"].toInt();  // Set transaction amount

                            // Determine transaction description (deposit or withdrawal)
                            if(json["amount"].toInt()>0)
                                tran_obj["descraption"]="deposit";
                            else
                                tran_obj["descraption"]="withdraw";

                            arr.push_front(tran_obj);  // Insert new transaction at the beginning
                            new_tran["transactions"]=arr;  // Set transactions array in new transaction
                            base->Add(new_tran);  // Add new transaction to history database
                        }
                    }
                }
            }
        }
        else
        {
            // Handle cases where current user type is not "user" (e.g., admin)
        }

        if(flag==false)  // If transaction was not successful
        {
            news["Request"]="MakeTransaction";
            news["Response"]="your balance not updated";
        }
        else
        {
            // Handle cases where transaction was successful
        }
    }
    else
    {
        if(pru!=nullptr)  // If there is a next handler in the chain
        {
            news= pru->Handling(json);  // Forward the request to the next handler
        }
        else{}  // Handle cases where there is no next handler
    }

    // qDebug()<<Handler::CurrentType<<"   "<<Handler::CurrentAcountNumber<<Qt::endl;
    return news;  // Return response object
}

void MakeTransactionHandler::SetNextHandler(Handler *pru)
{
    this->pru=pru;  // Set the next handler in the chain
}
