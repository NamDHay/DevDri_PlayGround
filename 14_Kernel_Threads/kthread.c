#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NamDHay");
MODULE_DESCRIPTION("Example for thread in LKM");

/* Global variables */
static struct task_struct *kthread_11;
static struct task_struct *kthread_12;
static struct task_struct *kthread_21;
static struct task_struct *kthread_22;
static int t11 = 1, t12 = 2;
static int t21 = 1, t22 = 2;

int thread_function1(void * thread_nr);
int thread_function2(void * thread_nr);

int thread_function1(void * thread_nr) {
  unsigned int i = 0;
  int t_nr = *(int *) thread_nr;

  while(!kthread_should_stop()) {
    printk("kthread - Function 1 Thread %d is execute! Counter val: %d\n", t_nr, i++);
    msleep(t_nr * 1000);
  }
  printk("kthread - Thread %d finished execution!\n", t_nr);
  return 0;
}

int thread_function2(void * thread_nr) {
  unsigned int i = 0;
  int t_nr = *(int *) thread_nr;

  while(!kthread_should_stop()) {
    printk("kthread - Function 2 Thread %d is execute! Counter val: %d\n", t_nr, i++);
    msleep(t_nr * 1000);
  }
  printk("kthread - Thread %d finished execution!\n", t_nr);
  return 0;
} 

static int __init ModuleInit(void) {
  printk("kthread - Init Thread\n");
  kthread_11 = kthread_create(thread_function1, &t11, "kthread_11");
  if(kthread_11 != NULL) {
    wake_up_process(kthread_11);
    printk("kthread - Thread 11 was created and is running now!\n");
  }
  else {
    printk("kthread - Thread 11 could not be created!\n");
    return -1;
  }
  kthread_12 = kthread_run(thread_function1, &t12, "kthread_12");
  if(kthread_12 != NULL) {
    printk("kthread - Thread 12 was created and is running now!\n");
  }
  else {
    printk("kthread - Thread 12 could not be created!\n");
    kthread_stop(kthread_11);
    return -1;
  }
  kthread_21 = kthread_create(thread_function2, &t21, "kthread_21");
  if(kthread_21 != NULL) {
    wake_up_process(kthread_21);
    printk("kthread - Thread 21 was created and is running now!\n");
  }
  else {
    printk("kthread - Thread 21 could not be created!\n");
    return -1;
  }
  kthread_22 = kthread_run(thread_function1, &t22, "kthread_22");
  if(kthread_22 != NULL) {
    printk("kthread - Thread 22 was created and is running now!\n");
  }
  else {
    printk("kthread - Thread 22 could not be created!\n");
    kthread_stop(kthread_21);
    return -1;
  }

  printk("kthread - All thread are running now!\n");
  return 0;
}

static void __exit ModuleExit(void) {
  printk("kthread - Stop both thread!\n");
  kthread_stop(kthread_11);
  kthread_stop(kthread_12);
  kthread_stop(kthread_21);
  kthread_stop(kthread_22);
}

module_init(ModuleInit);
module_exit(ModuleExit);

