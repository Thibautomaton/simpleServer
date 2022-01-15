#ifndef HARIBOLIST_H
#define HARIBOLIST_H

#include <QObject>
#include <QAbstractListModel>

class HariboList : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit HariboList(QObject *parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

signals:

};

#endif // HARIBOLIST_H
