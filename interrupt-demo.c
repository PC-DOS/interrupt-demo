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
static int iMajorDeviceNumber = 0; //Allocate device number automatically
static struct cdev interrupt_demo_cdev; //cdev structure

int interrupt_demo_open(struct inode * lpNode, struct file * lpFile){
    printk(KERN_INFO "InterruptDemo: Device file opending...\n");
    return 0;
}

static int interrupt_demo_release (struct inode * lpNode, struct file * lpFile){
    printk(KERN_INFO "InterruptDemo: Device file closing...\n");
    return 0;
}

ssize_t interrupt_demo_read(struct file * lpFile, char __user * lpszBuffer, size_t iSize, loff_t * lpOffset){
    printk(KERN_INFO "InterruptDemo: Reading data from device file...\n");
    return 0;
}

ssize_t interrupt_demo_write(struct file * lpFile, const char __user * lpszBuffer, size_t iSize,loff_t * lpOffset){
    printk(KERN_INFO "InterruptDemo: Wrtiting data to device file...\n");
    return 0;
}
 
static long interrupt_demo_unlocked_ioctl(struct file * lpFile, unsigned int iIoControlCommand, unsigned long lpIoControlParameters){  
    printk(KERN_INFO "InterruptDemo: Unlocked IOControl command %u with argument %lu received.\n", iIoControlCommand, lpIoControlParameters);
    return 0;
}

/* For kernels before 2.6.36
static int interrupt_demo_unlocked_ioctl(struct inode * lpNode, struct file *file, unsigned int iIoControlCommand, unsigned long lpIoControlParameters){  
    printk(KERN_INFO "InterruptDemo: IOControl command %u with argument %lu received.\n", iIoControlCommand, lpIoControlParameters);
    return 0;
}
*/
 
static struct file_operations interrupt_demo_driver_file_operations = {
    .owner = THIS_MODULE,
    .open  = interrupt_demo_open,  //Open device, executed when calling open()
    .release = interrupt_demo_release, //Release device, executed when calling close()
    .read  = interrupt_demo_read, //Read operations, executed when calling read()
    .write = interrupt_demo_write, //Write operations, executed when calling write()
    .unlocked_ioctl = interrupt_demo_unlocked_ioctl, //Unlocked IOControl, executed when calling ioctl()
    //.ioctl = interrupt_demo_ioctl, //For kernels before 2.6.36, use .ioctl and comment .unlocked_ioctl
};

/* Platform Device related functions */
static int interrupt_demo_probe(struct platform_device * lpPlatformDevice){
	printk(KERN_INFO "InterruptDemo: Initializing...\n");
	return 0;
}

static int interrupt_demo_remove(struct platform_device * lpPlatformDevice){
	printk(KERN_INFO "InterruptDemo: Removing...\n");
	return 0;
}

void interrupt_demo_shutdown(struct platform_device * lpPlatformDevice){
	printk(KERN_INFO "InterruptDemo: Shutting down...\n");
	return;
}

static int interrupt_demo_suspend(struct platform_device * lpPlatformDevice, pm_message_t iState){
	printk(KERN_INFO "InterruptDemo: Suspending...\n");
	return 0;
}

static int interrupt_demo_resume(struct platform_device * lpPlatformDevice){
	printk(KERN_INFO "InterruptDemo: Resuming...\n");
	return 0;
}

/* Init & Exit functions */
static void interrupt_demo_setup_cdev(struct cdev * lpCharDevice, int iMinorDeviceNumber, struct file_operations * lpFileOperations){ //Device setup functions, called by init()
    int iError, iDeviceDeviceNumber = MKDEV(iMajorDeviceNumber, iMinorDeviceNumber);
    cdev_init(lpCharDevice, lpFileOperations); //Initialize cdev
    lpCharDevice->owner = THIS_MODULE;
    lpCharDevice->ops = lpFileOperations;
    iError = cdev_add(lpCharDevice, iDeviceDeviceNumber, 1);
    if(iError){
        printk(KERN_NOTICE "InterruptDemo: Error %d adding device  %d.\n", iError, iMinorDeviceNumber);
    }
    printk("InterruptDemo: Device setup process finished.\n");
}

static int __init interrupt_demo_init(void){
    printk(KERN_INFO "InterruptDemo: Initializing...\n");
    int iResult;
    dev_t dev = MKDEV(iMajorDeviceNumber, 0);
    if(iMajorDeviceNumber){
        //Static device number
        iResult = register_chrdev_region(dev, 1, DRIVER_NAME);
        printk(KERN_INFO "InterruptDemo: register_chrdev_region().\n");
    }
    else{
        //Allocate device number
        iResult = alloc_chrdev_region(&dev, 0, 1, DRIVER_NAME);
        printk(KERN_INFO "InterruptDemo: alloc_chrdev_region().\n");
        iMajorDeviceNumber = MAJOR(dev);
    }
    if(iResult < 0){ //Errors occurred
        printk(KERN_WARNING "InterruptDemo: alloc_chrdev_region() failed.\n");
        return iResult;
    }
    interrupt_demo_setup_cdev(&interrupt_demo_cdev, 0, &interrupt_demo_driver_file_operations);
    printk(KERN_INFO "InterruptDemo: The major device number of this device is %d.\n", iMajorDeviceNumber);
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
    device_destroy(interrupt_demo_class,MKDEV(iMajorDeviceNumber, 0));
    class_destroy(interrupt_demo_class);
    cdev_del(&interrupt_demo_cdev);
    unregister_chrdev_region(MKDEV(iMajorDeviceNumber, 0), 1);
    //Use free_irq() to unregister interrupts here
	
    return;
}

/* Init function & Exit function */
module_init(interrupt_demo_init);
module_exit(interrupt_demo_exit);

/* Platform Driver related functions */
struct platform_driver interrupt_demo_driver = {
	.probe = interrupt_demo_probe,
	.remove = interrupt_demo_remove,
	.shutdown = interrupt_demo_shutdown,
	.suspend = interrupt_demo_suspend,
	.resume = interrupt_demo_resume,
	.driver = {
		.name = DRIVER_NAME
		//.owner = THIS_MODULE
	}
};

/* Licenses, Authors & Descriptions */
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Picsell-Dois@SEU-BME");
MODULE_DESCRIPTION("Exynos-4412 interrupt demo driver");
