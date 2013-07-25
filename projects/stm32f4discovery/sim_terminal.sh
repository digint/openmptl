#!/bin/bash

trap "stty echo icanon || echo failed to reset terminal settings!" SIGINT SIGTERM

# switch off line-buffering, echo; 
#stty -echo -icanon line 1
stty -echo -icanon

./stm32f4discovery-demo 2> /dev/null
