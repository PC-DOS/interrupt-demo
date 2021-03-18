/* Interrupt Demo Driver
 *
 * This is a character driver, which is used to demostrate Exynos-4412's interrupts
 * 
 * We use the following Interrupts:
 * || Private Definition || Pin Definition || INT ID (XEINT) || Label           ||
 * || PW_INT             || GM_INT2        || XEINT25        || EXYNOS4_GPX3(1) ||
 * || DAC_INT            || COMPASS_RDY    || XEINT28        || EXYNOS4_GPX3(4) ||
 * || S_INT              || XEINT1_BAK     || XEINT1         || EXYNOS4_GPX0(1) ||
 * || DP_INT             || XEINT20_BAK    || XEINT20        || EXYNOS4_GPX2(4) ||
 * || KEY_HOME           || UART_RING      || XEINT9         || EXYNOS4_GPX1(1) ||
 * || KEY_BACK           || SIM_DET        || XEINT10        || EXYNOS4_GPX1(2) ||
 * || KEY_SLEEP          || GYRO_INT       || XEINT27        || EXYNOS4_GPX3(3) ||
 * || KEY_VOL+           || KP_ROW1        || XEINT17        || EXYNOS4_GPX2(1) ||
 * || KEY_VOL-           || KP_ROW0        || XEINT16        || EXYNOS4_GPX2(0) ||
 * KEY_**** are only used in INTERRUPT_DEBUG mode
 *
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
#include <linux/uaccess.h>
/* Interrupt-related header files */
#include <linux/interrupt.h>
#include <linux/irq.h>
/* Local header files */
#include "interrupt-demo.h"

struct class *clsDriver; //Device node 
static int iMajorDeviceNumber = 0; //Set to 0 to allocate device number automatically
static struct cdev cdevDriver; //cdev structure

int arrDataBuffer[DATA_BUFFER_SIZE]={0};

/* Character Driver related functions */
int interrupt_demo_open(struct inode * lpNode, struct file * lpFile){
    DBGPRINT("Device file opending...\n");
    return 0;
}

static int interrupt_demo_release (struct inode * lpNode, struct file * lpFile){
    DBGPRINT("Device file closing...\n");
    return 0;
}

ssize_t interrupt_demo_read(struct file * lpFile, char __user * lpszBuffer, size_t iSize, loff_t * lpOffset){
    DBGPRINT("Reading data from device file...\n");
    //Sample data reading code
	ssize_t iResult;
    iResult=copy_to_user(lpszBuffer, arrDataBuffer, sizeof(arrDataBuffer));
	if (iResult){
		WRNPRINT("Failed to copy %ld Bytes of data to user RAM space.\n", iResult);
	}
    return iResult;
}

ssize_t interrupt_demo_write(struct file * lpFile, const char __user * lpszBuffer, size_t iSize,loff_t * lpOffset){
    DBGPRINT("Wrtiting data to device file...\n");
    return 0;
}
 
static long interrupt_demo_unlocked_ioctl(struct file * lpFile, unsigned int iIoControlCommand, unsigned long lpIoControlParameters){  
    DBGPRINT("Unlocked IOControl command %u with argument %lu received.\n", iIoControlCommand, lpIoControlParameters);
    return 0;
}

