#ifndef SERVERNETWORK_H
#define SERVERNETWORK_H

#include <QObject>
#include <QtNetwork>
#include <QStringList>

class serverNetwork
{
public:
    serverNetwork();
    QStringList Getip();
};

#endif // SERVERNETWORK_H
