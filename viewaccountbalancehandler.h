#ifndef VIEWACCOUNTBALANCEHANDLER_H
#define VIEWACCOUNTBALANCEHANDLER_H

#include <QObject>
#include "handler.h"
class ViewAccountBalanceHandler : public Handler
{
    Q_OBJECT
public:
    explicit ViewAccountBalanceHandler(QObject *parent = nullptr);
private:
    Handler* pru;
public:
    QJsonObject Handling(QJsonObject json) override;
    void SetNextHandler(Handler* pru)override;
signals:
};

#endif // VIEWACCOUNTBALANCEHANDLER_H