/* For kernels before 2.6.36
static int interrupt_demo_unlocked_ioctl(struct inode * lpNode, struct file *file, unsigned int iIoControlCommand, unsigned long lpIoControlParameters){  
    DBGPRINT("IOControl command %u with argument %lu received.\n", iIoControlCommand, lpIoControlParameters);
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

/* Interrupt Handlers */
//Interrupt handler of PW_INT/GM_INT2, Interrupt ID XEINT25, Label EXYNOS4_GPX3(1)
static irqreturn_t eint25_interrupt(int iIrq, void * lpDevId){
	DBGPRINT("Interrupt Handler: Interrupt %s, handler %s, at line %d.\n", XEINT25_NAME, __FUNCTION__, __LINE__);
	return IRQ_HANDLED;
}
//Interrupt handler of DAC_INT/COMPASS_RDY, Interrupt ID XEINT28, Label EXYNOS4_GPX3(4)
static irqreturn_t eint28_interrupt(int iIrq, void * lpDevId){
	DBGPRINT("Interrupt Handler: Interrupt %s, handler %s, at line %d.\n", XEINT28_NAME, __FUNCTION__, __LINE__);
	//Sample data generation code
	int i;
    for (i=0; i<DATA_BUFFER_SIZE; ++i){
        arrDataBuffer[i]=245000;
    }
	return IRQ_HANDLED;
}
//Interrupt handler of S_INT/XEINT1_BAK, Interrupt ID XEINT1, Label EXYNOS4_GPX0(1)
static irqreturn_t eint1_interrupt(int iIrq, void * lpDevId){
	DBGPRINT("Interrupt Handler: Interrupt %s, handler %s, at line %d.\n", XEINT1_NAME, __FUNCTION__, __LINE__);
	return IRQ_HANDLED;
}
//Interrupt handler of DP_INT/XEINT20_BAK, Interrupt ID XEINT20, Label EXYNOS4_GPX2(4)
static irqreturn_t eint20_interrupt(int iIrq, void * lpDevId){
	DBGPRINT("Interrupt Handler: Interrupt %s, handler %s, at line %d.\n", XEINT20_NAME, __FUNCTION__, __LINE__);
	return IRQ_HANDLED;
}

#ifdef IS_GPIO_INTERRUPT_DEBUG
//Interrupt handler of KEY_HOME/UART_RING, Interrupt ID XEINT9, Label EXYNOS4_GPX1(1)
static irqreturn_t eint9_interrupt(int iIrq, void * lpDevId){
	DBGPRINT("Interrupt Handler: Interrupt %s, handler %s, at line %d.\n", XEINT9_NAME, __FUNCTION__, __LINE__);
	return IRQ_HANDLED;
}
//Interrupt handler of KEY_BACK/SIM_DET, Interrupt ID XEINT10, Label EXYNOS4_GPX1(2)
static irqreturn_t eint10_interrupt(int iIrq, void * lpDevId){
	DBGPRINT("Interrupt Handler: Interrupt %s, handler %s, at line %d.\n", XEINT10_NAME, __FUNCTION__, __LINE__);
	return IRQ_HANDLED;
}
//Interrupt handler of KEY_SLEEP/GYRO_INT, Interrupt ID XEINT27, Label EXYNOS4_GPX3(3)
static irqreturn_t eint27_interrupt(int iIrq, void * lpDevId){
	DBGPRINT("Interrupt Handler: Interrupt %s, handler %s, at line %d.\n", XEINT27_NAME, __FUNCTION__, __LINE__);
	return IRQ_HANDLED;
}
//Interrupt handler of KEY_VOL+/KP_ROW1, Interrupt ID XEINT17, Label EXYNOS4_GPX2(1)
static irqreturn_t eint17_interrupt(int iIrq, void * lpDevId){
	DBGPRINT("Interrupt Handler: Interrupt %s, handler %s, at line %d.\n", XEINT17_NAME, __FUNCTION__, __LINE__);
	return IRQ_HANDLED;
}
//Interrupt handler of KEY_VOL-/KP_ROW0, Interrupt ID XEINT16, Label EXYNOS4_GPX2(0)
static irqreturn_t eint16_interrupt(int iIrq, void * lpDevId){
	DBGPRINT("Interrupt Handler: Interrupt %s, handler %s, at line %d.\n", XEINT16_NAME, __FUNCTION__, __LINE__);
	return IRQ_HANDLED;
}
#endif

/* Platform Device related functions */
static int interrupt_demo_probe(struct platform_device * lpPlatformDevice){
	DBGPRINT("Initializing...\n");
	return 0;
}

static int interrupt_demo_remove(struct platform_device * lpPlatformDevice){
	DBGPRINT("Removing...\n");
	return 0;
}

void interrupt_demo_shutdown(struct platform_device * lpPlatformDevice){
	DBGPRINT("Shutting down...\n");
	return;
}

static int interrupt_demo_suspend(struct platform_device * lpPlatformDevice, pm_message_t iState){
	DBGPRINT("Suspending...\n");
	return 0;
}

static int interrupt_demo_resume(struct platform_device * lpPlatformDevice){
	DBGPRINT("Resuming...\n");
	return 0;
}

