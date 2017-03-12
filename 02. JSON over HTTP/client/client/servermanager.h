#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

class ServerManager : public QObject
{
    Q_OBJECT
public:
    explicit ServerManager(QObject *parent = 0);

signals:
    void finished(QByteArray data);
    void parseFinished(const QString &action, const QString &status, const QVector<QHash<QString, QString> > &data);

private slots:
    void sendRequest(const QByteArray &data);
    void getReply(QNetworkReply *reply);
    void parse(const QByteArray &data);

public slots:
    void getLists();
    void addList(QString header = QString(), QString authorName = QString(), int type = 0);
    void updateList(int list_id, QHash<QString, QString> params);
    void deleteList(int list_id);
    void getItems(int list_id);
    void addItem(int list_id, QString body = QString(), int number = 0, int status = 0);
    void updateItem(int item_id, QHash<QString, QString> params);
    void deleteItem(int item_id);

private:
    const QString url = "http://localhost:8080/";
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
};

#endif // SERVERMANAGER_H
