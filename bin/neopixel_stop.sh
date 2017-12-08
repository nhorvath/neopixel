#!/bin/bash

if [ $# -eq 1 ] ; then
  DEV=$1
else
  DEV=`ls /dev/ttyUSB[0-9]* | head -1`
fi

TAIL_PROC=`ps aux | grep -v grep | grep "tail -f $DEV"`
if [ $? -eq 0 ] ; then
  TAIL_PID=`echo $TAIL_PROC | awk '{ print $2; }'`
  if [ "x$TAIL_PID" != "x" ] ; then
    kill $TAIL_PID
  fi
fi

