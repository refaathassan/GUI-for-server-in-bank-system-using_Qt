#include "getaccounhandler.h"

GetAccounHandler::GetAccounHandler(QObject *parent)
    : Handler{parent}
{
    pru = nullptr;  // Initialize pointer to next handler
}

QJsonObject GetAccounHandler::Handling(QJsonObject json)
{
    bool flag = false;  // Flag to indicate if account number was found
    QJsonObject news;  // Response JSON object

    if (json["Request"].toString() == "GetAccountNumber")
    {
        base->SetPath(QCoreApplication::applicationDirPath() + "\\base.json");  // Set database path
        base->InitDatatBase();  // Initialize the database

        if (Handler::CurrentType == "admin")
        {
            qDebug() << "Request from admin to Get Account Number to username  " << json["username"].toString() << Qt::endl;

            // Iterate through database to find matching username
            for (auto& vv : base->GetjsonVec())
            {
                if (vv["username"].toString() == json["username"].toString())
                {
                    flag = true;  // Username found, set flag to true
                    news["Request"] = "GetAccountNumber";
                    news["Response"] = vv["accountnumber"];  // Return account number
                }
            }
        }
        else
        {
            qDebug() << "Request from user  " << Handler::CurrentAcountNumber << " to Get Account Number  " << Qt::endl;
            flag = true;
            news["Request"] = "GetAccountNumber";
            news["Response"] = Handler::CurrentAcountNumber;  // Return current user's account number
        }

        if (flag == false)
        {
            news["Request"] = "GetAccountNumber";
            news["Response"] = "no account number like that";  // Handle case where no matching username/account number found
        }
    }
    else
    {
        // If the current handler cannot handle the request, pass it to the next handler
        if (pru != nullptr)
        {
            news = pru->Handling(json);
        }
    }

    // Optionally print current user type and account number for debugging
    // qDebug() << Handler::CurrentType << "   " << Handler::CurrentAcountNumber << Qt::endl;

    return news;  // Return the response JSON object
}

void GetAccounHandler::SetNextHandler(Handler *pru)
{
    this->pru = pru;  // Set the next handler in the chain
}
