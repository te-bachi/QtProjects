#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include "itemmodel.h"
#include "tablemodel.h"

#include "measurementdata.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    private:
        Ui::MainWindow *ui;
        ItemModel *itemModel;
        TableModel *tableModelInbox;
        TableModel *tableModelList;

        QList<MeasurementData> m_dataInbox;
        QList<MeasurementData> m_dataList;

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        void setupTableView(QTableView *tableView);

    public slots:
        void onLeftAdd();
        void onLeftRemove();
        void onRightAdd();
        void onRightRemove();

};

#endif // MAINWINDOW_H
