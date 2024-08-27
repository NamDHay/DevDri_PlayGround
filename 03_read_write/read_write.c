#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NamDHay");
MODULE_DESCRIPTION("Register a device nr and implenment some callback function");

static char buffer[255];
static int buffer_pointer;

static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs) {
  int to_copy, not_copied, delta;

  to_copy = min(count, buffer_pointer);

  not_copied = copy_to_user(user_buffer, buffer, to_copy);

  delta = to_copy - not_copied;
  
  return delta;
}

static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
  int to_copy, not_copied, delta;

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
  .release = driver_close
  .read = driver_read,
  .write = driver_write
};

#define MYMAJOR 90

static int __init ModuleInit(void) {
  int retval;
  retval = register_chrdev(MYMAJOR, "my_dev_nr", &fops);
  if(retval == 0) {
    printk("dev_nr - register Device Number Major: %d, Minor: %d\n", MYMAJOR, 0);
  }
  else if(retval > 0) {
    printk("dev_nr - register Device Number Major: %d, Minor: %d\n", retval >> 20, retval&0xfffff);
  }
  else {
    printk("Could not register device number\n");
  }
 
  return 0;
}

static void __exit ModuleExit(void) {
  unregister_chrdev(MYMAJOR, "my_dev_nr");
}

module_init(ModuleInit);
module_exit(ModuleExit);

