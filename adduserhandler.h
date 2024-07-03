#ifndef ADDUSERHANDLER_H
#define ADDUSERHANDLER_H

#include <QObject>
#include "handler.h"
class AddUserHandler : public Handler
{
    Q_OBJECT
public:
    explicit AddUserHandler(QObject *parent = nullptr);
private:
    Handler* pru;
public:
    QJsonObject Handling(QJsonObject json) override;
    void SetNextHandler(Handler* pru)override;
signals:
};

#endif // ADDUSERHANDLER_H

