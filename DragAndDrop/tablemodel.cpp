#include "tablemodel.h"

#include <QDebug>
#include <QFloat16>

TableModel::TableModel(QList<MeasurementData> measurementList, QObject *parent) :
    QAbstractTableModel(parent),
    m_measurementList(measurementList)
{
    //
}

int
TableModel::rowCount(const QModelIndex & /*parent*/) const
{
   return m_measurementList.size();
}

int
TableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 3;
}

QVariant
TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("first");
        case 1:
            return QString("second");
        case 2:
            return QString("third");
        }
    }
    if (role == Qt::DisplayRole && orientation == Qt::Vertical) {
        switch (section) {
        case 0:
            return QString("A");
        case 1:
            return QString("B");
        case 2:
            return QString("C");
        }
    }
    return QVariant();
}

QVariant
TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    int col = index.column();

    //qDebug() << row << "/" << col;
    if (row >= m_measurementList.size()) {
        qDebug() << "Should not happen!!";

        return QVariant();
    }

    switch (role) {
        case Qt::DisplayRole:
        {
            MeasurementData data = m_measurementList.at(row);
            //qDebug() << data->m_text;
            switch (col) {
                case 0:
                    return data.m_time.toString("HH:mm:ss.zzz");
                case 1:
                    return QString::number(static_cast<double>(data.m_value));
                case 2:
                    return data.m_text;
            }
        }

    }

    return QVariant();
}

Qt::ItemFlags
TableModel::flags(const QModelIndex & index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

MeasurementData
TableModel::itemForIndex(const QModelIndex &index) const
{
    return m_measurementList.at(index.row());
}


/*****************************************************************************/


bool
TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();

        auto measurement = m_measurementList.value(row);

        if (index.column() == 0)
            measurement.m_time = value.toDateTime();
        else if (index.column() == 1)
            measurement.m_value = value.toFloat();
        else if (index.column() == 2)
            measurement.m_text = value.toString();
        else
            return false;

        m_measurementList.replace(row, measurement);
        emit dataChanged(index, index, {role});

        return true;
    } else if (index.isValid() && role == Qt::DisplayRole) {
            qDebug() << "setData(row=" << index.row() << " col=" << index.column() << " Qt::DisplayRole)";
            //if (index.column() == 0) {
            //    removeRows(index.row(), 1, QModelIndex());
            //}
            //return true;
    }
    qDebug() << "size=" << m_measurementList.size();
    return false;
}

bool
TableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        m_measurementList.insert(position, { QDateTime(), 0.0f, QString() });

    endInsertRows();
    return true;
}

bool
TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), row, row + count - 1);

    qDebug() << "size=" << m_measurementList.size();
    for (int row = 0; row < m_measurementList.size(); ++row)
        qDebug() << count << "/" << row << " " << m_measurementList.value(row).m_text;

    for (int r = 0; r < count; ++r)
        m_measurementList.removeAt(row);

    endRemoveRows();
    return true;
}

bool
TableModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    Q_UNUSED(column);
    Q_UNUSED(count);
    Q_UNUSED(parent);

    return true;
}

/*****************************************************************************/

const QString MimeType = "application/vnd.qtrac.xml.task.z";

QStringList
TableModel::mimeTypes() const
{
    return QStringList() << MimeType;
}

const QString MeasurementTag("MEASUREMENT");
const QString TimeAttribute("TIME");
const QString ValueAttribute("VALUE");
const QString TextAttribute("TEXT");

const int MaxCompression = 9;

QMimeData *
TableModel::mimeData(const QModelIndexList &indexes) const
{
    Q_ASSERT(indexes.count());
    //if (indexes.count() != 1) {
    //    return 0;
    //}
    MeasurementData data = itemForIndex(indexes.at(0));
    QMimeData *mimeData = new QMimeData;
    QByteArray xmlData;
    QXmlStreamWriter writer(&xmlData);
    writer.writeStartElement(MeasurementTag);
    writer.writeAttribute(TimeAttribute, data.m_time.toString("HH:mm:ss.zzz"));
    writer.writeAttribute(ValueAttribute, QString::number(static_cast<double>(data.m_value)));
    writer.writeAttribute(TextAttribute, data.m_text);
    writer.writeEndElement();
    mimeData->setData(MimeType, qCompress(xmlData, MaxCompression));
    return mimeData;
}

bool
TableModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(data);
    Q_UNUSED(action);
    Q_UNUSED(row);
    //Q_UNUSED(column);
    Q_UNUSED(parent);

    if (column > 0) {
        return false;
    }

    return true;
}

bool
TableModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(action);
    Q_UNUSED(column);
    Q_UNUSED(parent);

    qDebug() << "action=" << action << " row=" << row << " col=" << column;

    if (action == Qt::IgnoreAction)
         return true;

    int beginRow = rowCount(QModelIndex());
    int rows = 1;

    QByteArray xmlData = qUncompress(data->data(MimeType));
    QXmlStreamReader reader(xmlData);

    insertRows(beginRow, rows, QModelIndex());
    MeasurementData m;
    readTasks(&reader, &m);

    QModelIndex idx = index(beginRow, 0, QModelIndex());
    setData(idx, QVariant(m.m_time));
    idx = index(beginRow, 1, QModelIndex());
    setData(idx, QVariant(m.m_value));
    idx = index(beginRow, 2, QModelIndex());
    setData(idx, QVariant(m.m_text));

    return true;
}

Qt::DropActions
TableModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::DropActions
TableModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

void TableModel::readTasks(QXmlStreamReader *reader, MeasurementData *m)
{
    while (!reader->atEnd()) {
        reader->readNext();
        if (reader->isStartElement()) {
            if (reader->name() == MeasurementTag) {
                m->m_time  = QDateTime::fromString(reader->attributes().value(TimeAttribute).toString(), "HH:mm:ss.zzz");
                m->m_value = reader->attributes().value(ValueAttribute).toString().toFloat();
                m->m_text = reader->attributes().value(TextAttribute).toString();
            }
        }
    }
}
