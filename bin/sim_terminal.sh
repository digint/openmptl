#!/bin/bash

SIM_EXE=$1

if [ -z $SIM_EXE ] ; then
  echo "usage: $0 <sim_executable>"
  exit 1
fi

# save configuration
OLDCONFIG=`stty -g`

trap "echo -e '\n+++ restoring terminal settings'; stty $OLDCONFIG || echo '!!! failed to reset terminal settings'" SIGINT SIGTERM

echo '+++ setting terminal: char-buffer, echo-off'
stty -echo -icanon line 1

$SIM_EXE 2> /dev/null
