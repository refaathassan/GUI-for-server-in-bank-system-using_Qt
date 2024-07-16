#include "deletehandler.h"

DeleteHandler::DeleteHandler(QObject *parent)
    : Handler{parent}
{
    pru = nullptr;  // Initialize pointer to next handler
}

QJsonObject DeleteHandler::Handling(QJsonObject json)
{
    bool flag = false;  // Flag to indicate if user was deleted
    QJsonObject news;  // Response JSON object

    if (json["Request"].toString() == "DeleteUser")
    {
        base->SetPath(QCoreApplication::applicationDirPath() + "\\base.json");  // Set database path
        base->InitDatatBase();  // Initialize the database

        if (Handler::CurrentType == "admin")
        {
            qDebug() << "Request from admin to Delete User " << json["accountnumber"].toString() << Qt::endl;

            // Iterate through database to find matching account number
            for (auto& vv : base->GetjsonVec())
            {
                if (vv["accountnumber"].toString() == json["accountnumber"].toString())
                {
                    flag = true;  // Account found, set flag to true
                    base->Delete(vv["accountnumber"].toString());  // Delete user from database
                    news["Request"] = "DeleteUser";
                    news["Response"] = "the user " + vv["fullname"].toString() + "  deleted";
                }
            }

            // Optionally, delete user history from a separate file
            base->SetPath(QCoreApplication::applicationDirPath() + "\\history.json");
            base->InitDatatBase();
            for (auto& vv : base->GetjsonVec())
            {
                if (vv["accountnumber"].toString() == json["accountnumber"].toString())
                {
                    base->Delete(vv["accountnumber"].toString());
                    // Optionally handle deletion response from history file
                }
            }
        }
        else
        {
            // Handle case where request is not from an admin (optional implementation)
        }

        if (flag == false)
        {
            news["Request"] = "DeleteUser";
            news["Response"] = "no account number like that";
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

void DeleteHandler::SetNextHandler(Handler *pru)
{
    this->pru = pru;  // Set the next handler in the chain
}
