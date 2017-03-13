#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include "servermanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void clearListInfo();
    bool lostListData();

private slots:
    void parseFinished(const QString &action, const QString &status, const QVector<QVariantHash> &data);

    void on_addList_clicked();
    void on_deleteList_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_header_editingFinished();
    void on_author_editingFinished();
    void on_isNumericList_clicked(bool state);

    void on_addItem_clicked();
    void on_deleteItem_clicked();
    void on_tableWidget_cellChanged(int row, int column);
    void on_itemUp_clicked();
    void on_itemDown_clicked();

private:
    Ui::MainWindow *ui;
    ServerManager *serverManager = new ServerManager(this);

    QVector<QVariantHash> todoLists;
    QVector<QVariantHash> listItems;
    int currentIndex = -1;

    const QString default_date = "2000-01-01T00:00:00.000000";
};

#endif // MAINWINDOW_H
