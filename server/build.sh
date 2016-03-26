#!/bin/sh
gcc ./src/door-common.c ./src/door-service.c -o ./bin/door-service
gcc ./src/door-common.c ./src/door-controller.c -o ./bin/door-controller
