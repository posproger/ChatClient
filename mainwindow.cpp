#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tbMainText->setReadOnly(true);
    ui->tbMainText->setEnabled(true);
    ui->pbReconnect->setEnabled(true);
    m_manager.mkConnect();
    m_curChannel = -1;
    connect(this,SIGNAL(newMsgForSend(QString,int)),&m_manager,SLOT(newMsgForSend(QString,int)));
    connect(this,SIGNAL(mkLogin(QString,QString,QString)),&m_manager,SLOT(mkLogin(QString,QString,QString)));
    connect(this,SIGNAL(mkConnect()),&m_manager,SLOT(mkConnect()));
    //connect(&m_manager,SIGNAL(msgReceived(QString)),this,SLOT(newMsgCome(QString)));
    connect(&m_manager,SIGNAL(connected()),this,SLOT(connectedToServer()));
    connect(&m_manager,SIGNAL(disconnected()),this,SLOT(disconnectedFromServer()));
    connect(&m_manager,SIGNAL(logined()),this,SLOT(logined()));
    connect(&m_manager,SIGNAL(unlogined()),this,SLOT(unlogined()));
    connect(&m_manager,SIGNAL(channelsReceived(QMap<int,CCChannel*>)),this,SLOT(channelsReceived(QMap<int,CCChannel*>)));
    connect(ui->pbReconnect,SIGNAL(clicked()),this,SLOT(reconnect()));
    connect(ui->pbLogin,SIGNAL(clicked()),this,SLOT(login()));
    connect(ui->pbSend,SIGNAL(clicked()),this,SLOT(sendMsgBegin()));
    connect(ui->cbChannel,SIGNAL(currentIndexChanged(int)),this,SLOT(channelChosen(int)));
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
    qDebug() << "MainWindow::login";
    if ( !ui->leLogin->text().isEmpty() ) {
if ( ui->leLogin->text()=="Max" )
emit mkLogin(ui->leLogin->text(),ui->leLogin->text(),"max@mail.mu");
else if ( ui->leLogin->text()=="Def" )
emit mkLogin(ui->leLogin->text(),ui->leLogin->text(),"defoer@mail.mu");
else
        emit mkLogin(ui->leLogin->text(),ui->leLogin->text(),"mail@mail.mu");
    }
}

void MainWindow::logined(void) {
    ui->leLogin->setEnabled(false);
    ui->pbLogin->setEnabled(false);
}

void MainWindow::unlogined(void) {
    ui->leLogin->setEnabled(true);
    ui->pbLogin->setEnabled(true);
    ui->leLogin->clear();
    ui->tbMainText->append(tr("Login error!"));
}

void MainWindow::newMsgCome(QString msg) {
    qDebug() << "MainWindow::MsgCome" << msg;
    ui->tbMainText->append(msg);
}

void MainWindow::sendMsgBegin(void) {
    if ( !ui->teMsg->toPlainText().isEmpty() ) {
        ui->pbSend->setEnabled(false);
        emit newMsgForSend(ui->teMsg->toPlainText(),m_curChannel);
        ui->tbMainText->append(QDateTime::currentDateTimeUtc().toString("HH:mm:ss") + " ME: " + ui->teMsg->toPlainText());
        ui->teMsg->clear();
        ui->pbSend->setEnabled(true);
    }
}

void MainWindow::channelsReceived(QMap<int,CCChannel*> map) {
    ui->cbChannel->clear();
    ui->cbChannel->setEnabled(false);
    ui->cbChannel->addItem(tr("choose channel"),QVariant(0));
    QMapIterator<int, CCChannel*> it(map);
    while (it.hasNext()) {
        it.next();
        ui->cbChannel->addItem(it.value()->getName(),QVariant(it.key()));
    }
    m_channels = map;
    ui->cbChannel->setEnabled(true);
}

void MainWindow::channelChosen(int index) {
    qDebug() << "MainWindow::channelChosen" << index;
    ui->pbSend->setEnabled(false);
    ui->teMsg->setEnabled(false);
    int res = ui->cbChannel->currentData().toInt();
    if ( res!=m_curChannel ) {
        if ( m_curChannel>0 ) {
            disconnect(m_channels.value(m_curChannel),SIGNAL(newMessage(QString)),this,SLOT(newMsgCome(QString)));
        }
        m_curChannel = res;
        if ( res==0 ) {
            ui->tbMainText->clear();
        } else {
            ui->tbMainText->clear();
            if ( m_channels.contains(res) ) {
                connect(m_channels.value(res),SIGNAL(newMessage(QString)),this,SLOT(newMsgCome(QString)));
                ui->tbMainText->append(m_channels.value(res)->getText());
                ui->pbSend->setEnabled(true);
                ui->teMsg->setEnabled(true);
            } else {
                m_curChannel = 0;
            }
        }
    } else {
        ui->pbSend->setEnabled(true);
        ui->teMsg->setEnabled(true);
    }
}

