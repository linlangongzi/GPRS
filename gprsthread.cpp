#include "gprsthread.h"
#include"QDebug"
gprsThread::gprsThread(QObject *parent) :
    QThread(parent)
{
    stopflag=false;
    //QString portname="/dev/ttyS3";
    QString portname="/dev/ttySAC2";

    gprscom=new Posix_QextSerialPort(portname,
                                   QextSerialBase::Polling);
    gprscom->open(QIODevice::ReadWrite);//open
    //set attribute of serial
    gprscom->setBaudRate(BAUD115200);
    gprscom->setDataBits(DATA_8);
    gprscom->setParity(PAR_NONE);
    gprscom->setStopBits(STOP_1);
    gprscom->setFlowControl(FLOW_OFF);
    gprscom->setTimeout(10);

    gprs_init();

    //gprscom->writeData("AT+CMGR=3\r", strlen("AT+CMGR=3\r"));
    gprscom->write("AT+CMGR=4\r", strlen("AT+CMGR=3\r"));

}
void gprsThread::stop()
{
    stopflag=true;

}


void gprsThread::SendMSG(QString number, QString message)
{
    char ctl[]={26,0}; //ctl[0]=26,ctrl+z
    if(gprscom != NULL){
        gprscom->write("at+cmgs=",strlen("at+cmgs="));
        gprscom->write("\"",1);
        gprscom->write(number.toStdString().c_str(),number.size());
        gprscom->write("\"",1);
        gprscom->write(";\r",2);
        gprscom->write(message.toStdString().c_str(),message.size());
        gprscom->write(ctl,1);
    }
}





void gprsThread::gprs_init()
{
    //gprs初始化
    qDebug()<<"init gprs start";
    gprscom->write("AT\r", strlen("at\r"));//测试GPRS模块状态
    usleep(1000);
    gprscom->write("ATE1\r", strlen("ate1\r"));//guanbi命令回显功能
    usleep(1000);
    gprscom->write("AT+CLVL=100\r", strlen("at+clvl=100\r"));	//设置输出音频信号增益：100，最大音量
    usleep(1000);
   gprscom->write("AT+CMGF=1\r", strlen("at+cmgf=1\r"));
    usleep(1000);
    gprscom->write("AT+CLIP=1\r", strlen("at+clip=1\r"));
    usleep(1000);
    gprscom->write("AT+CSCS=\"GSM\"\r", strlen("at+cscs=\"GSM\"\r"));
    usleep(1000);

    gprscom->write("AT+CNMI=2,1,0,0,0\r", strlen("at+cnmi=2,1,0,0,0\r"));
    usleep(1000);
    qDebug()<<"init gprs over";
}

void gprsThread::gprs_read_message(int index)
{
    char Cmd_Buffer[13]="at+cmgr=";

    char buffer[3]={'\0'};
    gcvt((double)index, 2, buffer);

    strcat(Cmd_Buffer,buffer);
    strcat(Cmd_Buffer,"\r");

    printf("sending----",Cmd_Buffer);
    gprscom->write(Cmd_Buffer, strlen(Cmd_Buffer));
}

void gprsThread::gprs_hold()
{
    qDebug("start hold");
    char revbuffer[20];
        gprscom->write("at\r", strlen("at\r"));
        gprscom->read(revbuffer,20);
        if(strstr(revbuffer,"OK") == 0 )
            printf("error\n");
        gprscom->write("ath\r", strlen("ath\r"));
        gprscom->read(revbuffer,20);
        if(strstr(revbuffer,"OK") == 0 )
            printf("error\n");
}

void gprsThread::gprs_ans()
{
    qDebug("start answer");
    char revbuffer[20];
        gprscom->write("at\r", strlen("at\r"));
        gprscom->read(revbuffer,20);
        if(strstr(revbuffer,"OK") == 0 )
            printf("error\n");
        gprscom->write("ata\r", strlen("ata\r"));
        gprscom->read(revbuffer,20);
        if(strstr(revbuffer,"OK") == 0 )
            printf("error\n");
}

