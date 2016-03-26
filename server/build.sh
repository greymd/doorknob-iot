#!/bin/sh
gcc ./src/door-common.c ./src/door-service-impl.c -o ./bin/door-service-impl
gcc ./src/door-common.c ./src/door-controller-impl.c -o ./bin/door-controller-impl
