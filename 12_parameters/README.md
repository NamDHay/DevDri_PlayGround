# Lesson 12: Using Parameters in a Linux Kernel Module

# Supported Parameter Types

## **1. Boolean Types**:

- `bool`: Represents a true or false value (stored as `int`).
- `invbool`: Inverts the boolean value (true becomes false and vice versa).

## **2. String Type**:

- `charp`: A pointer to a string, with memory allocated for user-provided values.

## **3. Integer Types**:

- `int`, `long`, `short`: Signed integer types.
- `uint`, `ulong`, `ushort`: Unsigned integer types.

## **4. Array Parameters**:

- Use `module_param_array(name, type, num, perm)` for array parameters, where:
    - `name`: Name of the array.
    - `type`: Type of array elements.
    - `num`: Variable to store the number of values.
    - `perm`: Permissions for accessing the array.

# Additional Notes

- Each parameter should have a default value. The `insmod` command can override this value if specified.
- The permission value controls access to the parameter in `sysfs`:
    - `S_IRUGO`: Readable by all, not writable.
    - `S_IRUGO | S_IWUSR`: Readable and writable by the root user.
    - A permission value of `0` means no `sysfs` entry is created.

# **Key Components of Module Parameters**

1. **Declaration**:
    - In your code, you declare two parameters:
    
    ```c
    static unsigned int gpio_nr = 12;
    static char *device_name = "testdevice";
    ```
    
2. **Module Parameter Macros:**
    - You use the `module_param` macro to define how the parameters can be set:
        
        ```c
        module_param(gpio_nr, uint, S_IRUGO);
        module_param(device_name, charp, S_IRUGO);
        ```
        
    - The first argument is the parameter name, the second specifies the type (`uint` for unsigned int and `charp` for a string), and the third argument defines permissions:
3. **Parameter Descriptions**:
    
    The `MODULE_PARM_DESC` macro provides descriptions for each parameter:
    
    ```c
    MODULE_PARM_DESC(gpio_nr, "Nr of GPIO use in this LKM");
    MODULE_PARM_DESC(device_name, "Device name use in this LKM");
    ```
    
4. **Using the Parameters**:
    - In the `ModuleInit` function, you access the parameters:
    
    ```c
    printk("gpio_nr = %u\n", gpio_nr);
    printk("device_name = %s\n", device_name);
    ```
    
    - When the module is loaded, these values will be printed to the kernel log, allowing you to confirm that they have been set correctly.
5. **Loading the Module with Parameters**
    
    When loading the module using `insmod`, you can specify the parameters like this:
    
    ```bash
    sudo insmod your_module.ko gpio_nr=15 device_name="mydevice"
    ```
