#ifndef _INTERRUPT_DEMO_H
#define _INTERRUPT_DEMO_H

#define DRIVER_NAME "interrupt-demo"
#define NODE_NAME "interrupt-demo"
#define CLASS_NAME "interrupt-demo-class"

#define DATA_BUFFER_SIZE 1024

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
#define XEINT25_NAME "PW_INT/GM_INT2/XEINT25"
#define XEINT28_NAME "DAC_INT/COMPASS_RDY/XEINT28"
#define XEINT1_NAME "S_INT/XEINT1_BAK/XEINT1"
#define XEINT20_NAME "DP_INT/XEINT20_BAK/XEINT20"
#define XEINT9_NAME "KEY_HOME/UART_RING/XEINT9"
#define XEINT10_NAME "KEY_BACK/SIM_DET/XEINT10"
#define XEINT27_NAME "KEY_SLEEP/GYRO_INT/XEINT27"
#define XEINT17_NAME "KEY_VOL+/KP_ROW1/XEINT17"
#define XEINT16_NAME "KEY_VOL-/KP_ROW0/XEINT16"

#endif
