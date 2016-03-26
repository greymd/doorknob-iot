#!/bin/sh

set -eu

sudo bash ./build.sh
sudo install -m 755 ./bin/door-controller /usr/sbin/door-controller
sudo install -m 755 ./bin/door-service /usr/sbin/door-service
sudo install -m 755 ./script/door-rc /etc/init.d/door-rc
sudo update-rc.d door-rc defaults

# Uninstall
# sudo update-rc.d -f door-rc remove
