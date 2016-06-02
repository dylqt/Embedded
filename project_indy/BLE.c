#include "BLE.h"

// ------------- 初始化 --------------------

void ble_init()
{
	P6M1 &= ~0xe0;	// BT_CTL MODE_CTL BRT 强输出
	P6M0 |= 0xe0;

	P6M1 |= 0x10;	// BUSY 开漏
	P6M0 |= 0x10;

	BT_OPEN;
	WEEK_UP_BT;
	TRAN_MODE_BLE_IN;	
}


// ------------- 设置波特率  ----------------------

// 成功 返回 1
// 失败 返回 -1

// 输入 id：
// 00：2400 bps
// 01：4800 bps
// 02：9600 bps
// 03：19200 bps
// 04：38400 bps
// 05：57600 bps
// 06：115200 bps
// --------------------------------------------------
signed char set_baud( unsigned char id )
{
	int i = 0;
	clear_uart2_data();
	// set
	SendData2( 0x01 );
    SendData2( 0xfc );
    SendData2( 0x01 );
    SendData2( 0x01 );
    SendData2( id );

	Delay20ms();

	// check
	//if( uart2_rec_cnt != 4 )
	//	return -1;
	if( uart2_rec_cnt == 0 )
		return -1;
	for(; i  < uart2_rec_cnt; i++){
		 SendData2( uart2_rec_data[i] );
	}
	if( uart2_rec_data[2] == 0x01 )
		return 1;	// success
	else 
		return -1; 	
}


// ---------------- 查询波特率 -----------------
// 成功 返回 id
// 失败 返回 -1
// ---------------------------------------------
signed char inquire_baud(void)
{
	clear_uart2_data();
	// inquire
	SendData2( 0x01 );
    SendData2( 0xfc );
    SendData2( 0x02 );
    SendData2( 0x00 );

	Delay20ms();

   	// check
	if( uart2_rec_cnt != 5 )
		return -1;
	if( uart2_rec_data[3] == 0x01 )
		return uart2_rec_data[4];	// success
	else 
		return -1; 
}

// ----------------- 发送数传 --------------
