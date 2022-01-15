#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QTcpServer>
#include "serverworker.h"
#include <map>

class ChatServer : public QTcpServer
{
    Q_OBJECT
    //Q_DISABLE_COPY(ChatServer)
public:
    explicit ChatServer(QObject *parent = nullptr);
    Q_INVOKABLE void toggleStartServer();
protected:
    void incomingConnection(qintptr socketDescriptor) override;
    void informConnectionStatus(ServerWorker *destination);
signals:
    void logMessage(const QString &msg);
    void serverStarted();
    void serverStopped();
    void newClientConnected(QString ip, int port);
    void newUserIdentified(QString ip, int port, QString username);
    void disconnectUserUI(QString username);

public slots:
    void stopServer();
private slots:
    void broadcast(const QJsonObject &message, ServerWorker *exclude);
    void jsonReceived(ServerWorker *sender, const QJsonObject &doc);
    void userDisconnected(ServerWorker *sender);
    void userError(ServerWorker *sender);

private:
    void jsonFromLoggedOut(ServerWorker *sender, const QJsonObject &doc);
    void jsonFromLoggedIn(ServerWorker *sender, const QJsonObject &doc);
    void sendJson(ServerWorker *destination, const QJsonObject &message);
    bool isUserRegistered(const QString user);
    QString findPasswordforUsername(QString userName);
    QVector<ServerWorker *> m_clients;
    QString currentIp;
    QString currentPort;
    QMap<QString, QString> m_agreedUsers = {
        {"user1", "haribo"},
        {"user2", "mistermagic"},
    };
};


#endif // CHATSERVER_H
