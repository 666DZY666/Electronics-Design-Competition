#include "tcpserver.h"
#include <qdebug.h>
#include <QThread>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QString>
#include <QByteArray>
TCPServer::TCPServer(QObject *parent, const QString &address, quint16 port):
    parent(parent),address(address),port(port)
{

}

bool TCPServer::init()
{
    this->server = new QTcpServer();
    if(!this->server->listen(QHostAddress(address),this->port))
    {
        qDebug() << "tcpserver init: " << this->server->errorString();
        return false;
    }   
    QObject::connect(this->server,&QTcpServer::newConnection, this, &TCPServer::onNewConnection);
    return true;
}

void TCPServer::onNewConnection(){

    if(this->socket != nullptr)
    {
        this->socket->close();
        this->socket = nullptr;
    }
     this->socket = this->server->nextPendingConnection();
     QObject::connect(this->socket, &QTcpSocket::readyRead,this,&TCPServer::onRecvMessage);    
}

void TCPServer::onRecvMessage()
{      
    QByteArray data = this->socket->readAll();
    emit this->newData(QString(data));    
}

TCPServer::~TCPServer()
{
   if(this->socket != nullptr) this->socket->close();
   this->server->close();
}
