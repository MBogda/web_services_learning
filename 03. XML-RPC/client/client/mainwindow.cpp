#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(serverManager, SIGNAL(responseGetListsFinished(QVector<QVariantHash>)),
            this, SLOT(responseGetListsFinished(QVector<QVariantHash>)));
    connect(serverManager, SIGNAL(responseGetItemsFinished(QVector<QVariantHash>)),
            this, SLOT(responseGetItemsFinished(QVector<QVariantHash>)));
    connect(serverManager, SIGNAL(faultResponseFinished(int,QString)),
            this, SLOT(faultResponseFinished(int,QString)));
    serverManager->getLists();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearListInfo()
{
    listItems.clear();
    currentIndex = -1;

    ui->header->clear();
    ui->created->setDateTime(QDateTime::fromString(default_date, Qt::ISODate));
    ui->author->clear();
    ui->isNumericList->setChecked(false);
    ui->tableWidget->setRowCount(0);
}

void MainWindow::responseGetListsFinished(const QVector<QVariantHash> &data)
{
    todoLists = data;
    ui->listWidget->clear();
    for (auto todoList : todoLists) {
        ui->listWidget->addItem(todoList["header"].toString());
    }
    if (currentIndex != -1) {
        ui->listWidget->setCurrentRow(currentIndex);
        on_listWidget_itemClicked(ui->listWidget->item(currentIndex));
    }
}

void MainWindow::responseGetItemsFinished(const QVector<QVariantHash> &data)
{
    listItems = data;
    ui->tableWidget->setRowCount(listItems.size());
    int index = 0;
    for (auto listItem : listItems) {
        QTableWidgetItem *item0 = new QTableWidgetItem(listItem["body"].toString());
        QTableWidgetItem *item1 = new QTableWidgetItem();
        if (listItem["status"].toInt() == 1)
            item1->setCheckState(Qt::Checked);
        else if (listItem["status"].toInt() == 0)
            item1->setCheckState(Qt::Unchecked);
        ui->tableWidget->setItem(index, 0, item0);
        ui->tableWidget->setItem(index, 1, item1);
        ++index;
    }

    QStringList labels;
    if (todoLists[currentIndex]["type"].toInt() == 1) {
        for (int i = 0; i < listItems.size(); ++i) {
            labels.push_back(QString::number(i + 1));
        }
    } else {
        for (auto item : listItems) {
            labels.push_back("⚫");
        }
    }
    ui->tableWidget->setVerticalHeaderLabels(labels);
}

void MainWindow::faultResponseFinished(int error, const QString &message)
{
    QMessageBox::warning(this, tr("Warning"),
                         QString("An error occurred, code: %1, message: '%2'.\nPlease try again.").arg(
                             QString::number(error), message));
}

void MainWindow::on_addList_clicked()
{
    bool ok;
    QString list_header = QInputDialog::getText(this, tr("Create to-do list"),
                                         tr("List header:"), QLineEdit::Normal,
                                         tr("List header"), &ok);
    if (ok) {
        serverManager->addList(list_header);
        currentIndex = todoLists.size();
        serverManager->getLists();
    }
}

void MainWindow::on_deleteList_clicked()
{
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();
    if (items.empty()) {
        QMessageBox::information(this, tr("Information"), tr("You didn't select any to-do lists!"));
    } else {
        auto ans = QMessageBox::question(this, tr("Confirm"),
                    tr("Do you want to delete selected to-do lists?"));
        if (ans == QMessageBox::Yes) {
            for (auto item : items) {
                int index = item->listWidget()->row(item);
                serverManager->deleteList(todoLists[index]["list_id"].toInt());
            }
            clearListInfo();
            serverManager->getLists();
        }
    }
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    currentIndex = item->listWidget()->row(item);
    auto todoList = todoLists[currentIndex];
    serverManager->getItems(todoList["list_id"].toInt());

    ui->header->setText(todoList["header"].toString());
//    ui->created->setDateTime(QDateTime::fromString(todoList["created"].toString(), Qt::ISODate));
    ui->created->setDateTime(todoList["created"].toDateTime());
    ui->author->setText(todoList["author_name"].toString());
    if (todoList["type"].toInt() == 1)
        ui->isNumericList->setCheckState(Qt::Checked);
    else
        ui->isNumericList->setCheckState(Qt::Unchecked);
}

