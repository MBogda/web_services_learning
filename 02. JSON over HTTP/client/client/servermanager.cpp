#include "servermanager.h"

ServerManager::ServerManager(QObject *parent) : QObject(parent)
{
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getReply(QNetworkReply*)));
    connect(this, SIGNAL(finished(QByteArray)), this, SLOT(parse(QByteArray)));
}

void ServerManager::sendRequest(const QByteArray &data)
{
    QNetworkRequest request{QUrl{url}};
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QString("application/json")));
    request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(data.length()));
    networkManager->post(request, data);
}

void ServerManager::getReply(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    reply->deleteLater();
    emit(finished(data));
}

void ServerManager::parse(const QByteArray &data)
{
    QJsonDocument d = QJsonDocument::fromJson(data);
    QJsonObject response = d.object();
    QString action = response["action"].toString();
    QString status = response["status"].toString();
    QVector<QHash<QString, QString> > return_data;
    if (action == "get_lists" || action == "get_items") {
        QString index;
        if (action == "get_lists")
            index = "todo_lists";
        else if (action == "get_items")
            index = "list_items";

        QJsonArray json_objects = response[index].toArray();
        for (int i = 0; i < json_objects.size(); ++i) {
            QJsonObject json_object = json_objects[i].toObject();

            // QVariantHash instead of this
            QHash<QString, QString> object;
            for (auto field : json_object.keys()) {
                object[field] = json_object[field].toString();
            }
            return_data.append(object);
        }
    }
    emit(parseFinished(action, status, return_data));
}

void ServerManager::getLists()
{
    QJsonObject request;
    request["action"] = "get_lists";
    QJsonDocument doc(request);
    sendRequest(doc.toJson());
}

void ServerManager::addList(QString header, QString authorName, int type)
{
    QJsonObject request;
    request["action"] = "add_list";

    QJsonObject todoList;
    todoList["header"] = header;
    todoList["author_name"] = authorName;
    todoList["type"] = type;

    request["todo_list"] = todoList;
    QJsonDocument doc(request);
    sendRequest(doc.toJson());
}

void ServerManager::updateList(int listId, QHash<QString, QString> params)
{
    QJsonObject request;
    request["action"] = "update_list";

    QJsonObject todoList;
    todoList["list_id"] = listId;
    if (params.find("header") != params.end())
        todoList["header"] = params["header"];
    if (params.find("author_name") != params.end())
        todoList["author_name"] = params["author_name"];
    if (params.find("type") != params.end())
        todoList["type"] = params["type"];

    request["todo_list"] = todoList;
    QJsonDocument doc(request);
    sendRequest(doc.toJson());
}

void ServerManager::deleteList(int listId)
{
    QJsonObject request;
    request["action"] = "delete_list";

    QJsonObject todoList;
    todoList["list_id"] = listId;

    request["todo_list"] = todoList;
    QJsonDocument doc(request);
    sendRequest(doc.toJson());
}

void ServerManager::getItems(int listId)
{
    QJsonObject request;
    request["action"] = "get_items";

    QJsonObject listItem;
    listItem["list_id"] = listId;

    request["list_item"] = listItem;
    QJsonDocument doc(request);
    sendRequest(doc.toJson());
}

void ServerManager::addItem(int listId, QString body, int number, int status)
{
    QJsonObject request;
    request["action"] = "add_item";

    QJsonObject listItem;
    listItem["list_id"] = listId;
    listItem["body"] = body;
    listItem["number"] = number;
    listItem["status"] = status;

    request["list_item"] = listItem;
    QJsonDocument doc(request);
    sendRequest(doc.toJson());
}

void ServerManager::updateItem(int itemId, QHash<QString, QString> params)
{
    QJsonObject request;
    request["action"] = "update_item";

    QJsonObject listItem;
    listItem["item_id"] = itemId;
    if (params.find("list_id") != params.end())
        listItem["list_id"] = params["list_id"];
    if (params.find("body") != params.end())
        listItem["body"] = params["body"];
    if (params.find("number") != params.end())
        listItem["number"] = params["number"];
    if (params.find("status") != params.end())
        listItem["status"] = params["status"];

    request["list_item"] = listItem;
    QJsonDocument doc(request);
    sendRequest(doc.toJson());
}

void ServerManager::deleteItem(int itemId)
{
    QJsonObject request;
    request["action"] = "delete_item";

    QJsonObject listItem;
    listItem["item_id"] = itemId;

    request["list_item"] = listItem;
    QJsonDocument doc(request);
    sendRequest(doc.toJson());
}
