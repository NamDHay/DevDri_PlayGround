#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x2e712c61, "module_layout" },
	{ 0x44dfb875, "driver_unregister" },
	{ 0x9de961c, "platform_driver_unregister" },
	{ 0x723d1b4d, "__platform_driver_register" },
	{ 0x9a165ec8, "__serdev_device_driver_register" },
	{ 0x2d20aa19, "serdev_device_write_buf" },
	{ 0xe00b69b4, "serdev_device_set_parity" },
	{ 0x30f649b2, "serdev_device_set_flow_control" },
	{ 0x6db22d48, "serdev_device_set_baudrate" },
	{ 0x83e26796, "serdev_device_open" },
	{ 0xd95e9316, "serdev_device_close" },
	{ 0xf799ae1d, "proc_create" },
	{ 0x3a02f1a7, "gpiod_get" },
	{ 0xba69e61c, "of_get_named_gpio_flags" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xb4007ce9, "gpiod_set_value" },
	{ 0xc9b30a5f, "proc_remove" },
	{ 0xcbb7404c, "gpiod_put" },
	{ 0xc5850110, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cserdev_write");
MODULE_ALIAS("of:N*T*Cserdev_writeC*");
MODULE_ALIAS("of:N*T*Coutput_gpio");
MODULE_ALIAS("of:N*T*Coutput_gpioC*");
MODULE_ALIAS("of:N*T*Cinput_gpio");
MODULE_ALIAS("of:N*T*Cinput_gpioC*");

MODULE_INFO(srcversion, "52CC3508985A89F017EF46C");
