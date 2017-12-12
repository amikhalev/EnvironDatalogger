#/bin/bash

ports=( $(shopt -s extglob && echo /dev/{tty.usbmodem*,ttyACM*}) )
idx=${1:-0}
port=${ports[$idx]}

if [ ! -e "$port" ]; then
    echo "No serial port(s) found"
    exit 1
fi

echo $port
