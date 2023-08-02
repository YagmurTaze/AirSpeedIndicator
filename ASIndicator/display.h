#ifndef DISPLAY_H
#define DISPLAY_H

#include <QGraphicsView>
#include <QGraphicsSvgItem>
#include <QUdpsocket>
#include <QTimer>

class display : public QGraphicsView
{
public:
    display(QWidget *parent = nullptr);

    void processMessage(const QByteArray &datagram);
    QPointF position;

private slots:
    void readyRead();
    void incrementAirspeed();

private:
    QGraphicsScene *scene;

    QGraphicsPixmapItem *asiItem;
    QGraphicsPixmapItem *dialItem;
    QUdpSocket *socket;

    QTimer* timer;

    double airspeed;

    void init();
    void updateView();
};

#endif // DISPLAY_H
