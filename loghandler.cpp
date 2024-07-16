
#include "loghandler.h"

LogHandler::LogHandler(QObject *parent)
    : Handler{parent}
{
    pru = nullptr;  // Initialize pointer to next handler
}

QJsonObject LogHandler::Handling(QJsonObject json)
{
    bool flag = false;  // Flag to indicate if login was successful
    QJsonObject news;  // Response JSON object

    if (json["Request"].toString() == "Log")
    {
        qDebug() << "log request" << Qt::endl;
        base->SetPath(QCoreApplication::applicationDirPath() + "\\base.json");  // Set database path
        base->InitDatatBase();  // Initialize the database

        // Iterate through database to find matching username and password
        for (auto& vv : base->GetjsonVec())
        {
            if ((vv["username"].toString() == json["username"].toString()) &&
                (vv["password"].toString() == json["password"].toString()))
            {
                flag = true;  // Username and password match found, set flag to true
                news["Request"] = "Log";
                news["Response"] = "1";  // Successful login response
                Handler::CurrentType = vv["type"].toString();  // Set current user type

                if (Handler::CurrentType == "admin")
                {
                    news["type"] = "admin";
                    Handler::CurrentAcountNumber = "";  // Clear current account number for admin
                }
                else
                {
                    news["type"] = "user";
                    Handler::CurrentAcountNumber = vv["accountnumber"].toString();  // Set current account number for user
                }
            }
        }

        if (flag == false)
        {
            news["Request"] = "Log";
            news["Response"] = "0";  // Unsuccessful login response
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

void LogHandler::SetNextHandler(Handler *pru)
{
    this->pru = pru;  // Set the next handler in the chain
}
