#!/usr/bin/env bash

# Example usage: ./monitor.sh /dev/ttyUSB0 57600

set -e

exec 3<> $1
stty $2 <&3
cat - <&3
