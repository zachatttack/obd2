#OBD2 data scanner

Using ELM 327 OBD2 port to usb (v1.5; clone)

CAN to Serial kernel module driver provided by https://github.com/norly/elmcan

## Instructions to use ELM 327 as a CAN interface

`git clone https://github.com/norly/elmcan`

`cd elmcan/modules`

`sudo make all #note linux-headers package must be installed on your system`

`sudo modprobe can-dev`

`sudo insmod elmcan.ko`

``./attach-me.sh /dev/ttyUSB0 #leave terminal open; start new terminal``

`sudo ip link set can0 up type can bitrate 500000 restart-ms 10`

`use can-utils candum to test connection`

