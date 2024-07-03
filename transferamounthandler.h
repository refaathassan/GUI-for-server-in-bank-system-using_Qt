#ifndef TRANSFERAMOUNTHANDLER_H
#define TRANSFERAMOUNTHANDLER_H

#include <QObject>
#include "handler.h"
class TransferAmountHandler : public Handler
{
    Q_OBJECT
public:
    explicit TransferAmountHandler(QObject *parent = nullptr);
private:
    Handler* pru;
public:
    QJsonObject Handling(QJsonObject json) override;
    void SetNextHandler(Handler* pru)override;
signals:
};

#endif // TRANSFERAMOUNTHANDLER_H


