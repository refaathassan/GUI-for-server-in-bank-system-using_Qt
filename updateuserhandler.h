
#ifndef UPDATEUSERHANDLER_H
#define UPDATEUSERHANDLER_H

#include <QObject>
#include "handler.h"  // Include base class definition

class UpdateUserHandler : public Handler
{
    Q_OBJECT  // Macro required for Qt's meta-object system
public:
    explicit UpdateUserHandler(QObject *parent = nullptr);  // Constructor declaration
private:
    Handler* pru;  // Private member variable
public:
    QJsonObject Handling(QJsonObject json) override;  // Method declaration
    void SetNextHandler(Handler* pru) override;  // Method declaration
signals:  // Qt signal declarations
};

#endif // UPDATEUSERHANDLER_H
