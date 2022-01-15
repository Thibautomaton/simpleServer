#include "infodisplay.h"
#include <QObject>

#include <QList>



void InfoDisplay::toggleStart()
{
    server.toggleStartServer();
    online = true;
    emit serverStarted();
}

void InfoDisplay::hasEstablishedConnexion(QString ip, int port)
{
    //connexion *connected = new connexion(ip, port);
    notsignedins->insert(new connexion(ip, port));
    this->notSignedInUsersChanged();
    qDebug()<<"new user connected";
}

void InfoDisplay::newConnectedUser(QString ip, int port, QString username)
{
    //connexion *signedIn = new connexion(ip, port, username);
    connectedusers->insert(new connexion(ip, port, username));
    this->connectedUsersChanged();

    //remove the notsignedIn user from the list
    notsignedins->remove(new connexion(ip, port));
    this->notSignedInUsersChanged();
}

/**
void InfoDisplay::disconnectedUser(QString username)
{
    for(QList<connexion*>::iterator it= _connectedUsers.begin(); it!=_connectedUsers.end(); ++it)
    {
        if((*it)->username==username)
        {
            _connectedUsers.erase(it);
        }
    }
}

void InfoDisplay::disconnectedConnexion(QString ip, int port)
{
    for(QList<connexion*>::iterator it= _notSignedInUsers.begin(); it!=_notSignedInUsers.end(); ++it)
    {
        if((*it)->ip==ip && (*it)->port==port)
        {
            _connectedUsers.erase(it);
        }
    }
}
**/
