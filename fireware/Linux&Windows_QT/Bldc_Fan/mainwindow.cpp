#include "mainwindow.h"
#include "ui_mainwindow.h"

#define IS_ARM_IMX_6ULL false

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(IS_ARM_IMX_6ULL)
    {
        this->setGeometry(0,0,QApplication::desktop()->screenGeometry().width(),QApplication::desktop()->screenGeometry().height());
        this->setWindowFlags(Qt::FramelessWindowHint);
        this->setStyleSheet("QMainWindow{background-image: url(./icon/qtbg.jpg);}");
        ui->label->setStyleSheet("QLabel{background-image: url(./icon/bilibilihead.png);}");
    }
    else
    {
        //this->setGeometry(0,0,800,480);
        this->setStyleSheet("QMainWindow{background-image: url(:/icon/qtbg.jpg);}");
        ui->label->setStyleSheet("QLabel{background-image: url(:/icon/bilibilihead.png);}");
    }

    serialPort = new QSerialPort(this);
    serialPort->setPortName("/dev/ttymxc2");
    serialPort->setBaudRate(115200);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    if(!serialPort->open(QIODevice::ReadWrite))
    {
        printf("SerialPort open error!!!\n");
    }
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(serialPortReadyRead()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::SendFOCData(QByteArray buf)
{
    //
    serialPort->write(buf);
    return true;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->label_8->setText(QString("%1 %2").arg("速度:").arg(value));
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->label_9->setText(QString("%1 %2").arg("角度:").arg(value));
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    ui->label_10->setText(QString("%1 %2").arg("幅度:").arg(value));
}

void MainWindow::on_pushButton_clicked()
{
    //
}

void MainWindow::on_pushButton_2_clicked()
{
    //
}

void MainWindow::serialPortReadyRead()
{
    QByteArray buf = serialPort->readAll();
    (void)buf;
}
