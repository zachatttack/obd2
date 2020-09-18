#include <stdio.h>
#include <string.h>
#include <linux/can.h>
#include <linux/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>


int main(int argc, char **argv)
{

    int s,nbytes;
    struct sockaddr_can addr;
    struct ifreq ifr;

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    strcpy(ifr.ifr_name, "vcan0" );
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    bind(s, (struct sockaddr *)&addr, sizeof(addr));

    struct can_frame frame;


    while(1){
        nbytes = read(s, &frame, sizeof(struct can_frame));

        if (nbytes < 0) {
            perror("can raw socket read");
            return 1;
        }

        /* paranoid check ... */
        if (nbytes < sizeof(struct can_frame)) {
            fprintf(stderr, "read: incomplete CAN frame\n");
            return 1;
        }
        for (int i=0; i < frame.can_dlc; i++){
            printf("%X", frame.data[i]);
        }
        printf("\n");
    }

    return 0;
}
