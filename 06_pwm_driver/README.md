# Lesson 6: PWM Module

First of all, add this command in `/boot/config.txt` 

```toml
dtoverlay=pwm-2chan,pin=12,func=4,pin2=13,func2=14
```

# 1. Include File

```c
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/pwm.h>
```

# 2. **Device Registration**

```c
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;
```

- `dev_t` is used to hold the device number.
- `struct class` represents the class of devices (to create device files in `/dev`).
- `struct cdev` is used for character device registration.

# 3. PWM **Components**

## 1. **PWM Device Structure**

```c
/**
 * struct pwm_device - PWM channel object
 * @label: name of the PWM device
 * @flags: flags associated with the PWM device
 * @hwpwm: per-chip relative index of the PWM device
 * @chip: PWM chip providing this PWM device
 * @args: PWM arguments
 * @state: last applied state
 * @last: last implemented state (for PWM_DEBUG)
 */
struct pwm_device {
	const char *label;
	unsigned long flags;
	unsigned int hwpwm;
	struct pwm_chip *chip;

	struct pwm_args args;
	struct pwm_state state;
	struct pwm_state last;
};

//Example
struct pwm_device *pwm0 = NULL;
u32 pwm_on_time = 500000; // Initial PWM on time
```

• Initializes a pointer to the PWM device and defines the initial value for the PWM on time.

## 2. **Functions in the PWM API**

### 1. **Requesting a PWM Device**

```c
struct pwm_device *pwm_request(unsigned int pwm, const char *label);
```

- **Parameters**:
    - `pwm`: The PWM channel number.
    - `label`: A string label used for debugging.
- **Returns**: A pointer to the `pwm_device` structure or `NULL` if the request fails.

### 2. **Configuring PWM**

```c
int pwm_config(struct pwm_device *pwm, int duty_ns, int period_ns);
```

- **Parameters**:
    - `pwm`: The PWM device pointer.
    - `duty_ns`: The duty cycle in nanoseconds (how long the signal is high).
    - `period_ns`: The total period of the PWM signal in nanoseconds.
- **Returns**: `0` on success, or a negative error code on failure.

### 3. **Enabling PWM**

```c
int pwm_enable(struct pwm_device *pwm);
```

- **Parameters**:
    - `pwm`: The PWM device pointer.
- **Returns**: `0` on success, or a negative error code on failure.

### 4. **Disabling PWM**

```c
void pwm_disable(struct pwm_device *pwm);
```

- **Parameters**:
    - `pwm`: The PWM device pointer.
- **Returns**: None.

### 5. **Freeing a PWM Device**:

```c
void pwm_free(struct pwm_device *pwm);
```

- **Parameters**:
    - `pwm`: The PWM device pointer.
- **Returns**: None.
- This function releases any resources associated with the PWM device.
