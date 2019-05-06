#include "itemmodel.h"

#include <QDebug>

ItemModel::ItemModel(QObject *parent) : QStandardItemModel(parent)
{
    //
}

int
ItemModel::rowCount(const QModelIndex & /*parent*/) const
{
   return 2;
}

int
ItemModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 3;
}

QVariant
ItemModel::headerData(int section, Qt::Orientation orientation, int role) const
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
ItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= 2 || index.row() < 0)
        return QVariant();

    int row = index.row();
    int col = index.column();
    // generate a log message when this method gets called
    qDebug() << QString("row %1, col%2, role %3").arg(row).arg(col).arg(role);

    switch (role) {
    case Qt::DisplayRole:
        if (row == 0 && col == 1) return QString("<--left");
        if (row == 1 && col == 1) return QString("right-->");

        return QString("%1, %2")
                .arg(row + 1)
                .arg(col +1);
    case Qt::FontRole:
        if (row == 0 && col == 0) { //change font only for cell(0,0)
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        }
        break;
    case Qt::BackgroundRole:
        if (row == 1 && col == 2)  //change background only for cell(1,2)
            return QBrush(Qt::red);
        break;
    case Qt::TextAlignmentRole:
        if (row == 1 && col == 1) //change text alignment only for cell(1,1)
            return Qt::AlignRight + Qt::AlignVCenter;
        break;
    case Qt::CheckStateRole:
        if (row == 1 && col == 0) //add a checkbox to cell(1,0)
            return Qt::Checked;
        break;
    }

    return QVariant();
}

Qt::ItemFlags
ItemModel::flags(const QModelIndex & index) const
{
    Q_UNUSED(index);
    //return Qt::ItemIsEnabled;
    return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
}
