#include "listconnexion.h"
#include <QDebug>

ListConnexion::ListConnexion(QObject *parent) : QAbstractListModel(parent)
{

}

int ListConnexion::rowCount(const QModelIndex &parent) const
{
    return mConnexions.size();
}

QVariant ListConnexion::data(const QModelIndex &index, int role) const
{
    connexion * item = mConnexions.at(index.row());

    if(role==NotSignedIn)
    {
        QString output = "anonyme on ip :" + item->ip.mid(5, item->ip.size()) + "  and port :" + QString::number(item->port);
        return QVariant::fromValue(output);
    }
    else if(role==SignedIn)
    {
        QString output = item->username +"is connected on ip :" +  item->ip.mid(5, item->ip.size()) + " and port :" + QString::number(item->port);
        return QVariant::fromValue(output);
    }

    //instead of QVariant we could have a QVariantMap, trnafering the data of the connexion in a dictionary
    //we would then be able to display the informations correctly
}


void ListConnexion::insert(connexion *item)
{
    beginInsertRows(QModelIndex(), 0, 0);
    mConnexions.push_front(item);
    endInsertRows();
}

void ListConnexion::remove(connexion *item)
{
    for(int i=0; i<mConnexions.size(); i++)
    {
        if(*mConnexions[i]==*item)
        {
            beginRemoveRows(QModelIndex(), i, i);
            mConnexions.removeAt(i);
            endRemoveRows();
            break;
        }
    }
}

QHash<int, QByteArray> ListConnexion::roleNames() const
{
    return {
        { NotSignedIn, "notSignedIn" },
        {SignedIn, "signedIn"}
    };

}
