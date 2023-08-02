#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int x = 700;
    int y = 0;

    this->move(x, y);

    udpSocket = new QUdpSocket(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::broadcastMessage()
{
    QString message = ui->messageLog->toPlainText();
    QByteArray datagram = message.toUtf8();

    QHostAddress broadcastAddress = QHostAddress("127.0.0.1");
    quint16 port = 1234;

    udpSocket->writeDatagram(datagram, broadcastAddress, port);

}



void MainWindow::on_pushButton_clicked()
{
    broadcastMessage();
}

