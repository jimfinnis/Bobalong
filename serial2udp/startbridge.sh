#!/bin/sh

if [ `hostname` = "minty2-control" ] ; then
    ./serial2udp /dev/serial/by-id/usb-9710_7840-if00-port3 9600 4323 4322 
else
    ./serial2udp /dev/ttyUSB0 9600 4322 4323
fi

