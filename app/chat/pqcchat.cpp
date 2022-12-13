#include "pqcchat.h"
#include "ui_pqcchat.h"

PqcChat::PqcChat(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PqcChat)
{
    ui->setupUi(this);
    rthread = new RThread();
    connect(rthread, SIGNAL(readMsg(char*)), this, SLOT(showMsg(char*)));
    ui->btnClose->setEnabled(false);
    ui->btnSend->setEnabled(false);
}

PqcChat::~PqcChat()
{
    delete ui;
}

void PqcChat::showMsg(char* msg) {

    ui->textChat->append(msg);

}

void PqcChat::on_btnConn_clicked() {
    std::string ip, port;
    ip = ui->ip->text().toStdString();
    port = ui->port->text().toStdString();

    if (!tc.connect(inet_addr(ip.data()), std::stoi(port.data())) ) {
        std::cerr << tc.error_ << std::endl;
        ui->textChat->append("연결 실패");
        return;
    }

    ui->textChat->append("서버 연결");
    rthread->tc = &tc;
    rthread->start();

    ui->btnConn->setEnabled(false);
    ui->btnClose->setEnabled(true);
    ui->btnSend->setEnabled(true);
}

void PqcChat::on_btnClose_clicked() {

    rthread->terminate();
    ui->textChat->append("연결 종료");
    tc.close();

    ui->btnConn->setEnabled(true);
    ui->btnClose->setEnabled(false);
    ui->btnSend->setEnabled(false);
}

void PqcChat::on_btnSend_clicked() {
    QString msg;
    msg = ui->msg->text();
    ui->msg->setText("");
    int writeLen = tc.write(msg.toStdString().data(), msg.size());

    if (writeLen != -1 )
        ui->textChat->append(msg);
}



