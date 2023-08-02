#include "display.h"
#include <QGraphicsScene>
#include <QGraphicsSvgItem>

display::display(QWidget *parent)
    : QGraphicsView(parent),
    scene(Q_NULLPTR),
    asiItem(Q_NULLPTR),
    dialItem(Q_NULLPTR),
    airspeed(0.0)
{
    scene = new QGraphicsScene(this);

    setScene(scene);

    init();

    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::Any, 1234);

    connect(socket, &QUdpSocket::readyRead, this, &display::readyRead);

    int x = 0;
    int y = 0;

    this->move(x, y);
}

void display::init()
{
    QPixmap asi(":/asi/images/ASI_FACE.png");
    asiItem = scene->addPixmap(asi);

    QPixmap dial(":/asi/images/missile-as-dial.png");
    int newWidth = 80;
    int newHeight = 50;
    QPixmap resizedDial = dial.scaled(newWidth, newHeight, Qt::KeepAspectRatio);

    dialItem = new QGraphicsPixmapItem(resizedDial);
    scene->addItem(dialItem);

    int centerX = asiItem->boundingRect().width() / 2 - 5;
    int centerY = asiItem->boundingRect().height() / 2 + 1;

    position.setX(centerX);
    position.setY(centerY);

    dialItem->setTransformOriginPoint(centerX,centerY);
    dialItem->setPos(position);

//    QGraphicsPixmapItem* dialItemCopy = new QGraphicsPixmapItem(dialItem);

//    QPixmap paintedDial(resizedDial.size());
//    paintedDial.fill(QColor(255, 0, 0, 100)); // Kırmızı renkte ve yarı saydamlıkta bir boyama

//    QPainter painter(&paintedDial);
//    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
//    painter.drawPixmap(0, 0, dialItemCopy->pixmap());
//    painter.end();

//    dialItem->setPixmap(paintedDial);


    int circleRadius = 10;
    QGraphicsEllipseItem* circle = new QGraphicsEllipseItem(centerX - circleRadius + 2,
                                                            centerY - circleRadius,
                                                            2 * circleRadius,
                                                            2 * circleRadius);
    circle->setPen(QPen(Qt::black));
    circle->setBrush(Qt::black);

    scene->addItem(circle);

    updateView();
}

void display::updateView()
{
    double angle = 0.0;

    if ( airspeed < 40.0 )
    {
        angle = 1.2 * airspeed - 90;
    }
    else if ( airspeed < 60.0 )
    {
        angle = 40.0 + 1.7 * ( airspeed - 40.0 ) - 80;
    }
    else if ( airspeed < 80.0 )
    {
        angle = 40.0 + 1.6 * ( airspeed - 40.0 ) - 80;
    }
    else if ( airspeed < 100.0 )
    {
        angle = 40.0 + 1.36 * ( airspeed - 40.0 ) - 70;
    }
    else if ( airspeed < 120.0 )
    {
        angle = 90.0 + 1.6 * ( airspeed - 70.0 ) - 85;
    }
    else if ( airspeed < 140.0 )
    {
        angle = 210.0 + 1.7 * ( airspeed - 130.0 ) - 105;
    }
    else if ( airspeed < 160.0 )
    {
        angle = 210.0 + 1.5 * ( airspeed - 130.0 ) - 105;
    }
    else if ( airspeed < 180.0 )
    {
        angle = 260.0 + 1.5 * ( airspeed - 160.0 ) - 110;
    }
    else
    {
        angle = 260.0 + 1.6 * ( airspeed - 160.0 ) - 115;
    }

    QTransform trans;
    trans.rotate(angle);
    dialItem->setTransform(trans);

    qDebug() << "SPEED: " << airspeed;

    scene->update();
}

void display::readyRead()
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

void display::processMessage(const QByteArray &datagram) {

    QStringList lines = QString(datagram).split("\n");
    int lineIndex = 13;
    QString dataAtLine14 = lines.value(lineIndex);

    if (!dataAtLine14.isEmpty())
    {
        int colonIndex = dataAtLine14.indexOf(":");
        QString numberPart = dataAtLine14.mid(colonIndex + 1);

        double numericValue = numberPart.toDouble();

        qDebug() << "14. satırdaki veri: " << numericValue;
                airspeed = numericValue;
        updateView();

        timer = new QTimer(this);

        connect(timer, &QTimer::timeout, this, &display::incrementAirspeed);
        timer->start(100);
    }
    else
    {
        qDebug() << "Hata: 14. satır boş!";
    }
}

void display::incrementAirspeed()
{
    airspeed++;
    updateView();
    if( airspeed > 199 )
    {
        timer->stop();
    }
}
