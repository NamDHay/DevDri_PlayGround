# Lesson 9: High Resolution Timer in Linux Kernel Modules

# 1. Include File

```c
#include <linux/jiffies.h>
#include <linux/hrtimer.h>
```

# 2. Timer **Components**

## 1. **Timer Structure**

1. **High-Resolution Timer**:
    - HRTIMERS offer better precision than traditional timers (like `struct timer_list`) by using the system clock with a finer granularity, allowing for scheduling in microseconds or even nanoseconds, depending on the hardware.
2. **Timer Structure**:
    - The main data structure used in the HRTIMER API is `struct hrtimer`. This structure holds information about the timer, including its state, expiration time, and callback function.

## 2. **Hrtimer Key Functions**

### 1. **Initialization**

```c
void hrtimer_init(struct hrtimer *timer, clockid_t clockid, enum hrtimer_mode mode);
```

- Initializes a high-resolution timer.
- **Parameters**:
    - `timer`: Pointer to the `hrtimer` structure.
    - `clockid`: Specifies the clock to be used (e.g., `CLOCK_MONOTONIC`).
    - `mode`: Specifies whether the timer is absolute or relative.

### 2. **Starting a Timer**

```c
int hrtimer_start(struct hrtimer *timer, ktime_t tim, enum hrtimer_mode mode);
```

- Starts or restarts the high-resolution timer.
- **Parameters**:
    - `tim`: The time until the timer expires, specified as a `ktime_t` value.
    - `mode`: Specifies whether the timer is absolute or relative.

### 3. **Cancelling a Timer**:

```c
int hrtimer_cancel(struct hrtimer *timer);
```

- Cancels the timer if it is currently active.
- Returns the status of the timer (e.g., whether it was active or not).

### **4. Timer Callback**

- The callback function is defined by the user and is executed when the timer expires. The function should match the following prototype:

```c
enum hrtimer_restart my_timer_callback(struct hrtimer *timer);
```

### **5. Retrieving Remaining Time**

```c
ktime_t hrtimer_get_remaining(const struct hrtimer *timer);
```

- Returns the remaining time for an active timer.

### 6. **Checking Timer Status**

```c
bool hrtimer_is_active(const struct hrtimer *timer);
```

- Checks if the timer is currently active.
