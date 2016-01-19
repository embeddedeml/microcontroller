#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "joystick.h"

static int joystick_fd = -1;

int open_joystick(char *joystick_device)
{
    struct js_event jse;

	joystick_fd = open(joystick_device, O_RDONLY | O_NONBLOCK); /* read write for force feedback? */
	while(read_joystick_event(&jse) == 1)
	{
	    /* flush buffer */
	}

	return joystick_fd;
}

int read_joystick_event(struct js_event *jse)
{
	int bytes;

	bytes = read(joystick_fd, jse, sizeof(*jse)); 

	if (bytes == -1)
		return 0;

	if (bytes == sizeof(*jse))
		return 1;

	printf("Unexpected bytes from joystick:%d\n", bytes);

	return -1;
}

void close_joystick()
{
	close(joystick_fd);
}

