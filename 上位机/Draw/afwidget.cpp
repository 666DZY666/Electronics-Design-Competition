#include "afwidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <qdebug.h>
#include <QSettings>
#include <QMessageBox>
#include <QDir>
#include <QApplication>
AFWidget::AFWidget(QWidget *parent) : QWidget(parent)
{

}
AFWidget::~AFWidget()
{

}

bool AFWidget::init()
{
    QDir dir;
    bool ok;
    QString path = dir.currentPath();
    path = path + "/config.ini";
    QSettings configIniRead(path, QSettings::IniFormat);
    QString ip = configIniRead.value("server/ip").toString();
    QString port = configIniRead.value("server/port").toString();
    this->drawValue = configIniRead.value("common/drawvalue").toInt();
    qDebug() << "ip : "<<ip<<endl;
    qDebug() << "port : "<<port<<endl;
    if(ip.isNull() || ip.isEmpty() || port.isNull() || port.isEmpty()){
        QMessageBox::warning(this,"ERROR","this application cant not run normally,\n\nplease check the config file named 'config.ini'         ");
    }

    this->xVector = new QVector<quint8>(40, 0);
    this->yVector = new QVector<quint8>(40, 0);
    this->tcpServer = new TCPServer(this,ip, port.toInt(&ok, 10));
    if(!this->tcpServer->init())
    {
        return false;
    }
    QObject::connect(this->tcpServer,&TCPServer::newData, this, &AFWidget::onDataReceived);
    return true;
}

void AFWidget::paintEvent(QPaintEvent *event)
{
    QRect rect = event->rect();
    int marginLeft = rect.width() / 30;
    int marginBottom = rect.height() / 20;
    int marginTop = marginBottom / 2;
    int gap = (rect.width() - 2 * marginLeft) / this->xVector->size();
    QPainter painter(this);

     // 画坐标轴横轴
     painter.drawLine(marginLeft, rect.height() - marginBottom, rect.width()-marginLeft, rect.height()-marginBottom);
     // 画坐标轴纵轴
     painter.drawLine(marginLeft, rect.height() - marginBottom, marginLeft, 0 + marginTop);
     /****************************************************************************************************************************/
     QFont font;
     font.setFamily("Microsoft YaHei");
     font.setPointSize(marginBottom / 3);
     painter.setFont(font);
     /****************************************************************************************************************************/
     // 绘制横坐标值
     for(int i = 1; i <= this->xVector->size(); i++){
         int xstart = marginLeft + i*gap;
         int ystart = rect.height() - marginBottom;
         const QRect rectangle = QRect(xstart - gap/2 - 2, ystart + 2, gap, marginBottom / 2);
         painter.drawText(rectangle,Qt::AlignCenter, QString::number(i, 10));
     }
     /****************************************************************************************************************************/
     // 绘制横坐标名称
     QRect rectangle = QRect(marginLeft, rect.height() - marginBottom / 2, rect.width() - marginLeft, marginBottom / 2);
     painter.drawText(rectangle, Qt::AlignCenter, "频率 / MHz");
     /****************************************************************************************************************************/
     // 绘制纵坐标值
     int vCell = (rect.height() - marginBottom - marginTop) / 16;
     font.setPointSize(marginLeft / 6);
     painter.setFont(font);
     for(int i = 0; i <= 19; i++)
     {
        const QRect rectangle = QRect(marginLeft / 2, rect.height() - marginBottom - i * vCell - vCell / 2, marginLeft / 2, (rect.height() - marginBottom - marginTop) / 16);
        int value = i * 16;
        if(value == 256) value = 255;
        painter.drawText(rectangle,Qt::AlignCenter, QString::number(value, 10));
     }
     /****************************************************************************************************************************/
     // 绘制纵坐标名称
     rectangle = QRect(0, marginTop, marginLeft / 2, rect.height() - marginTop - marginBottom);

     painter.drawText(rectangle, Qt::AlignCenter, "幅度");
     /****************************************************************************************************************************/
     painter.setFont(font);
     // 根据数据绘制点
     for(int i = 0; i < this->xVector->size(); i++)
     {
         int xstart = marginLeft + this->xVector->at(i)*gap;
         int ystart = rect.height() - marginBottom;
         int xend = xstart;
         int yend = (rect.height() - marginBottom - this->yVector->at(i) / 255.0 * (rect.height() - marginBottom - marginTop));
         if(this->yVector->at(i) <= 0){
             continue;
         }         
         QPen pen = QPen(QColor(0, 160, 230), 8);
         pen.setCapStyle(Qt::RoundCap);
         painter.setPen(pen);
         //painter.drawLine(xstart, ystart, xend, yend);
         painter.drawPoint(xend, yend);
         if(this->drawValue)
         {
             font.setPointSize(8);
             painter.setFont(font);
             painter.setPen(QPen(QColor(67,185,240), 4));
             painter.drawText(xend - gap / 6 , yend - 6,QString::number(this->yVector->at(i),10));
         }
     }
    //qDebug() << "width: " << rect.width() << "height: " << rect.height() <<endl;
}

void AFWidget::onDataReceived(QString data)
{
    bool ok;

    QStringList list = data.split(" ");
    QString curr = list.at(1);
    int x = curr.toInt(&ok,10);
    curr = list.at(2);
    double tempt = (double)curr.toInt(&ok,10)/4096*3.65/0.6*1000;
    int y = tempt;
    qDebug() << "y: "<<tempt <<endl;
    qDebug() << "value: "<< curr.toInt(&ok,10)<<endl;
    this->xVector->replace(x-1, x);
    this->yVector->replace(x-1, y);
    this->repaint();
}
