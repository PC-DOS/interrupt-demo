/* Interrupt Demo Driver
 *
 * This is a character device, which is used to demostrate Exynos-4412's interrupts
 * 
 * We use the following Interrupts:
 * || Private Definition || Pin Definition || INT ID (XEINT) || Label           || Meaning                                                                            ||
 * || S_INT              || XEINT1_BAK     || XEINT1         || EXYNOS4_GPX0(1) || A sampling sequence has finished, we should read data from device. Original EINT4. ||
 * || DP_INT             || XEINT20_BAK    || XEINT20        || EXYNOS4_GPX2(4) || Triggers 50 times per second (Hz), wakes up the UserApp. Original EINT1.           ||
 * || PW_INT             || GM_INT2        || XEINT25        || EXYNOS4_GPX3(1) || Power-Key interrupt. Original EINT16.                                              ||
 * || DAC_INT            || COMPASS_RDY    || XEINT28        || EXYNOS4_GPX3(4) || Digital-Analog Converter interrupt. Original EINT6.                                ||
 * || KEY_HOME           || UART_RING      || XEINT9         || EXYNOS4_GPX1(1) || iTop-4412 on-board Home Key.                                                       ||
 * || KEY_BACK           || SIM_DET        || XEINT10        || EXYNOS4_GPX1(2) || iTop-4412 on-board Back Key.                                                       ||
 * || KEY_SLEEP          || GYRO_INT       || XEINT27        || EXYNOS4_GPX3(3) || iTop-4412 on-board Sleep Key.                                                      ||
 * || KEY_VOL+           || KP_ROW1        || XEINT17        || EXYNOS4_GPX2(1) || iTop-4412 on-board Vol+ Key.                                                       ||
 * || KEY_VOL-           || KP_ROW0        || XEINT16        || EXYNOS4_GPX2(0) || iTop-4412 on-board Vol- Key.                                                       ||
 * * KEY_**** are only used in INTERRUPT_DEBUG mode, comment #define IS_GPIO_INTERRUPT_DEBUG in header file to disable this mode.
 * The disabling and enabling of IRQs are nested, the OS uses a variable to store the depth of disabling. Thus, you don't need other flags to mark IRQs' status.
 *
 */

/* Main header files */
#include <linux/init.h>
#include <linux/module.h>
/* Useful header files */
#include <asm/io.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <linux/uaccess.h>
#include <mach/gpio.h>
#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>
/* Interrupt-related header files */
#include <linux/interrupt.h>
#include <linux/irq.h>
/* Spin-Lock, for we need it in Interrupt Handlers, we can't use Mutex */
#include <linux/spinlock.h>
/* Library to generate random numbers */
#include <linux/random.h>
/* Local header files */
#include "MathFunctions.h"
#include "interrupt-demo.h"

//Device Data
static struct class * clsDevice; //Device node
static int iMajorDeviceNumber = 0; //Set to 0 to allocate device number automatically
static struct cdev cdevDevice; //cdev structure

//Spin-Locks
#define IS_DATA_BUFFER_SPINLOCK_REQUESTED //Switch of arrDataBuffer Spin-Lock
#ifdef IS_DATA_BUFFER_SPINLOCK_REQUESTED
static rwlock_t rwlkDataBufferLock; //Spin-Lock to protect arrDataBuffer, use Read-Write-Lock to improve concurrency performance
#endif
#define IS_IOCTL_OPERATION_SPINLOCK_REQUESTED //Switch of IoCtl operations Spin-Lock
#ifdef IS_IOCTL_OPERATION_SPINLOCK_REQUESTED
static spinlock_t spnlkIoCtlLock; //Spin-Lock to protect IoCtl operations
#endif

//Data Buffers
unsigned int arrDataBuffer[DATA_BUFFER_SIZE] = {0};
unsigned char arrCommandBuffer[CONTROL_COMMAND_BUFFER_SIZE] = {0};

/* Character Device Related Functions */
int interrupt_demo_open(struct inode * lpNode, struct file * lpFile) {
    //DBGPRINT("Device file opending...\n");
    return 0;
}

static int interrupt_demo_release(struct inode * lpNode, struct file * lpFile) {
    //DBGPRINT("Device file closing...\n");
    return 0;
}

