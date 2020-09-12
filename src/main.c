#include <stdio.h>
#include <linux/can.h>
#include <sys/ioctl.h>
#include <sys/socket.h>


int main(int argc, char **argv)
{

    int s;

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);



    return 0;
}
