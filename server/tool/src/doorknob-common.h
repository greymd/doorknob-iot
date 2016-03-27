#define DEFAULT_DEVICE "/dev/ttyACM0"
#define DEFAULT_BAUDRATE B9600
#define IS_CLOSED 0
#define IS_OPENED 1
#define IS_ON 0
#define IS_OFF 1
#define MAGNET_UPPER_THRESHOLD 700
#define MAGNET_LOWER_THRESHOLD 300
#define ACTION_CLOSE  '0'
#define ACTION_OPEN   '1'
#define ACTION_STATE  '2'

#include <asm/termbits.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <syslog.h>

void close_door(int fd);
void open_door(int fd);
int get_door_state(int fd);
