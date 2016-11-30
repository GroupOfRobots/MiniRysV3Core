#include "sonar.h"

Sonar::Sonar(){
	result = 0;
}

int Sonar::getDistance(int nsonar){

	struct pollfd pollfds[1];

	/* Open the rpmsg_pru character device file */
	pollfds[0].fd = open(DEVICE1_NAME, O_RDWR);

	/*
	 * If the RPMsg channel doesn't exist yet the character device
	 * won't either.
	 * Make sure the PRU firmware is loaded and that the rpmsg_pru
	 * module is inserted.
	 */
	if (pollfds[0].fd < 0) {
		printf("Failed to open %s\n", DEVICE1_NAME);
		return -1;
	}

	/* The RPMsg channel exists and the character device is opened */
	//printf("Opened %s, sending %d messages\n\n", DEVICE_NAME, 1);

	result = 0;

	/* Send data structure to the PRU through the RPMsg channel */
	result = write(pollfds[0].fd, "hello", 13);
	if (result > 0)
		//printf("Message %d: Sent to PRU\n", 1);

	result = read(pollfds[0].fd, readBuf, MAX_BUFFER_SIZE);
	//if (result > 0)

	/* Close the rpmsg_pru character device file */
	close(pollfds[0].fd);

	return readBuf[0];
}
