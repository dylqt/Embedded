#ifndef __KEYSCAN_H__
#define __KEYSCAN_H__

#include "stc15_adc.h"
#include "stc15_uart.h"
#include "stc15_oled.h"
#include "power.h"
#include "beep.h"

// for key scan
#define S0_NO_INPUT 0
#define S1_TUCH 1
#define S2_KEYP 2
#define S3_KEYM 3
#define S4_MODE 4
#define S5_ENTER 5


extern unsigned char mode_flag_t;

unsigned char keyScan(void);

unsigned char keyScanTest(void);

void pageScan(unsigned char key);

#endif
