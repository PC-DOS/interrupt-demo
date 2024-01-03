#ifndef INTERRUPT_DEMO_H
#define INTERRUPT_DEMO_H

/* Name Strings */
#define DRIVER_NAME "interrupt-demo"
#define DEVICE_NAME "interrupt-demo"
#define NODE_NAME   "interrupt-demo"
#define CLASS_NAME  "interrupt-demo-class"

/* Data Buffer Definitions */
//Structure of Data Buffer:
//[Wave(0)][Wave(1)]...[Wave(DATA_BUFFER_WAVE_DATA_SIZE - 1)][ExtData(0)][ExtData(1)]...[ExtData(DATA_BUFFER_EXTRA_DATA_SIZE - 1)]
#define DATA_BUFFER_WAVE_DATA_SIZE  520 //Size of wave data zone of Data Buffer
#define DATA_BUFFER_EXTRA_DATA_SIZE 0 //Size of extra data (non-wave data) of Data Buffer
#define DATA_BUFFER_SIZE            (DATA_BUFFER_WAVE_DATA_SIZE + DATA_BUFFER_EXTRA_DATA_SIZE) //Data Buffer (to store data and read) size. For consumer programs (e.g. UserApp), its buffer size is better to be the same as DATA_BUFFER_SIZE. Otherwise, a Segmentation Fault may occur.
#define DATA_MAX_VALUE              10 //Max data value
#define CONTROL_COMMAND_BUFFER_SIZE 2 //Command Buffer (for write() function) size

/* Information Printing Functions */
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

/* Interrupt Names */
#define IS_GPIO_INTERRUPT_DEBUG //Comment this when using GPIO keypad
#define S_INT_NAME              "S_INT__XEINT1_BAK__XEINT1"
#define XEINT20_NAME            "DP_INT__XEINT20_BAK__XEINT20"
#define PW_INT_NAME             "PW_INT__GM_INT2__XEINT25"
#define DAC_INT_NAME            "DAC_INT__COMPASS_RDY__XEINT28"
#ifdef IS_GPIO_INTERRUPT_DEBUG
#define KEY_HOME_NAME    "KEY_HOME__UART_RING__XEINT9"
#define KEY_BACK_NAME    "KEY_BACK__SIM_DET__XEINT10"
#define KEY_SLEEP_NAME   "KEY_SLEEP__GYRO_INT__XEINT27"
#define KEY_VOLUP_NAME   "KEY_VOL+__KP_ROW1__XEINT17"
#define KEY_VOLDOWN_NAME "KEY_VOL-__KP_ROW0__XEINT16"
#endif

/* Interrupt IDs and Labels */
//S_INT__XEINT1_BAK__XEINT1
#define S_INT_LABEL EXYNOS4_GPX0(1)
#define S_INT       IRQ_EINT(1)
//DP_INT__XEINT20_BAK__XEINT20
#define DP_INT_LABEL EXYNOS4_GPX2(4)
#define DP_INT       IRQ_EINT(20)
//PW_INT__GM_INT2__XEINT25
#define PW_INT_LABEL EXYNOS4_GPX3(1)
#define PW_INT       IRQ_EINT(25)
//DAC_INT__COMPASS_RDY__XEINT28
#define DAC_INT_LABEL EXYNOS4_GPX3(4)
#define DAC_INT       IRQ_EINT(28)
#ifdef IS_GPIO_INTERRUPT_DEBUG
//KEY_HOME__UART_RING__XEINT9
#define KEY_HOME_LABEL EXYNOS4_GPX1(1)
#define KEY_HOME       IRQ_EINT(9)
//KEY_BACK__SIM_DET__XEINT10
#define KEY_BACK_LABEL EXYNOS4_GPX1(2)
#define KEY_BACK       IRQ_EINT(10)
//KEY_SLEEP__GYRO_INT__XEINT27
#define KEY_SLEEP_LABEL EXYNOS4_GPX3(3)
#define KEY_SLEEP       IRQ_EINT(27)
//KEY_VOL+__KP_ROW1__XEINT17
#define KEY_VOLUP_LABEL EXYNOS4_GPX2(1)
#define KEY_VOLUP       IRQ_EINT(17)
//KEY_VOL-__KP_ROW0__XEINT16
#define KEY_VOLDOWN_LABEL EXYNOS4_GPX2(0)
#define KEY_VOLDOWN       IRQ_EINT(16)
#endif

