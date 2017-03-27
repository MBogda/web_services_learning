#include "servermanager.h"

ServerManager::ServerManager(QObject *parent) : QObject(parent) {}

void ServerManager::sendRequest(const QString &methodName, const QVariantList &args, const char *resposeSlot)
{
    client->call(methodName, args, this, resposeSlot, this, SLOT(faultResponse(int,QString)));
}

QVector<QVariantHash> ServerManager::parse(QVariant &data) {
    QVector<QVariantHash> new_data;
    for (auto elem : data.toList()) {
        QVariantMap new_elem = elem.toMap();
        QVariantHash appended_elem;
        auto it = new_elem.begin();
        while (it != new_elem.end()) {
            appended_elem[it.key()] = it.value();
            ++it;
        }
        new_data.append(appended_elem);
    }
    return new_data;
}

void ServerManager::responseGetLists(QVariant &data)
{
    responseGetListsFinished(parse(data));
}

void ServerManager::responseGetItems(QVariant &data)
{
    responseGetItemsFinished(parse(data));
}

void ServerManager::responseMethod(QVariant &data) {}

void ServerManager::faultResponse(int error, const QString &message) {
    qDebug() << "An Error occoured, Code: " << error << " Message: " << message;
    faultResponseFinished(error, message);
}

void ServerManager::getLists()
{
    sendRequest("get_all_lists", QVariantList(), SLOT(responseGetLists(QVariant&)));
}

void ServerManager::addList(QString header, QString authorName, int type)
{
    QVariantList args;
    args << header << authorName << type;
    sendRequest("add_list", args, SLOT(responseMethod(QVariant&)));
}

void ServerManager::updateList(int listId, QVariantHash params)
{
    QVariantList args;
    args << listId;
    if (params.find("header") != params.end())
        args << params["header"];
    else
        args << QVariant();
    if (params.find("author_name") != params.end())
        args << params["author_name"];
    else
        args << QVariant();
    if (params.find("type") != params.end())
        args << params["type"];
    else
        args << QVariant();
    sendRequest("update_list", args, SLOT(responseMethod(QVariant&)));
}

void ServerManager::deleteList(int listId)
{
    QVariantList args;
    args << listId;
    sendRequest("delete_list", args, SLOT(responseMethod(QVariant&)));
}

void ServerManager::getItems(int listId)
{
    QVariantList args;
    args << listId;
    sendRequest("get_list_items", args, SLOT(responseGetItems(QVariant&)));
}

void ServerManager::addItem(int listId, QString body, int number, int status)
{
    QVariantList args;
    args << listId << body;
    if (number != 0)
        args << number;
    else
        args << QVariant();
    args << status;
    sendRequest("add_item", args, SLOT(responseMethod(QVariant&)));
}

void ServerManager::updateItem(int itemId, QVariantHash params)
{
    QVariantList args;
    args << itemId;
    if (params.find("list_id") != params.end())
        args << params["list_id"];
    else
        args << QVariant();
    if (params.find("body") != params.end())
        args << params["body"];
    else
        args << QVariant();
    if (params.find("number") != params.end())
        args << params["number"];
    else
        args << QVariant();
    if (params.find("status") != params.end())
        args << params["status"];
    else
        args << QVariant();
    sendRequest("update_item", args, SLOT(responseMethod(QVariant&)));
}

void ServerManager::deleteItem(int itemId)
{
    QVariantList args;
    args << itemId;
    sendRequest("delete_item", args, SLOT(responseMethod(QVariant&)));
}
