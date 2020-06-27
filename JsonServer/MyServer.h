#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();

    QTcpSocket* socket;
    QByteArray data;

public slots:
    void incomingConnection(qintptr socketfd) override;
    void startServer();
    void socketReady();
    void socketDisconnected();
};

#endif // MYSERVER_H
