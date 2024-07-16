#include "databasehandler.h"

DataBaseHandler::DataBaseHandler(QObject *parent)
    : QObject{parent}
{
    path = QCoreApplication::applicationDirPath() + "\\base.json";  // Set default database file path
    InitDatatBase();  // Initialize the database by reading from the file
}

void DataBaseHandler::SetPath(QString path)
{
    this->path = path;  // Set a new path for the database file
}

void DataBaseHandler::Add(QJsonObject jsons)
{
    InitDatatBase();  // Ensure database is initialized

    QJsonArray jsonarr;
    for(auto& vv : jsonVec)
    {
        jsonarr.append(vv);  // Append existing objects to the array
    }
    jsonarr.append(jsons);  // Append new object to the array

    QFile file(path);
    if(file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        file.write(QJsonDocument(jsonarr).toJson());  // Write the updated JSON array to file
        file.close();
    }
}

void DataBaseHandler::Delete(QString AccountNumber)
{
    QJsonArray jsonarr;
    for(auto& vv : jsonVec)
    {
        if(vv["accountnumber"].toString() != AccountNumber)
            jsonarr.append(vv);  // Append objects except the one to be deleted
    }

    QFile file(path);
    if(file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
    {
        file.write(QJsonDocument(jsonarr).toJson());  // Write the updated JSON array to file (truncate existing content)
        file.close();
    }
}

void DataBaseHandler::UpDate(QJsonObject jso)
{
    InitDatatBase();  // Ensure database is initialized

    QJsonArray jsonarr;
    for(auto& vv : jsonVec)
    {
        if(vv["accountnumber"].toString() == jso["accountnumber"].toString())
            jsonarr.append(jso);  // Update the object if account number matches
        else
            jsonarr.append(vv);  // Append existing objects
    }

    QFile file(path);
    if(file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
    {
        file.write(QJsonDocument(jsonarr).toJson());  // Write the updated JSON array to file (truncate existing content)
        file.close();
    }
}

void DataBaseHandler::InitDatatBase(void)
{
    jsonVec.clear();  // Clear existing data

    QFile file(path);
    if(file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QByteArray Barray = file.readAll();  // Read all data from the file
        QJsonDocument Jdocument = QJsonDocument::fromJson(Barray);  // Convert to JSON document
        QJsonArray jsonar = Jdocument.array();  // Convert to JSON array

        for(auto ele : jsonar)
        {
            QJsonObject js = ele.toObject();  // Convert each element to JSON object
            jsonVec << js;  // Append to vector
        }

        file.close();  // Close the file
    }
}

QVector<QJsonObject> DataBaseHandler::GetjsonVec()
{
    return jsonVec;  // Return the vector of JSON objects
}
