# Device Driver Note

# Tutorial

[![YouTube](https://img.shields.io/badge/YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://www.youtube.com/playlist?list=PLCGpd0Do5-I3b5TtyqeF1UdyD4C-S-dMa)

[![GitHub](https://img.shields.io/badge/GitHub-181717?style=for-the-badge&logo=github&logoColor=white)](https://github.com/Johannes4Linux/Linux_Driver_Tutorial)
# **Prerequisites**

1. **Linux Environment**: Ensure you are running a Linux distribution.
2. **Kernel Headers**: You need the kernel headers that match your running kernel. You can install them with:
    
    ```bash
    sudo apt install linux-headers-$(uname -r)  # For Debian/Ubuntu-based systems
    ```
    
3. **Build Tools**: Install `make` and `gcc` if they are not already installed:
    
    ```bash
    sudo apt install build-essential
    ```
    

# How to run the module

## **Step 1: Create the Source File**

1. **Create a new directory for your module**:
    
    ```bash
    mkdir ~/hello_module
    cd ~/hello_module
    ```
    
2. Create file C code
    
    ```bash
    nvim file.c
    ```
    

## **Step 2: Create a Makefile**

```makefile
obj-m += file.o

all:
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

## **Step 3: Compile the Module**

```bash
make
```

You should see output indicating that the module is being compiled. If successful, this will generate a file named `file.ko` in your directory.

## **Step 4: Load the Module**

Use the `insmod` command to load the module into the kernel:

```bash
sudo insmod file.ko
```

To check if the module is loaded successfully, you can use:

```bash
lsmod | grep file
```

## Step 5: Check Kernel Messages

To see the "Hello Kernel" message, check the kernel log:

```bash
dmesg | tail
```

This will display the latest messages from the kernel log, including the message printed by your module.

## Step 6: Remove the Module

After testing, you can unload the module with:

```bash
sudo rmmod file
```

Check the kernel messages again to see the "Goodbye Kernel" message:

```bash
dmesg | tail
```

## Step 7: Clean Up

To remove the compiled files, run:

```bash
make clean
```

## Summary

1. **Create the module source file and Makefile.**
2. **Compile the module using `make`.**
3. **Load the module using `insmod`.**
4. **Check the kernel log for messages using `dmesg`.**
5. **Unload the module using `rmmod`.**
6. **Clean up compiled files with `make clean`.**

# OUTLINE

[**Lesson 1: Hello World Linux Kernel Module**](01_Hello_Kernel/README.md)

[**Lesson 2: Device Numbers and Files**](02_dev_nr/README.md)

[**Lesson 3: Auto Device File creation & Read- Write-Callbacks**](03_read_write/README.md)

[**Lesson 4: GPIO Driver**](04_gpio_driver/README.md)

[**Lesson 12: Using Parameters in a Linux Kernel Module**](12_parameters/README.md)

