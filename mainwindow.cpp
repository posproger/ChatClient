#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ccnetmanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pbReconnect->setEnabled(true);
    m_netmanager.make_connect();
    ui->pbSend->setEnabled(false);
    ui->teMsg->setEnabled(false);
    ui->tbMainText->setReadOnly(true);
    connect(this,SIGNAL(newMsgForSend(QString)),&m_netmanager,SLOT(newMsgForSend(QString)));
    connect(&m_netmanager,SIGNAL(msgReceived(QString)),this,SLOT(newMsgCome(QString)));
    connect(&m_netmanager,SIGNAL(connected()),this,SLOT(connectedToServer()));
    connect(&m_netmanager,SIGNAL(disconnected()),this,SLOT(disconnectedFromServer()));
    connect(ui->pbReconnect,SIGNAL(clicked()),this,SLOT(reconnect()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectedToServer() {
    ui->pbSend->setEnabled(true);
    ui->teMsg->setEnabled(true);
    ui->pbReconnect->setEnabled(false);
}

void MainWindow::disconnectedFromServer(){
    ui->pbSend->setEnabled(false);
    ui->teMsg->setEnabled(false);
    ui->pbReconnect->setEnabled(true);
}

void MainWindow::reconnect() {
    m_netmanager.make_connect();
}

void MainWindow::newMsgCome(QString msg) {
    qDebug() << "MsgCome" << msg;
    ui->tbMainText->append(msg);
}

void MainWindow::sendMsgBegin(void) {
    if ( !ui->teMsg->toPlainText().isEmpty() ) {
        ui->pbSend->setEnabled(false);
        emit newMsgForSend(QString() + ui->leNickName->text() + " : " + ui->teMsg->toPlainText());
        if ( !m_netmanager.isConnected() ) {
            m_netmanager.make_connect();
        } else {
            ui->tbMainText->append(QString() + "ME : " + ui->teMsg->toPlainText());
            ui->teMsg->clear();
        }
        ui->pbSend->setEnabled(true);
    }
}

void MainWindow::on_pbSend_clicked()
{
    sendMsgBegin();
}
