#ifndef GETACCOUNHANDLER_H
#define GETACCOUNHANDLER_H

#include <QObject>
#include "handler.h"  // Include base class definition

class GetAccounHandler : public Handler
{
    Q_OBJECT  // Macro required for Qt's meta-object system
public:
    explicit GetAccounHandler(QObject *parent = nullptr);  // Constructor declaration
private:
    Handler* pru;  // Private member variable
public:
    QJsonObject Handling(QJsonObject json) override;  // Method declaration
    void SetNextHandler(Handler* pru) override;  // Method declaration
signals:  // Qt signal declarations
};

#endif // GETACCOUNHANDLER_H
