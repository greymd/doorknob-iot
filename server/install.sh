#!/bin/sh
set -eu

sudo bash ./build.sh
sudo install -m 755 ./bin/door-controller-impl /usr/sbin/door-controller-impl
sudo install -m 755 ./bin/door-service-impl /usr/sbin/door-service-impl
sudo install -m 755 ./script/door-rc /etc/init.d/door-rc
sudo update-rc.d door-rc defaults
