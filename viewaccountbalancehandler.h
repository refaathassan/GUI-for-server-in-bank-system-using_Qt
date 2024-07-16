
#ifndef VIEWACCOUNTBALANCEHANDLER_H
#define VIEWACCOUNTBALANCEHANDLER_H

#include <QObject>
#include "handler.h"  // Include base class definition

class ViewAccountBalanceHandler : public Handler
{
    Q_OBJECT  // Macro required for Qt's meta-object system
public:
    explicit ViewAccountBalanceHandler(QObject *parent = nullptr);  // Constructor declaration
private:
    Handler* pru;  // Private member variable
public:
    QJsonObject Handling(QJsonObject json) override;  // Method declaration
    void SetNextHandler(Handler* pru) override;  // Method declaration
signals:  // Qt signal declarations
};

#endif // VIEWACCOUNTBALANCEHANDLER_H
