//
// Basic module to test file operations on module 
//

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
 
static char my_buf;
static dev_t n_dev; // Global variable to hold device numbers - really a u32 in code
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class

static int my_open(struct inode *i, struct file *f)
{
  printk(KERN_INFO "Test: open()\n");
  return 0;
}

static int my_close(struct inode *i, struct file *f)
{
  printk(KERN_INFO "Test: close()\n");
  return 0;
}

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
  printk(KERN_INFO "Test: read()\n");
  if (copy_to_user(buf, &my_buf, 1) != 0) {
  	return -EFAULT;
  }
  else {
  	return 1;
  }
}

static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
  printk(KERN_INFO "Test: write()\n");
  if (copy_from_user(&my_buf, buf, len) != 0) {
  	return -EFAULT;
  }
  else {
 	return len;
  }
}

static struct file_operations dev_fops =
{
  .owner   = THIS_MODULE,
  .open    = my_open,
  .release = my_close,
  .read    = my_read,
  .write   = my_write
};
 
static int __init test_init(void) 
{
  printk(KERN_INFO "Test Registered");

  if (alloc_chrdev_region(&n_dev, 5, 15, "MyTest") < 0) {
  	return -1;
  }

  printk(KERN_INFO "Test (Major, Minor): %d, %d\n", MAJOR(n_dev), MINOR(n_dev)); // Macros to get number

  if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL) {
    unregister_chrdev_region(n_dev, 5);  // Unregister dev number if fail
    return -1;
  }

  if (device_create(cl, NULL, n_dev, NULL, "mydev") == NULL) {
    class_destroy(cl);  // Destroy class if fail
    unregister_chrdev_region(n_dev, 5);  // Unregister dev number if fail
    return -1;
  }

  cdev_init(&c_dev, &dev_fops);

  if (cdev_add(&c_dev, n_dev, 1) == -1) {
    device_destroy(cl, n_dev);   // Destroy device if fail
    class_destroy(cl);  // Destroy class if fail
    unregister_chrdev_region(n_dev, 5);  // Unregister dev number if fail
    return -1;
  }

  return 0;
}
 
static void __exit test_exit(void)
{
    cdev_del(&c_dev);
    device_destroy(cl, n_dev);   // Destroy device 
    class_destroy(cl);  // Destroy class 
    unregister_chrdev_region(n_dev, 5);  // Unregister dev number 
    printk(KERN_INFO "Test Unregistered");
}
 
module_init(test_init);
module_exit(test_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brandon Miller");
MODULE_DESCRIPTION("Driver testing file operations");
