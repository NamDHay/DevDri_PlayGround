# Lesson 1: Hello World Linux Kernel Module

## 1. **Module Metadata**

```c
MODULE_LICENSE("GPL");
MODULE_AUTHOR("NamDHay");
MODULE_DESCRIPTION("A hello world LKM");
```

- These macros provide metadata about the module:
    - **License**: Specifies that the module is GPL licensed, which is important for kernel modules.
    - **Author**: Indicates the author of the module.
    - **Description**: A brief description of what the module does.

## 2. **Initialization Function**

```c
static int __init ModuleInit(void) {
    printk("Hello Kernel\n");
    return 0;
}
```

- **Purpose**: This function is called when the module is loaded into the kernel.
- **`__init`**: A macro indicating that the function is for initialization and may be discarded after the module is loaded.
- **`printk`**: A kernel function similar to `printf`, used for logging messages to the kernel log.
- The function returns `0`, indicating successful initialization.

## 3. **Exit Function**

```c
static void __exit ModuleExit(void) {
    printk("Goodbye Kernel\n");
}
```

- **Purpose**: This function is called when the module is removed from the kernel.
- **`__exit`**: A macro indicating that this function is called on module removal.
- It also uses `printk` to log a message.

## 4. **Module Entry and Exit Points**

```c
module_init(ModuleInit);
module_exit(ModuleExit);
```

• These macros register the initialization and exit functions with the kernel. They tell the kernel which functions to call when the module is loaded and unloaded.
