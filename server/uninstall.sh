#!/bin/sh

set -eu

sudo rm /usr/sbin/door-controller
sudo rm /usr/sbin/door-service
sudo rm /etc/init.d/door-rc

# Uninstall
sudo update-rc.d -f door-rc remove
