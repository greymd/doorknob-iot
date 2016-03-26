#define DEFAULT_DEVICE "/dev/ttyACM0"
#define DEFAULT_BAUDRATE 9600
#define IS_CLOSED 0
#define IS_OPENED 1
#define IS_ON 0
#define IS_OFF 1

#include <asm/termbits.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
