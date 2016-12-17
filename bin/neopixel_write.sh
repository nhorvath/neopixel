#!/bin/bash

DEV=/dev/ttyUSB0
DIR=`dirname $0`

if [ $# -ne 1 ] ; then
  echo "Usage: $0 [string]"
  echo "Writes [string] to $DEV"
  exit 1
fi

$DIR/neopixel_init.sh $DEV

echo "$1" > $DEV
#send extra line to clear buffers
echo "" > $DEV
