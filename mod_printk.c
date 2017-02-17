//
// Basic module to test printk
//

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
 
static int __init test_init(void) 
{
    printk(KERN_INFO "Test Registered");
    return 0;
}
 
static void __exit test_exit(void)
{
    printk(KERN_INFO "Test Unregistered");
}
 
module_init(test_init);
module_exit(test_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brandon Miller");
MODULE_DESCRIPTION("Driver testing printk");
