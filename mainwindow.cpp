#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingmanager.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->comboBox_Style->addItems(QStyleFactory::keys());
    ui->comboBox_Style->setCurrentIndex(-1);
    connect(ui->comboBox_Style, SIGNAL(currentIndexChanged(QString)), this, SLOT(setStyle(QString)));

    SettingManager::registerWidgetList(this, QList<QWidget *>() << ui->lineEdit_ip << ui->spinBox_port << ui->textEdit_tx << ui->checkBox_broadcast << ui->checkBox_clearAfterTx << ui->checkBox_convenient << ui->comboBox_Style);
    SettingManager::loadWidgetConfig(this);

    QSignalMapper *signalMapper = new QSignalMapper(this);
    connect(ui->pushButton_open, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->pushButton_close, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->pushButton_switch, SIGNAL(pressed()), signalMapper, SLOT(map()));
    signalMapper->setMapping(ui->pushButton_open, "0");
    signalMapper->setMapping(ui->pushButton_close, "1");
    signalMapper->setMapping(ui->pushButton_switch, "2");
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(sendData(QString)));

    udpSocket.bind(ui->spinBox_port->value(), QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint); // 绑定端口以接收
    connect(&udpSocket, SIGNAL(readyRead()), this, SLOT(udpReadyRead()));
    connect(ui->spinBox_port, SIGNAL(valueChanged(int)), this, SLOT(changePort()));
    QTimer::singleShot(1000, this, SLOT(onConvenient()));
}

MainWindow::~MainWindow() {
    SettingManager::saveWidgetConfig(this);
    delete ui;
}

void MainWindow::on_pushButton_send_clicked() {
    sendData(ui->textEdit_tx->toPlainText());
    if ( ui->checkBox_clearAfterTx->isChecked() ) // 发送后清空
        ui->textEdit_tx->clear();
}

void MainWindow::udpReadyRead() { // 接收消息
    while ( udpSocket.hasPendingDatagrams() ) {
        QByteArray datagram(udpSocket.pendingDatagramSize(), 0);
        udpSocket.readDatagram(datagram.data(),datagram.size());
        ui->textEdit_rx->append(QString(datagram));
        ui->textEdit_rx->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    }
}

void MainWindow::changePort() {
    udpSocket.abort();
    udpSocket.bind(ui->spinBox_port->value(), QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
}

void MainWindow::onConvenient() {
    if ( !ui->checkBox_convenient->isChecked() )
        return;
    emit ui->pushButton_switch->pressed();
    QTimer::singleShot(0, qApp, SLOT(quit()));
}

void MainWindow::sendData(const QString &msg) {
    QHostAddress addr = ui->checkBox_broadcast->isChecked() ? QHostAddress::Broadcast : QHostAddress(ui->lineEdit_ip->text());
    udpSocket.writeDatagram( msg.toUtf8(), addr, ui->spinBox_port->value() );
}

void MainWindow::setStyle(const QString &style) {
    QApplication::setStyle(style);
}
