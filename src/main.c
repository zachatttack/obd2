#include <stdio.h>
#include <string.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

void create_frame(struct can_frame *frame);

void *read_can(void *vargp)
{

    int s,nbytes;
    struct sockaddr_can addr;
    struct ifreq ifr;

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    strcpy(ifr.ifr_name, "can0" );
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    bind(s, (struct sockaddr *)&addr, sizeof(addr));

    struct can_frame frame;

    struct can_filter rfilter[8];

    rfilter[0].can_id   = 0x7E8;
    rfilter[1].can_id   = 0x7E9;
    rfilter[2].can_id   = 0x7EA;
    rfilter[3].can_id   = 0x7EB;
    rfilter[4].can_id   = 0x7EC;
    rfilter[5].can_id   = 0x7ED;
    rfilter[6].can_id   = 0x7EE;
    rfilter[7].can_id   = 0x7EF;
    for (int i=0;i<8;i++){
    rfilter[i].can_mask = CAN_SFF_MASK;
    }

    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

    while(1){

        nbytes = read(s, &frame, sizeof(struct can_frame));

        printf("%0X ", frame.can_id);
        for (int i=0; i < frame.can_dlc; i++){
            printf("%02X ", frame.data[i]);
        }
        printf("\n");

    }

}

void *write_can(void *vargp)
{
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    strcpy(ifr.ifr_name, "can0" );
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    bind(s, (struct sockaddr *)&addr, sizeof(addr));

    struct can_frame frame;

    while(1){
        create_frame(&frame);
        write(s, &frame, sizeof(struct can_frame));
        usleep(100000); //100ms
    }

}
void create_frame(struct can_frame *frame){
    frame->can_dlc = 8;
    frame->can_id = 0x7DF;
    u_int8_t data[] = {0x02, 0x01, 0x11, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC};
    memcpy(frame->data, data, sizeof(data));
}

void can_init(){



}

int main(int argc, char **argv)
{
    pthread_t thread_id, thread_id2;
    pthread_create(&thread_id, NULL, read_can, NULL);
    pthread_create(&thread_id2, NULL, write_can, NULL);
    pthread_join(thread_id, NULL);
    pthread_join(thread_id2, NULL);

    return 0;
}
