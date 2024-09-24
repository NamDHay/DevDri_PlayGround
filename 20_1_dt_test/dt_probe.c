#include <linux/init.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/property.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NamDHay");
MODULE_DESCRIPTION("Another simple LKM to parse the device tree for a specific "
                   "device and its properties");

static int dt_probe(struct platform_device *pdev);
static int dt_remove(struct platform_device *pdev);

static const struct of_device_id my_driver_ids[] = {
    { .compatible = "first_device" },
    { .compatible = "second_device" },
    {},
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
    const char *name;
    u32 my_value; // Changed variable name to avoid conflict
    u8 value[4];  // Renamed the variable to avoid conflict
    int ret;

    pr_info("dt_probe - Now I am in the probe function!\n");

    if (!device_property_present(&pdev->dev, "name")) {
        pr_err("dt_probe - Error! Device property 'name' not found!\n");
        return -EINVAL; // Changed return value
    }
    if (!device_property_present(&pdev->dev, "my_value")) {
        pr_err("dt_probe - Error! Device property 'my_value' not found!\n");
        return -EINVAL; // Changed return value
    }
    if (!device_property_present(&pdev->dev, "value")) {
        pr_err("dt_probe - Error! Device property 'value' not found!\n");
        return -EINVAL; // Changed return value
    }

    ret = device_property_read_string(&pdev->dev, "name", &name);
    if (ret) {
        pr_err("dt_probe - Error! Could not read 'name'\n");
        return -EINVAL; // Changed return value
    }
    pr_info("dt_probe - name: %s\n", name);

    ret = device_property_read_u32(&pdev->dev, "my_value", &my_value);
    if (ret) {
        pr_err("dt_probe - Error! Could not read 'my_value'\n");
        return -EINVAL; // Changed return value
    }
    pr_info("dt_probe - my_value: %d\n", my_value);

    ret = device_property_read_u8_array(&pdev->dev, "value", value, sizeof(value));
    if (ret) {
        pr_err("dt_probe - Error! Could not read 'value'\n");
        return -EINVAL; // Changed return value
    }
    pr_info("dt_probe - value: %02x %02x %02x %02x\n", value[0], value[1], value[2], value[3]);

    return 0;
}

static int dt_remove(struct platform_device *pdev) {
    pr_info("dt_remove - Now I am in the remove function\n"); // Corrected log message
    return 0;
}

static int __init ModuleInit(void) {
    pr_info("ModuleInit - Loading the driver...\n");
    if (platform_driver_register(&my_driver)) {
        pr_err("ModuleInit - Error! Could not load driver\n");
        return -EINVAL; // Changed return value
    }
    return 0;
}

static void __exit ModuleExit(void) {
    pr_info("ModuleExit - Unloading driver\n"); // Corrected log message
    platform_driver_unregister(&my_driver);
}

module_init(ModuleInit);
module_exit(ModuleExit);
