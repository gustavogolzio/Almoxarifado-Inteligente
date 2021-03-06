#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>        //biblioteca do Sql
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QSerialPort serial;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QIcon cadFechado; //Icone do Cadeado Fechado
    QIcon cadAberto;  //Icone do Cadeado Aberto
    static bool logado;      //Variavel que diz se há alguém logado
    static QString nome_colab, acesso_colab, username_colab;   //String de nome e estado do acesso
    static int id_colab;


private slots:
    void on_btn_bloquear_clicked();

    void on_pushButton_clicked();

    void on_actionEstoque_triggered();

    void on_actionColaboradores_triggered();

    void on_actionSair_triggered();

    void on_actionSobre_triggered();

    void on_pushButton_2_clicked();

    void on_btn_destravar_clicked();

    void on_btn_connect_clicked();

    void on_btn_disconnect_clicked();

    void dadosRecebidos();

    void on_btn_ligarluz_clicked();

    void on_btn_desligarluz_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
