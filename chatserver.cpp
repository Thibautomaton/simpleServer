#include "chatserver.h"

ChatServer::ChatServer(QObject *parent) : QTcpServer(parent)
{

    //get the signals from the ui to launch server and verifiy if it is listening


}

void ChatServer::incomingConnection(qintptr socketDescriptor)
{
  // This method will get called every time a client tries to connect.
  // We create an object that will take care of the communication with this client
  ServerWorker *worker = new ServerWorker(this);
  // we attempt to bind the worker to the client
  if (!worker->setSocketDescriptor(socketDescriptor)) {
      // if we fail we clean up
      worker->deleteLater();
      return;
  }
  // connect the signals coming from the object that will take care of the
  // communication with this client to the slots in the central server
  connect(worker, &ServerWorker::disconnectedFromClient, this, std::bind(&ChatServer::userDisconnected, this, worker));
  connect(worker, &ServerWorker::error, this, std::bind(&ChatServer::userError, this, worker));
  connect(worker, &ServerWorker::jsonReceived, this, std::bind(&ChatServer::jsonReceived, this, worker, std::placeholders::_1));
  connect(worker, &ServerWorker::logMessage, this, &ChatServer::logMessage);
  // we append the new worker to a list of all the objects that communicate to a single client
  m_clients.append(worker);
  qDebug()<<m_clients.last()->getPeerAdress();
  qDebug()<<m_clients.last()->getPeerPort();
  //connected() signal of TcpSocket will tell us if the connection is succesfully etablished.

  // we log the event
  emit logMessage(QStringLiteral("New client Connected"));
  emit newClientConnected();
}

//void ChatServer::informConnectionStatus(ServerWorker *destination)
//{
//    QJsonObject connectedMessage;
//    connectedMessage[QStringLiteral("type")] = QStringLiteral("newconnection");
//    connectedMessage[QStringLiteral("succes")] = QStringLiteral("true");

//    sendJson(destination, connectedMessage);
//}

void ChatServer::sendJson(ServerWorker *destination, const QJsonObject &message)
{
    Q_ASSERT(destination); // make sure destination is not null
    destination->sendJson(message); // call directly the worker method
}

bool ChatServer::isUserRegistered(const QString user)
{
    for(QMap<QString, QString>::iterator it = m_agreedUsers.begin(); it!=m_agreedUsers.end(); it++)
    {
        if(it.key()==user)
        {
            return true;
        }
    }
    return false;
}

QString ChatServer::findPasswordforUsername(QString userName)
{
    QMap<QString, QString>::const_iterator it = m_agreedUsers.find(userName);
    qDebug()<<it.value();
    return it.value();
}

void ChatServer::broadcast(const QJsonObject &message, ServerWorker *exclude)
{
    // iterate over all the workers that interact with the clients
    for (ServerWorker *worker : m_clients) {
        if (worker == exclude)
            continue; // skip the worker that should be excluded
        sendJson(worker, message); //send the message to the worker
    }
}

void ChatServer::jsonReceived(ServerWorker *sender, const QJsonObject &doc)
{
    Q_ASSERT(sender);
    emit logMessage(QLatin1String("JSON received ") + QString::fromUtf8(QJsonDocument(doc).toJson()));
    if (sender->userName().isEmpty())
        return jsonFromLoggedOut(sender, doc);
    jsonFromLoggedIn(sender, doc);
}

void ChatServer::userDisconnected(ServerWorker *sender)
{
    m_clients.removeAll(sender);
    const QString userName = sender->userName();
    if (!userName.isEmpty()) {
        QJsonObject disconnectedMessage;
        disconnectedMessage[QStringLiteral("type")] = QStringLiteral("userdisconnected");
        disconnectedMessage[QStringLiteral("username")] = userName;
        broadcast(disconnectedMessage, nullptr);
        emit logMessage(userName + QLatin1String(" disconnected"));
    }
    sender->deleteLater();
}

void ChatServer::userError(ServerWorker *sender)
{
    Q_UNUSED(sender)
    emit logMessage(QLatin1String("Error from ") + sender->userName());
}

void ChatServer::toggleStartServer()
{

    //QHostAddress ipAddress = QHostAddress(ip);

    if (this->isListening()) {
        this->stopServer();
        emit serverStopped();
    } else {
        qDebug()<<this->serverAddress().toString();

        if (!this->listen(QHostAddress::Any, 1967)) {
            qDebug()<< tr("Error")+ tr("Unable to start the server");
        }

        qDebug()<<tr("Server Started");
        emit serverStarted();
    }
}


