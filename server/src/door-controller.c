#define DEFAULT_DEVICE "/dev/ttyACM0"
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

void close_door(int fd) {
	char action = '0';
	write(fd, &action, 1);
}

void open_door(int fd) {
	char action = '1';
	write(fd, &action, 1);
}

/**
 * get door state
 * return 0:closed, 1:opened
 */
int get_door_state(int fd) {
	int state = -1;
	int magnetValue = -1;
	int numOfvar = -1;
	char action = '2';
	char res[128];
	do {
		//dispose for all data keeped in communication buffer
		tcflush(fd, TCIFLUSH);
		write(fd, &action, 1);
		read(fd, res, sizeof(res));
		numOfvar = sscanf(res, "[%d]", &magnetValue);
	} while (numOfvar != 1);

	//North or Souch of magnet value
	if (magnetValue >= 700 || magnetValue <= 300){
		state = IS_CLOSED;
	} else {
		state = IS_OPENED;
	}
	return state;
}


int main(int argc, char *argv[]) {
	int now_door_state = -1;
	int old_door_state = -1;
	int now_felica_flag = -1;
	int old_felica_flag = -1;

	/* variables for serial communication */
	int fd, c;
	struct termios oldtio, newtio;
	char buf[1024];
	char devfile[1024];
	char action;
	int baudrate = 9600;

	/* settings for serial communication */
	bzero(buf, sizeof(buf));
	strncpy(devfile, DEFAULT_DEVICE, sizeof(devfile));
	for (c = 1; c < argc; c++) {
		if (1 == sscanf(argv[c], "--devfile=%s", devfile)) {
			continue;
		}

		if (1 == sscanf(argv[c], "--baudrate=%d", &baudrate)) {
			continue;
		}
	}

	switch (baudrate) {
		case 9600:
			baudrate = B9600;
			break;
		case 19200:
			baudrate = B19200;
			break;
		case 38400:
			baudrate = B38400;
			break;
		case 57600:
			baudrate = B57600;
			break;
		case 115200:
			baudrate = B115200;
			break;
		case 230400:
			baudrate = B230400;
			break;
		default:
			perror("baudrate");
			exit(-1);
			break;
	}

	fd = open(devfile, O_RDWR | O_NOCTTY);
	if (fd < 0) {
		perror(devfile);
		exit(-1);
	}

	tcgetattr(fd, &oldtio);
	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag = baudrate | CRTSCTS | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR | ICRNL;
	newtio.c_oflag = 0;
	newtio.c_lflag = ICANON;
	newtio.c_cc[VEOF] = 4;
	newtio.c_cc[VMIN] = 1;

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);

	for (c = 1; c < argc; c++) {
		if (strcmp(argv[c], "--close") == 0)
		{
			close_door(fd);
			break;
		}

		if (strcmp(argv[c], "--open") == 0)
		{
			open_door(fd);
			break;
		}
	}

	{
		int state = get_door_state(fd);
		if(state == IS_OPENED)
		{
			puts("opened");
		}

		if(state == IS_CLOSED)
		{
			puts("closed");
		}
	}

	/* close actions for serial communication */
	tcsetattr(fd, TCSANOW, &oldtio);
	close(fd);

	fflush(stdout);
	return 0;
}
