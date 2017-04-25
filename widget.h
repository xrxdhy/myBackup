#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "ui_widget.h"
#include <QSerialPort>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void InitMyPlot();

private slots:
    void on_pushButton_5_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void updateMyPlot();

    void on_pushButton_6_clicked();
    void handleReadyRead();

private:
    int i = 20;
    int check = 1;
    int stopFlag = 0;
    int maxRange_temp = 0;
    int maxRange_damp = 0;
    int maxRange_light = 0;
    int maxRange_dust = 0;
    Ui::Widget *ui;
    QVector<double> tempData;
    QVector<double> dampData;
    QVector<double> lightData;
    QVector<double> dustData;
    QVector<double> regenData;


    QVector<double> ticks;
    QCPBars *regen;
    QTimer *timer;
    QVector<QString> labels;
    QSharedPointer<QCPAxisTickerText> textTicker =
         QSharedPointer<QCPAxisTickerText>(new QCPAxisTickerText);
    QSerialPort serialPort;
    QByteArray readData;
};

#endif // WIDGET_H
