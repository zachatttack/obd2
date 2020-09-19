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

void create_frame(struct can_frame *frame);

void *read_can(void *vargp)
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

    struct can_frame frame_r, frame_w;

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

        nbytes = read(s, &frame_r, sizeof(struct can_frame));

        printf("%0X ", frame_r.can_id);
        for (int i=0; i < frame_r.can_dlc; i++){
            printf("%02X ", frame_r.data[i]);
        }
        printf("\n");

    }

}

void *write_can(void *vargp)
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

    struct can_frame frame_r, frame_w;

    struct can_filter rfilter[8];

    rfilter[0].can_id   = 0x7E8;
    rfilter[1].can_id   = 0x7E9;
    rfilter[2].can_id   = 0x7EA;
    rfilter[3].can_id   = 0x7EB;
    rfilter[4].can_id   = 0x7EC;
    rfilter[5].can_id   = 0x7ED;
    rfilter[6].can_id   = 0x7EE;
    rfilter[7].can_id   = 0x7EF;
    for (int i=0;i<9;i++){
    rfilter[i].can_mask = CAN_SFF_MASK;
    }

    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

    while(1){

        create_frame(&frame_w);
        write(s, &frame_w, sizeof(struct can_frame));
    }

}
void create_frame(struct can_frame *frame){
    frame->can_dlc = 8;
    frame->can_id = 0x7DF;
    u_int8_t data[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xB, 0xEF};
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
