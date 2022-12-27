#include "mainwindow.h"
#include "qevent.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <cmath>
#include <QTime> // for delay

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = QCursor::pos();
    //qDebug() << "X: " << pos.x() << ", Y: " << pos.y();
    if(arduino_esta_disponible && arduino->isWritable())
    {
        QString s = "x";
        s += QString::number(pos.x());
        s += "*";
        s += "y";
        s += QString::number(pos.y());
        s += "*";
        arduino->write(s.toUtf8());
        qDebug() << s.toUtf8();
    }
    QWidget::mouseMoveEvent(event);
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
}

void MainWindow::on_buscar_arduino_clicked()
{
    buscaYconectaArduino();
}


void MainWindow::on_apagar_button_clicked(bool checked)
{
    qApp->exit();
}


void MainWindow::on_botonDisparo_clicked(bool checked)
{
    if(arduino_esta_disponible && arduino->isWritable())
    {
         if(!checked)
         {
             QString s = "d";
             s += "*";
             arduino->write(s.toUtf8());
             qDebug() << s.toUtf8();
         }
         delay(500);
     }
}

void MainWindow::on_botonRafaga_clicked(bool checked)
{
    if(arduino_esta_disponible && arduino->isWritable())
    {
         if(!checked)
         {
             QString s = "a";
             s += "*";
             arduino->write(s.toUtf8());
             qDebug() << s.toUtf8();
         }
     }
}
void MainWindow::on_botonRecarga_clicked(bool checked)
{
    if(arduino_esta_disponible && arduino->isWritable())
    {
         if(!checked)
         {
             QString s = "r";
             s += "*";
             arduino->write(s.toUtf8());
             qDebug() << s.toUtf8();
         }
     }
}

void MainWindow::buscaYconectaArduino(){
    //Parte # 1, declaración inicial de las variables
    arduino_esta_disponible = false;
    arduino_puerto = "";
    arduino = new QSerialPort;
    QString nombreDispositivoSerial = "";
    int nombreProductID = 0;

    //Parte # 2,buscar puertos con los identificadores de Arduino
    qDebug() << "Puertos disponibles: " << QSerialPortInfo::availablePorts().length();
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        qDebug() << "Identificador del fabricante (VENDOR ID): " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier()){
            qDebug() << "ID Vendedor " << serialPortInfo.vendorIdentifier();
            qDebug() << "ID Producto: " << serialPortInfo.productIdentifier();

            if(serialPortInfo.productIdentifier() == arduino_UNO || serialPortInfo.productIdentifier() == arduino_MEGA || serialPortInfo.productIdentifier() == arduino_LEONARDO
                    || serialPortInfo.productIdentifier() == ESP32){
                arduino_esta_disponible = true;
                nombreDispositivoSerial = serialPortInfo.portName();
                nombreProductID = serialPortInfo.productIdentifier();
            }
        }

    }

    //Parte # 3, conexión del puerto encontrado con Arduino

    if(arduino_esta_disponible){
        arduino_puerto = nombreDispositivoSerial;
        arduino ->setPortName(arduino_puerto);
        arduino->open(QIODevice::ReadWrite);
        arduino->setDataBits(QSerialPort::Data8);
        arduino ->setBaudRate(QSerialPort::Baud115200);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl); 
        ui->label->clear();
        qDebug() << "Producto: " << nombreProductID;
        if(nombreProductID == 67) ui->label->setText("Arduino UNO R3 conectado");
        else if(nombreProductID == 68) ui->label->setText("Arduino Mega conectado");
        else if(nombreProductID == 32822) ui->label->setText("Arduino Leonardo conectado");
        else if(nombreProductID == ESP32) ui->label->setText("ESP32 conectado");
        else ui->label->setText("Error 3");

    }
    else{
        ui->label->clear();
        ui->label->setText("No hay arduino");
    }
}