/* 
 * interrupt_demo_read() Function
 *
 * This function copies arrDataBuffer to user RAM space.
 * The user space data buffer is an array, whose data type is char (Byte).
 * Thus, the size of user space data buffer must be 4 times of the size of arrDataBuffer (for unsigned int type data).
 * It's suggested that the size of user space data buffer is larger than 4 times of the size of arrDataBuffer (for unsigned int type data) in order to avoid Segmentation Fault.
 * To reconstruct data (pesudo C++ code):
 * 
 * [[code type="Cpp"]]
 * unsigned int arrData[DATA_SIZE];
 * char * chrData = new char[DATA_SIZE * 4];
 * OpenFile(...);
 * read(chrData, DATA_SIZE * 4);
 * for (int i=0; i<DATA_SIZE; ++i){
 *      arrData[i]=(int(chrData[i*4])) + (int(chrData[i*4+1])<<8) + (int(chrData[i*4+2])<<16) + (int(chrData[i*4+3])<<24);
 * }
 * [[/code]]
 * 
 */
ssize_t interrupt_demo_read(struct file * lpFile, char __user * lpszBuffer, size_t iSize, loff_t * lpOffset) {
    //DBGPRINT("Reading data from device file...\n");
    //Sample data reading code
    disable_irq(S_INT); //Disable S_INT to avoid unwanted DataBuffer refresh
#ifdef IS_DATA_BUFFER_SPINLOCK_REQUESTED
    read_lock(&rwlkDataBufferLock); //Begin reading, locks arrDataBuffer
#endif
    ssize_t iResult;
    iResult = copy_to_user(lpszBuffer, arrDataBuffer, GetMin(sizeof(arrDataBuffer), iSize));
    if (iResult) {
        WRNPRINT("Failed to copy %ld Bytes of data to user RAM space.\n", iResult);
    }
#ifdef IS_DATA_BUFFER_SPINLOCK_REQUESTED
    read_unlock(&rwlkDataBufferLock); //Don't forget to unlock me!
#endif
    enable_irq(S_INT); //Enable S_INT
    return iResult;
}

/* 
 * interrupt_demo_write() Function
 *
 * This function copies IO control commands from user RAM space to kernel RAM space (arrCommandBuffer).
 * Array arrCommandBuffer has 2 unsigned char (Byte) spaces:
 * The first one (arrCommandBuffer[0]) contains commands (iIoControlCommand);
 * The second one (arrCommandBuffer[1]) contains arguments (lpIoControlParameters);
 * 
 */
ssize_t interrupt_demo_write(struct file * lpFile, const char __user * lpszBuffer, size_t iSize, loff_t * lpOffset) {
    DBGPRINT("Wrtiting data to device file...\n");
    ssize_t iResult;
    iResult = copy_from_user(arrCommandBuffer, lpszBuffer, GetMin(CONTROL_COMMAND_BUFFER_SIZE, iSize));
    if (iResult) {
        WRNPRINT("Failed to copy %ld Bytes of data to kernel RAM space.\n", iResult);
        return iResult;
    }
#ifdef IS_IOCTL_OPERATION_SPINLOCK_REQUESTED
    spin_lock(&spnlkIoCtlLock); //Locks IoCtl operations
#endif
    unsigned int iIoControlCommand = arrCommandBuffer[0];
    unsigned long lpIoControlParameters = arrCommandBuffer[1];
    DBGPRINT("IOControl command %u with argument %lu received.\n", iIoControlCommand, lpIoControlParameters);
    ProcessIoControlCommand(iIoControlCommand, lpIoControlParameters);
#ifdef IS_IOCTL_OPERATION_SPINLOCK_REQUESTED
    spin_unlock(&spnlkIoCtlLock); //Don't forget to unlock me!
#endif
    return iResult;
}

/* 
 * interrupt_demo_unlocked_ioctl() Function
 * 
 * This function processes IO control commands and parameters.
 * 
 */
static long interrupt_demo_unlocked_ioctl(struct file * lpFile, unsigned int iIoControlCommand, unsigned long lpIoControlParameters) {
    DBGPRINT("Unlocked IOControl command %u with argument %lu received.\n", iIoControlCommand, lpIoControlParameters);
#ifdef IS_IOCTL_OPERATION_SPINLOCK_REQUESTED
    spin_lock(&spnlkIoCtlLock); //Locks IoCtl operations
#endif
    ProcessIoControlCommand(iIoControlCommand, lpIoControlParameters);
#ifdef IS_IOCTL_OPERATION_SPINLOCK_REQUESTED
    spin_unlock(&spnlkIoCtlLock); //Don't forget to unlock me!
#endif
    return 0;
}

