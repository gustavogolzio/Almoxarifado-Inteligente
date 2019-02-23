#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "almox_logar.h"
#include "almox_novoitem.h"
#include "almox_gestaocolab.h"
#include "almox_gestaoestoque.h"

#include<QSerialPort>
#include<QSerialPortInfo>
#include<QJsonDocument>
#include<QJsonObject>

int MainWindow::id_colab;
QString MainWindow::nome_colab;
QString MainWindow::acesso_colab;
QString MainWindow::username_colab;
bool MainWindow::logado;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    logado = false;     //Estado do login
    cadFechado.addFile(":/imagens/imgs/cadeado_fechado.png");   //Atribui as imagens dos cadeados
    cadAberto.addFile(":/imagens/imgs/cadeado_aberto.png");

   // QPixmap bkgnd("Z:/20181610042/Desktop/Almoxarifado-Bajampa-master/almoxarifado/imgs/bckg2.jpg");
     //   bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
       // QPalette palette;
        //palette.setBrush(QPalette::Background, bkgnd);
        //this->setPalette(palette);
   // this->centralWidget()->setStyleSheet("background-image:url("Z:/20181610042/Desktop/Almoxarifado-Bajampa-master/almoxarifado/imgs/bckg2.jpg"); background-position: center; ");
   //for(auto&item:QSerialPortInfo::availablePorts())
    //ui->box_serial->addItem(item.portName());
    ui->btn_bloquear->setText("");
    ui->btn_bloquear->setIcon(cadFechado);
    ui->statusBar->addWidget(ui->btn_bloquear);     //Adciona o botão de bloquear na barra de status
    ui->statusBar->addWidget(ui->lb_nome);     //Adciona o botão de bloquear na barra de status
    ui->statusBar->addWidget(ui->lb_acesso);

    for(auto&item:QSerialPortInfo::availablePorts())
      ui->box_serial->addItem(item.portName());

    for(auto&item:QSerialPortInfo::standardBaudRates())
      ui->box_velocidade->addItem(QString::number(item));

    connect(&serial,SIGNAL(readyRead()),this,SLOT(dadosRecebidos()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_bloquear_clicked()
{

}

void MainWindow::on_pushButton_clicked()
{
    if(logado){
        almox_novoItem a_novoItem;
        a_novoItem.exec();
    }else {
        QMessageBox::information(this, "Login", "Usuário não Logado!");
    }
}

void MainWindow::on_actionEstoque_triggered()
{
    if(logado){
        if(acesso_colab=="A" || acesso_colab == "B"){
            almox_gestaoEstoque a_gestaoEstoque;
            a_gestaoEstoque.exec();
        }else {
            QMessageBox::information(this, "Acesso", "Acesso não Permitido!");
        }
    }else {
        QMessageBox::information(this, "Login", "Usuário não Logado!");
    }
}


void MainWindow::on_actionColaboradores_triggered()
{
    if(logado){
        if(acesso_colab=="A"){
            almox_gestaoColab a_gestaoColab;
            a_gestaoColab.exec();
        }else {
            QMessageBox::information(this, "Acesso", "Acesso não Permitido!");
        }
    }else {
        QMessageBox::information(this, "Login", "Usuário não Logado!");
    }
}



void MainWindow::on_actionSair_triggered()
{
    QCoreApplication::quit();
}

void MainWindow::on_actionSobre_triggered()
{
    QMessageBox::information(this, "SOBRE", "Almoxarifado BAJAMPA \nFeito por Joarles Macêdo\n24/12/2018.");
}

void MainWindow::on_pushButton_2_clicked()
{
    if(!logado){
        almox_logar a_logar;
        a_logar.exec();

        if(logado){
            ui->pushButton_2->setText("Logout");
            ui->btn_bloquear->setIcon(cadAberto);
            ui->lb_nome->setText(nome_colab);
            ui->lb_acesso->setText("Acesso: "+acesso_colab);
        }
    }else {
        logado = false;
        ui->pushButton_2->setText("Login");
        ui->btn_bloquear->setIcon(cadFechado);
        ui->lb_nome->setText("Sem Colaborador");
        ui->lb_acesso->setText("");
    }
}

void MainWindow::on_btn_destravar_clicked()
{
    serial.write("{\"LED01\":1}");
}

void MainWindow::on_btn_connect_clicked()
{
    serial.setPortName(ui->box_serial->currentText());
    serial.setBaudRate(ui->box_velocidade->currentText().toInt());

    if(serial.open(QIODevice::ReadWrite)){
        ui->status_conexao->setText("Status:Conectado");
    }
}

void MainWindow::on_btn_disconnect_clicked()
{
    serial.close();
    ui->status_conexao->setText("Status:Desconectado");
}

void MainWindow::dadosRecebidos()
{
    auto data=serial.readAll();
    auto dados=QJsonDocument::fromJson(data).object().toVariantMap();

    if(dados.contains("TEMP"))
        ui->temp_value->setText(dados["TEMP"].toString());

    if(dados.contains("PIR")==1)
        ui->pir_value->setText("Detectado");
    else {
        ui->pir_value->setText("Detectado");
    }

    if(dados.contains("LED01"))
        ui->lbl_fechadura->setEnabled(dados["LED01"].toBool());
    else {
        ui->lbl_fechadura->setText("Travado");
    }
    if(dados.contains("LED02")==1)
        ui->luz_status->setText("Ligado");
    else {
        ui->luz_status->setText("Desligado");
    }
}

void MainWindow::on_btn_ligarluz_clicked()
{
    serial.write("{\"LED02\":1}");
}

void MainWindow::on_btn_desligarluz_clicked()
{
    serial.write("{\"LED02\":0}");
}
