#ifndef TCPSERVERTHREAD_H
#define TCPSERVERTHREAD_H

#include <QTcpServer>
#include <QByteArray>
class TCPServer:public QObject
{
    Q_OBJECT
public:
    explicit TCPServer(QObject *parent = 0, const QString &address = "127.0.0.1", quint16 port=3000);
    bool init();
    ~TCPServer();
public slots:
    void onNewConnection();
    void onRecvMessage();
private:
    QObject *parent = 0;
    QTcpServer *server = 0;
    QTcpSocket *socket = 0;
    QString address;
    quint16 port;
signals:
    void newData(QString data);
};

#endif // TCPSERVERTHREAD_H
