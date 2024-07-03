#ifndef DELETEHANDLER_H
#define DELETEHANDLER_H

#include <QObject>
#include "handler.h"
class DeleteHandler : public Handler
{
    Q_OBJECT
public:
    explicit DeleteHandler(QObject *parent = nullptr);
private:
    Handler* pru;
public:
    QJsonObject Handling(QJsonObject json) override;
    void SetNextHandler(Handler* pru)override;
signals:
};

#endif // DELETEHANDLER_H
