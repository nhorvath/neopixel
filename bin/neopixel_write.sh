#!/bin/bash

DEV=`ls /dev/ttyUSB[0-9] | head -1`
DIR=`dirname $0`

if [ $# -ne 1 ] ; then
  echo "Usage: $0 [string]"
  echo "Writes [string] to $DEV"
  exit 1
fi

$DIR/neopixel_init.sh $DEV

echo "$1" > $DEV
#send extra lines to clear buffers
echo "" > $DEV
sleep 1
echo "" > $DEV
