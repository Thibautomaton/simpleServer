#ifndef LISTCONNEXION_H
#define LISTCONNEXION_H

#include <QObject>
#include <QVector>
#include <QAbstractListModel>

struct connexion {
    Q_GADGET
public:
    connexion(): ip(""),port(0), username("anonyme") {};
    connexion(QString ip_input, int port_input, QString username = "anonyme"): ip(ip_input), port(port_input), username(username) {
    };
    bool operator==(const connexion& a) const {
        return ip==a.ip && port==a.port;
    }

    QString ip;

    int port;
    QString username;

    Q_PROPERTY(QString ip MEMBER ip)
    Q_PROPERTY(int port MEMBER port)
    Q_PROPERTY(QString username MEMBER username)
};
Q_DECLARE_METATYPE(connexion)

class ListConnexion : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        SignedIn,
        NotSignedIn
    };
    explicit ListConnexion(QObject *parent = nullptr);

        // QAbstractItemModel interface
    public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    //Q_INVOKABLE connexion get(int index) const;

public slots:
    void insert(connexion* item);
    void remove(connexion* item);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<connexion*> mConnexions;

};

#endif // LISTCONNEXION_H
