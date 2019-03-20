#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtNetwork>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_send_clicked();
    void udpReadyRead();
    void changePort();
    void onConvenient();
    void sendData(const QString &msg);
    void setStyle(const QString &style);
private:
    Ui::MainWindow *ui;
    QUdpSocket udpSocket;
};

#endif // MAINWINDOW_H
