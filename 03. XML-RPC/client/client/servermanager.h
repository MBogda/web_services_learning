#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include "maiaXmlRpcClient.h"

class ServerManager : public QObject
{
    Q_OBJECT
public:
    explicit ServerManager(QObject *parent = 0);

signals:
    void responseGetListsFinished(const QVector<QVariantHash> &data);
    void responseGetItemsFinished(const QVector<QVariantHash> &data);
    void faultResponseFinished(int error, const QString &message);

private:
    QVector<QVariantHash> parse(QVariant &data);

private slots:
    void sendRequest(const QString &methodName, const QVariantList &args, const char *resposeSlot);
    void responseGetLists(QVariant &data);
    void responseGetItems(QVariant &data);
    void responseMethod(QVariant &data);
    void faultResponse(int error, const QString &message);

public slots:
    void getLists();
    void addList(QString header = QString(), QString authorName = QString(), int type = 0);
    void updateList(int listId, QVariantHash params);
    void deleteList(int listId);
    void getItems(int listId);
    void addItem(int listId, QString body = QString(), int number = 0, int status = 0);
    void updateItem(int itemId, QVariantHash params);
    void deleteItem(int itemId);

private:
    const QUrl url {"http://localhost:8080/"};
    MaiaXmlRpcClient *client = new MaiaXmlRpcClient(url, this);
};

#endif // SERVERMANAGER_H
