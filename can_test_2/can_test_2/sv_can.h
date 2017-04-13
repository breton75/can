#ifndef SVCAN_H
#define SVCAN_H



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <net/if.h>
#include <sys/types.h>

//#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include <QObject>
#include <QThread>

/* ******** WRITER ************ */
class SvCAN_Writer: public QObject
{
    Q_OBJECT

public:
    explicit SvCAN_Writer(QObject *parent, int id);
    ~SvCAN_Writer();

    int init(QString dev_name);
    int writeData(quint32 id, QByteArray data);

    int id = -1;
    int sock = 0;
    struct sockaddr_can addr;
    struct can_frame frame;
    struct ifreq ifr;

};

/* ******** READER ************ */
class SvCAN_Reader : public QThread
{
    Q_OBJECT

public:
    explicit SvCAN_Reader(int id);

    int init(QString dev_name, can_frame *out);

    int id = -1;
    int sock = 0;
    struct sockaddr_can addr;
    struct can_frame frame;
    struct ifreq ifr;

    can_frame* _out;

    void run() Q_DECL_OVERRIDE;

signals:
    void dataReceived();
    void stepped();

};

#endif // SVCAN_H
