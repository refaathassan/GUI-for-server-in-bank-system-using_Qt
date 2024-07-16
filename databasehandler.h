#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QCoreApplication>
//#include <QDebug>
#include <QVector>

class DataBaseHandler : public QObject
{
    Q_OBJECT  // Macro required for Qt's meta-object system

public:
    explicit DataBaseHandler(QObject *parent = nullptr);  // Constructor declaration
    void Add(QJsonObject jsons);  // Method declaration
    void Delete(QString AccountNumber);  // Method declaration
    void UpDate(QJsonObject jso);  // Method declaration
    void InitDatatBase(void);  // Method declaration
    QVector<QJsonObject> GetjsonVec(void);  // Method declaration
    void SetPath(QString path);  // Method declaration
private:
    QVector<QJsonObject> jsonVec;  // Private member variable
    QString path;  // Private member variable
signals:  // Qt signal declarations
};

#endif // DATABASEHANDLER_H
