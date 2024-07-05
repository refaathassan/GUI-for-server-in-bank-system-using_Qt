#ifndef UPDATEUSERHANDLER_H
#define UPDATEUSERHANDLER_H

#include <QObject>
#include "handler.h"
class UpdateUserHandler : public Handler
{
    Q_OBJECT
public:
    explicit UpdateUserHandler(QObject *parent = nullptr);
private:
    Handler* pru;
public:
    QJsonObject Handling(QJsonObject json) override;
    void SetNextHandler(Handler* pru)override;
signals:
};

#endif // UPDATEUSERHANDLER_H
