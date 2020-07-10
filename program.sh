#!/bin/bash

CFG_FILE=""
HEX_FILE=""

if [ -n "$1" ]
    then
        CFG_FILE=$1
fi

if [ -n "$2" ]
    then
        HEX_FILE=$2
fi

openocd -f ${CFG_FILE} -c "program build/${HEX_FILE} verify reset exit"
