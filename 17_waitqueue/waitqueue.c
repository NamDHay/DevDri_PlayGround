#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/fs.h>

#include <linux/wait.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NamDHay");
MODULE_DESCRIPTION("A simple example for thread in LKM");

#define MYMAJOR 64

static struct task_struct *kthread_1;
static struct task_struct *kthread_2;

static long int watch_var = 0;

DECLARE_WAIT_QUEUE_HEAD(wq1);
static wait_queue_head_t wq2;

int thread_function(void * wait_sel) {
	int selection = *(int *) wait_sel;

	switch(selection) {
		case 1:
			wait_event(wq1, watch_var == 11);
			printk("waitqueue - watch_var is now 11!\n");
			break;
		case 2:
			while(wait_event_timeout(wq2, watch_var == 22, msecs_to_jiffies(5000)) == 0)
				printk("waitqueue - watch_var is still not 22, but timeout elapsed!\n");
			printk("waitqueue - watch_var is now 22!\n");
			break;
		default:
			break;
	}
	printk("waitqueue - Thread monitoring wq%d finished execution!\n", selection);
	return 0;
}

static ssize_t my_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	size_t to_copy, not_copied, delta;
	char buffer[16];
	printk("waitqueue - write callback called\n");

	memset(buffer, 0, sizeof(buffer));
	to_copy = min(count, sizeof(buffer));

	not_copied = copy_from_user(buffer, user_buffer, to_copy);

	delta = to_copy - not_copied;

	/* Convert string to long int */
	if(kstrtol(buffer, 10, &watch_var) == -EINVAL)
		printk("waitqueue - Error converting input!\n");
	printk("waitqueue - watch_var is now %ld\n", watch_var);

	wake_up(&wq1);
	wake_up(&wq2);

	return delta;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.write = my_write
};

static int __init ModuleInit(void) {
	int t1 = 1, t2 = 2;

	/* Init dynamically created waitqueue */
	init_waitqueue_head(&wq2);

	printk("waitqueue - Init threads\n");

	/* Register device number */
	if(register_chrdev(MYMAJOR, "my_dev_nr", &fops) != 0) {
		printk("waitqueue - Could not register device number!\n");
		return -1;
	}
	printk("waitqueue - Device Number registered!\n");

	kthread_1 = kthread_run(thread_function, &t1, "kthread_1");
	if(kthread_1 != NULL){
		printk("waitqueue - Thread 1 was created and is running now!\n");
	}
	else {
		printk("waitqueue - Thread 1 could not be created!\n");
		unregister_chrdev(MYMAJOR, "my_dev_nr");
		return -1;
	}
	kthread_2 = kthread_run(thread_function, &t2, "kthread_2");
	if(kthread_2 != NULL)
		printk("waitqueue - Thread 2 was created and is running now!\n");
	else {
		printk("waitqueue - Thread 2 could not be created!\n");
		watch_var = 11;
		wake_up(&wq1);
		mdelay(10);
		unregister_chrdev(MYMAJOR, "my_dev_nr");
		return -1;
	}
	printk("waitqueue - Both threads are running now!\n");

	return 0;
}

static void __exit ModuleExit(void) {
	printk("waitqueue - Stop both threads\n");
	watch_var = 11;
	wake_up(&wq1);
	mdelay(10);
	watch_var = 22;
	wake_up(&wq2);
	mdelay(10);
	unregister_chrdev(MYMAJOR, "my_dev_nr");
}

module_init(ModuleInit);
module_exit(ModuleExit);

