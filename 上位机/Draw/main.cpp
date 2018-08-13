#include "mainwindow.h"
#include <QApplication>
#include "afwidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    AFWidget *af = new AFWidget(&w);
    af->init();
    w.setCentralWidget(af);
    w.resize(1207, 511);
    w.setWindowIcon(QIcon(QStringLiteral(":/image/images/app.ico")));
    w.show();
    return a.exec();
}
