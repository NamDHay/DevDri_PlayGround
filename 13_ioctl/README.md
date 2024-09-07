# Lesson 2: Device Numbers and Files

## 1. **File Operations Functions**

- **`driver_open`**
    
    ```c
    static int driver_open(struct inode *device_file, struct file *instance) {
        printk("dev_nr - open was called\n");
        return 0;
    }
    ```
    
    - This function is called when the device file is opened.
    - It logs a message and returns `0` to indicate success.
- **`driver_close`**
    
    ```c
    static int driver_close(struct inode *device_file, struct file *instance) {
        printk("dev_nr - close was called\n");
        return 0;
    }
    ```
    
    - This function is called when the device file is closed.
    - It also logs a message and returns `0`.

## 2. **File Operations Structure**

```c
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close
};
```

- This structure defines the operations for the character device. It associates the `open` and `release` functions with the device.

## 3. **Device Major Number Definition**

```c
#define MYMAJOR 90
```

- This defines a fixed major number (`90`) for the character device. Major numbers identify the driver associated with the device.

## **4. Module Initialization**

```c
static int __init ModuleInit(void) {
    int retval;
    retval = register_chrdev(MYMAJOR, "my_dev_nr", &fops);
    if (retval == 0) {
        printk("dev_nr - register Device Number Major: %d, Minor: %d\n", MYMAJOR, 0);
    } else if (retval > 0) {
        printk("dev_nr - register Device Number Major: %d, Minor: %d\n", retval >> 20, retval & 0xfffff);
    } else {
        printk("Could not register device number\n");
    }
    return 0;
}
```

- This function is called when the module is loaded.
- It registers the character device using `register_chrdev`.
- Depending on the return value, it logs whether the registration was successful and prints the major and minor numbers.

## 5. Module exit

```c
static void __exit ModuleExit(void) {
    unregister_chrdev(MYMAJOR, "my_dev_nr");
}
```

## Run the module

To interact with the device, create a device file using `mknod`:

```bash
sudo mknod /dev/my_dev_nr c 90 0
```

You can write a simple user-space program to interact with the device, or you can use tools like `cat` or `echo` to test it. However, since the device does not currently handle any data read/write, you will only see log messages when opening and closing the device.

After testing, you can unload the module with:

```bash
sudo rmmod my_char_device
```
