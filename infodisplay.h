#ifndef INFODISPLAY_H
#define INFODISPLAY_H

#include <QObject>
#include "chatserver.h"
#include "haribolist.h"
#include "listconnexion.h"

#include <QQmlListProperty>



class InfoDisplay : public QObject
{
    /**
     * This class takes the other inputs of serer, the buttons, the settings etc
     * */
    Q_OBJECT
public:
    explicit InfoDisplay( ListConnexion *notSignedInUsers, ListConnexion *connectedUsers, QObject *parent = nullptr):notsignedins(notSignedInUsers), connectedusers(connectedUsers) {
        QObject::connect(&server, &ChatServer::newClientConnected, this, &InfoDisplay::hasEstablishedConnexion);
        //connect(&server, &ChatServer::disconnectUserUI,  this, &InfoDisplay::disconnectedUser);

        //for disconnected Connexion, put a timer to delete connexion if user has attempted connexion a long time ago
        connect(&server, &ChatServer::newUserIdentified, this, &InfoDisplay::newConnectedUser);
    };


    //Q_INVOKABLE connexion getUser(int index);
    //Q_INVOKABLE int getConnectedUsersCount();
    //Q_INVOKABLE connexion getNotSignedIn(int index);
    //Q_INVOKABLE  int getNotSignedInUsersCount();
    Q_INVOKABLE void toggleStart();

    /**
    const QQmlListProperty<connexion> &connectedUsers() {
        return QQmlListProperty<connexion>(this, &this->_connectedUsers);
    };

    const QQmlListProperty<connexion> &notSignedInUsers() {
        return QQmlListProperty<connexion>(this, &this->_notSignedInUsers);
    };
    **/
signals:
    void serverStarted();

public slots:
    void hasEstablishedConnexion(QString ip, int port);
    void newConnectedUser(QString ip, int port, QString username);

    //void disconnectedUser(QString username);
    //void disconnectedConnexion(QString ip, int port);

private:
    ListConnexion *notsignedins;
    ListConnexion *connectedusers;

    int numberNotConnected;
    ChatServer server;
    bool online;

signals:

    void connectedUsersChanged();
    void notSignedInUsersChanged();
};

#endif // INFODISPLAY_H
