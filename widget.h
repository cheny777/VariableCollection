#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimerEvent>
#include <QList>
#include <QPainter>
#include "drawline.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

    QList<double> pointlist;
    int timeid;

    int numbers;
    int CInum;

    DrawLine *mdrawline;

private slots:
    void slotConnect();
    void startcollection();
    void stopcollecton();
    void slotsavecollecton();

    void on_pushButton_5_clicked();

protected:
    void timerEvent(QTimerEvent  *);
};

#endif // WIDGET_H
