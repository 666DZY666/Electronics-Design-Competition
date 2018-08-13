#ifndef AFWIDGET_H
#define AFWIDGET_H

#include <QWidget>
#include <QVector>
#include "tcpserver.h"
class AFWidget : public QWidget
{
    Q_OBJECT
public:
    AFWidget(QWidget *parent = 0);
    bool init();
    ~AFWidget();
protected:
    void paintEvent(QPaintEvent *event);
private:
    QVector<quint8> *xVector;
    QVector<quint8> *yVector;
    TCPServer *tcpServer;
    int drawValue = 0;
signals:

public slots:
    void onDataReceived(QString data);
};

#endif // AFWIDGET_H
