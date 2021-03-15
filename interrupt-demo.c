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

struct class *interrupt_demo_class; //Device node 
static int major = 0; //Allocate device ID automatically
static struct cdev interrupt_demo_cdev; //cdev structure

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

/* Platform Device related functions */
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

/* Init & Exit functions */
static void interrupt_demo_setup_cdev(struct cdev *dev, int minor, struct file_operations *fops){ //Device setup functions, called by init()
    int err, devno = MKDEV(major, minor);
    cdev_init(dev, fops); //Initialize cdev
    dev->owner = THIS_MODULE;
    dev->ops = fops;
    err = cdev_add(dev, devno, 1);
    if(err){
        printk(KERN_NOTICE "InterruptDemo: Error %d adding device %d.\n", err, minor);
    }
    printk("InterruptDemo: Device setup process finished.\n");
}

static int __init interrupt_demo_init(void){
	printk(KERN_INFO "InterruptDemo: Initializing...\n");
	int result;
    dev_t dev = MKDEV(major, 0);
    if(major){
        //Static device ID
        result = register_chrdev_region(dev, 1, DEVICE_NAME);
        printk(KERN_INFO "InterruptDemo: register_chrdev_region().\n");
    }
    else{
        //Allocate device ID
        result = alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME);
        printk(KERN_INFO "InterruptDemo: alloc_chrdev_region().\n");
        major = MAJOR(dev);
    }
    if(result < 0){ //Errors occurred
        printk(KERN_WARNING "InterruptDemo: alloc_chrdev_region() failed.\n");
        return result;
    }
    interrupt_demo_setup_cdev(&interrupt_demo_cdev, 0, &interrupt_demo_driver_file_operations);
    printk(KERN_INFO "InterruptDemo: The major device ID of this device is %d.\n", major);
    //Use request_irq() to register interrupts here
	
    //Create device node
    interrupt_demo_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(interrupt_demo_class)){
        printk(KERN_INFO "InterruptDemo: failed in creating device class.\n");
        return 0;
    }
    device_create(interrupt_demo_class, NULL, dev, NULL, NODE_NAME);
	return 0;
}

static void __exit interrupt_demo_exit(void){
	printk(KERN_INFO "InterruptDemo: Exiting...\n");
	device_destroy(interrupt_demo_class,MKDEV(major, 0));
    class_destroy(interrupt_demo_class);
    cdev_del(&interrupt_demo_cdev);
    unregister_chrdev_region(MKDEV(major, 0), 1)
    //Use free_irq() to unregister interrupts here
	
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
