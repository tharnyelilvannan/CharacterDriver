#include "driver.h"

static uint8_t major;
struct class *d_class;
struct device *d_device;

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Linux Character Driver");
MODULE_AUTHOR("Tharani Elilvannan");

struct dev_data {
    struct cdev cdev;
};

static int dopen(struct inode *inode, struct file *file) {
    struct dev_data *d_dev_data;
    d_dev_data = container_of(inode->i_cdev, struct dev_data, cdev);
    file->private_data = d_dev_data;
    printk(KERN_INFO "Opened.");
    return 0;
}

static ssize_t dread(struct file *file, char __user *u_buffer, size_t size, loff_t *offset) {
    struct dev_data *d_dev_data;
    d_dev_data = (struct dev_data *) file->private_data;

    unsigned long byte = size - *offset;
    char k_buffer[1024];

    int err = copy_to_user(u_buffer, k_buffer, byte);

    if (err != 0) {
        printk(KERN_ERR "Failed to read %d bytes.", err);
        return -EFAULT;
    }

    printk(KERN_INFO "Read.");
    return 0;
}

static ssize_t dwrite(struct file *file, const char __user *u_buffer, size_t size, loff_t *offset) {
    struct dev_data *d_dev_data;
    d_dev_data = (struct dev_data *) file->private_data;

    unsigned long byte = size - *offset;
    char k_buffer[1024];

    int err = copy_from_user(k_buffer, u_buffer, byte);

    if (err != 0) {
        printk(KERN_ERR "Failed to write %d bytes.", err);
        return -EFAULT;
    }

    printk(KERN_INFO "Wrote.");
    return 0;
}

static int drelease(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Released.");
    return 0;
}

const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dopen,
    .read = dread,
    .write = dwrite,
    .release = drelease
};

int init_driver(void) {
    // static allocation
    major = register_chrdev(0, "driver", &fops);
    if (major < 0) {
        printk(KERN_ERR "Error allocating major.");
        return major;
    }
    else {
        printk(KERN_INFO "Major number is %d.", major);
    }

    d_class = class_create(THIS_MODULE, "d_class");
    if (IS_ERR(d_class)) {
        printk(KERN_ERR "Error creating class.");
        return -1;
    }
    else {
        printk(KERN_INFO "Created class.");
    }

    d_device = device_create(d_class, NULL, MKDEV(major, 0), NULL, "d_device");
    if (IS_ERR(d_device)) {
        printk(KERN_ERR "Error creating device.");
        class_destroy(d_class);
        return -1;
    }
    else {
        printk(KERN_INFO "Created device.");
    }

    printk(KERN_INFO "Initialized.");
    return 0;

}

void exit_driver(void) {
    unregister_chrdev(major, "driver");
    device_destroy(d_class, MKDEV(major, 0));
    class_destroy(d_class);
    printk(KERN_INFO "Exited.");
}

module_init(init_driver);
module_exit(exit_driver);