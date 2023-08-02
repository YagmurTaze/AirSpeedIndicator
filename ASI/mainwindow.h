#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QUdpSocket>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void HelloUDP();
    void processMessage(const QByteArray &datagram);

private slots:
    void on_doubleSpinBox_valueChanged(double arg1);
    void readyRead();

private:
    Ui::MainWindow *ui;
    QUdpSocket *socket;
};
#endif // MAINWINDOW_H
