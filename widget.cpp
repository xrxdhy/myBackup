#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QDateTime>
#include <QDebug>

#define MAXBARS 30
#define GETDATA "1"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->InitMyPlot();
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateMyPlot()));
    timer->start(1000);

    /*Button*/
    ui->pushButton->setCheckable(true);
    ui->pushButton_2->setCheckable(true);
    ui->pushButton_3->setCheckable(true);
    ui->pushButton_4->setCheckable(true);

    ui->pushButton_2->setChecked(true);

    /*serialport*/
    serialPort.setPortName("/dev/ttyUSB0");
    serialPort.setBaudRate(QSerialPort::Baud9600);
    serialPort.setDataBits(QSerialPort::Data8);
    int findDev = 1;
    while(findDev)
    {
        if(!serialPort.open(QIODevice::ReadWrite))
        {
            QMessageBox msgBox;
            msgBox.setText("连接设备失败了_(:3 」∠)_");
            msgBox.setInformativeText("想再连一次吗？(ง •̀_•́)ง");
            msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Cancel);
            int ret = msgBox.exec();
            switch(ret)
            {
                case QMessageBox::Retry:
                    break;
                case QMessageBox::Cancel:
                    findDev = 0;
                    break;
            }
        }
        else
            break;
    }
    connect(&serialPort,SIGNAL(readyRead()),this,SLOT(handleReadyRead()));
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_clicked()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_2_clicked()));
    connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_3_clicked()));
    connect(ui->pushButton_4,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_4_clicked()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::handleReadyRead()
{
    /*get the new data from board*/
    QByteArray getData = serialPort.readAll();
    int temp = (getData[0]-'0') * 10 + (getData[1]-'0');
    qDebug() << "temp:" <<temp;

    int damp = (getData[2]-'0') * 10 + (getData[3]-'0');
    qDebug() << "damp:" <<damp;

    int light = (getData[4]-'0') * 100 + (getData[5]-'0') * 10 + (getData[6]-'0');
    qDebug() << "light:" <<light;

    int dust = (getData[7]-'0') * 100 + (getData[8]-'0') * 10 + (getData[9]-'0');
    qDebug() << "dust:" <<dust;

    tempData << temp;
    dampData << damp;
    lightData << light;
    dustData << dust;

    if(damp > maxRange_damp)
        maxRange_damp = damp;
    if(temp > maxRange_temp)
        maxRange_temp = temp;
    if(light > maxRange_light)
        maxRange_light = light;
    if(dust > maxRange_dust)
        maxRange_dust = dust;

    /*update the plot*/
    ticks << i;
    labels << QDateTime::currentDateTime().toString("hh:mm:ss");
    textTicker->addTicks(ticks, labels);
    if(!stopFlag)
    {
        ui->customPlot->xAxis->setTicker(textTicker);
        if(i < MAXBARS)
            ui->customPlot->xAxis->setRange(0, i);
        else
            ui->customPlot->xAxis->setRange(i-30,i);
        switch(check)
        {
        case 1:
            ui->customPlot->yAxis->setRange(0, maxRange_temp*4/3);
            regen->setData(ticks,tempData);
            break;
        case 2:
            ui->customPlot->yAxis->setRange(0, maxRange_damp*4/3);
            regen->setData(ticks,dampData);
            break;
        case 3:
            ui->customPlot->yAxis->setRange(0, maxRange_light*4/3);
            regen->setData(ticks,lightData);
            break;
        case 4:
            ui->customPlot->yAxis->setRange(0, maxRange_dust*4/3);
            regen->setData(ticks,dustData);
            break;

        }
        ui->customPlot->replot();
    }
    i++;
}

void Widget::InitMyPlot()
{
    /* backgrade */
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    ui->customPlot->setBackground(QBrush(gradient));



    /* prepare x axis with country labels: */
    ticks << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7<< 8<< 9
          << 10 << 11 << 12 << 13 << 14 << 15 << 16 << 17
          << 18<< 19;
    labels << " " << " " << " " <<
              " " << " " << " " <<
              " " << " " << " " <<
              " " << " " << " " <<
              " " << " " << " " <<
              " " << " " << " " <<
              " " << " ";

    textTicker->addTicks(ticks, labels);

    ui->customPlot->xAxis->setTicker(textTicker);
    ui->customPlot->xAxis->setTickLabelRotation(60);//xuan zhuan jiao du
    ui->customPlot->xAxis->setSubTicks(false);
    ui->customPlot->xAxis->setTickLength(0, 4);
    ui->customPlot->xAxis->setRange(0, MAXBARS);
    ui->customPlot->xAxis->setBasePen(QPen(Qt::white));
    ui->customPlot->xAxis->setTickPen(QPen(Qt::white));
    ui->customPlot->xAxis->grid()->setVisible(true);
    ui->customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    ui->customPlot->xAxis->setTickLabelColor(Qt::white);
    ui->customPlot->xAxis->setLabelColor(Qt::white);

    //ui->customPlot->yAxis->setRange(0, 12.1);
    ui->customPlot->yAxis->setPadding(5); // a bit more space to the left border
    ui->customPlot->yAxis->setLabel("°C摄氏度");
    ui->customPlot->yAxis->setBasePen(QPen(Qt::white));
    ui->customPlot->yAxis->setTickPen(QPen(Qt::white));
    ui->customPlot->yAxis->setSubTickPen(QPen(Qt::white));
    ui->customPlot->yAxis->grid()->setSubGridVisible(true);//xu xian
    ui->customPlot->yAxis->setTickLabelColor(Qt::white);
    ui->customPlot->yAxis->setLabelColor(Qt::white);
    ui->customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));//zhi xian
    ui->customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));//xu xian

    /*bars*/
    regen = new QCPBars(ui->customPlot->xAxis,ui->customPlot->yAxis);
    regen->setAntialiased(false);//柱形图的边缘 抗锯齿 平滑  关闭
    regen->setWidth(0.8);
    regen->setStackingGap(1);
    regen->setName("温度");
    regen->setPen(QPen(QColor(255, 102, 51).lighter(00)));
    regen->setBrush(QColor(255, 102, 51));


    tempData << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0
              << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0;
    dampData << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0
              << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0;
    lightData << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0
              << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0;
    dustData << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0
              << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0;

    ui->customPlot->legend->setVisible(true);
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    ui->customPlot->legend->setBrush(QColor(255, 255, 255, 100));
    ui->customPlot->legend->setBorderPen(Qt::NoPen);//qu diao bian kuang
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->customPlot->legend->setFont(legendFont);
    //keyi yong gonglun kongzhi zuo biao zhou
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    ui->customPlot->replot();
}

