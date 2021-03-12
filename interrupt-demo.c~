/* Interrupt Demo Driver
 *
 * This is a character driver, which is used to demostrate Exynos-4412's interrupts
 */

/* Main header files */
#include <linux/init.h>
#include <linux/module.h>
/* Samsung header files */
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
/* Local header files */
#include "interrput-demo.h"

static int interrupt_demo_init(void){
	printk(KERN_INFO "InterruptDemo: Initializing...");
	return 0;
}

static void interrupt_demo_exit(void){
	printk(KERN_INFO "InterrputDemo: Exiting...");
	return;
}

/* Init function & Exit function */
module_init(interrupt_demo_init);
module_exit(interrupt_demo_exit);

/* Licenses, Authors & Descriptions */
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Picsell-Dois@SEU-BME");
MODULE_DESCRIPTION("Exynos-4412 interrupt demo driver");
