#ifndef _INTERRUPT_DEMO_H
#define _INTERRUPT_DEMO_H

#define DRIVER_NAME "interrupt-demo"
#define NODE_NAME "interrupt-demo"
#define CLASS_NAME "interrupt-demo-class"

#define DATA_BUFFER_SIZE 1000 //Data Buffer (to store data and read) size. For consumer programs (e.g. UserApp), its buffer size is better to be larger than DATA_BUFFER_SIZE. Otherwise, a Segmentation Fault may occur.
#define DATA_MAX_VALUE 10 //Max data value
#define CTL_COMMAND_BUFFER_SIZE 2 //Command Buffer (for write() function) size

//DBGPRINT() is used to print debug messages, comment #define IS_IN_DEBUG to disable them
#define IS_IN_DEBUG
#ifdef IS_IN_DEBUG
#define DBGPRINT(sInfo...) printk(KERN_INFO "InterruptDemo: " sInfo)
#else
#define DBGPRINT(sInfo...)
#endif
//printk() in different levels
#define NFOPRINT(sInfo...) printk(KERN_INFO "InterruptDemo - Info: " sInfo)
#define WRNPRINT(sInfo...) printk(KERN_WARNING "InterruptDemo - Warning: " sInfo)
#define ERRPRINT(sInfo...) printk(KERN_ERR "InterruptDemo - Error: " sInfo)

//Interrupt Names
#define IS_GPIO_INTERRUPT_DEBUG //Comment this when using GPIO keypad
#define XEINT1_NAME "S_INT__XEINT1_BAK__XEINT1"
#define XEINT20_NAME "DP_INT__XEINT20_BAK__XEINT20"
#define XEINT25_NAME "PW_INT__GM_INT2__XEINT25"
#define XEINT28_NAME "DAC_INT__COMPASS_RDY__XEINT28"
#ifdef IS_GPIO_INTERRUPT_DEBUG
#define XEINT9_NAME "KEY_HOME__UART_RING__XEINT9"
#define XEINT10_NAME "KEY_BACK__SIM_DET__XEINT10"
#define XEINT27_NAME "KEY_SLEEP__GYRO_INT__XEINT27"
#define XEINT17_NAME "KEY_VOL+__KP_ROW1__XEINT17"
#define XEINT16_NAME "KEY_VOL-__KP_ROW0__XEINT16"
#endif

//Interrupt IDs and Labels
//S_INT__XEINT1_BAK__XEINT1
#define		S_INT				IRQ_EINT(1)
#define		XEINT1_BAK			IRQ_EINT(1)
#define		XEINT1				IRQ_EINT(1)
#define		S_INT_LABEL			EXYNOS4_GPX0(1)
#define		XEINT1_BAK_LABEL	EXYNOS4_GPX0(1)
#define		XEINT1_LABEL		EXYNOS4_GPX0(1)
//DP_INT__XEINT20_BAK__XEINT20
#define		DP_INT				IRQ_EINT(20)
#define		XEINT20_BAK			IRQ_EINT(20)
#define		XEINT20				IRQ_EINT(20)
#define		DP_INT_LABEL		EXYNOS4_GPX2(4)
#define		XEINT20_BAK_LABEL	EXYNOS4_GPX2(4)
#define		XEINT20_LABEL		EXYNOS4_GPX2(4)
//PW_INT__GM_INT2__XEINT25
#define		PW_INT				IRQ_EINT(25)
#define		GM_INT2				IRQ_EINT(25)
#define		XEINT25				IRQ_EINT(25)
#define		PW_INT_LABEL		EXYNOS4_GPX3(1)
#define		GM_INT2_LABEL		EXYNOS4_GPX3(1)
#define		XEINT25_LABEL		EXYNOS4_GPX3(1)
//DAC_INT__COMPASS_RDY__XEINT28
#define		DAC_INT				IRQ_EINT(28)
#define		COMPASS_RDY			IRQ_EINT(28)
#define		XEINT28				IRQ_EINT(28)
#define		DAC_INT_LABEL		EXYNOS4_GPX3(4)
#define		COMPASS_RDY_LABEL	EXYNOS4_GPX3(4)
#define		XEINT28_LABEL		EXYNOS4_GPX3(4)
#ifdef IS_GPIO_INTERRUPT_DEBUG
//KEY_HOME__UART_RING__XEINT9
#define		KEY_HOME			IRQ_EINT(9)
#define		UART_RING			IRQ_EINT(9)
#define		XEINT9				IRQ_EINT(9)
#define		KEY_HOME_LABEL		EXYNOS4_GPX1(1)
#define		UART_RING_LABEL		EXYNOS4_GPX1(1)
#define		XEINT9_LABEL		EXYNOS4_GPX1(1)
//KEY_BACK__SIM_DET__XEINT10
#define		KEY_BACK			IRQ_EINT(10)
#define		SIM_DET				IRQ_EINT(10)
#define		XEINT10				IRQ_EINT(10)
#define		KEY_BACK_LABEL		EXYNOS4_GPX1(2)
#define		SIM_DET_LABEL		EXYNOS4_GPX1(2)
#define		XEINT10_LABEL		EXYNOS4_GPX1(2)
//KEY_SLEEP__GYRO_INT__XEINT27
#define		KEY_SLEEP			IRQ_EINT(27)
#define		GYRO_INT			IRQ_EINT(27)
#define		XEINT27				IRQ_EINT(27)
#define		KEY_SLEEP_LABEL		EXYNOS4_GPX3(3)
#define		GYRO_INT_LABEL		EXYNOS4_GPX3(3)
#define		XEINT27_LABEL		EXYNOS4_GPX3(3)
//KEY_VOL+__KP_ROW1__XEINT17
#define		KEY_VOLUP			IRQ_EINT(17)
#define		KP_ROW1				IRQ_EINT(17)
#define		XEINT17				IRQ_EINT(17)
#define		KEY_VOLUP_LABEL		EXYNOS4_GPX2(1)
#define		KP_ROW1_LABEL		EXYNOS4_GPX2(1)
#define		XEINT17_LABEL		EXYNOS4_GPX2(1)
//KEY_VOL-__KP_ROW0__XEINT16
#define		KEY_VOLDOWN			IRQ_EINT(16)
#define		KP_ROW0				IRQ_EINT(16)
#define		XEINT16				IRQ_EINT(16)
#define		KEY_VOLDOWN_LABEL	EXYNOS4_GPX2(0)
#define		KP_ROW0_LABEL		EXYNOS4_GPX2(0)
#define		XEINT16_LABEL		EXYNOS4_GPX2(0)
#endif

