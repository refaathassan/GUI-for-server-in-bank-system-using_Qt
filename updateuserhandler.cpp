#include "updateuserhandler.h"  // Include the header file for UpdateUserHandler class

UpdateUserHandler::UpdateUserHandler(QObject *parent)
    : Handler{parent}  // Constructor for UpdateUserHandler, inheriting from Handler class
{
    pru = nullptr;  // Initialize pointer to next handler as nullptr
}

// Method to handle the "UpdateUser" request
QJsonObject UpdateUserHandler::Handling(QJsonObject json)
{
    bool flag = false;  // Flag to indicate if the update operation was successful
    QJsonObject news;  // JSON object to store response

    // Check if the request is for "UpdateUser"
    if (json["Request"].toString() == "UpdateUser")
    {
        // Set path for the database file and initialize the database
        base->SetPath(QCoreApplication::applicationDirPath() + "\\base.json");
        base->InitDatatBase();

        // Check if the current user type is "admin"
        if (Handler::CurrentType == "admin")
        {
            // Output debug information about the update request from admin
            qDebug() << "Request from admin to UpdateUser " << json["accountnumber"].toString() << Qt::endl;

            flag = true;  // Assume success by default

            // Check if the provided username already exists
            for (auto& vv : base->GetjsonVec())
            {
                if (vv["username"].toString() == json["username"].toString())
                {
                    flag = false;  // Username already exists, update fails
                }
            }

            // If username does not exist, proceed with updating user information
            if (flag == true)
            {
                flag = false;

                // Iterate through accounts to find the one to update
                for (auto& vv : base->GetjsonVec())
                {
                    if (vv["accountnumber"].toString() == json["accountnumber"].toString())
                    {
                        flag = true;

                        // Update fields if provided in the request JSON
                        if (json["username"] != "")
                            vv["username"] = json["username"];

                        if (json["fullname"].toString() != "")
                            vv["fullname"] = json["fullname"];

                        if (json["password"].toString() != "")
                            vv["password"] = json["password"];

                        if (json["email"].toString() != "")
                            vv["email"] = json["email"];

                        vv["type"] = json["type"];

                        if (json["balance"].toString() != "")
                            vv["balance"] = json["balance"].toString().toInt();

                        // Prepare response JSON object
                        news["Request"] = "UpdateUser";
                        news["Response"] = "the user " + vv["fullname"].toString() + " Updated";

                        // Update the user information in the database
                        base->UpDate(vv);
                    }
                }

                // If account number was not found, update fails
                if (flag == false)
                {
                    news["Request"] = "UpdateUser";
                    news["Response"] = "the user not updated";
                }
            }
            else
            {
                // Username already exists, update fails
                news["Request"] = "UpdateUser";
                news["Response"] = "the user not updated";
            }
        }
        else
        {
            // Not an admin, do nothing
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
void UpdateUserHandler::SetNextHandler(Handler *pru)
{
    this->pru = pru;  // Assign provided handler as the next handler
}
