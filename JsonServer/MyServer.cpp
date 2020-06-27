#include "MyServer.h"
#include <QJsonArray>
#include <QJsonObject>

Server::Server() {};

void Server::startServer()
{
    if (this->listen(QHostAddress::Any, 12345))
    {
        qDebug() << "Listening...";
    }
    else
    {
        qDebug() << "Problems with listening";
    }
}

void Server::incomingConnection(qintptr socketfd)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketfd);

    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

    qDebug() << socketfd << ": Client connected";

    socket->write("You are connected");
    socket->flush();
}

void Server::socketReady()
{
    data = socket->readAll();

    QJsonDocument doc;
    QJsonParseError err;
    doc = QJsonDocument::fromJson(data, &err);

    if (err.errorString().toInt() != QJsonParseError::NoError)
    {
        qDebug() << "Error reading json";
        return;
    }

    QJsonObject obj = doc.object();

    QJsonArray arr = obj["data"].toArray();
    QString rectDescr2 = arr.at(1).toObject().value("rectDescription2").toString();
    QString resultMessage = "{\"result\":\"" + rectDescr2 + "\"}";
    socket->write(resultMessage.toUtf8());
}

void Server::socketDisconnected()
{
    qDebug() << "Disconnected";
    socket->deleteLater();
}




