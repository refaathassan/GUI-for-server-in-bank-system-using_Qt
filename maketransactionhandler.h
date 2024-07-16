
#ifndef MAKETRANSACTIONHANDLER_H
#define MAKETRANSACTIONHANDLER_H

#include <QObject>
#include "handler.h"  // Include base class definition

class MakeTransactionHandler : public Handler
{
    Q_OBJECT  // Macro required for Qt's meta-object system
public:
    explicit MakeTransactionHandler(QObject *parent = nullptr);  // Constructor declaration
private:
    Handler* pru;  // Private member variable
public:
    QJsonObject Handling(QJsonObject json) override;  // Method declaration
    void SetNextHandler(Handler* pru) override;  // Method declaration
signals:  // Qt signal declarations
};

#endif // MAKETRANSACTIONHANDLER_H
