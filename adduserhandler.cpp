#include "adduserhandler.h"

AddUserHandler::AddUserHandler(QObject *parent)
    : Handler{parent}
{
    pru = nullptr;  // Initialize pointer to next handler
}

QJsonObject AddUserHandler::Handling(QJsonObject json)
{
    bool flag = false;  // Flag to indicate successful user addition
    bool isvalid = false;  // Flag to validate account number uniqueness
    QJsonObject news;  // Response JSON object
    QJsonObject user;  // User JSON object to be added

    if (json["Request"].toString() == "AddUser")
    {
        base->SetPath(QCoreApplication::applicationDirPath() + "\\base.json");  // Set database path
        base->InitDatatBase();  // Initialize database

        if (Handler::CurrentType == "admin")
        {
            qDebug() << "Request from admin to AddUser  " << json["username"].toString() << Qt::endl;
            flag = true;  // Assume user can be added

            // Check if the username already exists in the database
            for (auto& vv : base->GetjsonVec())
            {
                if (vv["username"].toString() == json["username"].toString())
                {
                    flag = false;  // Username already exists, cannot add user
                }
            }

            if (flag == true)
            {
                user["username"] = json["username"];

                if (json["type"] == "user")
                {
                    int randomNumber;
                    QString str;

                    // Generate a unique account number
                    do
                    {
                        isvalid = false;
                        randomNumber = QRandomGenerator::global()->bounded(1, 1000000);
                        str = QString::number(randomNumber);

                        // Check if the generated account number already exists
                        for (auto& vv : base->GetjsonVec())
                        {
                            if (vv["accountnumber"].toString() == str)
                            {
                                isvalid = true;  // Account number already exists, generate a new one
                            }
                        }
                    } while (isvalid == true);

                    user["accountnumber"] = str;
                    user["email"] = json["email"].toString();
                    user["balance"] = 0;
                }

                user["fullname"] = json["fullname"];
                user["password"] = json["password"];
                user["type"] = json["type"];

                news["Request"] = "AddUser";
                news["Response"] = "the user " + user["fullname"].toString() + " add";

                base->Add(user);  // Add the user to the database
            }
        }
        else
        {
            // Handle case where the request is not from an admin (optional implementation)
        }

        if (flag == false)
        {
            news["Request"] = "AddUser";
            news["Response"] = "please choose another username";
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

    return news;  // Return the response JSON object
}

void AddUserHandler::SetNextHandler(Handler *pru)
{
    this->pru = pru;  // Set the next handler in the chain
}
