#!/bin/bash

if [ $# -eq 1 ] ; then
  DEV=$1
else
  DEV=/dev/ttyUSB0
fi

stty -F $DEV cs8 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts

TAIL_PROC=`ps aux | grep -v grep | grep "tail -f $DEV"`
if [ $? -eq 1 ] ; then
  tail -f $DEV &
fi

