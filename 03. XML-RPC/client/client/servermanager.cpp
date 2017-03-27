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
//    QJsonObject request;
//    request["action"] = "add_list";

//    QJsonObject todoList;
//    todoList["header"] = header;
//    todoList["author_name"] = authorName;
//    todoList["type"] = type;

//    request["todo_list"] = todoList;
//    QJsonDocument doc(request);
//    sendRequest(doc.toJson());
}

void ServerManager::updateList(int listId, QVariantHash params)
{
//    QJsonObject request;
//    request["action"] = "update_list";

//    QJsonObject todoList;
//    todoList["list_id"] = listId;
//    if (params.find("header") != params.end())
//        todoList["header"] = params["header"].toString();
//    if (params.find("author_name") != params.end())
//        todoList["author_name"] = params["author_name"].toString();
//    if (params.find("type") != params.end())
//        todoList["type"] = params["type"].toInt();

//    request["todo_list"] = todoList;
//    QJsonDocument doc(request);
//    sendRequest(doc.toJson());
}

void ServerManager::deleteList(int listId)
{
//    QJsonObject request;
//    request["action"] = "delete_list";

//    QJsonObject todoList;
//    todoList["list_id"] = listId;

//    request["todo_list"] = todoList;
//    QJsonDocument doc(request);
//    sendRequest(doc.toJson());
}

void ServerManager::getItems(int listId)
{
//    QJsonObject request;
//    request["action"] = "get_items";

//    QJsonObject listItem;
//    listItem["list_id"] = listId;

//    request["list_item"] = listItem;
//    QJsonDocument doc(request);
//    sendRequest(doc.toJson());
}

void ServerManager::addItem(int listId, QString body, int number, int status)
{
//    QJsonObject request;
//    request["action"] = "add_item";

//    QJsonObject listItem;
//    listItem["list_id"] = listId;
//    listItem["body"] = body;
//    if (number != 0)
//        listItem["number"] = number;
//    listItem["status"] = status;

//    request["list_item"] = listItem;
//    QJsonDocument doc(request);
//    sendRequest(doc.toJson());
}

void ServerManager::updateItem(int itemId, QVariantHash params)
{
//    QJsonObject request;
//    request["action"] = "update_item";

//    QJsonObject listItem;
//    listItem["item_id"] = itemId;
//    if (params.find("list_id") != params.end())
//        listItem["list_id"] = params["list_id"].toInt();
//    if (params.find("body") != params.end())
//        listItem["body"] = params["body"].toString();
//    if (params.find("number") != params.end())
//        listItem["number"] = params["number"].toInt();
//    if (params.find("status") != params.end())
//        listItem["status"] = params["status"].toInt();

//    request["list_item"] = listItem;
//    QJsonDocument doc(request);
//    sendRequest(doc.toJson());
}

void ServerManager::deleteItem(int itemId)
{
//    QJsonObject request;
//    request["action"] = "delete_item";

//    QJsonObject listItem;
//    listItem["item_id"] = itemId;

//    request["list_item"] = listItem;
//    QJsonDocument doc(request);
//    sendRequest(doc.toJson());
}
