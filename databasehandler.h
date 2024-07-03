#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include <QVector>
class DataBaseHandler : public QObject
{
    Q_OBJECT

public:
    explicit DataBaseHandler(QObject *parent = nullptr);
    void Add(QJsonObject jsons);
    void Delete(QString AccountNumber);
    void UpDate(QJsonObject jso);
    void InitDatatBase(void);
    QVector<QJsonObject> GetjsonVec(void);
private:
    QVector<QJsonObject> jsonVec;
    QString path;
signals:
};

#endif // DATABASEHANDLER_H
