#include "doorknob-common.h"

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
	int magnet_value = -1;
	int num_of_var = -1;
	int magnet_try = 0;
	char res[128];
	do {
		//dispose for all data keeped in communication buffer
		tcflush(fd, TCIFLUSH);
		write(fd, &action, 1);
		read(fd, res, sizeof(res));
		num_of_var = sscanf(res, "[%d]", &magnet_value);
		printf("magnet_value : %d\n", magnet_value);
		if(magnet_try >= 1000){
			exit(3);
		}
		magnet_try++;
	} while (num_of_var != 1);

	//North or Souch of magnet value
	if (magnet_value >= MAGNET_UPPER_THRESHOLD || magnet_value <= MAGNET_LOWER_THRESHOLD){
		state = IS_CLOSED;
	} else {
		state = IS_OPENED;
	}
	return state;
}
