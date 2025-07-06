#!/bin/sh
set -eux
if [ ! -d bcpd ]; then
    git clone https://github.com/ohirose/bcpd.git
fi
./build_dependency.sh
make