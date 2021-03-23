#ifndef _INTERRUPT_DEMO_H
#define _INTERRUPT_DEMO_H

#define DRIVER_NAME "interrupt-demo"
#define NODE_NAME "interrupt-demo"
#define CLASS_NAME "interrupt-demo-class"

#define DATA_BUFFER_SIZE 1024
#define DATA_MAX_VALUE 400

//DBGPRINT() is used to print debug messages, comment #define IS_IN_DEBUG to disable them
#define IS_IN_DEBUG
#ifdef IS_IN_DEBUG
#define DBGPRINT(sInfo...) printk(KERN_INFO "InterruptDemo: " sInfo)
#else
#define DBGPRINT(sInfo...)
#endif
//printk() in different levels
#define NFOPRINT(sInfo...) printk(KERN_INFO "InterruptDemo: " sInfo)
#define WRNPRINT(sInfo...) printk(KERN_WARNING "InterruptDemo: " sInfo)
#define ERRPRINT(sInfo...) printk(KERN_ERR "InterruptDemo: " sInfo)

//Interrupt Names
#define IS_GPIO_INTERRUPT_DEBUG //Comment this when using GPIO keypad
#define XEINT25_NAME "PW_INT__GM_INT2__XEINT25"
#define XEINT28_NAME "DAC_INT__COMPASS_RDY__XEINT28"
#define XEINT1_NAME "S_INT__XEINT1_BAK__XEINT1"
#define XEINT20_NAME "DP_INT__XEINT20_BAK__XEINT20"
#ifdef IS_GPIO_INTERRUPT_DEBUG
#define XEINT9_NAME "KEY_HOME__UART_RING__XEINT9"
#define XEINT10_NAME "KEY_BACK__SIM_DET__XEINT10"
#define XEINT27_NAME "KEY_SLEEP__GYRO_INT__XEINT27"
#define XEINT17_NAME "KEY_VOL+__KP_ROW1__XEINT17"
#define XEINT16_NAME "KEY_VOL-__KP_ROW0__XEINT16"
#endif

#endif
