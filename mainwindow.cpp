#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string.h>
#include <QHostInfo>
#include <QHostAddress>
#include <QTcpSocket>
#include <QMessageBox>
#include <winsock2.h>
#include <windows.h>

using namespace std;

bool status;
int port;
QString IP;
QHostAddress *serverIP;
QString userName;
QTcpSocket *tcpSocket;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->username_edit->setText("zero");
    ui->host_edit->setText("49.234.61.170");
    ui->port_edit->setText("10001");
    IP = "49.234.61.170";
    port = 10001;
    serverIP = new QHostAddress();
}

MainWindow::~MainWindow()
{

    delete ui;
}



void MainWindow::on_connect_button_clicked()
{
    if(!status)
    {
        /* 完成输入合法性检验 */
        QString ip = ui->host_edit->text();
        if(!serverIP->setAddress(ip))
        {
            QMessageBox::information(this,tr("error"),tr("server ip address error!"));
            return;
        }
        if(ui->username_edit->text()=="")
        {
            QMessageBox::information(this,tr("error"),tr("User name error!"));
            return;
        }
        userName = ui->username_edit->text();
        /* 创建了一个QTcpSocket类对象，并将信号/槽连接起来 */
        tcpSocket = new QTcpSocket(this);
        connect(tcpSocket,SIGNAL(connected()),this,SLOT (slotConnected()));
        connect(tcpSocket,SIGNAL(disconnected()),this,SLOT (slotDisconnected ()));
        connect(tcpSocket,SIGNAL(readyRead()),this,SLOT (dataReceived()));
        tcpSocket->connectToHost(*serverIP,port);
        status=true;
    }
    else
    {
        int length=0;
        QString msg = userName + tr(":Leave ChatRoom");
        if((length=tcpSocket->write(msg.toLatin1(),msg. length()))!=msg. length())
        {
            return;
        }
        tcpSocket->disconnectFromHost();
        status=false;                                   //将status状态复位
    }

}


void MainWindow::slotConnected()
{
    ui->send_button->setEnabled(true);
    ui->connect_button->setText(tr("Leave"));
    int length=0;
    QString msg=userName+tr(":Enter ChatRoom");
    if((length=tcpSocket->write(msg.toLatin1(),msg. length()))!=msg.length())
    {
        return;
    }
}

void MainWindow::slotDisconnected()
{
    ui->send_button->setEnabled(false);
    ui->connect_button->setText(tr("Connect"));
}

void MainWindow::dataReceived()
{
    while(tcpSocket->bytesAvailable()>0)
    {
        QByteArray datagram;
        datagram.resize(tcpSocket->bytesAvailable());
        tcpSocket->read(datagram.data(),datagram.size());
        QString msg=datagram.data();
        ui->textBrowser->append(msg.left(datagram.size()));

    }
}

void MainWindow::on_send_button_clicked()
{
    if(ui->message_edit->toPlainText()=="")
    {
        return ;
    }
    //QString msg=userName+":"+ui->message_edit->toPlainText();
    QString msg = ui->message_edit->toPlainText();
    tcpSocket->write(msg.toLatin1(), msg.length());
    ui->message_edit->clear();
    ui->message_edit->setFocus();
}


void MainWindow::on_clear_button_clicked()
{
    ui->textBrowser->clear();
}


void MainWindow::on_textBrowser_sourceChanged(const QUrl &arg1)
{
    ui->textBrowser->moveCursor(QTextCursor::End);
}



