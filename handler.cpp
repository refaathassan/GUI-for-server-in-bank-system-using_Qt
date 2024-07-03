#include "handler.h"

Handler::Handler(QObject *parent)
    : QObject{parent}
{
    base=new DataBaseHandler;
}
QString Handler::CurrentAcountNumber="";
QString Handler::CurrentType="";
