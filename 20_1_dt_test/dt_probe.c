#include <linux/module.h>
#include <linux/init.h>
#include <linux/mod_devicetable.h>
#include <linux/property.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NamDHay");
MODULE_DESCRIPTION("Another simple LKM to parse the device tree for a specific device and its properties");

static int dt_probe(struct platform_device *pdev);
static int dt_remove(struct platform_device *pdev);

static struct of_device_id my_driver_ids[] = {
	{
		.compatible = "first_device",
	},
	{
		.compatible = "second_device", 
	}
};
MODULE_DEVICE_TABLE(of, my_driver_ids);

static struct platform_driver my_driver = {
	.probe = dt_probe,
	.remove = dt_remove,
	.driver = {
		.name = "my_device_driver",
		.of_match_table = my_driver_ids,
	},
};

static int dt_probe(struct platform_device *pdev) {
	struct device_node *dev = &pdev->dev.of_node;
	const char *name;
	u32 value;
	u8 value[4];

	printk("dt_probe - Now I am in the probe function!\n");

	if(!device_property_present(dev, "name")) {
		printk("dt_probe - Error! Device property 'name' not found!\n");
		return -1;
	}
	if(!device_property_present(dev, "my_value")) {
		printk("dt_probe - Error! Device property 'my_value' not found!\n");
		return -1;
	}
	if(!device_property_present(dev, "value")) {
		printk("dt_probe - Error! Device property 'value' not found!\n");
		return -1;
	}
	ret = device_property_read_string(dev, "label", &label);
	if(ret) {
		printk("dt_probe - Error! Could not read 'label'\n");
		return -1;
	}
	printk("dt_probe - label: %s\n", label);
	ret = device_property_read_u32(dev, "my_value", &my_value);
	if(ret) {
		printk("dt_probe - Error! Could not read 'my_value'\n");
		return -1;
	}
	printk("dt_probe - my_value: %d\n", my_value);

	return 0;
}

static int dt_remove(struct platform_device *pdev) {
	printk("dt_probe - Now I am in the remove function\n");
	return 0;
}


static int __init ModuleInit(void) {
	printk("dt_probe - Loading the driver...\n");
	if(platform_driver_register(&my_driver)) {
		printk("dt_probe - Error! Could not load driver\n");
		return -1;
	}
	return 0;
}

static void __exit ModuleExit(void) {
	printk("dt_probe - Unload driver");
	platform_driver_unregister(&my_driver);
}

module_init(ModuleInit);
module_exit(ModuleExit);

