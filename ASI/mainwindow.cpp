#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int x = 0;
    int y = 0;

    this->move(x, y);

    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::Any, 1234);

    connect(socket, &QUdpSocket::readyRead, this, &MainWindow::readyRead);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    ui->graphicsASI->setAirspeed(arg1);
    ui->graphicsASI->redraw();
}

void MainWindow::HelloUDP()
{
    QByteArray Data;
    Data.append("Hello from UDP");

    socket->writeDatagram(Data, QHostAddress::LocalHost, 1234);
}

void MainWindow::readyRead()
{
    QByteArray datagram;
    datagram.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    socket->readDatagram(datagram.data(), datagram.size(),
                         &sender, &senderPort);

    qDebug() << "Message from: " << sender.toString();
    qDebug() << "Message port: " << senderPort;
    qDebug() << "Message: " << datagram;

    QStringList lines = QString(datagram).split("\n");
    if (lines.size() >= 6)
    {
        processMessage(datagram);
    } else {
        qDebug() << "Hatalı datagram!";
    }
}

void MainWindow::processMessage(const QByteArray &datagram) {

    QStringList lines = QString(datagram).split("\n");
    int lineIndex = 13;
    QString dataAtLine14 = lines.value(lineIndex);

    if (!dataAtLine14.isEmpty())
    {
        int colonIndex = dataAtLine14.indexOf(":");
        QString numberPart = dataAtLine14.mid(colonIndex + 1);

        double numericValue = numberPart.toDouble();

        qDebug() << "6. satırdaki veri: " << numericValue;

        ui->graphicsASI->setAirspeed(numericValue);
        ui->graphicsASI->redraw();
    }
    else
    {
        qDebug() << "Hata: 6. satır boş!";
    }
}

