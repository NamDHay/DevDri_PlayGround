#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NamDHay");
MODULE_DESCRIPTION("Module creates a folder and file in procfs and implements "
                   "read and write callbacks");

static struct proc_dir_entry *proc_folder;
static struct proc_dir_entry *proc_file;

static ssize_t my_read(struct file *File, char *user_buffer, size_t count,
                       loff_t *offs) {
  char text[] = "Hello from procfs file!\n";
  size_t to_copy, not_copied, delta;

  /* Get amount of data to copy */
  to_copy = min(count, sizeof(text));

  /* Copy data to user */
  not_copied = copy_to_user(user_buffer, text, to_copy);

  /* Calculate data */
  delta = to_copy - not_copied;

  return delta;
}

static ssize_t my_write(struct file *File, const char *user_buffer,
                        size_t count, loff_t *offs) {
  char text[255];
  size_t to_copy, not_copied, delta;

  memset(text, 0, sizeof(text));
  to_copy = min(count, sizeof(text));

  not_copied = copy_from_user(text, user_buffer, to_copy);
  printk("procfs_test - You have written %s to me!\n", text);

  delta = to_copy - not_copied;

  return delta;
}

static struct proc_ops fops = {.proc_read = my_read, .proc_write = my_write};

static int __init ModuleInit(void) {
  proc_folder = proc_mkdir("hello", NULL);
  if (proc_folder == NULL) {
    printk("procfs_test - Error creating /proc/hello!\n");
    return -ENOMEM;
  }

  proc_file = proc_create("dummy", 0666, proc_folder, &fops);
  if (proc_file == NULL) {
    printk("procfs_test - Error creating /proc/hello/dummy!\n");
    proc_remove(proc_folder);
    return -ENOMEM;
  }

  printk("procfs_test - Creating /proc/hello/dummy!\n");
  return 0;
}

static void __exit ModuleExit(void) {
  printk("procfs_test - Remove /proc/hello/dummy!\n");
  proc_remove(proc_file);
  proc_remove(proc_folder);
}

module_init(ModuleInit);
module_exit(ModuleExit);