void Widget::updateMyPlot()
{
    serialPort.write(GETDATA);
}

void Widget::on_pushButton_5_clicked()
{
    if(!ui->lineEdit->text().isEmpty())
    {
        int checkTime;
        checkTime = ui->lineEdit->text().toInt();
        if(checkTime <= 0)
        {
            QMessageBox msg;
            msg.setText("周期必须大于0!");
            msg.exec();
            checkTime = 1;
        }
        timer->start(checkTime * 1000);
    }
}

void Widget::on_pushButton_6_clicked()
{
    if(stopFlag == 0)
    {
        ui->pushButton_6->setText("开始");
        stopFlag = 1;
    }
    else
    {
        ui->pushButton_6->setText("暂停");
        stopFlag = 0;
    }

}
void Widget::on_pushButton_4_clicked()
{
    ui->pushButton->setChecked(false);
    ui->pushButton_2->setChecked(false);
    ui->pushButton_3->setChecked(false);
    ui->pushButton_4->setChecked(true);
    ui->customPlot->yAxis->setLabel("Lx光照强度");
    regen->setName("光照强度");
    regen->setPen(QPen(QColor(238, 238, 0).lighter(0)));
    regen->setBrush(QColor(238, 238, 0));
    ui->customPlot->clearGraphs();
    check = 3;
}

void Widget::on_pushButton_3_clicked()
{
    ui->pushButton->setChecked(false);
    ui->pushButton_2->setChecked(false);
    ui->pushButton_3->setChecked(true);
    ui->pushButton_4->setChecked(false);
    ui->customPlot->yAxis->setLabel("%RH相对湿度(Relative Humidity)");
    regen->setName("湿度");
    regen->setPen(QPen(QColor(0, 168, 140).lighter(130)));
    regen->setBrush(QColor(0, 168, 140));
    ui->customPlot->clearGraphs();
    check = 2;
}

void Widget::on_pushButton_2_clicked()
{
    ui->pushButton->setChecked(false);
    ui->pushButton_2->setChecked(true);
    ui->pushButton_3->setChecked(false);
    ui->pushButton_4->setChecked(false);
    ui->customPlot->yAxis->setLabel("°C摄氏度");
    regen->setName("温度");
    regen->setPen(QPen(QColor(255, 102, 51).lighter(00)));
    regen->setBrush(QColor(255, 102, 51));
    ui->customPlot->clearGraphs();
    check = 1;
}

void Widget::on_pushButton_clicked()
{
    ui->pushButton->setChecked(true);
    ui->pushButton_2->setChecked(false);
    ui->pushButton_3->setChecked(false);
    ui->pushButton_4->setChecked(false);
    ui->customPlot->yAxis->setLabel("ug/m³微克每立方米");
    regen->setName("可吸入颗粒物");
    regen->setPen(QPen(QColor(139, 126, 102).lighter(130)));
    regen->setBrush(QColor(139, 126, 102));
    ui->customPlot->clearGraphs();
    check = 4;
}
