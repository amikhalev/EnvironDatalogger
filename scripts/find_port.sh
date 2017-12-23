#/bin/bash

shopt -s extglob
port_pattern="/dev/*(tty.usbmodem*|ttyACM*)"
ports="$port_pattern"
idx=${1:-0}
port=$(echo ${ports[$idx]})

if [ ! -e "$port" ]; then
    echo "No serial port(s) found"
    exit 1
fi

echo $port
