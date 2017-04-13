#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->bnCan0Send, SIGNAL(clicked(bool)), this, SLOT(can_send()));
    connect(ui->bnCan1Send, SIGNAL(clicked(bool)), this, SLOT(can_send()));

    connect(ui->bnStartCan0, SIGNAL(clicked(bool)), this, SLOT(startCan()));
    connect(ui->bnStartCan1, SIGNAL(clicked(bool)), this, SLOT(startCan()));
    connect(ui->bnStopCan0, SIGNAL(clicked(bool)), this, SLOT(stopCan()));
    connect(ui->bnStopCan1, SIGNAL(clicked(bool)), this, SLOT(stopCan()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    /** **** проверяем состояние CAN портов и запускаем их **** **/
    ui->bnStartCan0->click();
    ui->bnStartCan1->click();
}

void MainWindow::can_data()
{
    SvCAN_Reader* cr = (SvCAN_Reader*)sender();
    QLineEdit *ed;
    QString dev_name = "";
    int id = -1;

    id = cr->id;
    if(id == -1) return;

    dev_name = "can" + QString::number(id);

    ed = id == 0 ? ui->editCan0Id : ui->editCan1Id;
    if(frame[id]->can_id == ed->text().toInt())
        return;

    QString data = "";
    for (int i = 0; i < frame[id]->can_dlc; i++)
        data += QString::number(frame[id]->data[i], 16);

    QString recv = QString("id=%1;  cout=%2;  data=%3")
                        .arg(int(frame[id]->can_id) & CAN_EFF_MASK)
                        .arg(frame[id]->can_dlc)
                        .arg(data);

    ed = id == 0 ? ui->editCan0Recv : ui->editCan1Recv;
    ed->setText(recv);
}

void MainWindow::can_send()
{
    QPushButton* bn = (QPushButton*)sender();
    QLineEdit *ed;
    QString dev_name = "";
    int id = -1;

    if(bn->objectName().toLower().contains("can0")) id = 0;
    else if(bn->objectName().toLower().contains("can1")) id = 1;

    if(id == -1) return;

    dev_name = "can" + QString::number(id);

    ed = id == 0 ? ui->editCan0Id : ui->editCan1Id;

    bool ok = false;
    quint32 can_id = ed->text().toInt(&ok);

    if(!ok)
    {
       QMessageBox::critical(this, "Error", "Wrong CAN ID");
       return;
    }

    ed = id == 0 ? ui->editCan0Data : ui->editCan1Data;
    QByteArray b = QByteArray::fromHex(ed->text().toLatin1());

    can_writer[id]->writeData(can_id, b);

}

void MainWindow::can_up(QString dev_name)
{
    QStringList args = QStringList();
    QProcess* p = new QProcess(this);

    args << "/home/user/proj/up.py";
    args << dev_name;

    p->start("/usr/bin/python", args);

    if(p->waitForStarted())
    {
        p->waitForReadyRead();
//        qDebug() << p->readAllStandardOutput();
        p->close();
    }
    else
    {
        p->close();
        perror("python not started");
    }

    delete p;
}


void MainWindow::can_down(QString dev_name)
{
    QStringList args = QStringList();
    QProcess* p = new QProcess(this);

    args << "/home/user/proj/down.py";
    args << dev_name;

    p->start("/usr/bin/python", args);

    if(p->waitForStarted())
    {
        p->waitForReadyRead();
//        qDebug() << p->readAllStandardOutput();
        p->close();
    }
    else
    {
        p->close();
        perror("python not started");
    }

    delete p;
}

QByteArray MainWindow::can_status(QString dev_name)
{
    QStringList args = QStringList();
    QByteArray result = QByteArray();
    QProcess* p = new QProcess(this);

    args << "/home/user/proj/status.py";
    args << dev_name;

    p->start("/usr/bin/python", args);

    if(p->waitForStarted())
    {
        p->waitForReadyRead();
        result = p->readAllStandardOutput();
        p->close();
    }
    else
    {
        p->close();
        perror("python not started");
    }

    delete p;

    return result;
}

void MainWindow::startCan()
{
    QPushButton* bn = (QPushButton*)sender();
    QString dev_name = "";
    int id = -1;

    if(bn->objectName().toLower().contains("can0")) id = 0;
    else if(bn->objectName().toLower().contains("can1")) id = 1;

    if(id == -1) return;

    dev_name = "can" + QString::number(id);

    can_up(dev_name);

    QByteArray b = can_status(dev_name);
    qDebug() << b;

    bool stat = b.contains(",UP,");

    /* включаем контролы для CAN портов */
    QGroupBox* gb = id == 0 ? ui->gbCan0 : ui->gbCan1;

    foreach(QObject *w, gb->children())
      if(w->isWidgetType())
        ((QWidget*)(w))->setEnabled(stat);

    bn->setEnabled(!stat);

    if(stat)
    {
        can_writer[id] = new SvCAN_Writer(this, id);
        can_writer[id]->init(dev_name);

        frame[id] = (struct can_frame*)malloc(sizeof(can_frame));

        can_reader[id] = new SvCAN_Reader(id);
        can_reader[id]->init(dev_name, frame[id]);

        connect(can_reader[id], SIGNAL(dataReceived()), this, SLOT(can_data()));
        connect(can_reader[id], SIGNAL(finished()), can_reader[id], SLOT(deleteLater()));
    }
}

void MainWindow::stopCan()
{
    QPushButton* bn = (QPushButton*)sender();
    QString dev_name = "";
    int id = -1;

    if(bn->objectName().toLower().contains("can0")) id = 0;
    else if(bn->objectName().toLower().contains("can1")) id = 1;

    if(id == -1) return;

    dev_name = "can" + QString::number(id);

    can_down(dev_name);

    QByteArray b = can_status(dev_name);
    qDebug() << b;

    bool stat = b.contains(",UP,");

    /* выключаем контролы для CAN портов */
    QGroupBox* gb = id == 0 ? ui->gbCan0 : ui->gbCan1;
    foreach(QObject *w, gb->children())
      if(w->isWidgetType())
        ((QWidget*)(w))->setEnabled(stat);

    bn = id == 0 ? ui->bnStartCan0 : ui->bnStartCan1;
    bn->setEnabled(!stat);

    if(!stat)
    {
        can_writer[id]->~SvCAN_Writer();
        delete(frame[id]);
//        can_reader[id]->terminate(); // поток убивается когда останавливается устройство
    }
}
