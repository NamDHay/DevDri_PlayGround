#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NamDHay");
MODULE_DESCRIPTION("Register a device nr and implenment some callback function");

static char buffer[255];
static int buffer_pointer = 0;

static dev_t my_device_nr;
static struct class *myclass;
static struct cdev my_device;

#define DRIVER_NAME "DummyDriver"
#define DRIVER_CLASS "MyModuleClass"

static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs) {
  size_t to_copy, not_copied, delta;

  to_copy = min(count, (size_t)buffer_pointer);

  not_copied = copy_to_user(user_buffer, buffer, to_copy);

  delta = to_copy - not_copied;
  
  return delta;
}

static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
  size_t to_copy, not_copied, delta;

  to_copy = min(count, sizeof(buffer));

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
  if( alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0) {
    printk("Devive Nr. could not be allocated!\n");
    return -1;
  }
  printk("read_write - Device Nr. Major: %d, Minor: %d was registered!\n", my_device_nr>>20, my_device_nr&0xfffff);
  
  if((myclass = class_create(DRIVER_CLASS)) == NULL) {
    printk("Device class can not be created\n");
    goto ClassError;
  }

  if(device_create(myclass, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL) {
    printk("Can not create device file\n");
    goto FileError;
  }

  cdev_init(&my_device, &fops);
  if(cdev_add(&my_device, my_device_nr, 1) == -1) {
    printk("Register of device to kernel fail!\n");
    goto AddError;
  }
  return 0;
AddError:
  device_destroy(myclass, my_device_nr);
FileError:
  class_destroy(myclass);
ClassError:
  unregister_chrdev(my_device_nr, DRIVER_NAME);
  return -1;
}

static void __exit ModuleExit(void) {
  cdev_del(&my_device);
  device_destroy(myclass, my_device_nr);
  class_destroy(myclass);
  unregister_chrdev(my_device_nr, DRIVER_NAME);
}

module_init(ModuleInit);
module_exit(ModuleExit);

