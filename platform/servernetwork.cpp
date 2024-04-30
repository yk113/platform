#include "servernetwork.h"


serverNetwork::serverNetwork()
{

}

QStringList serverNetwork::Getip()
{
    QStringList list_ip;
    QList<QHostAddress> list =QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
       if(address.protocol() ==QAbstractSocket::IPv4Protocol)
       {
           //我们使用IPv4地址
           list_ip << address.toString();
           qDebug() <<"My localhost IPv4 address: "<< address.toString();
       }
    }
    return list_ip;

    //return 0;
}
