/*#ifndef SOCKET_H
#define SOCKET_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QUrl>
#include <QQueue>
#include "robotstatus.h"

namespace irys {

class Communication : public QObject
{
    Q_OBJECT
    //TODO: add host IP address
    const QUrl HOST_SERVER_URL = "ws://192.168.0.xxx:8888";
public:
    explicit Communication(const QUrl &url, bool debug = false, QObject *parent = Q_NULLPTR);

    void sendRobotStatus(RobotStatus &rs);
    bool isNewRobotStatusToGet() const;
    RobotStatus getReceivedRobotStatus();

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onBinaryMessageReceived(QByteArray data);

private:
    QWebSocket webSocket;
    QQueue<RobotStatus> receivedRobotStatuses;
};

} // namespace irys

#endif // SOCKET_H*/