/*
 * interrupt_demo_compact_ioctl() Function
 * 
 * This function processes IO control commands and parameters.
 * compact_ioctl is designed for 64-bit drivers to process 32-bit user application's ioctl() calls. This driver is currently designed for ARM32 (AArch32) platform.
 * 
static long interrupt_demo_compact_ioctl(struct file * lpFile, unsigned int iIoControlCommand, unsigned long lpIoControlParameters){  
    DBGPRINT("Unlocked IOControl command %u with argument %lu received.\n", iIoControlCommand, lpIoControlParameters);
#ifdef IS_IOCTL_OPERATION_SPINLOCK_REQUESTED
    spin_lock(&spnlkIoCtlLock); //Locks IoCtl operations
#endif
    ProcessIoControlCommand(iIoControlCommand, lpIoControlParameters);
#ifdef IS_IOCTL_OPERATION_SPINLOCK_REQUESTED
    spin_unlock(&spnlkIoCtlLock); //Don't forget to unlock me!
#endif
    return 0;
}
*/

/* For kernels before 2.6.36
 * 
 * In newer kernels, use unlocked_ioctl() instead.
 * Otherwise, an error will occur when compiling.
 * 
static int interrupt_demo_ioctl(struct inode * lpNode, struct file *file, unsigned int iIoControlCommand, unsigned long lpIoControlParameters){  
    DBGPRINT("IOControl command %u with argument %lu received.\n", iIoControlCommand, lpIoControlParameters);
#ifdef IS_IOCTL_OPERATION_SPINLOCK_REQUESTED
    spin_lock(&spnlkIoCtlLock); //Locks IoCtl operations
#endif
    ProcessIoControlCommand(iIoControlCommand, lpIoControlParameters);
#ifdef IS_IOCTL_OPERATION_SPINLOCK_REQUESTED
    spin_unlock(&spnlkIoCtlLock); //Don't forget to unlock me!
#endif
    return 0;
}
*/

/* Pointers to Character Device Related Functions */
static struct file_operations interrupt_demo_device_file_operations = {
    .owner = THIS_MODULE,
    .open = interrupt_demo_open, //Open device, executed when calling open()
    .release = interrupt_demo_release, //Release device, executed when calling close()
    .read = interrupt_demo_read, //Read operations, executed when calling read()
    .write = interrupt_demo_write, //Write operations, executed when calling write()
    .unlocked_ioctl = interrupt_demo_unlocked_ioctl, //Unlocked IOControl, executed when calling ioctl()
    //.compact_ioctl = interrupt_demo_compact_ioctl, //Compact IOControl, executed when calling ioctl() from 32-bit user application on 64-bit platform
    //.ioctl = interrupt_demo_ioctl, //For kernels before 2.6.36, use .ioctl and comment .unlocked_ioctl
};

/* Interrupt Handlers */
//Interrupt handler of S_INT
static irqreturn_t s_int_interrupt(int iIrq, void * lpDevId) {
    //DBGPRINT("Interrupt Handler: Interrupt %s, handler %s, at line %d.\n", S_INT_NAME, __FUNCTION__, __LINE__);
    disable_irq_nosync(S_INT); //Use disable_irq_nosync() in Interrupt Handlers. Use disable_irq() in normal functions
    //Sample data generation code
#ifdef IS_DATA_BUFFER_SPINLOCK_REQUESTED
    write_lock(&rwlkDataBufferLock); //Begin writing, locks arrDataBuffer
#endif
    int i;
    for (i = 0; i < DATA_BUFFER_SIZE; ++i) {
        arrDataBuffer[i] = arrDataDef[i] + random32() % DATA_MAX_VALUE;
    }
#ifdef IS_DATA_BUFFER_SPINLOCK_REQUESTED
    write_unlock(&rwlkDataBufferLock); //Don't forget to unlock me!
#endif
    enable_irq(S_INT); //enable_irq() before returning
    return IRQ_HANDLED;
}
//Interrupt handler of DP_INT
static irqreturn_t dp_int_interrupt(int iIrq, void * lpDevId) {
    //DBGPRINT("Interrupt Handler: Interrupt %s, handler %s, at line %d.\n", XEINT20_NAME, __FUNCTION__, __LINE__);
    return IRQ_HANDLED;
}
//Interrupt handler of PW_INT
static irqreturn_t pw_int_interrupt(int iIrq, void * lpDevId) {
    //DBGPRINT("Interrupt Handler: Interrupt %s, handler %s, at line %d.\n", PW_INT_NAME, __FUNCTION__, __LINE__);
    return IRQ_HANDLED;
}
//Interrupt handler of DAC_INT
static irqreturn_t dac_int_interrupt(int iIrq, void * lpDevId) {
    //DBGPRINT("Interrupt Handler: Interrupt %s, handler %s, at line %d.\n", DAC_INT_NAME, __FUNCTION__, __LINE__);
    return IRQ_HANDLED;
}

