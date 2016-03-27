#include "./doorknob-common.h"

int main(int argc, char *argv[]) {
	int now_door_state = -1;
	int old_door_state = -1;

	/* variables for serial communication */
	int fd, c;
	struct termios current_io, new_io;
	char buf[1024];
	char devfile[1024];
	int baudrate = DEFAULT_BAUDRATE;

	bzero(buf, sizeof(buf));
	strncpy(devfile, DEFAULT_DEVICE, sizeof(devfile));
	for (c = 1; c < argc; c++) {
		if (1 == sscanf(argv[c], "--devfile=%s", devfile)) {
			continue;
		}
	}

	fd = open(devfile, O_RDWR | O_NOCTTY);
	if (fd < 0) {
		perror(devfile);
		exit(-1);
	}

	//wait until Arduino starts
	usleep(200*10000);

	tcgetattr(fd, &current_io);
	bzero(&new_io, sizeof(new_io));
	new_io.c_cflag = baudrate | CRTSCTS | CS8 | CLOCAL | CREAD;
	new_io.c_iflag = IGNPAR | ICRNL;
	new_io.c_oflag = 0;
	new_io.c_lflag = ICANON;
	new_io.c_cc[VEOF] = 4;
	new_io.c_cc[VMIN] = 1;

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &new_io);

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

	/* restore current io settings */
	tcsetattr(fd, TCSANOW, &current_io);

	/* close serial communication */
	close(fd);

	return 0;
}
