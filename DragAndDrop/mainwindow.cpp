#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MeasurementData a = MeasurementData(QDateTime::fromString("12:55:13.123", "HH:mm:ss.zzz"), 100.0f, QString("Test 1"));
    MeasurementData b = MeasurementData(QDateTime::fromString("12:55:14.156", "HH:mm:ss.zzz"), 101.4f, QString("Test 2"));
    MeasurementData c = MeasurementData(QDateTime::fromString("12:55:15.023", "HH:mm:ss.zzz"), 99.05f, QString("Test 3"));
    MeasurementData d = MeasurementData(QDateTime::fromString("12:55:16.835", "HH:mm:ss.zzz"), 104.9f, QString("Test 4"));

    m_dataInbox.append(a);
    m_dataInbox.append(b);
    m_dataInbox.append(c);
    m_dataInbox.append(d);

    //itemModel = new ItemModel(this);
    tableModelInbox = new TableModel(m_dataInbox, this);
    tableModelList  = new TableModel(m_dataList, this);
    ui->inbox->setModel(tableModelInbox);
    ui->list->setModel(tableModelList);
    setupTableView(ui->inbox);
    setupTableView(ui->list);

    //connect(ui->inbox->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
    //        SLOT(onSelectionChanged(const QItemSelection &, const QItemSelection &)));
    connect(ui->inbox->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
            SLOT(onCurrentRowChanged(const QModelIndex &, const QModelIndex &)));

}

void
MainWindow::setupTableView(QTableView *tableView)
{
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->verticalHeader()->hide();
    tableView->setDragEnabled(true);
    tableView->setAcceptDrops(true);
    tableView->setDropIndicatorShown(true);

    /* void QAbstractItemViewPrivate::clearOrRemove() */
    /* overwrite */
    tableView->setDragDropOverwriteMode(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::onLeftAdd()
{
    qDebug() << "onLeftAdd";
}

void
MainWindow::onLeftRemove()
{
    qDebug() << "onLeftRemove";
}

void
MainWindow::onRightAdd()
{
    qDebug() << "onRightAdd";
}

void
MainWindow::onRightRemove()
{
    qDebug() << "onRightRemove";
}

void
MainWindow::onCellActivate(const QModelIndex &index)
{
    qDebug() << "row=" << index.row() << " column=" << index.column();
}

void
MainWindow::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    if (!selected.indexes().isEmpty())
        qDebug() << "onSelectionChanged: row=" << selected.indexes().first().row() << " column=" << selected.indexes().first().column();
}

void
MainWindow::onCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    qDebug() << "onCurrentRowChanged: row=" << current.row() << " column=" << current.column();
}