#ifdef IS_GPIO_INTERRUPT_DEBUG
//Interrupt handler of KEY_HOME
static irqreturn_t key_home_interrupt(int iIrq, void * lpDevId) {
    DBGPRINT("Interrupt Handler: Interrupt %s, handler %s, at line %d.\n", KEY_HOME_NAME, __FUNCTION__, __LINE__);
    return IRQ_HANDLED;
}
//Interrupt handler of KEY_BACK
static irqreturn_t key_back_interrupt(int iIrq, void * lpDevId) {
    DBGPRINT("Interrupt Handler: Interrupt %s, handler %s, at line %d.\n", KEY_BACK_NAME, __FUNCTION__, __LINE__);
    return IRQ_HANDLED;
}
//Interrupt handler of KEY_SLEEP/GYRO_INT, Interrupt ID XEINT27, Label EXYNOS4_GPX3(3)
static irqreturn_t key_sleep_interrupt(int iIrq, void * lpDevId) {
    DBGPRINT("Interrupt Handler: Interrupt %s, handler %s, at line %d.\n", KEY_SLEEP_NAME, __FUNCTION__, __LINE__);
    return IRQ_HANDLED;
}
//Interrupt handler of KEY_VOL+/KP_ROW1, Interrupt ID XEINT17, Label EXYNOS4_GPX2(1)
static irqreturn_t key_volup_interrupt(int iIrq, void * lpDevId) {
    DBGPRINT("Interrupt Handler: Interrupt %s, handler %s, at line %d.\n", KEY_VOLUP_NAME, __FUNCTION__, __LINE__);
    return IRQ_HANDLED;
}
//Interrupt handler of KEY_VOL-
static irqreturn_t key_voldown_interrupt(int iIrq, void * lpDevId) {
    DBGPRINT("Interrupt Handler: Interrupt %s, handler %s, at line %d.\n", KEY_VOLDOWN_NAME, __FUNCTION__, __LINE__);
    return IRQ_HANDLED;
}
#endif

/* Platform Device Related Functions */
static int interrupt_demo_probe(struct platform_device * lpPlatformDevice) {
    DBGPRINT("Initializing...\n");
    return 0;
}

static int interrupt_demo_remove(struct platform_device * lpPlatformDevice) {
    DBGPRINT("Removing...\n");
    return 0;
}

void interrupt_demo_shutdown(struct platform_device * lpPlatformDevice) {
    DBGPRINT("Shutting down...\n");
    return;
}

static int interrupt_demo_suspend(struct platform_device * lpPlatformDevice, pm_message_t iState) {
    DBGPRINT("Suspending...\n");
    return 0;
}

static int interrupt_demo_resume(struct platform_device * lpPlatformDevice) {
    DBGPRINT("Resuming...\n");
    return 0;
}