/* Init & Exit functions */
static void interrupt_demo_setup_cdev(struct cdev * lpCharDevice, int iMinorDeviceNumber, struct file_operations * lpFileOperations){ //Device setup function, called by init()
    int iError, iDeviceDeviceNumber = MKDEV(iMajorDeviceNumber, iMinorDeviceNumber);
    cdev_init(lpCharDevice, lpFileOperations); //Initialize cdev
    lpCharDevice->owner = THIS_MODULE;
    lpCharDevice->ops = lpFileOperations;
    iError = cdev_add(lpCharDevice, iDeviceDeviceNumber, 1);
    if (iError){
        WRNPRINT("Error %d adding device  %d.\n", iError, iMinorDeviceNumber);
    }
    NFOPRINT("Device setup process finished.\n");
}

static int __init interrupt_demo_init(void){
    NFOPRINT("Initializing...\n");
    int iResult;
    dev_t dev = MKDEV(iMajorDeviceNumber, 0);
    if (iMajorDeviceNumber){
        //Static device number
        iResult = register_chrdev_region(dev, 1, DRIVER_NAME);
        DBGPRINT("register_chrdev_region().\n");
    }
    else{
        //Allocate device number
        iResult = alloc_chrdev_region(&dev, 0, 1, DRIVER_NAME);
        DBGPRINT("alloc_chrdev_region().\n");
        iMajorDeviceNumber = MAJOR(dev);
    }
    if (iResult < 0){ //Errors occurred
        WRNPRINT("alloc_chrdev_region() failed.\n");
        return iResult;
    }
    interrupt_demo_setup_cdev(&cdevDriver, 0, &interrupt_demo_driver_file_operations);
    DBGPRINT("The major device number of this device is %d.\n", iMajorDeviceNumber);
    //Use request_irq() to register interrupts here
	int iIrqResult;
	//Request interrupt PW_INT/GM_INT2, Interrupt ID XEINT25, Label EXYNOS4_GPX3(1)
	iIrqResult=gpio_request(EXYNOS4_GPX3(1), XEINT25_NAME);
	if (0==iIrqResult){
		s3c_gpio_cfgpin(EXYNOS4_GPX3(1), S3C_GPIO_SFN(0xF));
		s3c_gpio_setpull(EXYNOS4_GPX3(1), S3C_GPIO_PULL_UP);
		gpio_free(EXYNOS4_GPX3(1));
		
		iIrqResult=request_irq(IRQ_EINT(25), eint25_interrupt, IRQ_TYPE_EDGE_FALLING, XEINT25_NAME, NULL);
		if (iIrqResult<0) {
			WRNPRINT("Request IRQ %d failed with return code %d.\n", IRQ_EINT(25), iIrqResult);
		}
	}
	else{
		WRNPRINT("Request GPIO %d failed with return code %d.\n", EXYNOS4_GPX3(1), iIrqResult);
	}
	//Request interrupt DAC_INT/COMPASS_RDY, Interrupt ID XEINT28, Label EXYNOS4_GPX3(4)
	iIrqResult=gpio_request(EXYNOS4_GPX3(4), XEINT28_NAME);
	if (0==iIrqResult){
		s3c_gpio_cfgpin(EXYNOS4_GPX3(4), S3C_GPIO_SFN(0xF));
		s3c_gpio_setpull(EXYNOS4_GPX3(4), S3C_GPIO_PULL_UP);
		gpio_free(EXYNOS4_GPX3(4));
		
		iIrqResult=request_irq(IRQ_EINT(28), eint28_interrupt, IRQ_TYPE_EDGE_FALLING, XEINT28_NAME, NULL);
		if (iIrqResult<0) {
			WRNPRINT("Request IRQ %d failed with return code %d.\n", IRQ_EINT(28), iIrqResult);
		}
	}
	else{
		WRNPRINT("Request GPIO %d failed with return code %d.\n", EXYNOS4_GPX3(4), iIrqResult);
	}
	//Request interrupt S_INT/XEINT1_BAK, Interrupt ID XEINT1, Label EXYNOS4_GPX0(1)
	iIrqResult=gpio_request(EXYNOS4_GPX0(1), XEINT1_NAME);
	if (0==iIrqResult){
		s3c_gpio_cfgpin(EXYNOS4_GPX0(1), S3C_GPIO_SFN(0xF));
		s3c_gpio_setpull(EXYNOS4_GPX0(1), S3C_GPIO_PULL_UP);
		gpio_free(EXYNOS4_GPX0(1));
		
		iIrqResult=request_irq(IRQ_EINT(1), eint1_interrupt, IRQ_TYPE_EDGE_FALLING, XEINT1_NAME, NULL);
		if (iIrqResult<0) {
			WRNPRINT("Request IRQ %d failed with return code %d.\n", IRQ_EINT(1), iIrqResult);
		}
	}
	else{
		WRNPRINT("Request GPIO %d failed with return code %d.\n", EXYNOS4_GPX0(1), iIrqResult);
	}
	//Request interrupt DP_INT/XEINT20_BAK, Interrupt ID XEINT20, Label EXYNOS4_GPX2(4)
	iIrqResult=gpio_request(EXYNOS4_GPX2(4), XEINT20_NAME);
	if (0==iIrqResult){
		s3c_gpio_cfgpin(EXYNOS4_GPX2(4), S3C_GPIO_SFN(0xF));
		s3c_gpio_setpull(EXYNOS4_GPX2(4), S3C_GPIO_PULL_UP);
		gpio_free(EXYNOS4_GPX2(4));
		
		iIrqResult=request_irq(IRQ_EINT(20), eint20_interrupt, IRQ_TYPE_EDGE_FALLING, XEINT20_NAME, NULL);
		if (iIrqResult<0) {
			WRNPRINT("Request IRQ %d failed with return code %d.\n", IRQ_EINT(20), iIrqResult);
		}
	}
	else{
		WRNPRINT("Request GPIO %d failed with return code %d.\n", EXYNOS4_GPX2(4), iIrqResult);
	}
	#ifdef IS_GPIO_INTERRUPT_DEBUG
	WRNPRINT("You have enabled on-board GPIO keys\' interrupts. These interrupts need disabling \'GPIO Buttons\' driver in Kernel-Config\'s \'Device Drivers -> Input device support -> Keyboards\' menu to work. If you did so, GPIO keypads may not be available.\n");
	//Request interrupt KEY_HOME/UART_RING, Interrupt ID XEINT9, Label EXYNOS4_GPX1(1)
	iIrqResult=gpio_request(EXYNOS4_GPX1(1), XEINT9_NAME);
	if (0==iIrqResult){
		s3c_gpio_cfgpin(EXYNOS4_GPX1(1), S3C_GPIO_SFN(0xF));
		s3c_gpio_setpull(EXYNOS4_GPX1(1), S3C_GPIO_PULL_UP);
		gpio_free(EXYNOS4_GPX1(1));
		
		iIrqResult=request_irq(IRQ_EINT(9), eint9_interrupt, IRQ_TYPE_EDGE_FALLING, XEINT9_NAME, NULL);
		if (iIrqResult<0) {
			WRNPRINT("Request IRQ %d failed with return code %d.\n", IRQ_EINT(9), iIrqResult);
		}
	}
	else{
		WRNPRINT("Request GPIO %d failed with return code %d.\n", EXYNOS4_GPX1(1), iIrqResult);
	}
	//Request interrupt KEY_BACK/SIM_DET, Interrupt ID XEINT10, Label EXYNOS4_GPX1(2)
	iIrqResult=gpio_request(EXYNOS4_GPX1(2), XEINT10_NAME);
	if (0==iIrqResult){
		s3c_gpio_cfgpin(EXYNOS4_GPX1(2), S3C_GPIO_SFN(0xF));
		s3c_gpio_setpull(EXYNOS4_GPX1(2), S3C_GPIO_PULL_UP);
		gpio_free(EXYNOS4_GPX1(2));
		
		iIrqResult=request_irq(IRQ_EINT(10), eint10_interrupt, IRQ_TYPE_EDGE_FALLING, XEINT10_NAME, NULL);
		if (iIrqResult<0) {
			WRNPRINT("Request IRQ %d failed with return code %d.\n", IRQ_EINT(10), iIrqResult);
		}
	}
	else{
		WRNPRINT("Request GPIO %d failed with return code %d.\n", EXYNOS4_GPX1(2), iIrqResult);
	}
	//Request interrupt KEY_SLEEP/GYRO_INT, Interrupt ID XEINT27, Label EXYNOS4_GPX3(3)
	iIrqResult=gpio_request(EXYNOS4_GPX3(3), XEINT27_NAME);
	if (0==iIrqResult){
		s3c_gpio_cfgpin(EXYNOS4_GPX3(3), S3C_GPIO_SFN(0xF));
		s3c_gpio_setpull(EXYNOS4_GPX3(3), S3C_GPIO_PULL_UP);
		gpio_free(EXYNOS4_GPX3(3));
		
		iIrqResult=request_irq(IRQ_EINT(27), eint27_interrupt, IRQ_TYPE_EDGE_FALLING, XEINT27_NAME, NULL);
		if (iIrqResult<0) {
			WRNPRINT("Request IRQ %d failed with return code %d.\n", IRQ_EINT(27), iIrqResult);
		}
	}
	else{
		WRNPRINT("Request GPIO %d failed with return code %d.\n", EXYNOS4_GPX3(3), iIrqResult);
	}
	//Request interrupt KEY_VOL+/KP_ROW1, Interrupt ID XEINT17, Label EXYNOS4_GPX2(1)
	iIrqResult=gpio_request(EXYNOS4_GPX2(1), XEINT17_NAME);
	if (0==iIrqResult){
		s3c_gpio_cfgpin(EXYNOS4_GPX2(1), S3C_GPIO_SFN(0xF));
		s3c_gpio_setpull(EXYNOS4_GPX2(1), S3C_GPIO_PULL_UP);
		gpio_free(EXYNOS4_GPX2(1));
		
		iIrqResult=request_irq(IRQ_EINT(17), eint17_interrupt, IRQ_TYPE_EDGE_FALLING, XEINT17_NAME, NULL);
		if (iIrqResult<0) {
			WRNPRINT("Request IRQ %d failed with return code %d.\n", IRQ_EINT(17), iIrqResult);
		}
	}
	else{
		WRNPRINT("Request GPIO %d failed with return code %d.\n", EXYNOS4_GPX2(1), iIrqResult);
	}
	//Request interrupt KEY_VOL-/KP_ROW0, Interrupt ID XEINT16, Label EXYNOS4_GPX2(0)
	iIrqResult=gpio_request(EXYNOS4_GPX2(0), XEINT16_NAME);
	if (0==iIrqResult){
		s3c_gpio_cfgpin(EXYNOS4_GPX2(0), S3C_GPIO_SFN(0xF));
		s3c_gpio_setpull(EXYNOS4_GPX2(0), S3C_GPIO_PULL_UP);
		gpio_free(EXYNOS4_GPX2(0));
		
		iIrqResult=request_irq(IRQ_EINT(16), eint16_interrupt, IRQ_TYPE_EDGE_FALLING, XEINT16_NAME, NULL);
		if (iIrqResult<0) {
			WRNPRINT("Request IRQ %d failed with return code %d.\n", IRQ_EINT(16), iIrqResult);
		}
	}
	else{
		WRNPRINT("Request GPIO %d failed with return code %d.\n", EXYNOS4_GPX2(0), iIrqResult);
	}
	#endif
    //Create device node
    clsDriver = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(clsDriver)){
        WRNPRINT("failed in creating device class.\n");
        return 0;
    }
    device_create(clsDriver, NULL, dev, NULL, NODE_NAME);
	return 0;
}

static void __exit interrupt_demo_exit(void){
    DBGPRINT("Exiting...\n");
    device_destroy(clsDriver,MKDEV(iMajorDeviceNumber, 0));
    class_destroy(clsDriver);
    cdev_del(&cdevDriver);
    unregister_chrdev_region(MKDEV(iMajorDeviceNumber, 0), 1);
    //Use free_irq() to unregister interrupts here
	free_irq(IRQ_EINT(25), NULL);
	free_irq(IRQ_EINT(28), NULL);
	free_irq(IRQ_EINT(1), NULL);
	free_irq(IRQ_EINT(20), NULL);
	#ifdef IS_GPIO_INTERRUPT_DEBUG
	free_irq(IRQ_EINT(9), NULL);
	free_irq(IRQ_EINT(10), NULL);
	free_irq(IRQ_EINT(27), NULL);
	free_irq(IRQ_EINT(7), NULL);
	free_irq(IRQ_EINT(6), NULL);
	#endif
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
