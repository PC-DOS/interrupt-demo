#ifndef _INTERRUPT_DEMO_H
#define _INTERRUPT_DEMO_H

#define DRIVER_NAME "interrupt-demo"
#define NODE_NAME "interrupt-demo"
#define CLASS_NAME "interrupt-demo-class"

#define IS_IN_DEBUG
#ifdef IS_IN_DEBUG
#define DBGPRINT(sInfo...) printk(KERN_DEBUG "InterruptDemo - Debug:" sInfo)
#else
#define DBGPRINT(sInfo...)
#endif

#endif
