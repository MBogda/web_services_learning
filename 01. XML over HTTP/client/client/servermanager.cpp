#include "servermanager.h"

ServerManager::ServerManager(QObject *parent) : QObject(parent)
{
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getReply(QNetworkReply*)));
    connect(this, SIGNAL(finished(QByteArray)), this, SLOT(parse(QByteArray)));
}

void ServerManager::sendRequest(const QByteArray &data)
{
    QNetworkRequest request{QUrl{url}};
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QString("text/xml")));
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
    QXmlStreamReader xmlReader{data};
    if (xmlReader.readNextStartElement() && xmlReader.name() == "response") {
        // error handling!
        QString action = xmlReader.attributes().value("action").toString();
        QString status = xmlReader.attributes().value("status").toString();
        QVector<QHash<QString, QString> > data;
        if (action == "get_lists" && xmlReader.readNextStartElement() && xmlReader.name() == "todo_lists") {
            while (xmlReader.readNextStartElement() && xmlReader.name() == "todo_list") {
                QHash<QString, QString> todo_list;
                while (xmlReader.readNextStartElement()) {
                    todo_list[xmlReader.name().toString()] = xmlReader.readElementText();
                }
                data.append(todo_list);
            }
        }
        else if (action == "get_items" && xmlReader.readNextStartElement() && xmlReader.name() == "list_items") {
            while (xmlReader.readNextStartElement() && xmlReader.name() == "list_item") {
                QHash<QString, QString> list_item;
                while (xmlReader.readNextStartElement()) {
                    list_item[xmlReader.name().toString()] = xmlReader.readElementText();
                }
                data.append(list_item);
            }
        }
        else {
            // error
        }
        emit(parseFinished(action, status, data));
    }
    else {
        // todo it
    }
}

void ServerManager::getLists()
{
    QByteArray requestData;
    QXmlStreamWriter xmlWriter(&requestData);
    xmlWriter.writeStartDocument();
//    xmlWriter.writeStartElement("request");
    xmlWriter.writeEmptyElement("request");
    xmlWriter.writeAttribute("action", "get_lists");
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    sendRequest(requestData);
}

void ServerManager::addList(QString header, QString author_name, int type)
{
    QByteArray request_data;
    QXmlStreamWriter xmlWriter(&request_data);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("request");
    xmlWriter.writeAttribute("action", "add_list");
    xmlWriter.writeStartElement("todo_list");
    xmlWriter.writeTextElement("header", header);
    xmlWriter.writeTextElement("author_name", author_name);
    xmlWriter.writeTextElement("type", QString::number(type));
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    sendRequest(request_data);
}

void ServerManager::updateList(int list_id, QHash<QString, QString> params)
{
    QByteArray request_data;
    QXmlStreamWriter xmlWriter(&request_data);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("request");
    xmlWriter.writeAttribute("action", "update_list");
    xmlWriter.writeStartElement("todo_list");
    xmlWriter.writeTextElement("list_id", QString::number(list_id));
    if (params.find("header") != params.end())
        xmlWriter.writeTextElement("header", params["header"]);
    if (params.find("author_name") != params.end())
        xmlWriter.writeTextElement("author_name", params["author_name"]);
    if (params.find("type") != params.end())
        xmlWriter.writeTextElement("type", params["type"]);
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    sendRequest(request_data);
}

void ServerManager::deleteList(int list_id)
{
    QByteArray request_data;
    QXmlStreamWriter xmlWriter(&request_data);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("request");
    xmlWriter.writeAttribute("action", "delete_list");
    xmlWriter.writeStartElement("todo_list");
    xmlWriter.writeTextElement("list_id", QString::number(list_id));
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    sendRequest(request_data);
}

void ServerManager::getItems(int list_id)
{
    QByteArray requestData;
    QXmlStreamWriter xmlWriter(&requestData);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("request");
    xmlWriter.writeAttribute("action", "get_items");
    xmlWriter.writeStartElement("list_item");
    xmlWriter.writeTextElement("list_id", QString::number(list_id));
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    sendRequest(requestData);
}

void ServerManager::addItem(int list_id, QString body, int number, int status)
{
    QByteArray request_data;
    QXmlStreamWriter xmlWriter(&request_data);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("request");
    xmlWriter.writeAttribute("action", "add_item");
    xmlWriter.writeStartElement("list_item");
    xmlWriter.writeTextElement("list_id", QString::number(list_id));
    xmlWriter.writeTextElement("body", body);
    if (number != 0)
        xmlWriter.writeTextElement("number", QString::number(number));
    xmlWriter.writeTextElement("status", QString::number(status));
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    sendRequest(request_data);
}

void ServerManager::updateItem(int item_id, QHash<QString, QString> params)
{
    QByteArray request_data;
    QXmlStreamWriter xmlWriter(&request_data);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("request");
    xmlWriter.writeAttribute("action", "update_item");
    xmlWriter.writeStartElement("list_item");
    xmlWriter.writeTextElement("item_id", QString::number(item_id));
    if (params.find("list_id") != params.end())
        xmlWriter.writeTextElement("list_id", params["list_id"]);
    if (params.find("body") != params.end())
        xmlWriter.writeTextElement("body", params["body"]);
    if (params.find("number") != params.end())
        xmlWriter.writeTextElement("number", params["number"]);
    if (params.find("status") != params.end())
        xmlWriter.writeTextElement("status", params["status"]);
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    sendRequest(request_data);
}

void ServerManager::deleteItem(int item_id)
{
    QByteArray request_data;
    QXmlStreamWriter xmlWriter(&request_data);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("request");
    xmlWriter.writeAttribute("action", "delete_item");
    xmlWriter.writeStartElement("list_item");
    xmlWriter.writeTextElement("item_id", QString::number(item_id));
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    sendRequest(request_data);
}
