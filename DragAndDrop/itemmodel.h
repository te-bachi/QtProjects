#ifndef INBOXMODEL_H
#define INBOXMODEL_H

#include <QObject>
#include <QStandardItemModel>

class ItemModel : public QStandardItemModel
{
    public:
        explicit ItemModel(QObject *parent = nullptr);

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags (const QModelIndex & index) const override;
};

#endif // INBOXMODEL_H
