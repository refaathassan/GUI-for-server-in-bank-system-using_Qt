#include "transferamounthandler.h"  // Include the header file for TransferAmountHandler class

TransferAmountHandler::TransferAmountHandler(QObject *parent)
    : Handler{parent}  // Constructor for TransferAmountHandler, inheriting from Handler class
{
    pru = nullptr;  // Initialize pointer to next handler as nullptr
}

// Method to handle the "TransferAmount" request
QJsonObject TransferAmountHandler::Handling(QJsonObject json)
{
    bool flag = false;  // Flag to indicate if the transfer operation was successful
    bool tran_flag1 = false;  // Flag to indicate if sender's transaction history was updated
    bool tran_flag2 = false;  // Flag to indicate if receiver's transaction history was updated
    QJsonObject tran_obj1;  // JSON object to store sender's transaction details
    QJsonObject tran_obj2;  // JSON object to store receiver's transaction details

    QJsonObject news;  // JSON object to store response

    // Check if the request is for "TransferAmount"
    if (json["Request"].toString() == "TransferAmount")
    {
        // Output debug information about the transfer request
        qDebug() << "Request from user  " << Handler::CurrentAcountNumber
                 << " to Transfer Amount " << json["amount"].toInt()
                 << " to " << json["accountnumber"].toString() << Qt::endl;

        // Set path for the database file and initialize the database
        base->SetPath(QCoreApplication::applicationDirPath() + "\\base.json");
        base->InitDatatBase();

        // Check if the current user type is "user"
        if (Handler::CurrentType == "user")
        {
            // Iterate through all accounts in the database
            for (auto& ss : base->GetjsonVec())
            {
                // Find the account matching the current account number
                if (ss["accountnumber"].toString() == CurrentAcountNumber)
                {
                    // Check if the balance is sufficient for the transfer
                    if (ss["balance"].toInt() >= json["amount"].toInt())
                    {
                        // Iterate through accounts again to find the recipient account
                        for (auto& rr : base->GetjsonVec())
                        {
                            if (rr["accountnumber"].toString() == json["accountnumber"].toString())
                            {
                                //the client can not transfer amount to his self
                                if(json["accountnumber"].toString()!=CurrentAcountNumber)
                                {
                                    flag = true;  // Set flag indicating successful transfer

                                    // Update balances of sender and recipient
                                    ss["balance"] = ss["balance"].toInt() - json["amount"].toInt();
                                    rr["balance"] = rr["balance"].toInt() + json["amount"].toInt();

                                    // Send notification emails to sender and recipient
                                    sendEmail(ss["email"].toString(), "bank notification",
                                              "you transfred amount of " + QString::number(json["amount"].toInt()) + " $ to "
                                                  + rr["accountnumber"].toString());
                                    sendEmail(rr["email"].toString(), "bank notification",
                                              "you received   " + QString::number(json["amount"].toInt()) + " $ from "
                                                  + ss["accountnumber"].toString());

                                    // Update sender's and recipient's account information in the database
                                    base->UpDate(ss);
                                    base->UpDate(rr);

                                    // Prepare response JSON object
                                    news["Request"] = "TransferAmount";
                                    news["Response"] = " the operation successed ......... ";

                                    // Set path for the history database and initialize it
                                    base->SetPath(QCoreApplication::applicationDirPath() + "\\history.json");
                                    base->InitDatatBase();

                                    // Update sender's transaction history
                                    for (auto& sen : base->GetjsonVec())
                                    {
                                        if (sen["accountnumber"].toString() == ss["accountnumber"].toString())
                                        {
                                            tran_flag1 = true;
                                            QDateTime Date = QDateTime::currentDateTime();
                                            QString TimeString = Date.toString("yyyy-MM-dd HH:mm:ss");
                                            tran_obj1["date"] = TimeString;
                                            tran_obj1["amount"] = json["amount"].toInt();
                                            tran_obj1["descraption"] = "send to " + json["accountnumber"].toString();
                                            QJsonArray arr = sen["transactions"].toArray();
                                            arr.push_front(tran_obj1);
                                            sen["transactions"] = arr;
                                            base->UpDate(sen);
                                        }
                                    }

                                    // If sender's transaction history was not updated, create new entry
                                    if (tran_flag1 == false)
                                    {
                                        QJsonArray arr;
                                        QJsonObject new_tran;
                                        new_tran["accountnumber"] = CurrentAcountNumber;
                                        QDateTime Date = QDateTime::currentDateTime();
                                        QString TimeString = Date.toString("yyyy-MM-dd HH:mm:ss");
                                        tran_obj1["date"] = TimeString;
                                        tran_obj1["amount"] = json["amount"].toInt();
                                        if (json["amount"].toInt() > 0)
                                            tran_obj1["descraption"] = "send to " + json["accountnumber"].toString();
                                        arr.push_front(tran_obj1);
                                        new_tran["transactions"] = arr;
                                        base->Add(new_tran);
                                    }

                                    // Update recipient's transaction history
                                    for (auto& rec : base->GetjsonVec())
                                    {
                                        if (rec["accountnumber"].toString() == rr["accountnumber"].toString())
                                        {
                                            tran_flag2 = true;
                                            QDateTime Date = QDateTime::currentDateTime();
                                            QString TimeString = Date.toString("yyyy-MM-dd HH:mm:ss");
                                            tran_obj2["date"] = TimeString;
                                            tran_obj2["amount"] = json["amount"].toInt();
                                            tran_obj2["descraption"] = "recieved from " + CurrentAcountNumber;
                                            QJsonArray arr = rec["transactions"].toArray();
                                            arr.push_front(tran_obj2);
                                            rec["transactions"] = arr;
                                            base->UpDate(rec);
                                        }
                                    }

                                    // If recipient's transaction history was not updated, create new entry
                                    if (tran_flag2 == false)
                                    {
                                        QJsonArray arr;
                                        QJsonObject new_tran;
                                        new_tran["accountnumber"] = json["accountnumber"].toString();
                                        QDateTime Date = QDateTime::currentDateTime();
                                        QString TimeString = Date.toString("yyyy-MM-dd HH:mm:ss");
                                        tran_obj2["date"] = TimeString;
                                        tran_obj2["amount"] = json["amount"].toInt();
                                        tran_obj2["descraption"] = "recieved from " + CurrentAcountNumber;
                                        arr.push_front(tran_obj2);
                                        new_tran["transactions"] = arr;
                                        base->Add(new_tran);
                                    }
                                }
                                else
                                {

                                }
                            }
                        }
                    }
                }
            }
        }

        // If flag is still false, the transfer operation failed
        if (flag == false)
        {
            news["Request"] = "TransferAmount";
            news["Response"] = "there is some thng wrong in the operation";
        }
    }
    else
    {
        // If pru (next handler) is not nullptr, delegate handling to it
        if (pru != nullptr)
        {
            news = pru->Handling(json);
        }
        else
        {
            // Do nothing if pru is nullptr
        }
    }

    // Return the JSON response object
    return news;
}

// Method to set the next handler in the chain
void TransferAmountHandler::SetNextHandler(Handler *pru)
{
    this->pru = pru;  // Assign provided handler as the next handler
}
