#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/wait.h>

static uint8_t major;
struct class *d_class;
struct device *d_device;
struct cdev d_cdev;

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Linux Character Driver");
MODULE_AUTHOR("Tharani Elilvannan");

struct dev_data {
    struct cdev cdev;
};

struct k_buffer {
    int head;
    int tail;
    char *buf;
    int size;
    int max_len;
};

void increment_head(void) {
}

void increment_tail(void) {
}

void decrement_head(void) {
}

void decrement_tail(void) {
}

struct k_buffer buffer;

// opens file
static int dopen(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Opened.");
    return 0;
}

// allows userspace to read
static ssize_t dread(struct file *file, char __user *u_buffer, size_t size, loff_t *offset) {
    int i = 0;
    int y = 0;
    int k_buffer_size = buffer.size;
    int err;
    char *c;

    if (!u_buffer) {
        return -EFAULT;
    }

    while (i < k_buffer_size) {
        c = buffer.buf[buffer.head];
        if (!c) {
            return -EFAULT;
        }

        err = put_user(c, u_buffer);

        if (err != 0) {
            printk(KERN_ERR "Error reading.");
            return -EFAULT;
        }

        u_buffer++;
        buffer.size = buffer.size - 1;
        buffer.head = (buffer.head + 1);
        i++;
    }

    while (y < i) {
        u_buffer--;
        y++;
    }

    printk(KERN_INFO "Read.");
    return 0;
}

// allows userspace to write
static ssize_t dwrite(struct file *file, const char __user *u_buffer, size_t size, loff_t *offset) {
    char x;
    int i = 0;
    int err;

    while (i < size) {
        err = get_user(x, u_buffer++);

        if (err != 0) {
            printk(KERN_ERR "Error writing.");
            return -EFAULT;
        }

        buffer.buf[buffer.tail] = x;
        buffer.size = buffer.size + 1;
        buffer.tail = (buffer.tail + 1);
        i++;
    }

    printk(KERN_INFO "Wrote.");
    return 0;
}

// release resources
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
    int add;

    // static allocation
    major = register_chrdev(0, "driver", &fops);
    if (major < 0) {
        printk(KERN_ERR "Error allocating major.");
        return major;
    }
    else {
        printk(KERN_INFO "Major number is %d.", major);
    }

    cdev_init(&d_cdev, &fops);
    d_cdev.owner = THIS_MODULE;
    add = cdev_add(&d_cdev, major, 0);

    buffer.max_len = 1024;
    buffer.head = 0;
    buffer.tail = 0;
    buffer.buf = kmalloc(buffer.max_len*sizeof(char), GFP_KERNEL);

    if (add < 0) {
        printk(KERN_ERR "Error adding device.");
        unregister_chrdev(major, "driver");
        return -EFAULT;
    }

    d_class = class_create(THIS_MODULE, "d_class");
    if (IS_ERR(d_class)) {
        printk(KERN_ERR "Error creating class.");
        cdev_del(&d_cdev);
        unregister_chrdev(major, "driver");
        return -EFAULT;
    }
    else {
        printk(KERN_INFO "Created class.");
    }

    d_device = device_create(d_class, NULL, MKDEV(major, 0), NULL, "d_device");
    if (IS_ERR(d_device)) {
        printk(KERN_ERR "Error creating device.");
        cdev_del(&d_cdev);
        unregister_chrdev(major, "driver");
        class_destroy(d_class);
        return -EFAULT;
    }
    else {
        printk(KERN_INFO "Created device.");
    }

    printk(KERN_INFO "Initialized.");
    return 0;

}

void exit_driver(void) {
    unregister_chrdev(major, "driver");
    cdev_del(&d_cdev);
    device_destroy(d_class, MKDEV(major, 0));
    class_destroy(d_class);
    printk(KERN_INFO "Exited.");
}

module_init(init_driver);
module_exit(exit_driver);