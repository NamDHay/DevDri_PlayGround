# Lesson 4: GPIO Driver

# 1. Include File

```c
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
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

# 3. GPIO API

## **1. Requesting a GPIO Pin**

```c
int gpio_request(unsigned int gpio, const char *label);
```

- **Parameters**:
    - `gpio`: The GPIO number you want to request.
    - `label`: A string label for the GPIO, used for debugging.
- **Returns**:
    - `0` on success.
    - A negative error code on failure.

## **2. Setting GPIO Direction (Output)**

```c
int gpio_direction_output(unsigned gpio, int value);
```

- **Parameters**:
    - `gpio`: The GPIO number to set as output.
    - `value`: The initial value to set the GPIO (0 for low, 1 for high).
- **Returns**:
    - `0` on success.
    - A negative error code on failure.

## 3. **Setting GPIO Direction (Input)**

```c
int gpio_direction_input(unsigned gpio);
```

- Parameters
    - **`gpio`**: The GPIO number you want to configure as an input.
- Returns
    - **`0`** on success.
    - A negative error code on failure (e.g., if the GPIO pin is already configured in a way that prevents it from being set as an input).

## 4. Get GPIO Value

```c
int gpio_get_value(unsigned gpio);
```

- Parameters
    - **`gpio`**: The GPIO number you want to read the value from.
- Returns
    - **`1`** if the GPIO pin is high.
    - **`0`** if the GPIO pin is low.
    - **Negative error code** if the pin cannot be read (e.g., if it has not been configured as an input).

## 5. Set GPIO Value

```c
void gpio_set_value(unsigned gpio, int value);
```

- Parameters
    - **`gpio`**: The GPIO number you want to set the value for.
    - **`value`**: The value to set the GPIO pin to:
        - `0` for low.
        - `1` for high.

## 6. Free GPIO Pin

```c
void gpio_free(unsigned gpio);
```

- Parameters
    - **`gpio`**: The GPIO number that you want to free.