//Control Commands
/* Original command defines in old Driver source code
#define MASK_EINT1_IRQ		0x0
#define UNMASK_EINT1_IRQ	0x1
#define SET_USER_PID		0x2
#define SET_DCTL_H			0x3  //Delay Control - High Byte
#define SET_DCTL_L			0x4  //Delay Control - Low Byte
#define SET_RTCL_L			0x5  //Rate Control
#define SET_COMP_H			0x6  //Compress Rate (Scale) - High Byte
#define SET_COMP_L			0x7  //Compress Rate (Scale) - Low Byte
#define SET_COMP_INT		0x8  //int data
#define SET_COMP_FLOAT		0x9  //float data
#define SET_GAIN_BASE		0x0a //Set Gain
#define SET_CHANNEL			0x0c //Set Channel
#define SET_VAR_12			0x12 //Reserved
#define SET_VAR_14			0x14
#define SET_VAR_16			0x16
#define SET_VAR_18			0x18
#define SET_VAR_1A			0x1a
#define SET_VAR_1C			0x1c
#define SET_VAR_1E			0x1e
*/
#define CTL_DISABLE_IRQ			0x00 //Disable IRQ
#define CTL_ENABLE_IRQ			0x01 //Enable IRQ
#define CTL_SET_USER_APP_PID	0x02 //Set the PID (Process ID) of user app
#define CTL_SET_DELAY_HIGH_BYTE	0x03 //Set Delay (High Byte)
#define CTL_SET_DELAY_LOW_BYTE	0x04 //Set Delay (Low Byte)
#define CTL_SET_RATE			0x05 //Set Rate
#define CTL_SET_SCALE_HIGH_BYTE	0x06 //Set Scale (Compress Rate) (High Byte)
#define CTL_SET_SCALE_LOW_BYTE	0x07 //Set Scale (Compress Rate) (Low Byte)
#define CTL_SET_SCALE_INT		0x08 //Set Sacle in INT
#define CTL_SET_SCALE_FLOAT		0x09 //Set Sacle in FLOAT
#define CTL_SET_GAIN			0x0a //Set Gain
#define CTL_SET_CHANNEL			0x0c //Set Channel
#define CTL_RESERVED_12			0x12 //Reserved
#define CTL_RESERVED_14			0x14 //Reserved
#define CTL_RESERVED_16			0x16 //Reserved
#define CTL_RESERVED_18			0x18 //Reserved
#define CTL_RESERVED_1A			0x1a //Reserved
#define CTL_RESERVED_1C			0x1c //Reserved
#define CTL_RESERVED_1E			0x1e //Reserved

//Interrupt Names for CTL_XXXX_IRQ Commands
#define CTL_IRQ_NAME_NULL			0x00
#define CTL_IRQ_NAME_S_INT			0x01
#define CTL_IRQ_NAME_DP_INT			0x02
#define CTL_IRQ_NAME_PW_INT			0x03
#define CTL_IRQ_NAME_DAC_INT		0x04
#ifdef IS_GPIO_INTERRUPT_DEBUG
#define CTL_IRQ_NAME_KEY_HOME		0x11
#define CTL_IRQ_NAME_KEY_BACK		0x12
#define CTL_IRQ_NAME_KEY_SLEEP		0x13
#define CTL_IRQ_NAME_KEY_VOLUP		0x14
#define CTL_IRQ_NAME_KEY_VOLDOWN	0x14
#endif

//Function Signatures
void ProcessIoControlCommand(unsigned int iIoControlCommand, unsigned long lpIoControlParameters);

//Sample Data
unsigned int arrDataDef[DATA_BUFFER_SIZE] = {350, 355, 345, 343, 354, 352, 351, 350, 350, 345, 338, 300, 245, 183, 134, 76, 20, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 45, 90, 125, 165, 200, 245, 243, 249, 245, 250, 245, 244, 245, 249, 250, 245, 225, 175, 130, 96, 50, 25, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 20, 50, 80, 124, 125, 124, 125, 125, 123, 125, 124, 124, 126, 75, 45, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 25, 49, 45, 50, 55, 52, 54, 50, 52, 51, 48, 20, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
#endif
