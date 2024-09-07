#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "ioctl_test.h"

int main() {
	int answer;
	struct mystruct test = {4, "NamDHay"};
	int dev = open("/dev/dummy", O_RDWR);
	if(dev == -1) {
		printf("Open was not possible!\n");
		return -1;
	}

	ioctl(dev, RD_VALUE, &answer);
	printf("The answer is %d!\n", answer);

	answer = 123;
	ioctl(dev, WR_VALUE, &answer);
	ioctl(dev, RD_VALUE, &answer);
	printf("The answer is %d now!\n", answer);

	ioctl(dev, GREETER, &test);

	printf("Open was successfully!\n");
	close(dev);
	return 0;
}
