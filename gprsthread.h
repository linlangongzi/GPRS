#ifndef GPRSTHREAD_H
#define GPRSTHREAD_H

#include <QThread>
#include"serial/posix_qextserialport.h"
class gprsThread : public QThread
{
    Q_OBJECT
public:
    explicit gprsThread(QObject *parent = 0);
    void stop();
    void SendMSG(QString number, QString message);
    void gprs_hold();
    void gprs_call(char *number, int num);

signals:
    void signalGprsData(char *,char *);

public slots:

protected:
    void run();

private:
void gprs_init();
    bool stopflag;
    Posix_QextSerialPort *gprscom;
    void gprs_read_message(int index);
    //void gprs_hold();
    void gprs_ans();
    //void gprs_call(char *number, int num);


};

#endif // GPRSTHREAD_H