void MainWindow::on_header_editingFinished()
{
    QString newHeader = ui->header->text();
    if (currentIndex != -1 && todoLists[currentIndex]["header"] != newHeader) {
        QVariantHash params;
        params["header"] = newHeader;
        serverManager->updateList(todoLists[currentIndex]["list_id"].toInt(), params);
        serverManager->getLists();
    }
}

void MainWindow::on_author_editingFinished()
{
    QString newAuthor = ui->author->text();
    if (currentIndex != -1 && todoLists[currentIndex]["author_name"] != newAuthor) {
        QVariantHash params;
        params["author_name"] = newAuthor;
        serverManager->updateList(todoLists[currentIndex]["list_id"].toInt(), params);
        serverManager->getLists();
    }
}

void MainWindow::on_isNumericList_clicked(bool state)
{
    int newType;
    if (state) {
        newType = 1;
    } else {
        newType = 0;
    }
    if (currentIndex != -1 && todoLists[currentIndex]["type"] != newType) {
        QVariantHash params;
        params["type"] = newType;
        serverManager->updateList(todoLists[currentIndex]["list_id"].toInt(), params);
        serverManager->getLists();
    }
}

void MainWindow::on_addItem_clicked()
{
    if (currentIndex == -1) {
        QMessageBox::information(this, tr("Information"), tr("You didn't select any todo list!"));
    } else {
        bool ok;
        QString item_body = QInputDialog::getText(this, tr("Add list item"),
                                             tr("Item body:"), QLineEdit::Normal,
                                             tr("Item body"), &ok);
        if (ok) {
            serverManager->addItem(todoLists[currentIndex]["list_id"].toInt(), item_body);
            serverManager->getItems(todoLists[currentIndex]["list_id"].toInt());
        }
    }
}

void MainWindow::on_deleteItem_clicked()
{
    if (currentIndex == -1) {
        QMessageBox::information(this, tr("Information"), tr("You didn't select any todo list!"));
    } else {
        QModelIndexList indexes =  ui->tableWidget->selectionModel()->selectedRows();
        if (indexes.empty()) {
            QMessageBox::information(this, tr("Information"), tr("You didn't select any items!"));
        } else {
            QMessageBox::StandardButton ans = QMessageBox::question(this, tr("Confirm"),
                                                            tr("Do you want to delete selected items?"));
            if (ans == QMessageBox::Yes) {
                for (int i = indexes.count() - 1; i >= 0; i--) {
                    serverManager->deleteItem(listItems[indexes.at(i).row()]["item_id"].toInt());
                }
                serverManager->getItems(todoLists[currentIndex]["list_id"].toInt());
            }
        }
    }
}

void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    if (column == 0) {
        QString newBody = ui->tableWidget->item(row, column)->text();
        if (listItems[row]["body"] != newBody) {
            QVariantHash params;
            params["body"] = newBody;
            serverManager->updateItem(listItems[row]["item_id"].toInt(), params);
            serverManager->getItems(todoLists[currentIndex]["list_id"].toInt());
        }
    } else if (column == 1) {
        auto state = ui->tableWidget->item(row, column)->checkState();
        QString newStatus;
        if (state == Qt::Checked) {
            newStatus = "1";

            auto item = ui->tableWidget->item(row, column - 1);
            QFont font = item->font();
            font.setStrikeOut(true);
            font.setItalic(true);
            item->setFont(font);

            item = ui->tableWidget->item(row, column);
            font = item->font();
            font.setItalic(true);
            item->setFont(font);
            item->setText(tr("Done!"));
        } else {
            newStatus = "0";

            auto item = ui->tableWidget->item(row, column - 1);
            QFont font = item->font();
            font.setStrikeOut(false);
            font.setItalic(false);
            item->setFont(font);

            item = ui->tableWidget->item(row, column);
            font = item->font();
            font.setItalic(true);
            item->setFont(font);
            item->setText(tr("In process..."));
        }
        if (listItems[row]["status"] != newStatus) {
            QVariantHash params;
            params["status"] = newStatus;
            serverManager->updateItem(listItems[row]["item_id"].toInt(), params);
            serverManager->getItems(todoLists[currentIndex]["list_id"].toInt());
        }
    }
}

