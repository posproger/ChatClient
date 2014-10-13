#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tbMainText->setReadOnly(true);
    ui->pbReconnect->setEnabled(true);
    m_manager.mkConnect();
    connect(this,SIGNAL(newMsgForSend(QString)),&m_manager,SLOT(newMsgForSend(QString)));
    connect(this,SIGNAL(mkLogin(QString)),&m_manager,SLOT(mkLogin(QString)));
    connect(this,SIGNAL(mkConnect()),&m_manager,SLOT(mkConnect()));
    connect(&m_manager,SIGNAL(msgReceived(QString)),this,SLOT(newMsgCome(QString)));
    connect(&m_manager,SIGNAL(connected()),this,SLOT(connectedToServer()));
    connect(&m_manager,SIGNAL(disconnected()),this,SLOT(disconnectedFromServer()));
    connect(ui->pbReconnect,SIGNAL(clicked()),this,SLOT(reconnect()));
    connect(ui->pbLogin,SIGNAL(clicked()),this,SLOT(login()));
    connect(ui->pbSend,SIGNAL(clicked()),this,SLOT(sendMsgBegin()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectedToServer(void) {
    ui->pbLogin->setEnabled(true);
    ui->leLogin->setEnabled(true);
    ui->pbReconnect->setEnabled(false);
}

void MainWindow::disconnectedFromServer(void){
    ui->pbSend->setEnabled(false);
    ui->teMsg->setEnabled(false);
    ui->pbLogin->setEnabled(false);
    ui->leLogin->setEnabled(false);
    ui->pbReconnect->setEnabled(true);
}

void MainWindow::reconnect(void) {
    emit mkConnect();
}

void MainWindow::login(void) {
    if ( !ui->leLogin->text().isEmpty() ) {
if ( ui->leLogin->text()=="Max" )
emit mkLogin(ui->leLogin->text(),ui->leLogin->text(),"max@mail.mu");
else
        emit mkLogin(ui->leLogin->text(),ui->leLogin->text(),"mail@mail.mu");
    }
}

void MainWindow::logined(void) {
    //ui->pbSend->setEnabled(true); /// - После выбора канала... (авто?)
    //ui->teMsg->setEnabled(true);
}

void MainWindow::newMsgCome(QString msg) {
    qDebug() << "MainWindow::MsgCome" << msg;
    ui->tbMainText->append(msg);
}

void MainWindow::sendMsgBegin(void) {
    if ( !ui->teMsg->toPlainText().isEmpty() ) {
        ui->pbSend->setEnabled(false);
        emit newMsgForSend(ui->teMsg->toPlainText());
        ui->tbMainText->append(QDateTime::currentDateTimeUtc().toString("HH:mm:ss") + " ME: " + ui->teMsg->toPlainText());
        ui->teMsg->clear();
        ui->pbSend->setEnabled(true);
    }
}

