#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define MSG_SIZE 8192
#define PROC_NAME "myproc"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shmakov");
MODULE_DESCRIPTION("a educational kernel module for lab2");

static int len;
static char *msg;

static struct proc_dir_entry *proc_file;

static ssize_t read_proc(struct file *filp, char *buf, size_t count, loff_t *offp)
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

static ssize_t write_proc(struct file *filp, const char *buf, size_t count, loff_t *offp)
{

    loff_t write_pos = *offp;

    if (filp->f_flags & O_APPEND)
    {
        write_pos = len;
    }
    if (write_pos + count > MSG_SIZE)
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

static const struct proc_ops proc_fops = {
    .proc_read = read_proc,
    .proc_write = write_proc,
};

static int create_new_proc_entry(void)
{
    proc_file = proc_create(PROC_NAME, S_IRUGO | S_IWUSR, NULL, &proc_fops);
    if (proc_file == NULL)
    {
        pr_alert("Error: Could not initialize /proc/%s\n", PROC_NAME);

        return -ENOMEM;
    }
    pr_info("/proc/%s created\n", PROC_NAME);
    msg = kmalloc(MSG_SIZE * sizeof(char), GFP_KERNEL);
    if (!msg)
    {
        proc_remove(proc_file);
        return -ENOMEM;
    }
    memset(msg, 0, MSG_SIZE);
    return 0;
}

static int __init proc_init(void)
{
    int result = create_new_proc_entry();
    if (result < 0)
    {
        return result;
    }
    return 0;
}

static void __exit proc_cleanup(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    kfree(msg);
}

module_init(proc_init);
module_exit(proc_cleanup);