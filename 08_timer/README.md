# Lesson 8: Timer in Linux Kernel Modules

# 1. Include File

```c
#include <linux/jiffies.h>
#include <linux/timer.h>
```

# 2. Timer **Components**

## 1. **Timer Structure**

• `struct timer_list`: This structure is used to define a timer in the kernel. It contains fields for the timer's state, expiration time, and the callback function to execute when the timer expires.

## 2. Timer **Key Functions**

### 1. Function setup timer

```c
void timer_setup(struct timer_list *timer, void (*callback)(struct timer_list *), unsigned int flags);
```

• Initializes a timer. The `callback` function is called when the timer expires.

### 2. Function modify timer

```c
int mod_timer(struct timer_list *timer, unsigned long expires);
```

• Reschedules the timer to expire at a specified time. The `expires` parameter is usually calculated using jiffies.

### 3. Function delete timer

```c
int del_timer(struct timer_list *timer);
```

• Deletes the timer, preventing it from expiring.

## 3. **Jiffies API**

### 1. Converts milliseconds to jiffies

```c
unsigned long msecs_to_jiffies(unsigned int msecs);
```

### 2. Jiffies back to milliseconds

```c
unsigned int jiffies_to_msecs(unsigned long jiffies);
```
