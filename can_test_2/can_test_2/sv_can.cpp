#include "sv_can.h"
#include "sys/socket.h"

/* ******** WRITER ************ */
SvCAN_Writer::SvCAN_Writer(QObject *parent, int id):
    QObject(parent)
{
    this->id = id;
}

SvCAN_Writer::~SvCAN_Writer()
{
    close(sock);
    deleteLater();
}

int SvCAN_Writer::init(QString dev_name)
{
    const char *ifname = dev_name.toStdString().c_str();

    if((sock = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        perror("Error while opening socket");
        return -1;
    }

    strcpy(ifr.ifr_name, ifname);
    ioctl(sock, SIOCGIFINDEX, &ifr);

    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("Error in socket bind on init");
        return -2;
    }

}

int SvCAN_Writer::writeData(quint32 id, QByteArray data)
{
    frame.can_id = canid_t(id);
    frame.can_dlc = data.length();

    for(int i = 0; i < data.length(); i++)
        frame.data[i] = data[i];

    int nbytes = write(sock, &frame, sizeof(struct can_frame));

    if(data.length() != nbytes)
        return -1;
    else
        return nbytes;

}


/* ******** READER ************ */
SvCAN_Reader::SvCAN_Reader(int id)
{
    this->id = id;
}

int SvCAN_Reader::init(QString dev_name, can_frame* out)
{
    const char *ifname = dev_name.toStdString().c_str();
    this->_out = out;

    if((sock = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        perror("Error while opening socket");
        return -1;
    }

    strcpy(ifr.ifr_name, ifname);
    ioctl(sock, SIOCGIFINDEX, &ifr);

    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("Error in socket 1 bind");
        return -2;
    }

    this->start();
}

void SvCAN_Reader::run()
{
    while (1)
    {
        memset(&frame, 0, sizeof(frame));
        int nbytes = read(sock, &frame, sizeof(struct can_frame));

        if (nbytes < 0)
        {
            perror("read error");
            break;
        }
        else if (nbytes < sizeof(struct can_frame))
        {
            perror("read: incomplete CAN frame\n");
            break;
        }
        else
        {
            memset(_out, 0, sizeof(can_frame));
            memcpy(_out, &frame, nbytes);

            emit dataReceived();

            fflush(stdout);
        }
    }

    close(sock);
}
