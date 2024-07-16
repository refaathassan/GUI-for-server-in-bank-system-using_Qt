#include "viewtransactionhistoryhandler.h"  // Include the header file for ViewTransactionHistoryHandler class

ViewTransactionHistoryHandler::ViewTransactionHistoryHandler(QObject *parent)
    : Handler{parent}  // Constructor for ViewTransactionHistoryHandler, inheriting from Handler class
{
    pru = nullptr;  // Initialize pointer to next handler as nullptr
}

// Method to handle the "ViewTransactionHistory" request
QJsonObject ViewTransactionHistoryHandler::Handling(QJsonObject json)
{
    bool flag = false;  // Flag to indicate if transaction history was found
    QJsonObject news;   // JSON object to store response

    // Check if the request is for "ViewTransactionHistory"
    if (json["Request"].toString() == "ViewTransactionHistory")
    {
        // Set path for the history database file and initialize the database
        base->SetPath(QCoreApplication::applicationDirPath() + "\\history.json");
        base->InitDatatBase();

        // Check if the current user type is "admin"
        if (Handler::CurrentType == "admin")
        {
            // Output debug information about the view transaction history request from admin
            qDebug() << "Request from admin to View Transaction History to " << json["accountnumber"].toString() << Qt::endl;

            // Iterate through history database records
            for (auto& vv : base->GetjsonVec())
            {
                if (vv["accountnumber"].toString() == json["accountnumber"].toString())
                {
                    flag = true;
                    QJsonArray arr = vv["transactions"].toArray();
                    QJsonArray arr1;

                    // Determine the number of transactions to return based on the count parameter
                    if ((arr.size() <= json["count"].toInt()) || (json["count"].toInt() == 0))
                    {
                        arr1 = arr;
                    }
                    else
                    {
                        for (int i = 0; i < json["count"].toInt(); i++)
                        {
                            QJsonObject jsons = arr[i].toObject();
                            arr1.push_back(jsons);
                        }
                    }

                    // Prepare response JSON object with the transaction history
                    news["Request"] = "ViewTransactionHistory";
                    news["Response"] = arr1;
                }
                else
                {
                    // Do nothing if account number does not match
                }
            }
        }
        else
        {
            // Output debug information about the view transaction history request from user
            qDebug() << "Request from user  " << CurrentAcountNumber << " to View Transaction History" << Qt::endl;

            // Iterate through history database records
            for (auto& vv : base->GetjsonVec())
            {
                if (vv["accountnumber"].toString() == CurrentAcountNumber)
                {
                    flag = true;
                    QJsonArray arr = vv["transactions"].toArray();
                    QJsonArray arr1;

                    // Determine the number of transactions to return based on the count parameter
                    if ((arr.size() <= json["count"].toInt()) || (json["count"].toInt() == 0))
                    {
                        arr1 = arr;
                    }
                    else
                    {
                        for (int i = 0; i < json["count"].toInt(); i++)
                        {
                            QJsonObject jsons = arr[i].toObject();
                            arr1.push_back(jsons);
                        }
                    }

                    // Prepare response JSON object with the transaction history
                    news["Request"] = "ViewTransactionHistory";
                    news["Response"] = arr1;
                }
                else
                {
                    // Do nothing if account number does not match
                }
            }
        }

        // If transaction history was not found, prepare a response indicating no transaction history
        if (flag == false)
        {
            news["Request"] = "ViewTransactionHistory";
            news["Response"] = "no transection history";
        }
        else
        {
            // Do nothing if transaction history was found
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
void ViewTransactionHistoryHandler::SetNextHandler(Handler *pru)
{
    this->pru = pru;  // Assign provided handler as the next handler
}
