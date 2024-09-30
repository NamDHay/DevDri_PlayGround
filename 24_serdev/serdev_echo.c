#include <linux/init.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/serdev.h>
#include <linux/gpio/consumer.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NamDHay");
MODULE_DESCRIPTION("Another simple LKM to parse the device tree for a UART and GPIO device");

static int serdev_echo_probe(struct serdev_device *serdev);
static void serdev_echo_remove(struct serdev_device *serdev);
static int gpio_probe(struct platform_device *pdev);
static int gpio_remove(struct platform_device *pdev);

static const struct of_device_id gpio_driver_ids[] = {
	{ .compatible = "output_gpio" },
	{ .compatible = "input_gpio" },
	{},
};
static const struct of_device_id serdev_driver_ids[] = {
	{ .compatible = "serdev_write" },
	{},
};
MODULE_DEVICE_TABLE(of, gpio_driver_ids);
MODULE_DEVICE_TABLE(of, serdev_driver_ids);

static struct platform_driver gpio_driver = {
	.probe = gpio_probe,
	.remove = gpio_remove,
	.driver = {
		.name = "gpio_device_driver",
		.of_match_table = gpio_driver_ids,
	},
};
static struct serdev_device_driver serdev_echo_driver = {
	.probe = serdev_echo_probe,
	.remove = serdev_echo_remove,
	.driver = {
		.name = "serdev-echo",
		.of_match_table = serdev_driver_ids,
	},
};

static struct gpio_desc *my_led = NULL;
static struct proc_dir_entry *proc_file;

static ssize_t my_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	switch (user_buffer[0]) {
		case '0':
		case '1':
			gpiod_set_value(my_led, user_buffer[0] - '0');
		default:
			break;
	}
	return count;
}

static struct proc_ops fops = {
	.proc_write = my_write,
};

static int gpio_probe(struct platform_device *pdev) {
	printk("dt_probe - Now I am in the probe function!\n");

	printk("dt_probe - Device name: %s\n", pdev->dev.of_node->name);
	if(strcmp(pdev->dev.of_node->name,"output")) {
		printk("dt_probe - Error! Name does not match\n");
		return -EINVAL;
	}

	my_led = gpiod_get(&pdev->dev, "pin", GPIOD_OUT_LOW);
	if(IS_ERR(my_led)) {
		printk("dt_gpio - Error! Could not setup the GPIO\n");
		return -1 * IS_ERR(my_led);
	}

	proc_file = proc_create("my-led", 0666, NULL, &fops);
	if(proc_file == NULL) {
		printk("proc_test - Error creating /proc/my-led\n");
		gpiod_put(my_led);
		return -ENOMEM;
	}

	return 0;
}

static int gpio_remove(struct platform_device *pdev) {
	printk("dt_gpio - Now I am in the remove function\n");
	gpiod_put(my_led);
	proc_remove(proc_file);
	return 0;
}

static int serdev_echo_recv(struct serdev_device *serdev, const unsigned char *buffer, size_t size) {
	printk("serdev_echo - Received %d bytes with \"%s\"\n", size, buffer);
	return 	serdev_device_write_buf(serdev, buffer, size);
}

static const struct serdev_device_ops serdev_echo_ops = {
	.receive_buf = serdev_echo_recv,
};

static int serdev_echo_probe(struct serdev_device *serdev) {
	int status;
	printk("serdev_echo - Now I am in the probe function!\n");

	serdev_device_set_client_ops(serdev, &serdev_echo_ops);
	status = serdev_device_open(serdev);
	if(status) {
		printk("serdev_echo - Error opening serial port!\n");
		return -status;
	}

	serdev_device_set_baudrate(serdev, 9600);
	serdev_device_set_flow_control(serdev, false);
	serdev_device_set_parity(serdev, SERDEV_PARITY_NONE);

	status = serdev_device_write_buf(serdev, "Type something: ", sizeof("Type something: "));
	printk("serdev_echo - Wrote %d bytes.\n", status);

	return 0;
}

static void serdev_echo_remove(struct serdev_device *serdev) {
	printk("serdev_echo - Now I am in the remove function\n");
	serdev_device_close(serdev);
}

static int __init ModuleInit(void) {
	printk("ModuleInit - Loading the driver...\n");
	if(serdev_device_driver_register(&serdev_echo_driver)) {
		printk("serdev_echo - Error! Could not load serdev driver\n");
		return -EINVAL;
	}
	if (platform_driver_register(&gpio_driver)) {
		printk("dt_gpio - Error! Could not load gpio driver\n");
		return -EINVAL; // Changed return value
	}
	return 0;
}

static void __exit ModuleExit(void) {
	printk("ModuleExit - Unloading driver\n"); // Corrected log message
	platform_driver_unregister(&gpio_driver);
	serdev_device_driver_unregister(&serdev_echo_driver);
}

module_init(ModuleInit);
module_exit(ModuleExit);