void ChatServer::stopServer()
{
    for (ServerWorker *worker : m_clients) {
        worker->disconnectFromClient();
    }
    close();
}

void ChatServer::jsonFromLoggedOut(ServerWorker *sender, const QJsonObject &docObj)
{
    Q_ASSERT(sender);
    const QJsonValue typeVal = docObj.value(QLatin1String("type"));
    if (typeVal.isNull() || !typeVal.isString())
        return;
    if (typeVal.toString().compare(QLatin1String("login"), Qt::CaseInsensitive) != 0)
        return;
    const QJsonValue usernameVal = docObj.value(QLatin1String("username"));
    const QJsonValue passwordVal = docObj.value(QLatin1String("text"));
    qDebug()<<passwordVal.toString();
    if (usernameVal.isNull() || !usernameVal.isString())
        return;
    const QString newUserName = usernameVal.toString().simplified();
    if (newUserName.isEmpty())
        return;
    for (ServerWorker *worker : qAsConst(m_clients)) {
        if (worker == sender)
            continue;
        if (worker->userName().compare(newUserName, Qt::CaseInsensitive) == 0) {
            QJsonObject message;
            message[QStringLiteral("type")] = QStringLiteral("login");
            message[QStringLiteral("success")] = false;
            message[QStringLiteral("reason")] = QStringLiteral("duplicate username");
            sendJson(sender, message);
            return;
        }
    }
    if(!isUserRegistered(newUserName))
    {
        //adds a condition to return if user name is not on the list of agree users;
        QJsonObject message;
        message[QStringLiteral("type")] = QStringLiteral("login");
        message[QStringLiteral("success")] =false;
        message[QStringLiteral("reason")] = QStringLiteral("user was not registered");
        sendJson(sender, message);
        return;
    }

    else if (findPasswordforUsername(newUserName) != passwordVal.toString())
    {
        //if user is registered we still have to check the password
        qDebug()<<"called";
        QJsonObject message;
        message[QStringLiteral("type")] = QStringLiteral("login");
        message[QStringLiteral("success")] =false;
        message[QStringLiteral("reason")] = QStringLiteral("wrong password");
        sendJson(sender, message);
         return;
     }

    sender->setUserName(newUserName);
    QJsonObject successMessage;
    successMessage[QStringLiteral("type")] = QStringLiteral("login");
    successMessage[QStringLiteral("success")] = true;
    sendJson(sender, successMessage);
    QJsonObject connectedMessage;
    connectedMessage[QStringLiteral("type")] = QStringLiteral("newuser");
    connectedMessage[QStringLiteral("username")] = newUserName;
    broadcast(connectedMessage, sender);

}


void ChatServer::jsonFromLoggedIn(ServerWorker *sender, const QJsonObject &docObj)
{
    Q_ASSERT(sender);
    const QJsonValue typeVal = docObj.value(QLatin1String("type"));
    if (typeVal.isNull() || !typeVal.isString())
        return;
    if(typeVal.toString().compare(QLatin1String("login"), Qt::CaseInsensitive) == 0)
    {
        qDebug()<<"Received Login message";

        //TODO: rajouter les messages qui specifient que le message de login peut fonctionner une fois et la condition dans le worker.
        //peut-être ordonner les workers s'ils sont actuellement connectés ou pas ?
        //send back json with success=true, pour updater le statut du client en logged in
        QString user = sender->userName();
        for(QMap<QString, QString>::iterator it = m_agreedUsers.begin(); it!=  m_agreedUsers.end(); it++)
        {
            if(it.key()==user)
            {
                const QJsonValue textVal = docObj.value(QLatin1String("text"));
                if(it.value()==textVal.toString())
                {
                    qDebug()<<"logged in";
                    emit newUserIdentified(user);
                }
            }
            else {
                qDebug()<<"wrong password entered";
            }
        }
    }
    if (typeVal.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) != 0)
        return;
    const QJsonValue textVal = docObj.value(QLatin1String("text"));
    if (textVal.isNull() || !textVal.isString())
        return;
    const QString text = textVal.toString().trimmed();
    if (text.isEmpty())
        return;
    QJsonObject message;
    message[QStringLiteral("type")] = QStringLiteral("message");
    message[QStringLiteral("text")] = text;
    message[QStringLiteral("sender")] = sender->userName();

    if(message[QStringLiteral("type")]== QStringLiteral("login"))
    {

        exit(-1);
    }

    broadcast(message, sender);
}
