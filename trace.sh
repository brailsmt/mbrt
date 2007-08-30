#!/bin/bash

if [[ $# == 0 ]]; then
    echo Usage:
    echo
    echo   trace.sh  path/to/scenefile
    echo
    exit 1
fi
#get the name of the rendered image from the XML file.
imgname=`grep 'file name=' $1 | gawk -F'"' '{ print $2 }' | xargs -i{} basename {} .ppm`

./rt --scene $1

# Use imagemagick instead of ppmtojpeg if it exists
which convert > /dev/null 2>&1
if [[ $? == 0 ]]; then
    convert renders/$imgname.ppm renders/$imgname.png
else
    ppmtojpeg renders/$imgname.ppm > renders/$imgname.jpg
fi

# mbrt is trashing my terminal. Could be a Cygwin issue
#
reset
