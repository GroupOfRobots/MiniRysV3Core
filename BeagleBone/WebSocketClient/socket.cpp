#include "socket.h"
#include <QtCore/QDebug>

QT_USE_NAMESPACE

irys::Communication::Communication(QObject *parent) :
    QObject(parent)
{
    qDebug() << "WebSocket server:" << HOST_SERVER_URL;
    connect(&webSocket, &QWebSocket::connected, this, &Communication::onConnected);
    connect(&webSocket, &QWebSocket::disconnected, this, &Communication::closed);
    this->connect();
}


void irys::Communication::connect() {
    webSocket.open(HOST_SERVER_URL);
}

bool irys::Communication::isConnected() {
    webSocket.isValid();
}

void irys::Communication::onConnected()
{
    connect(&webSocket, &QWebSocket::textMessageReceived,
            this, &Communication::onTextMessageReceived);
    connect(&webSocket, &QWebSocket::binaryMessageReceived,
            this, &Communication::onBinaryMessageReceived);
    qDebug() << "WebSocket connected";
}

void irys::Communication::onTextMessageReceived(QString message)
{
    qDebug() << "Message received:" << message;
}

void irys::Communication::onBinaryMessageReceived(QByteArray data)
{
    RobotStatus *rs = reinterpret_cast<RobotStatus *>(data.data());
    receivedRobotStatuses.enqueue(*rs);
}


bool irys::Communication::isNewRobotStatusToGet() const {
    return receivedRobotStatuses.size();
}

irys::RobotStatus irys::Communication::getReceivedRobotStatus() {
    return receivedRobotStatuses.dequeue();
}
