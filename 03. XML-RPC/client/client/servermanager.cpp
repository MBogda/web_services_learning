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

void ServerManager::addList(const QVariantHash &args)
{
    QVariantList args_list;
    if (args.find("header") != args.end())
        args_list << args["header"];
    else
        args_list << QVariant();
    if (args.find("author_name") != args.end())
        args_list << args["author_name"];
    else
        args_list << QVariant();
    if (args.find("type") != args.end())
        args_list << args["type"];
    else
        args_list << QVariant();
    sendRequest("add_list", args_list, SLOT(responseMethod(QVariant&)));
}

void ServerManager::updateList(int listId, const QVariantHash &args)
{
    QVariantList args_list;
    args_list << listId;
    if (args.find("header") != args.end())
        args_list << args["header"];
    else
        args_list << QVariant();
    if (args.find("author_name") != args.end())
        args_list << args["author_name"];
    else
        args_list << QVariant();
    if (args.find("type") != args.end())
        args_list << args["type"];
    else
        args_list << QVariant();
    sendRequest("update_list", args_list, SLOT(responseMethod(QVariant&)));
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

void ServerManager::addItem(int listId, const QVariantHash &args)
{
    QVariantList args_list;
    args_list << listId;
    if (args.find("body") != args.end())
        args_list << args["body"];
    else
        args_list << QVariant();
    if (args.find("number") != args.end())
        args_list << args["number"];
    else
        args_list << QVariant();
    if (args.find("status") != args.end())
        args_list << args["status"];
    else
        args_list << QVariant();
    sendRequest("add_item", args_list, SLOT(responseMethod(QVariant&)));
}

void ServerManager::updateItem(int itemId, const QVariantHash &args)
{
    QVariantList args_list;
    args_list << itemId;
    if (args.find("list_id") != args.end())
        args_list << args["list_id"];
    else
        args_list << QVariant();
    if (args.find("body") != args.end())
        args_list << args["body"];
    else
        args_list << QVariant();
    if (args.find("number") != args.end())
        args_list << args["number"];
    else
        args_list << QVariant();
    if (args.find("status") != args.end())
        args_list << args["status"];
    else
        args_list << QVariant();
    sendRequest("update_item", args_list, SLOT(responseMethod(QVariant&)));
}

void ServerManager::deleteItem(int itemId)
{
    QVariantList args;
    args << itemId;
    sendRequest("delete_item", args, SLOT(responseMethod(QVariant&)));
}