/* IOControl Handlers */
void ProcessIoControlCommand(unsigned int iIoControlCommand, unsigned long lpIoControlParameters) {
    //disable_irq(S_INT); //Disable S_INT to avoid unwanted DataBuffer refresh
    switch (iIoControlCommand) {
    case CTL_CMD_DISABLE_IRQ:
        switch (lpIoControlParameters) {
        case CTL_ARG_IRQ_NAME_NULL:
            break;
        case CTL_ARG_IRQ_NAME_S_INT:
            DBGPRINT("Disabling IRQ: S_INT.\n");
            disable_irq(S_INT);
            break;
        case CTL_ARG_IRQ_NAME_DP_INT:
            DBGPRINT("Disabling IRQ: DP_INT.\n");
            disable_irq(DP_INT);
            break;
        case CTL_ARG_IRQ_NAME_PW_INT:
            DBGPRINT("Disabling IRQ: PW_INT.\n");
            disable_irq(PW_INT);
            break;
        case CTL_ARG_IRQ_NAME_DAC_INT:
            DBGPRINT("Disabling IRQ: DAC_INT.\n");
            disable_irq(DAC_INT);
            break;
#ifdef IS_GPIO_INTERRUPT_DEBUG
        case CTL_ARG_IRQ_NAME_KEY_HOME:
            DBGPRINT("Disabling IRQ: KEY_HOME.\n");
            disable_irq(KEY_HOME);
            break;
        case CTL_ARG_IRQ_NAME_KEY_BACK:
            DBGPRINT("Disabling IRQ: KEY_BACK.\n");
            disable_irq(KEY_BACK);
            break;
        case CTL_ARG_IRQ_NAME_KEY_SLEEP:
            DBGPRINT("Disabling IRQ: KEY_SLEEP.\n");
            disable_irq(KEY_SLEEP);
            break;
        case CTL_ARG_IRQ_NAME_KEY_VOLUP:
            DBGPRINT("Disabling IRQ: KEY_VOLUP.\n");
            disable_irq(KEY_VOLUP);
            break;
        case CTL_ARG_IRQ_NAME_KEY_VOLDOWN:
            DBGPRINT("Disabling IRQ: KEY_VOLDOWN.\n");
            disable_irq(KEY_VOLDOWN);
            break;
#endif
        default:
            //Disables S_INT by default
            DBGPRINT("Disabling IRQ: S_INT.\n");
            disable_irq(S_INT);
            break;
        }
        break;
    case CTL_CMD_ENABLE_IRQ:
        switch (lpIoControlParameters) {
        case CTL_ARG_IRQ_NAME_NULL:
            break;
        case CTL_ARG_IRQ_NAME_S_INT:
            DBGPRINT("Enabling IRQ: S_INT.\n");
            enable_irq(S_INT);
            break;
        case CTL_ARG_IRQ_NAME_DP_INT:
            DBGPRINT("Enabling IRQ: DP_INT.\n");
            enable_irq(DP_INT);
            break;
        case CTL_ARG_IRQ_NAME_PW_INT:
            DBGPRINT("Enabling IRQ: PW_INT.\n");
            enable_irq(PW_INT);
            break;
        case CTL_ARG_IRQ_NAME_DAC_INT:
            DBGPRINT("Enabling IRQ: DAC_INT.\n");
            enable_irq(DAC_INT);
            break;
#ifdef IS_GPIO_INTERRUPT_DEBUG
        case CTL_ARG_IRQ_NAME_KEY_HOME:
            DBGPRINT("Enabling IRQ: KEY_HOME.\n");
            enable_irq(KEY_HOME);
            break;
        case CTL_ARG_IRQ_NAME_KEY_BACK:
            DBGPRINT("Enabling IRQ: KEY_BACK.\n");
            enable_irq(KEY_BACK);
            break;
        case CTL_ARG_IRQ_NAME_KEY_SLEEP:
            DBGPRINT("Enabling IRQ: KEY_SLEEP.\n");
            enable_irq(KEY_SLEEP);
            break;
        case CTL_ARG_IRQ_NAME_KEY_VOLUP:
            DBGPRINT("Enabling IRQ: KEY_VOLUP.\n");
            enable_irq(KEY_VOLUP);
            break;
        case CTL_ARG_IRQ_NAME_KEY_VOLDOWN:
            DBGPRINT("Enabling IRQ: KEY_VOLDOWN.\n");
            enable_irq(KEY_VOLDOWN);
            break;
#endif
        default:
            //Enables S_INT by default
            DBGPRINT("Enabling IRQ: S_INT.\n");
            enable_irq(S_INT);
            break;
        }
        break;
    case CTL_CMD_SET_USER_APP_PID:

        break;
    case CTL_CMD_SET_DELAY_HIGH_BYTE:

        break;
    case CTL_CMD_SET_DELAY_LOW_BYTE:

        break;
    case CTL_CMD_SET_RATE:

        break;
    case CTL_CMD_SET_COMPRESS_COUNT_HIGH_BYTE:

        break;
    case CTL_CMD_SET_COMPRESS_COUNT_LOW_BYTE:

        break;
    case CTL_CMD_SET_COMPRESS_STEP_INT_PART:

        break;
    case CTL_CMD_SET_COMPRESS_STEP_FLOAT_PART:

        break;
    case CTL_CMD_SET_GAIN:

        break;
    case CTL_CMD_SET_CHANNEL:

        break;
    default:

        break;
    }
    //enable_irq(S_INT); //Enable S_INT
    return;
}

