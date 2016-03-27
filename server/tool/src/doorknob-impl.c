#include "./doorknob-common.h"

int main(int argc, char *argv[]) {
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

	/* restore current io settings */
	tcsetattr(fd, TCSANOW, &current_io);

	/* close serial communication */
	close(fd);

	fflush(stdout);
	return 0;
}
