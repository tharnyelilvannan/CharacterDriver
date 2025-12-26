#include "driver.h"

static uint8_t major;

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
    printk(KERN_INFO "Read.");
    return 0;
}

static ssize_t dwrite(struct file *file, const char __user *u_buffer, size_t size, loff_t *offset) {
    struct dev_data *d_dev_data;
    d_dev_data = (struct dev_data *) file->private_data;
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
        printk(KERN_INFO "Initialized. Major number is %d", major);
        return 0;
    }
}

void exit_driver(void) {
    unregister_chrdev(major, "driver");
    printk(KERN_INFO "Exited.");
}

module_init(init_driver);
module_exit(exit_driver);