#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/hrtimer.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NamDHay");
MODULE_DESCRIPTION("A simple LKM's high resolution timer");

static struct hrtimer my_hrtimer;
u8 status = 0;
static u64 start_t;
static enum hrtimer_restart hrtimer_handler(struct hrtimer * timer) {
	u64 now_t = jiffies;
	status = status == 0 ? 1: 0;
	gpio_set_value(4, status);
	printk("start_t = %u\n", jiffies_to_msecs(start_t));
	printk("start_t - now_t = %u\n", jiffies_to_msecs(now_t - start_t));
	
	start_t = jiffies;
	hrtimer_start(&my_hrtimer, ms_to_ktime(100), HRTIMER_MODE_REL);
	return HRTIMER_RESTART;
}

static int __init ModuleInit(void) {
	printk("Hello Kernel\n");
	if(gpio_request(4, "rpi_gpio_4")) {
		printk("Cannot allocate GPIO4!\n");
		return -1;
	}

	if(gpio_direction_output(4, 0)) {
		printk("Can not set GPIO 4 to output!\n");
		gpio_free(4);
		return -1;
	}
	gpio_set_value(4, status);
	hrtimer_init(&my_hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	my_hrtimer.function = &hrtimer_handler;
	start_t = jiffies;
	hrtimer_start(&my_hrtimer, ms_to_ktime(100), HRTIMER_MODE_REL);
	return 0;
}

static void __exit ModuleExit(void) {
	hrtimer_cancel(&my_hrtimer);
	gpio_free(4);
	printk("Goodbye Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

