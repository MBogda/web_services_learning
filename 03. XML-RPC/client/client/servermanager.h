#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class ServerManager : public QObject
{
    Q_OBJECT
public:
    explicit ServerManager(QObject *parent = 0);

signals:
    void finished(QByteArray data);
    void parseFinished(const QString &action, const QString &status, const QVector<QVariantHash> &data);

private slots:
    void sendRequest(const QByteArray &data);
    void getReply(QNetworkReply *reply);
    void parse(const QByteArray &data);

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
    const QString url = "http://localhost:8080/";
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
};

#endif // SERVERMANAGER_H
