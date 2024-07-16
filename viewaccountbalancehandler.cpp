#include "viewaccountbalancehandler.h"  // Include the header file for ViewAccountBalanceHandler class

ViewAccountBalanceHandler::ViewAccountBalanceHandler(QObject *parent)
    : Handler{parent}  // Constructor for ViewAccountBalanceHandler, inheriting from Handler class
{
    pru = nullptr;  // Initialize pointer to next handler as nullptr
}

// Method to handle the "ViewAccountBalance" request
QJsonObject ViewAccountBalanceHandler::Handling(QJsonObject json)
{
    bool flag = false;  // Flag to indicate if the account balance was found
    QJsonObject news;   // JSON object to store response

    // Check if the request is for "ViewAccountBalance"
    if (json["Request"].toString() == "ViewAccountBalance")
    {
        // Set path for the database file and initialize the database
        base->SetPath(QCoreApplication::applicationDirPath() + "\\base.json");
        base->InitDatatBase();

        // Check if the current user type is "admin"
        if (Handler::CurrentType == "admin")
        {
            // Output debug information about the view account balance request from admin
            qDebug() << "Request from admin to View Account Balance  " << json["accountnumber"].toString() << Qt::endl;

            // Iterate through accounts to find the one with the requested account number
            for (auto& vv : base->GetjsonVec())
            {
                if (vv["accountnumber"].toString() == json["accountnumber"].toString())
                {
                    flag = true;
                    // Prepare response JSON object with the account balance
                    news["Request"] = "ViewAccountBalance";
                    news["Response"] = vv["balance"];
                }
                else
                {
                    // Do nothing if account number does not match
                }
            }
        }
        else
        {
            // Output debug information about the view account balance request from user
            qDebug() << "Request from user  " << CurrentAcountNumber << " to View Account Balance " << Qt::endl;

            // Iterate through accounts to find the one with the current account number
            for (auto& vv : base->GetjsonVec())
            {
                if (vv["accountnumber"].toString() == CurrentAcountNumber)
                {
                    flag = true;
                    // Prepare response JSON object with the account balance
                    news["Request"] = "ViewAccountBalance";
                    news["Response"] = vv["balance"];
                }
                else
                {
                    // Do nothing if account number does not match
                }
            }
        }

        // If account balance was not found, prepare a response indicating no matching account number
        if (flag == false)
        {
            news["Request"] = "GetAccountNumber";
            news["Response"] = "no account number like that";
        }
        else
        {
            // Do nothing if account balance was found
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
void ViewAccountBalanceHandler::SetNextHandler(Handler *pru)
{
    this->pru = pru;  // Assign provided handler as the next handler
}
