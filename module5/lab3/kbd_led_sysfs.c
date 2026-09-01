#include <linux/atomic.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h> /* for sprintf() */
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/uaccess.h> /* for get_user and put_user */
#include <linux/version.h>
#include <linux/tty.h> /* For fg_console, MAX_NR_CONSOLES */
#include <linux/kd.h>  /* For KDSETLED */
#include <linux/vt_kern.h>
#include <linux/vt.h>
#include <linux/console_struct.h>

#define SYS_NAME "kbd_leds"
#define RESTORE_LEDS 0xFF
#define LED_MASK_MIN 0
#define LED_MASK_MAX 7

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shmakov");
MODULE_DESCRIPTION("a educational kernel module for lab3");

static struct kobject *kobj;
static int test;

static void set_leds(int led_val)
{
    if (vc_cons[fg_console].d && vc_cons[fg_console].d->port.tty)
    {
        struct tty_struct *tty = vc_cons[fg_console].d->port.tty;

        if (tty->driver && tty->driver->ops && tty->driver->ops->ioctl)
        {
            tty->driver->ops->ioctl(tty, KDSETLED, led_val);
        }
    }
}

static ssize_t state_show(struct kobject *kobj, struct kobj_attribute *attr,
                          char *buf)
{
    return sprintf(buf, "%d\n", test);
}

static ssize_t state_store(struct kobject *kobj, struct kobj_attribute *attr,
                           const char *buf, size_t count)
{
    int temp_value;
    if (kstrtoint(buf, 0, &temp_value) < 0)
    {
        return -EINVAL;
    }

    if ((temp_value < LED_MASK_MIN || temp_value > LED_MASK_MAX) && temp_value != RESTORE_LEDS)
    {
        return -EINVAL;
    }

    test = temp_value;
    set_leds(test);
    return count;
}

static struct kobj_attribute foo_attribute = __ATTR(state, 0660, state_show, state_store);

static int __init
sys_init(void)
{
    int error;
    if (!vc_cons[fg_console].d || !vc_cons[fg_console].d->port.tty)
    {
        pr_info("Active console TTY is not available\n");
        return -ENOTTY;
    }

    kobj = kobject_create_and_add(SYS_NAME, kernel_kobj);
    if (!kobj)
    {
        return -ENOMEM;
    }

    error = sysfs_create_file(kobj, &foo_attribute.attr);
    if (error)
    {
        pr_info("failed to create the sysfs file\n");
        kobject_put(kobj);
        return error;
    }
    pr_info("Module initialized successfully \n");
    return 0;
}

static void __exit sys_exit(void)
{
    
    set_leds(RESTORE_LEDS);
    kobject_put(kobj);
    pr_info("Module uninitialized successfully \n");
}

module_init(sys_init);
module_exit(sys_exit);