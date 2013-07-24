#!/bin/bash

make SIMULATION=1

# switch off line-buffering, echo; 
stty -echo -icanon line 1

./stm32f4discovery-demo 2> /dev/null

stty sane
