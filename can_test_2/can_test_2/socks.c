
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

int can1_create_sock(void)
{
    int s;
    int s1;
    int nbytes;
    struct sockaddr_can addr;
    struct sockaddr_can addr1;
    struct can_frame frame;
    struct ifreq ifr;

    char *ifname = "can1";
    char *ifname1 = "can0";

    if((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Error while opening socket");
        return -1;
    }

    if((s1 = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Error while opening socket1");
        return -1;
    }

    //0
    strcpy(ifr.ifr_name, ifname);
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    printf("%s at index %d\n", ifname, ifr.ifr_ifindex);

    //1
    strcpy(ifr.ifr_name, ifname1);
    ioctl(s1, SIOCGIFINDEX, &ifr);

    addr1.can_family  = AF_CAN;
    addr1.can_ifindex = ifr.ifr_ifindex;
    printf("%s at index %d\n", ifname1, ifr.ifr_ifindex);


    if(bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Error in socket bind");
        return -2;
    }

    frame.can_id  = 0x123;
    frame.can_dlc = 4;
    frame.data[0] = 0x44;
    frame.data[1] = 0x55;
    frame.data[2] = 0x11;

    nbytes = write(s, &frame, sizeof(struct can_frame));

    printf("Wrote %d bytes\n", nbytes);

    if(bind(s1, (struct sockaddr *)&addr1, sizeof(addr1)) < 0) {
        perror("Error in socket 1 bind");
        return -2;
    }

    memset(&frame, 0, sizeof(frame));

    int i;
    while (1)
    {
        printf("nbytes=000000\n", nbytes);
        nbytes = read(s1, &frame, sizeof(struct can_frame));
        printf("nbytes=%d\n", nbytes);
        if (nbytes < 0)
        {
            perror("read");
            return 1;
        }
        else if (nbytes < sizeof(struct can_frame))
        {
            fprintf(stderr, "read: incomplete CAN frame\n");
            return 1;
        }
        else
        {
            if (frame.can_id & CAN_EFF_FLAG)
                printf("%8X  ", frame.can_id & CAN_EFF_MASK);
            else
                printf("%3X  ", frame.can_id & CAN_SFF_MASK);

            printf("[%d] ", frame.can_dlc);

            for (i = 0; i < frame.can_dlc; i++) {
                printf("%02X ", frame.data[i]);
            }
            if (frame.can_id & CAN_RTR_FLAG)
                printf("remote request");
            printf("\n");
            fflush(stdout);
        }
    }

    close(s);

    return 0;
}
