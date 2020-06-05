#include "widget.h"
#include "ui_widget.h"

#include <QString>
#include <QByteArray>
#include <QStringList>
#include "ciprotal.h"

#include <QFile>
#include <QHBoxLayout>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->pushButton ,SIGNAL(clicked()),this,SLOT(slotConnect()));
    connect(ui->pushButton_2 ,SIGNAL(clicked()),this,SLOT(startcollection()));

    connect(ui->pushButton_3 ,SIGNAL(clicked()),this,SLOT(stopcollecton()));

    connect(ui->pushButton_4 ,SIGNAL(clicked()),this,SLOT(slotsavecollecton()));

    for (int i =0 ;i<10;i++)
    {
        ui->comboBox->addItem(QString::number(160+i));
    }

    mdrawline = new DrawLine(ui->widget);

    QHBoxLayout *layout = new QHBoxLayout(ui->widget);
    layout->addWidget(mdrawline);
    layout->setSpacing(0);
    layout->setMargin(0);

    this->resize(1200,500);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::slotConnect()
{
    QString strip = ui->lineEdit->text();

    /*QByteArray strb = strip.toLatin1();
        char *strc = strb.data();

        if (remote_new_connect(strc) >= 0 )
        {
        ui.label->setText(QString::fromLocal8Bit("成功"));
        }
        else
        {
        ui.label->setText(QString::fromLocal8Bit("失败"));
        }*/

    QStringList strlist = strip.split(".");

    char ipchar[4];
    ipchar[0] = strlist[0].toInt();
    ipchar[1] = strlist[1].toInt();
    ipchar[2] = strlist[2].toInt();
    ipchar[3] = strlist[3].toInt();

    if(OnLinkMachine(0,ipchar[0],ipchar[1],ipchar[2],ipchar[3])== false)
    {
        ui->label->setText("失败");
    }
    else
    {
        ui->label->setText("成功");

        long   addr[10];
        addr[0] = 160;
        addr[1] = 161;
        addr[2] = 162;
        addr[3] = 163;
        addr[4] = 164;
        addr[5] = 165;
        addr[6] = 166;
        addr[7] = 167;
        addr[8] = 168;
        addr[9] = 169;

        if( OnRegCiValue(0,addr,10,0.002,0.002) == false)
        {
            printf("OnRegCiValue ERROR\n");
            return;
        }
    }
}

void Widget::startcollection()
{
    pointlist.clear();
    numbers = 0;
    CInum = ui->comboBox->currentIndex();
    timeid = startTimer(10);
}

void Widget::stopcollecton()
{
    killTimer(timeid);
}

void Widget::slotsavecollecton()
{
    QFile file("num.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    QString xstr;
    double pingjun=0;
    double maxnum = -100;
    double minnum = 10000;

    for (int i =0;i<pointlist.size();i++)
    {
        xstr+="X"+QString::number(pointlist[i])+"\n";
        pingjun+=pointlist[i];
        if (pointlist[i]>maxnum)
        {
            maxnum = pointlist[i];
        }
        if (pointlist[i]<minnum)
        {
            minnum = pointlist[i];
        }
    }

    QString sss = "max:";
    xstr += "\n\n";
    xstr +="max:";
    xstr +=QString::number(maxnum);
    xstr +="\n";
    xstr +="min:";
    xstr +=QString::number(minnum);
    xstr +="\n";
    xstr +="pingjun:";
    xstr +=QString::number(pingjun/pointlist.size());
    xstr +="\n";

    file.write(xstr.toLatin1().data());
    file.close();
}

void Widget::timerEvent(QTimerEvent *)
{
    double v[10];
    unsigned long c;
    int ret = OnGetCiValue(0,v,&c,10);
    if (ret != 0)
    {
        pointlist.push_back(v[CInum]);
        mdrawline->setData(v[CInum]);
        numbers++;
        ui->label->setText(QString::number(numbers));
        ui->label_2->setText("差值："+QString::number(mdrawline->getcha()));
    }
}

void Widget::on_pushButton_5_clicked()
{
    mdrawline->test();
}
