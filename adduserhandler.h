#ifndef ADDUSERHANDLER_H
#define ADDUSERHANDLER_H

#include <QObject>
#include "handler.h"  // Include base class definition

class AddUserHandler : public Handler
{
    Q_OBJECT  // Macro required for Qt's meta-object system
public:
    explicit AddUserHandler(QObject *parent = nullptr);  // Constructor declaration
private:
    Handler* pru;  // Private member variable
public:
    QJsonObject Handling(QJsonObject json) override;  // Method declaration
    void SetNextHandler(Handler* pru) override;  // Method declaration
signals:  // Qt signal declarations
};

#endif // ADDUSERHANDLER_H