void MainWindow::on_itemUp_clicked()
{
    QModelIndexList indexes = ui->tableWidget->selectionModel()->selectedRows();
    if (indexes.empty()) {
        QMessageBox::information(this, tr("Information"), tr("You didn't select any items!"));
    } else {
        QVector<int> sortedRows;
        for (auto index: indexes) {
            sortedRows.append(index.row());
        }
        std::sort(sortedRows.begin(), sortedRows.end());

        QVector<QVariant> newNumbers(listItems.size());
        for (int i = 0; i < listItems.size(); ++i)
            newNumbers[i] = listItems[i]["number"];

        // change selection
        for (auto row : sortedRows) {
            if (row != 0) {
                int higher = row - 1;
                int lower = row;
                // bad code =(
                ui->tableWidget->item(higher, 0)->setSelected(true);
                ui->tableWidget->item(higher, 1)->setSelected(true);
                ui->tableWidget->item(lower, 0)->setSelected(false);
                ui->tableWidget->item(lower, 1)->setSelected(false);
            }
            else return;
        }

        std::reverse(sortedRows.begin(), sortedRows.end());

        // change numbers
        for (auto row : sortedRows) {
            if (row != 0) {
                int higher = row - 1;
                int lower = row;
                newNumbers[higher].swap(newNumbers[lower]);
            }
            else return;
        }

        QVariantHash params;
        for (int i = 0; i < listItems.size(); ++i) {
            if (newNumbers[i] != listItems[i]["number"]) {
                params["number"] = newNumbers[i];
                serverManager->updateItem(listItems[i]["item_id"].toInt(), params);
            }
        }
        serverManager->getItems(todoLists[currentIndex]["list_id"].toInt());
    }
}

void MainWindow::on_itemDown_clicked()
{
    QModelIndexList indexes = ui->tableWidget->selectionModel()->selectedRows();
    if (indexes.empty()) {
        QMessageBox::information(this, tr("Information"), tr("You didn't select any items!"));
    } else {
        QVector<int> sortedRows;
        for (auto index: indexes) {
            sortedRows.append(index.row());
        }
        std::sort(sortedRows.begin(), sortedRows.end());

        QVector<QVariant> newNumbers(listItems.size());
        for (int i = 0; i < listItems.size(); ++i)
            newNumbers[i] = listItems[i]["number"];

        // change numbers
        for (auto row : sortedRows) {
            if (row != listItems.size() - 1) {
                int higher = row;
                int lower = row + 1;
                newNumbers[higher].swap(newNumbers[lower]);
            }
            else return;
        }

        std::reverse(sortedRows.begin(), sortedRows.end());

        // change selection
        for (auto row : sortedRows) {
            if (row != listItems.size() - 1) {
                int higher = row;
                int lower = row + 1;
                // bad code =(
                ui->tableWidget->item(higher, 0)->setSelected(false);
                ui->tableWidget->item(higher, 1)->setSelected(false);
                ui->tableWidget->item(lower, 0)->setSelected(true);
                ui->tableWidget->item(lower, 1)->setSelected(true);
            }
            else return;
        }

        QVariantHash params;
        for (int i = 0; i < listItems.size(); ++i) {
            if (newNumbers[i] != listItems[i]["number"]) {
                params["number"] = newNumbers[i];
                serverManager->updateItem(listItems[i]["item_id"].toInt(), params);
            }
        }
        serverManager->getItems(todoLists[currentIndex]["list_id"].toInt());
    }
}
