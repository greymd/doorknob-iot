#include "./door-common.h"

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
	int magnetTry = 0;
	char action = '2';
	char res[128];
	do {
		//dispose for all data keeped in communication buffer
		tcflush(fd, TCIFLUSH);
		write(fd, &action, 1);
		read(fd, res, sizeof(res));
		numOfvar = sscanf(res, "[%d]", &magnetValue);
		printf("magnet_value : %d\n", magnetValue);
		if(magnetTry >= 1000){
			exit(1);
		}
		magnetTry++;
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

	/* variables for serial communication */
	int fd, c;
	struct termios oldtio, newtio;
	char buf[1024];
	char devfile[1024];
	char action;
	int baudrate = DEFAULT_BAUDRATE;

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

	//wait until Arduino starts
	usleep(200*10000);

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

	/* start monitoring */
	while (1) {
		now_door_state = get_door_state(fd);
		printf("door_state: %d\n", now_door_state);
		if (now_door_state == IS_CLOSED && old_door_state == IS_OPENED) {
			close_door(fd);
			puts("close_door");
		}
		old_door_state = now_door_state;
		fflush(stdout);
		usleep(100 * 1000); //0.5 sec
	}

	/* close actions for serial communication */
	tcsetattr(fd, TCSANOW, &oldtio);
	close(fd);

	return 0;
}
