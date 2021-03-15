/* Interrupt Demo Driver
 *
 * This is a character driver, which is used to demostrate Exynos-4412's interrupts
 */

/* Main header files */
#include <linux/init.h>
#include <linux/module.h>
/* Useful header files */
#include <linux/kernel.h>
#include <linux/fs.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <mach/regs-gpio.h>
#include <asm/io.h>
#include <linux/regulator/consumer.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/device.h>
/* Interrupt-related header files */
#include <linux/interrupt.h>
#include <linux/irq.h>
/* Local header files */
#include "interrupt-demo.h"

int interrupt_demo_open(struct inode *inode, struct file *filp){
    printk(KERN_INFO "InterruptDemo: Device file opending...\n");
    return 0;
}

static int interrupt_demo_release (struct inode *node, struct file *file){
    printk(KERN_INFO "InterruptDemo: Device file closing...\n");
    return 0;
}

ssize_t interrupt_demo_read(struct file *file,char __user *buff,size_t count,loff_t *offp){
    printk(KERN_INFO "InterruptDemo: Reading data from device file...\n");
    return 0;
}

ssize_t interrupt_demo_write(struct file *file,const char __user *buff,size_t count,loff_t *offp){
    printk(KERN_INFO "InterruptDemo: Wrtiting data to device file...\n");
    return 0;
}
 
static int interrupt_demo_ioctl(struct inode *inode,struct file *file,unsigned int cmd,unsigned long arg){  
    printk(KERN_INFO "InterruptDemo: IOControl command %u with argument %u received.\n", cmd, arg);
    return 0;
}
 
static struct file_operations interrupt_demo_driver_file_operations = {
    .owner = THIS_MODULE,
    .open  = interrupt_demo_open,  //Open device, executed when calling open()
    .release = interrupt_demo_release, //Release device, executed when calling close()
    .read  = interrupt_demo_read, //Read operations, executed when calling read()
    .write = interrupt_demo_write, //Write operations, executed when calling write()
    .ioctl = interrupt_demo_ioctl, //IOControl, executed when calling ioctl()
};

static int interrupt_demo_probe(struct platform_device *pdv){
	printk(KERN_INFO "InterruptDemo: Initializing...\n");
	return 0;
}

static int interrupt_demo_remove(struct platform_device *pdv){
	printk(KERN_INFO "InterruptDemo: Removing...\n");
	return 0;
}

static int interrupt_demo_shutdown(struct platform_device *pdv){
	printk(KERN_INFO "InterruptDemo: Shutting down...\n");
	return 0l
}

static int interrupt_demo_suspend(struct platform_device *pdv){
	printk(KERN_INFO "InterruptDemo: Suspending...\n");
	return 0;
}

static int interrupt_demo_resume(struct platform_device *pdv){
	printk(KERN_INFO "InterruptDemo: Resuming...\n");
	return 0;
}

static int __init interrupt_demo_init(void){
	printk(KERN_INFO "InterruptDemo: Initializing...\n");
	return 0;
}

static void __exit interrupt_demo_exit(void){
	printk(KERN_INFO "InterruptDemo: Exiting...\n");
	return;
}

/* Init function & Exit function */
module_init(interrupt_demo_init);
module_exit(interrupt_demo_exit);

/* Platform Driver related functions */
struct platform_driver interrupt_demo_driver{
	.probe = interrupt_demo_probe,
	.remove = interrupt_demo_remove,
	.shutdown = interrupt_demo_shutdown,
	.suspend = interrupt_demo_suspend,
	.resume = interrupt_demo_suspend,
	.driver = {
		.name = DRIVER_NAME
		.owner = THIS_MODULE
	}
};


/* Licenses, Authors & Descriptions */
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Picsell-Dois@SEU-BME");
MODULE_DESCRIPTION("Exynos-4412 interrupt demo driver");
