#include "door-common.h"

void close_door(int fd) {
	char action = ACTION_CLOSE;
	write(fd, &action, 1);
}

void open_door(int fd) {
	char action = ACTION_OPEN;
	write(fd, &action, 1);
}

/**
 * get door state
 * return 0:closed, 1:opened
 */
int get_door_state(int fd) {
	char action = ACTION_STATE;
	int state = -1;
	int magnetValue = -1;
	int numOfvar = -1;
	int magnetTry = 0;
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
	if (magnetValue >= MAGNET_UPPER_THRESHOLD || magnetValue <= MAGNET_LOWER_THRESHOLD){
		state = IS_CLOSED;
	} else {
		state = IS_OPENED;
	}
	return state;
}
