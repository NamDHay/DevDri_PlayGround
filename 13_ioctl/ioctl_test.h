#ifndef __IOCTL_TEST_H__
#define __IOCTL_TEST_H__

struct mystruct {
	int repeat;
	char name[64];
};

#define WR_VALUE _IOW('a', 'b', int32_t *)
#define RD_VALUE _IOR('a', 'b', int32_t *)
#define GREETER  _IOW('a', 'c', struct mystruct *)
#endif
