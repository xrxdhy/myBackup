#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
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
    ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7<< 8<< 9;
    labels << "time1" << "time2" << "time3" <<
              "time4" << "time5" << "time6" <<
              "time7"<< "time8"<<"time9";
    textTicker->addTicks(ticks, labels);

    ui->customPlot->xAxis->setTicker(textTicker);
    ui->customPlot->xAxis->setTickLabelRotation(60);//xuan zhuan jiao du
    ui->customPlot->xAxis->setSubTicks(false);
    ui->customPlot->xAxis->setTickLength(0, 4);
    ui->customPlot->xAxis->setRange(0, 10);
    ui->customPlot->xAxis->setBasePen(QPen(Qt::white));
    ui->customPlot->xAxis->setTickPen(QPen(Qt::white));
    ui->customPlot->xAxis->grid()->setVisible(true);
    ui->customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    ui->customPlot->xAxis->setTickLabelColor(Qt::white);
    ui->customPlot->xAxis->setLabelColor(Qt::white);

    //ui->customPlot->yAxis->setRange(0, 12.1);
    ui->customPlot->yAxis->setPadding(5); // a bit more space to the left border
    ui->customPlot->yAxis->setLabel("用于本次毕业设计");
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
    regen->setStackingGap(1);
    regen->setName("项目一");
    regen->setPen(QPen(QColor(0, 168, 140).lighter(130)));
    regen->setBrush(QColor(0, 168, 140));
    regenData   << 1 << 2 << 3 << 4 << 5 << 6 << 5<< 4 << 3;
    regen->setData(ticks,regenData);

    ui->customPlot->legend->setVisible(true);
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    ui->customPlot->legend->setBrush(QColor(255, 255, 255, 100));
    ui->customPlot->legend->setBorderPen(Qt::NoPen);//qu diao bian kuang
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->customPlot->legend->setFont(legendFont);
    //keyi yong gonglun kongzhi zuo biao zhou
    //ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);


//    QVector<double> x(101), y(101);
//    for(int i=0;i<101;i++)
//    {
//        x[i]=i/50.0-1;
//        y[i]=x[i]*x[i];
//    }
//    ui->customPlot->addGraph();
//    ui->customPlot->graph(0)->setData(x,y);
//    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
//    ui->customPlot->graph(0)->setPen(QPen(Qt::green));

//    //give some labels
//    ui->customPlot->xAxis->setLabel("x");
//    ui->customPlot->yAxis->setLabel("y");

//    //give the range
//    ui->customPlot->xAxis->setRange(-1,1);
//    ui->customPlot->yAxis->setRange(0,1);

    ui->customPlot->replot();
}

void Widget::on_pushButton_5_clicked()
{
    ticks << i++;
    labels << "addhere";
    textTicker->addTicks(ticks, labels);
    ui->customPlot->xAxis->setTicker(textTicker);
    ui->customPlot->xAxis->setRange(0, i++);

    //regenData.removeFirst();
    regenData << regenData.last()+2;
    ui->customPlot->yAxis->setPadding(5);
    ui->customPlot->yAxis->setRange(0, regenData.last());

    regen->setData(ticks,regenData);
    ui->customPlot->replot();
}
