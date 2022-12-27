#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_buscar_arduino_clicked();

    void on_apagar_button_clicked(bool checked);

    void on_botonDisparo_clicked(bool checked);

    void on_botonRafaga_clicked(bool checked);

    void on_botonRecarga_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_UNO = 66;
    static const quint16 arduino_MEGA = 67;
    static const quint16 arduino_LEONARDO = 32822;
    static const quint16 ESP32 = 60000;
    QString arduino_puerto;
    bool arduino_esta_disponible;
    void buscaYconectaArduino();
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};
#endif // MAINWINDOW_H
