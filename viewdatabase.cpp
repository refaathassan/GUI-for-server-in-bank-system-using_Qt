#include "viewdatabase.h"  // Include the header file for ViewDataBase class

ViewDataBase::ViewDataBase(QObject *parent)
    : Handler{parent}  // Constructor for ViewDataBase, inheriting from Handler class
{
    pru = nullptr;  // Initialize pointer to next handler as nullptr
}

// Method to handle the "ViewDataBase" request
QJsonObject ViewDataBase::Handling(QJsonObject json)
{
    bool flag = false;  // Flag to indicate if database view was successful
    QJsonObject news;   // JSON object to store response
    QJsonArray jsonarr; // JSON array to store database records

    // Check if the request is for "ViewDataBase"
    if (json["Request"].toString() == "ViewDataBase")
    {
        // Set path for the database file and initialize the database
        base->SetPath(QCoreApplication::applicationDirPath() + "\\base.json");
        base->InitDatatBase();

        // Check if the current user type is "admin"
        if (Handler::CurrentType == "admin")
        {
            // Output debug information about the view database request from admin
            qDebug() << "Request from admin to View Data Base" << Qt::endl;

            // Iterate through database records and append them to jsonarr
            for (auto& vv : base->GetjsonVec())
            {
                jsonarr.append(vv);
            }

            flag = true;
            // Prepare response JSON object with the database records
            news["Request"] = "ViewDataBase";
            news["Response"] = jsonarr;
        }
        else
        {
            // Do nothing if user type is not "admin"
        }

        // If database view was unsuccessful, prepare a response indicating no matching account number
        if (flag == false)
        {
            news["Request"] = "GetAccountNumber";
            news["Response"] = "no account number like that";
        }
        else
        {
            // Do nothing if database view was successful
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
void ViewDataBase::SetNextHandler(Handler *pru)
{
    this->pru = pru;  // Assign provided handler as the next handler
}
