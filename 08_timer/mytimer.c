#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/jiffies.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NamDHay");
MODULE_DESCRIPTION("A simple LKM's timer");

static struct timer_list my_timer;
uint8_t status = 0;
void timer_callback(struct timer_list * data) {
	status = status == 0 ? 1: 0;
	gpio_set_value(4, status);
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));
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
	timer_setup(&my_timer, timer_callback, 0);
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));
	return 0;
}

static void __exit ModuleExit(void) {
	del_timer(&my_timer);
	gpio_free(4);
	printk("Goodbye Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

