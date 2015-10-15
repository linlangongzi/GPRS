#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gth=new gprsThread(this);

    connect(gth,SIGNAL(signalGprsData(char *,char*)),this,SLOT(slotGprsData(char *,char*)));

    gth->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::closeEvent(QCloseEvent *e)
{
    if(gth->isRunning())
        gth->stop();

    gth->wait();


    e->accept();
}

void MainWindow::on_pbddial_clicked()
{
    QString num=ui->lesendnum->text();
    gth->gprs_call((char*)num.toStdString().c_str(),num.size());
}

void MainWindow::on_pbhold_clicked()
{
    gth->gprs_hold();
}

void MainWindow::slotGprsData(char *snum, char *stext)
{
    qDebug()<<"gprs recv:"<<snum<<stext;
    ui->lerecvnum->setText(QString(snum));
    ui->terecvtext->insertPlainText(QString(stext));
}
void MainWindow::on_pushButton_clicked()
{
    QString num=ui->lesendnum->text();
    QString text=ui->lesendtext->text();
    gth->SendMSG(num,text);
}
