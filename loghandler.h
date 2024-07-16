#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include <QObject>
#include "handler.h"  // Include base class definition

class LogHandler : public Handler
{
    Q_OBJECT  // Macro required for Qt's meta-object system
public:
    explicit LogHandler(QObject *parent = nullptr);  // Constructor declaration
private:
    Handler* pru;  // Private member variable
public:
    QJsonObject Handling(QJsonObject json) override;  // Method declaration
    void SetNextHandler(Handler* pru) override;  // Method declaration
signals:  // Qt signal declarations
};

#endif // LOGHANDLER_H
