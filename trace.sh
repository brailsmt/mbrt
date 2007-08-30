#!/bin/bash

if [[ $# == 0 ]]; then
    echo Usage:
    echo
    echo   trace.sh  scenefile
    echo
    exit 1
fi
./rt --scene scenes/$1.xml
ppmtojpeg renders/$1.ppm > renders/$1.jpg

# mbrt is trashing my terminal. Could be a Cygwin issue
#
reset
