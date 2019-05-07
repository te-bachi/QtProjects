#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QList>
#include <QMimeData>

#include <QXmlStreamWriter>

#include "measurementdata.h"

class TableModel : public QAbstractTableModel
{

    private:
        QList<MeasurementData> m_measurementList;

    public:
        explicit TableModel(QList<MeasurementData> measurementList, QObject *parent = nullptr);

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex & index) const override;

        bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
        bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
        bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
        bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

        QStringList mimeTypes() const override;
        QMimeData *mimeData(const QModelIndexList &indexes) const override;
        bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
        bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
        Qt::DropActions supportedDropActions() const override;
        Qt::DropActions supportedDragActions() const override;

        MeasurementData itemForIndex(const QModelIndex &index) const;
        void readTasks(QXmlStreamReader *reader, MeasurementData *task);
};

#endif // TABLEMODEL_H
