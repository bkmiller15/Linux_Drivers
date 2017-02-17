//
// Basic module to test major and minor numbers
//

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
 
static dev_t dvar; // Global variable to hold device numbers - really a u32 in code
 
static int __init test_init(void) 
{
    printk(KERN_INFO "Test Registered");

    if (alloc_chrdev_region(&dvar, 5, 15, "MyTest") < 0) {
        return -1;
    }

    printk(KERN_INFO "Test (Major, Minor): %d, %d\n", MAJOR(dvar), MINOR(dvar)); // Macros to get number
    return 0;
}
 
static void __exit test_exit(void) 
{
    unregister_chrdev_region(dvar, 5);
    printk(KERN_INFO "Test Unregistered");
}

module_init(test_init);
module_exit(test_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brandon Miller");
MODULE_DESCRIPTION("Driver testing module major and minor numbers");