/* Init & Exit Functions */
static void interrupt_demo_setup_cdev(struct cdev * lpCharDevice, int iMinorDeviceNumber, struct file_operations * lpFileOperations) { //Device setup function, called by init()
    int iError, iDeviceDeviceNumber = MKDEV(iMajorDeviceNumber, iMinorDeviceNumber);
    cdev_init(lpCharDevice, lpFileOperations); //Initialize cdev
    lpCharDevice->owner = THIS_MODULE;
    lpCharDevice->ops = lpFileOperations;
    iError = cdev_add(lpCharDevice, iDeviceDeviceNumber, 1);
    if (iError) {
        WRNPRINT("Error %d adding device  %d.\n", iError, iMinorDeviceNumber);
    }
    NFOPRINT("Device setup process finished.\n");
}

static int __init interrupt_demo_init(void) {
    NFOPRINT("Initializing...\n");
    int iResult;
    dev_t devDeviceNumber = MKDEV(iMajorDeviceNumber, 0);
    if (iMajorDeviceNumber) {
        //Static device number
        iResult = register_chrdev_region(devDeviceNumber, 1, DEVICE_NAME);
        DBGPRINT("register_chrdev_region().\n");
    }
    else {
        //Allocate device number
        iResult = alloc_chrdev_region(&devDeviceNumber, 0, 1, DEVICE_NAME);
        DBGPRINT("alloc_chrdev_region().\n");
        iMajorDeviceNumber = MAJOR(devDeviceNumber);
    }
    if (iResult < 0) { //Errors occurred
        WRNPRINT("alloc_chrdev_region() failed.\n");
        return iResult;
    }
    interrupt_demo_setup_cdev(&cdevDevice, 0, &interrupt_demo_device_file_operations);
    DBGPRINT("The major device number of this device is %d.\n", iMajorDeviceNumber);
#ifdef IS_DATA_BUFFER_SPINLOCK_REQUESTED
    //Initialize Read-Write-Lock for arrDataBuffer
    rwlock_init(&rwlkDataBufferLock);
#endif
#ifdef IS_IOCTL_OPERATION_SPINLOCK_REQUESTED
    //Initialize Spin-Lock for IO Control
    spin_lock_init(&spnlkIoCtlLock);
#endif
    //Use request_irq() to register interrupts here
    int iIrqResult;
    //Request interrupt S_INT
    iIrqResult = gpio_request(S_INT_LABEL, S_INT_NAME);
    if (0 == iIrqResult) {
        s3c_gpio_cfgpin(S_INT_LABEL, S3C_GPIO_SFN(0xF));
        s3c_gpio_setpull(S_INT_LABEL, S3C_GPIO_PULL_UP);
        gpio_free(S_INT_LABEL);

        iIrqResult = request_irq(S_INT, s_int_interrupt, IRQ_TYPE_EDGE_FALLING, S_INT_NAME, NULL);
        if (iIrqResult < 0) {
            WRNPRINT("Request IRQ %d failed with return code %d.\n", S_INT, iIrqResult);
        }
    }
    else {
        WRNPRINT("Request GPIO %d failed with return code %d.\n", S_INT_LABEL, iIrqResult);
    }
    //Request interrupt DP_INT
    iIrqResult = gpio_request(DP_INT_LABEL, XEINT20_NAME);
    if (0 == iIrqResult) {
        s3c_gpio_cfgpin(DP_INT_LABEL, S3C_GPIO_SFN(0xF));
        s3c_gpio_setpull(DP_INT_LABEL, S3C_GPIO_PULL_UP);
        gpio_free(DP_INT_LABEL);

        iIrqResult = request_irq(DP_INT, dp_int_interrupt, IRQ_TYPE_EDGE_FALLING, XEINT20_NAME, NULL);
        if (iIrqResult < 0) {
            WRNPRINT("Request IRQ %d failed with return code %d.\n", DP_INT, iIrqResult);
        }
    }
    else {
        WRNPRINT("Request GPIO %d failed with return code %d.\n", DP_INT_LABEL, iIrqResult);
    }
    //Request interrupt PW_INT
    iIrqResult = gpio_request(PW_INT_LABEL, PW_INT_NAME);
    if (0 == iIrqResult) {
        s3c_gpio_cfgpin(PW_INT_LABEL, S3C_GPIO_SFN(0xF));
        s3c_gpio_setpull(PW_INT_LABEL, S3C_GPIO_PULL_UP);
        gpio_free(PW_INT_LABEL);

        iIrqResult = request_irq(PW_INT, pw_int_interrupt, IRQ_TYPE_EDGE_FALLING, PW_INT_NAME, NULL);
        if (iIrqResult < 0) {
            WRNPRINT("Request IRQ %d failed with return code %d.\n", PW_INT, iIrqResult);
        }
    }
    else {
        WRNPRINT("Request GPIO %d failed with return code %d.\n", PW_INT_LABEL, iIrqResult);
    }
    //Request interrupt DAC_INT
    iIrqResult = gpio_request(DAC_INT_LABEL, DAC_INT_NAME);
    if (0 == iIrqResult) {
        s3c_gpio_cfgpin(DAC_INT_LABEL, S3C_GPIO_SFN(0xF));
        s3c_gpio_setpull(DAC_INT_LABEL, S3C_GPIO_PULL_UP);
        gpio_free(DAC_INT_LABEL);

        iIrqResult = request_irq(DAC_INT, dac_int_interrupt, IRQ_TYPE_EDGE_FALLING, DAC_INT_NAME, NULL);
        if (iIrqResult < 0) {
            WRNPRINT("Request IRQ %d failed with return code %d.\n", DAC_INT, iIrqResult);
        }
    }
    else {
        WRNPRINT("Request GPIO %d failed with return code %d.\n", DAC_INT_LABEL, iIrqResult);
    }
#ifdef IS_GPIO_INTERRUPT_DEBUG
    WRNPRINT("You have enabled on-board GPIO keys\' interrupts. These interrupts need disabling \'GPIO Buttons\' driver in Kernel-Config\'s \'Device Drivers -> Input device support -> Keyboards\' menu to work. If you did so, GPIO keypads may not be available.\n");
    //Request interrupt KEY_HOME
    iIrqResult = gpio_request(KEY_HOME_LABEL, KEY_HOME_NAME);
    if (0 == iIrqResult) {
        s3c_gpio_cfgpin(KEY_HOME_LABEL, S3C_GPIO_SFN(0xF));
        s3c_gpio_setpull(KEY_HOME_LABEL, S3C_GPIO_PULL_UP);
        gpio_free(KEY_HOME_LABEL);

        iIrqResult = request_irq(KEY_HOME, key_home_interrupt, IRQ_TYPE_EDGE_FALLING, KEY_HOME_NAME, NULL);
        if (iIrqResult < 0) {
            WRNPRINT("Request IRQ %d failed with return code %d.\n", KEY_HOME, iIrqResult);
        }
    }
    else {
        WRNPRINT("Request GPIO %d failed with return code %d.\n", KEY_HOME_LABEL, iIrqResult);
    }
    //Request interrupt KEY_BACK
    iIrqResult = gpio_request(KEY_BACK_LABEL, KEY_BACK_NAME);
    if (0 == iIrqResult) {
        s3c_gpio_cfgpin(KEY_BACK_LABEL, S3C_GPIO_SFN(0xF));
        s3c_gpio_setpull(KEY_BACK_LABEL, S3C_GPIO_PULL_UP);
        gpio_free(KEY_BACK_LABEL);

        iIrqResult = request_irq(KEY_BACK, key_back_interrupt, IRQ_TYPE_EDGE_FALLING, KEY_BACK_NAME, NULL);
        if (iIrqResult < 0) {
            WRNPRINT("Request IRQ %d failed with return code %d.\n", KEY_BACK, iIrqResult);
        }
    }
    else {
        WRNPRINT("Request GPIO %d failed with return code %d.\n", KEY_BACK_LABEL, iIrqResult);
    }
    //Request interrupt KEY_SLEEP
    iIrqResult = gpio_request(KEY_SLEEP_LABEL, KEY_SLEEP_NAME);
    if (0 == iIrqResult) {
        s3c_gpio_cfgpin(KEY_SLEEP_LABEL, S3C_GPIO_SFN(0xF));
        s3c_gpio_setpull(KEY_SLEEP_LABEL, S3C_GPIO_PULL_UP);
        gpio_free(KEY_SLEEP_LABEL);

        iIrqResult = request_irq(KEY_SLEEP, key_sleep_interrupt, IRQ_TYPE_EDGE_FALLING, KEY_SLEEP_NAME, NULL);
        if (iIrqResult < 0) {
            WRNPRINT("Request IRQ %d failed with return code %d.\n", KEY_SLEEP, iIrqResult);
        }
    }
    else {
        WRNPRINT("Request GPIO %d failed with return code %d.\n", KEY_SLEEP_LABEL, iIrqResult);
    }
    //Request interrupt KEY_VOL+
    iIrqResult = gpio_request(KEY_VOLUP_LABEL, KEY_VOLUP_NAME);
    if (0 == iIrqResult) {
        s3c_gpio_cfgpin(KEY_VOLUP_LABEL, S3C_GPIO_SFN(0xF));
        s3c_gpio_setpull(KEY_VOLUP_LABEL, S3C_GPIO_PULL_UP);
        gpio_free(KEY_VOLUP_LABEL);

        iIrqResult = request_irq(KEY_VOLUP, key_volup_interrupt, IRQ_TYPE_EDGE_FALLING, KEY_VOLUP_NAME, NULL);
        if (iIrqResult < 0) {
            WRNPRINT("Request IRQ %d failed with return code %d.\n", KEY_VOLUP, iIrqResult);
        }
    }
    else {
        WRNPRINT("Request GPIO %d failed with return code %d.\n", KEY_VOLUP_LABEL, iIrqResult);
    }
    //Request interrupt KEY_VOL-
    iIrqResult = gpio_request(KEY_VOLDOWN_LABEL, KEY_VOLDOWN_NAME);
    if (0 == iIrqResult) {
        s3c_gpio_cfgpin(KEY_VOLDOWN_LABEL, S3C_GPIO_SFN(0xF));
        s3c_gpio_setpull(KEY_VOLDOWN_LABEL, S3C_GPIO_PULL_UP);
        gpio_free(KEY_VOLDOWN_LABEL);

        iIrqResult = request_irq(KEY_VOLDOWN, key_voldown_interrupt, IRQ_TYPE_EDGE_FALLING, KEY_VOLDOWN_NAME, NULL);
        if (iIrqResult < 0) {
            WRNPRINT("Request IRQ %d failed with return code %d.\n", KEY_VOLDOWN, iIrqResult);
        }
    }
    else {
        WRNPRINT("Request GPIO %d failed with return code %d.\n", KEY_VOLDOWN_LABEL, iIrqResult);
    }
#endif
    //Create device node
    clsDevice = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(clsDevice)) {
        WRNPRINT("failed in creating device class.\n");
        return 0;
    }
    device_create(clsDevice, NULL, devDeviceNumber, NULL, NODE_NAME);
    return 0;
}

