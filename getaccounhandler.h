#ifndef GETACCOUNHANDLER_H
#define GETACCOUNHANDLER_H

#include <QObject>
#include "handler.h"
class GetAccounHandler : public Handler
{
    Q_OBJECT
public:
    explicit GetAccounHandler(QObject *parent = nullptr);
private:
    Handler* pru;
public:
    QJsonObject Handling(QJsonObject json) override;
    void SetNextHandler(Handler* pru)override;
signals:
};
#endif // GETACCOUNHANDLER_H
