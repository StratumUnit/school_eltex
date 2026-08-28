
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/poll.h>
#include <linux/uaccess.h>
#include <linux/version.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shmakov");
MODULE_DESCRIPTION("a educational kernel module for lab4");

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "chardev"
#define BUF_LEN 256

static int major;

static char msg[BUF_LEN];
static int len;
static struct class *cls;

static struct file_operations chardev_fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
};

static int __init chardev_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &chardev_fops);

    if (major < 0)
    {
        pr_alert("Registering char device failed with %d\n", major);
        return major;
    }
    pr_info("I was assigned major number %d.\n", major);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
    cls = class_create(DEVICE_NAME);
#else
    cls = class_create(THIS_MODULE, DEVICE_NAME);
#endif
    device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    pr_info("Device created on /dev/%s\n", DEVICE_NAME);
    return SUCCESS;
}

static void __exit chardev_exit(void)
{
    device_destroy(cls, MKDEV(major, 0));
    class_destroy(cls);

    unregister_chrdev(major, DEVICE_NAME);
}

static int device_open(struct inode *inode, struct file *file)
{
    try_module_get(THIS_MODULE);
    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
    module_put(THIS_MODULE);
    return SUCCESS;
}

static ssize_t device_read(struct file *filp, char *buf, size_t count, loff_t *offp)
{
    if (*offp >= len)
    {
        return 0;
    }

    if (len - *offp < count)
    {
        count = len - *offp;
    }

    if (copy_to_user(buf, msg + *offp, count) > 0)
    {
        return -EFAULT;
    }
    *offp += count;

    return count;
}

static ssize_t device_write(struct file *filp, const char *buf, size_t count, loff_t *offp)
{

    loff_t write_pos = *offp;

    if (filp->f_flags & O_APPEND)
    {
        write_pos = len;
    }
    if (write_pos + count > BUF_LEN)
    {
        return -ENOSPC;
    }
    if (copy_from_user(msg + write_pos, buf, count) > 0)
    {
        return -EFAULT;
    }
    len = write_pos + count;
    msg[len] = '\0';
    *offp = len;
    return count;
}

module_init(chardev_init);
module_exit(chardev_exit);