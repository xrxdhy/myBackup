#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "ui_widget.h"

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

private:
    Ui::Widget *ui;
    QVector<double> regenData;
    QVector<double> ticks;
    QCPBars *regen;
    int i = 10;

    QVector<QString> labels;
    QSharedPointer<QCPAxisTickerText> textTicker =
         QSharedPointer<QCPAxisTickerText>(new QCPAxisTickerText);
};

#endif // WIDGET_H