static void __exit interrupt_demo_exit(void) {
    DBGPRINT("Exiting...\n");
    device_destroy(clsDevice, MKDEV(iMajorDeviceNumber, 0));
    class_destroy(clsDevice);
    cdev_del(&cdevDevice);
    unregister_chrdev_region(MKDEV(iMajorDeviceNumber, 0), 1);
    //Use free_irq() to unregister interrupts here
    free_irq(S_INT, NULL);
    free_irq(DP_INT, NULL);
    free_irq(PW_INT, NULL);
    free_irq(DAC_INT, NULL);
#ifdef IS_GPIO_INTERRUPT_DEBUG
    free_irq(KEY_HOME, NULL);
    free_irq(KEY_BACK, NULL);
    free_irq(KEY_SLEEP, NULL);
    free_irq(KEY_VOLUP, NULL);
    free_irq(KEY_VOLDOWN, NULL);
#endif
    return;
}

/* Pointers to Init & Exit Functions */
module_init(interrupt_demo_init);
module_exit(interrupt_demo_exit);

/* Pointers to Platform Driver Related Functions */
static struct platform_driver interrupt_demo_driver = {
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
MODULE_AUTHOR("Picsell-Dois@SCP-EQ");
MODULE_DESCRIPTION("Exynos-4412 interrupt demo driver");
