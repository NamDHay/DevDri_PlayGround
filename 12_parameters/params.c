#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NamDHay");
MODULE_DESCRIPTION("A simple LKM to demonstrate the use of parameteA simple LKM to demonstrate the use of parameter");

/* Kernel Module parameters */
static unsigned int gpio_nr = 12;
static char *device_name = "testdevice";

module_param(gpio_nr, uint, S_IRUGO);
module_param(device_name, charp, S_IRUGO);

MODULE_PARM_DESC(gpio_nr, "Nr of GPIO use in this LKM");
MODULE_PARM_DESC(device_name, "Device name use in this LKM");

static int __init ModuleInit(void) {
  printk("gpio_nr = %u\n", gpio_nr);
  printk("device_name = %s\n", device_name);
  printk("Hello Kernel\n");
  return 0;
}

static void __exit ModuleExit(void) {
  printk("Goodbye Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

