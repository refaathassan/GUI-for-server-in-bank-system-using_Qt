#ifndef VIEWDATABASE_H
#define VIEWDATABASE_H

#include <QObject>
#include "handler.h"
class ViewDataBase : public Handler
{
    Q_OBJECT
public:
    explicit ViewDataBase(QObject *parent = nullptr);
private:
    Handler* pru;
public:
    QJsonObject Handling(QJsonObject json) override;
    void SetNextHandler(Handler* pru)override;
signals:
};

#endif // VIEWDATABASE_H