/* Control Commands */
//Control commands are defined in CTL_CMD_ format
/* Original command defines in old Driver source code
#define MASK_EINT1_IRQ        0x0
#define UNMASK_EINT1_IRQ      0x1
#define SET_USER_PID          0x2
#define SET_DCTL_H            0x3  //Delay Control - High Byte
#define SET_DCTL_L            0x4  //Delay Control - Low Byte
#define SET_RTCL_L            0x5  //Rate Control
#define SET_COMP_H            0x6  //Compress Rate (Scale) - High Byte
#define SET_COMP_L            0x7  //Compress Rate (Scale) - Low Byte
#define SET_COMP_INT          0x8  //int data
#define SET_COMP_FLOAT        0x9  //float data
#define SET_GAIN_BASE         0x0a //Set Gain
#define SET_CHANNEL           0x0c //Set Channel
#define SET_VAR_12            0x12 //Reserved
#define SET_VAR_14            0x14
#define SET_VAR_16            0x16
#define SET_VAR_18            0x18
#define SET_VAR_1A            0x1a
#define SET_VAR_1C            0x1c
#define SET_VAR_1E            0x1e
*/
#define CTL_CMD_DISABLE_IRQ                  0x00 //Disable IRQ
#define CTL_CMD_ENABLE_IRQ                   0x01 //Enable IRQ
#define CTL_CMD_SET_USER_APP_PID             0x02 //Set the PID (Process ID) of user app
#define CTL_CMD_SET_DELAY_HIGH_BYTE          0x03 //Set Delay (High Byte)
#define CTL_CMD_SET_DELAY_LOW_BYTE           0x04 //Set Delay (Low Byte)
#define CTL_CMD_SET_RATE                     0x05 //Set Rate
#define CTL_CMD_SET_COMPRESS_COUNT_HIGH_BYTE 0x06 //Set Compress Count (How many points are merged), High Byte
#define CTL_CMD_SET_COMPRESS_COUNT_LOW_BYTE  0x07 //Set Compress Count (How many points are merged), Low Byte
#define CTL_CMD_SET_COMPRESS_STEP_INT_PART   0x08 //Set Compress Step's integer part
#define CTL_CMD_SET_COMPRESS_STEP_FLOAT_PART 0x09 //Set Compress Step's decimal part
#define CTL_CMD_SET_GAIN                     0x0a //Set Gain
#define CTL_CMD_SET_CHANNEL                  0x0c //Set Channel
#define CTL_CMD_RESERVED_12                  0x12 //Reserved
#define CTL_CMD_RESERVED_14                  0x14 //Reserved
#define CTL_CMD_RESERVED_16                  0x16 //Reserved
#define CTL_CMD_RESERVED_18                  0x18 //Reserved
#define CTL_CMD_RESERVED_1A                  0x1a //Reserved
#define CTL_CMD_RESERVED_1C                  0x1c //Reserved
#define CTL_CMD_RESERVED_1E                  0x1e //Reserved

/* Control Arguments */
//Arguments are defined in CTL_ARG_ format
#define CTL_ARG_IRQ_NAME_NULL    0x00
#define CTL_ARG_IRQ_NAME_S_INT   0x01
#define CTL_ARG_IRQ_NAME_DP_INT  0x02
#define CTL_ARG_IRQ_NAME_PW_INT  0x03
#define CTL_ARG_IRQ_NAME_DAC_INT 0x04
#ifdef IS_GPIO_INTERRUPT_DEBUG
#define CTL_ARG_IRQ_NAME_KEY_HOME    0x11
#define CTL_ARG_IRQ_NAME_KEY_BACK    0x12
#define CTL_ARG_IRQ_NAME_KEY_SLEEP   0x13
#define CTL_ARG_IRQ_NAME_KEY_VOLUP   0x14
#define CTL_ARG_IRQ_NAME_KEY_VOLDOWN 0x15
#endif

//Function Signatures
static void ProcessIoControlCommand(unsigned int iIoControlCommand, unsigned long lpIoControlParameters);

//Sample Data
static unsigned int arrDataDef[DATA_BUFFER_SIZE] = {350, 355, 345, 343, 354, 352, 351, 350, 350, 345, 338, 300, 245, 183, 134, 76, 20, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 45, 90, 125, 165, 200, 245, 243, 249, 245, 250, 245, 244, 245, 249, 250, 245, 225, 175, 130, 96, 50, 25, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 20, 50, 80, 124, 125, 124, 125, 125, 123, 125, 124, 124, 126, 75, 45, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 25, 49, 45, 50, 55, 52, 54, 50, 52, 51, 48, 20, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
#endif
