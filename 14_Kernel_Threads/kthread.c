#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>

/* Global variables */
static struct task_struct *kthread_1;
static struct task_struct *kthread_2;
static int t1 = 1, t2 = 2;

int thread_function(void * thread_nr);

int thread_function(void * thread_nr) {
  unsigned int i = 0;
  int t_nr = *(int *) thread_nr;

  while(!kthread_should_stop()) {
    printk("kthread - Thread %d is execute! Counter val: %d\n", t_nr, i++);
    msleep(t_nr * 1000);
  }
  printk("kthread - Thread %d finished execution!\n", t_nr);
  return 0;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NamDHay");
MODULE_DESCRIPTION("Example for thread in LKM");

static int __init ModuleInit(void) {
  printk("kthread - Init Thread\n");
  kthread_1 = kthread_create(thread_function, &t1, "kthread_1");
  if(kthread_1 != NULL) {
    wake_up_process(kthread_1);
    printk("kthread - Thread 1 was created and is running now!\n");
  }
  else {
    printk("kthread - Thread 1 could not be created!\n");
    return -1;
  }
  kthread_2 = kthread_run(thread_function, &t2, "kthread_2");
  if(kthread_2 != NULL) {
    printk("kthread - Thread 2 was created and is running now!\n");
  }
  else {
    printk("kthread - Thread 2 could not be created!\n");
    kthread_stop(kthread_1);
    return -1;
  }
  printk("kthread - Both thread are running now!\n");
  return 0;
}

static void __exit ModuleExit(void) {
  printk("kthread - Stop both thread!\n");
  kthread_stop(kthread_1);
  kthread_stop(kthread_2);
}

module_init(ModuleInit);
module_exit(ModuleExit);

