#ifndef VIEWTRANSACTIONHISTORYHANDLER_H
#define VIEWTRANSACTIONHISTORYHANDLER_H

#include <QObject>
#include "handler.h"
class ViewTransactionHistoryHandler : public Handler
{
    Q_OBJECT
public:
    explicit ViewTransactionHistoryHandler(QObject *parent = nullptr);
private:
    Handler* pru;
public:
    QJsonObject Handling(QJsonObject json) override;
    void SetNextHandler(Handler* pru)override;
signals:
};

#endif // VIEWTRANSACTIONHISTORYHANDLER_H
