#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QDebug>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QLineEdit>

//#include "socks.c"
#include "sv_can.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void init();

    SvCAN_Writer* can_writer[2];
//    SvCAN_Writer *can0_writer;

    SvCAN_Reader* can_reader[2];

//    SvCAN_Writer *can1_writer;
//    SvCAN_Reader* can1_reader;

    can_frame* frame[2];
//    struct can_frame* frame0;
//    struct can_frame* frame1;

private:
    Ui::MainWindow *ui;

private slots:
    void can_data();
//    void can1_data();

    void can_send();
//    void can1_send();

    void startCan();
    void stopCan();

    QByteArray can_status(QString dev_name);
    void can_up(QString dev_name);
    void can_down(QString dev_name);

};

#endif // MAINWINDOW_H
