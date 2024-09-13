# Lesson 3: Auto Device File creation & Read- Write-Callbacks

## 1. **Global Variables**

```c
static char buffer[255];
static int buffer_pointer = 0;

static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;
```

- **`buffer`**: An array to store data written to the device.
- **`buffer_pointer`**: Keeps track of the amount of data currently stored in the buffer.
- **`my_device_nr`**: Holds the device number allocated for the character device.
- **`my_class`**: Pointer to the device class.
- **`my_device`**: The character device structure.

## **2. File Operations Functions**

- **`driver_read`**
    
    ```c
    static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs) {
    	size_t to_copy, not_copied, delta;
    
    	/* Get amount of data to copy */
    	to_copy = min(count, (size_t)buffer_pointer);
    
    	/* Copy data to user */
    	not_copied = copy_to_user(user_buffer, buffer, to_copy);
    
    	/* Calculate data */
    	delta = to_copy - not_copied;
    
    	return delta;
    }
    ```
    
    - Reads data from the kernel buffer to the user-space buffer.
    - Uses `copy_to_user` to transfer data and returns the number of bytes actually copied.
- **`driver_write`**
    
    ```c
    static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
    	int to_copy, not_copied, delta;
    
    	/* Get amount of data to copy */
    	to_copy = min(count, sizeof(buffer));
    
    	/* Copy data to user */
    	not_copied = copy_from_user(buffer, user_buffer, to_copy);
    	buffer_pointer = to_copy;
    
    	/* Calculate data */
    	delta = to_copy - not_copied;
    
    	return delta;
    }
    ```
    
    - Writes data from the user-space buffer to the kernel buffer.
    - Uses `copy_from_user` to transfer data and updates `buffer_pointer` with the number of bytes written.

## 3. **File Operations Structure**

```c
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close,
    .read = driver_read,
    .write = driver_write
};
```

- This structure links the file operations to their respective functions.

## 4. Module Init

- **Allocate Character Device Number**
    
    ```c
    if( alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0) {
        printk("Device Nr. could not be allocated!\n");
        return -1;
    }
    ```
    
    - **`alloc_chrdev_region`**: Allocates a range of character device numbers.
        - The first argument is a pointer to `my_device_nr`, which will receive the allocated device number.
        - The second argument (`0`) specifies that no starting minor number is requested, and the third argument (`1`) requests one device number.
        - The last argument is the name of the driver.
    - If allocation fails (returns a negative value), it logs an error message and returns `1` to indicate failure.
- **Log the Allocated Device Number**
    
    ```c
    printk("read_write - Device Nr. Major: %d, Minor: %d was registered!\n", my_device_nr >> 20, my_device_nr & 0xfffff);
    ```
    
    - This logs the major and minor numbers of the allocated device. The major number is extracted by shifting right by 20 bits, and the minor number is obtained using a bitwise AND operation.
- **Create Device Class**
    
    ```c
    if((my_class = class_create(DRIVER_CLASS)) == NULL) {
        printk("Device class can not be created!\n");
        goto ClassError;
    }
    ```
    
    - **`class_create`**: Creates a device class to group devices.
        - If the class creation fails (returns `NULL`), it logs an error and jumps to `ClassError` for cleanup.
- **Create Device File**
    
    ```c
    if(device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL) {
        printk("Can not create device file!\n");
        goto FileError;
    }
    ```
    
    - **`device_create`**: Creates a device file in `/dev` associated with the device class.
        - If this fails (returns `NULL`), it logs an error and jumps to `FileError` for cleanup.
- **Initialize Character Device Structure**
    
    ```c
    cdev_init(&my_device, &fops);
    ```
    
    - **`cdev_init`**: Initializes the character device structure (`my_device`) with the file operations defined in `fops`.
- **Add Character Device to the Kernel**
    
    ```c
    if(cdev_add(&my_device, my_device_nr, 1) == -1) {
        printk("Registering of device to kernel failed!\n");
        goto AddError;
    }
    ```
    
    - **`cdev_add`**: Registers the character device with the kernel.
        - The first argument is the initialized device structure, the second is the device number, and the last specifies how many devices to register.
        - If this fails (returns `1`), it logs an error and jumps to `AddError` for cleanup.
- **Error Handling Sections**
    - Each error handling section (`AddError`, `FileError`, `ClassError`) is responsible for cleaning up resources that were allocated before the failure. This ensures that there are no memory leaks or dangling resources left when an error occurs:
        - **`device_destroy`**: Cleans up the device file.
        - **`class_destroy`**: Cleans up the device class.
        - **`unregister_chrdev_region`**: Deallocates the character device number.

## **5. Interface with module**

- Insert module

```bash
sudo insmod read_write.ko #insert module to kernel
ls /dev/DummyDriver #Check is driver insert
sudo chmod 666 /dev/DummyDriver #enable read write access for driver
```

- Communicate with driver
    - Bash Script:
        
        ```bash
        	echo "Hello NamDHay" > /dev/DummyDriver
        	head -n 1 /dev/DummyDriver
        ```
        
    - C code:
        
        ```c
        #include <fcntl.h>
        #include <unistd.h>
        #include <stdio.h>
        #include <string.h>
        
        int main() {
            int fd = open("/dev/DummyDriver", O_RDWR);
            if (fd == -1) {
                perror("Failed to open device");
                return 1;
            }
        
            const char *msg = "Hello, Kernel!";
            write(fd, msg, strlen(msg));
        
            char buffer[255];
            lseek(fd, 0, SEEK_SET);  // Reset offset to read from the beginning
            read(fd, buffer, sizeof(buffer));
        
            printf("Read from driver: %s\n", buffer);
        
            close(fd);
            return 0;
        }
        ```
        
        Compile by
        
        ```bash
        gcc -o test_driver test_driver.c
        ```
