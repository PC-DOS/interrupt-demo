#ifndef _INTERRUPT_DEMO_H
#define _INTERRUPT_DEMO_H

#define DRIVER_NAME "interrupt-demo"
#define NODE_NAME "interrupt-demo"
#define CLASS_NAME "interrupt-demo-class"

#define DATA_BUFFER_SIZE 1024

#define IS_IN_DEBUG
#ifdef IS_IN_DEBUG
#define DBGPRINT(sInfo...) printk(KERN_INFO "InterruptDemo: " sInfo)
#else
#define DBGPRINT(sInfo...)
#endif

#define NFOPRINT(sInfo...) printk(KERN_INFO "InterruptDemo: " sInfo)
#define WRNPRINT(sInfo...) printk(KERN_WARNING "InterruptDemo: " sInfo)
#define ERRPRINT(sInfo...) printk(KERN_ERR "InterruptDemo: " sInfo)

#endif
