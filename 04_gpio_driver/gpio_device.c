#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NamDHay");
MODULE_DESCRIPTION("A simple gpio for led and reading button");

static char buffer[255];
static int buffer_pointer;

static dev_t my_device_nr;
static struct class *myclass;
static struct cdev my_device;

#define DRIVER_NAME "MyGPIODriver"
#define DRIVER_CLASS "MyModuleClass"

static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs) {
  int to_copy, not_copied, delta;
  char tmp[3]= " \n";

  to_copy = umin(count, sizeof(tmp));

  not_copied = copy_to_user(user_buffer, &tmp, to_copy);

  delta = to_copy - not_copied;
  
  return delta;
}

static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
  int to_copy, not_copied, delta;

  to_copy = umin(count, sizeof(buffer));

  not_copied = copy_to_user(buffer, user_buffer, to_copy);
  buffer_pointer = to_copy;

  delta = to_copy - not_copied;
  
  return delta;
}

static int driver_open(struct inode *device_file, struct file *instance){
  printk("dev_nr - open was called\n");
  return 0;
}

static int driver_close(struct inode *device_file, struct file *instance){
  printk("dev_nr - close was called\n");
  return 0;
}

static struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = driver_open,
  .release = driver_close,
  .read = driver_read,
  .write = driver_write
};

static int __init ModuleInit(void) {
  if( alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) == NULL) {
    printk("Devive Nr. could not be allocated!\n");
    return -1;
  }
  printk("read_write - Device Nr. Major: %d, Minor: %d was registered!\n", my_device_nr>>20, my_device_nr&0xfffff);
  
  if((myclass = class_create(DRIVER_CLASS)) == NULL) {
    printk("Device class can not be created\n");
    goto ClassError;
  }

  if(device_create(myclass, NULL, my_device_nr, NULL, DRIVER_NAME) < 0) {
    printk("Can not create device file\n");
    goto FileError;
  }

  cdev_init(&my_device, &fops);
  if(cdev_add(&my_device, my_device_nr, 1) == -1) {
    printk("Register of device to kernel fail!\n");
    goto AddError;
  }
  if(gpio_request(4, "gpio-4")) {
    printk("Can not allcated GPIO 4\n");
    goto AddError;
  }
  if(gpio_direction_output(4, 0)) {
    printk("Can not set GPIO 4 to output\n");
    goto Gpio4Error;
  }
  if(gpio_request(17, "gpio-17")) {
    printk("Can not allcated GPIO 17\n");
    goto AddError;
  }
  if(gpio_direction_input(17)) {
    printk("Can not set GPIO 17 to input\n");
    goto Gpio17Error;
  }
  return 0;
Gpio17Error:
  gpio_free(17);
Gpio4Error:
  gpio_free(4);
AddError:
  device_destroy(myclass, my_device_nr);
FileError:
  class_destroy(myclass);
ClassError:
  unregister_chrdev(my_device_nr, DRIVER_NAME);
  return -1;
}

static void __exit ModuleExit(void) {
  gpio_set_value(4, 0);
  
  gpio_free(17);
  gpio_free(4);
  cdev_del(&my_device);
  device_destroy(myclass, my_device_nr);
  class_destroy(myclass);
  unregister_chrdev(my_device_nr, DRIVER_NAME);
}

module_init(ModuleInit);
module_exit(ModuleExit);