void gprsThread::gprs_call(char *number, int num)
{
    qDebug("start call");
    char revbuffer[20];
    gprscom->write("atd", strlen("atd"));
    gprscom->write(number, num);
    gprscom->write(";\r", strlen(";\r"));
    int ret;
    for(;;){
         ret = gprscom->read(revbuffer,20);
        if(ret > 0){
                break;
        }
        }
    if(strstr(revbuffer,"NO ANSWER") != 0){
        printf("error\n");
        return ;
    }
    if(strstr(revbuffer,"NO CARRIER") != 0){
        printf("error\n");
        return ;
    }
    if(strstr(revbuffer,"BUSY") != 0){
        printf("error\n");
        return ;
    }
    if(strstr(revbuffer,"+COLP") != 0){
        printf("connect ok\n");
        gprscom->read(revbuffer,20);
        if(strstr(revbuffer,"OK") == 0){
            printf("error\n");
        }
    }
    qDebug("call end");
        //usleep(200000);
}

void gprsThread::run()
{   QString datatosend;

    char c;
    char tmp[5];
    char msgindex_c[4];//“ę¶ĢŠÅŌŚsimæØÖŠµÄĖ÷ŅżÖµ
    int msgindex_i;
    char telenum[16];
    char msgtime[23];
    char msgcontent[200];
    int i;
    while(!stopflag)
    {

       gprscom->read(&c,1);
        qDebug()<<"c="<<c;
        if(c=='+')
        {

            gprscom->read(tmp,4);

            tmp[4]='\0';
            qDebug()<<"tmp="<<tmp;
            if(strcmp(tmp,"CMTI")==0)//ÓŠŠĀ¶ĢĻūĻ¢Ą“  +CMTI:"SM",1
            {

                gprscom->read(&c,1);
                while(c!=',')
                {
                    qDebug()<<"c="<<c;
                   gprscom->read(&c,1);
                }
                gprscom->read(msgindex_c,2);


                msgindex_c[2]='\0';
                qDebug()<<"messageindexccc="<<msgindex_c;

                msgindex_i=atoi(msgindex_c);
                qDebug()<<"messageindexiiii="<<msgindex_i;
                gprs_read_message(msgindex_i);

                msleep(1000);

            }
            else if(strcmp(tmp,"CMGR")==0)//½ÓŹÕ¶ĢĻūĻ¢ÄŚČŻ
            {
                gprscom->read(&c,1);
                if(c=='=')
                    continue;

                while(c!=',')
                {
                    qDebug()<<",,c="<<c;
                    printf(",,c=%c\n",c);
                    gprscom->read(&c,1);

                }                             //µ½“Ė¶Įµ½µŚŅ»øö¶ŗŗÅ
                gprscom->read(&c,1);

                printf("000c=%c\n",c);
                gprscom->read(&c,1);

                i=0;
                while(c!='\"')
                {
                    printf("111c=%c\n",c);
                    telenum[i++]=c;
                    gprscom->read(&c,1);
                }
                telenum[i]='\0';//µ½“Ė¶ĮĶźµē»°ŗÅĀė¼°ŗó±ßµÄĖ«ŅżŗÅ
                gprscom->read(&c,1);//,
                gprscom->read(&c,1);//"
                gprscom->read(&c,1);//"
                gprscom->read(&c,1);//,
               gprscom->read(&c,1);//"

                gprscom->read(&c,1);
                i=0;
                while(c!='\"')
                {
                    printf("222c=%c  i=%d\n",c,i);
                    msgtime[i++]=c;
                    gprscom->read(&c,1);
                }
                msgtime[i]='\0';//µ½“Ė¶ĮĶźµē»°ŗÅĀė¼°ŗó±ßµÄĖ«ŅżŗÅ

                qDebug()<<"msgtime="<<msgtime;

                gprscom->read(&c,1);//    \n
                gprscom->read(&c,1);//    \n



                i=0;
                gprscom->read(&c,1);

                while(c!='\n')
                {
                    printf("444c=%c\n",c);
                    msgcontent[i++]=c;
                    gprscom->read(&c,1);

                }//¶ĮĶź¶ĢŠÅŗó±ßµÄ»Ų³µ
                msgcontent[i]='\0';
                qDebug()<<"msgcontent="<<msgcontent;

                datatosend.clear();
                datatosend.append(telenum);
                datatosend.append("#");
                datatosend.append(msgcontent);
                qDebug()<<"datatosend="<<datatosend;
                emit signalGprsData(telenum,msgcontent);
            }
        }


        msleep(100);
    }

    stopflag=false;

}
