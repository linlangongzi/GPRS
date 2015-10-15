#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"gprsthread.h"
#include"QCloseEvent"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
void closeEvent(QCloseEvent *e);

private slots:

void slotGprsData(char *snum,char*stext);
void on_pbddial_clicked();

void on_pbhold_clicked();

void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
        gprsThread *gth;
};

#endif // MAINWINDOW_H
