#include "BLE.h"
#include "beep.h"	

struct BLERECVSTATE bleRecvState;
struct BLESTATE bleState;

// ------------- 初始化 --------------------

void ble_init()
{
	P6M1 &= ~0xe0;	// BT_CTL MODE_CTL BRT 强输出
	P6M0 |= 0xe0;

	P6M1 |= 0x10;	// BUSY 开漏
	P6M0 |= 0x10;

	BT_OPEN;
	WEEK_UP_BT;
	BLE_TRAN_MODE_IN;	

	/*bleRecvState.hand = 0;
	bleRecvState.recState = 0;
	bleRecvState.setEnSendState = 0;
	bleRecvState.enSend = 0;
	bleRecvState.setEnSendState = 0;
	memset(bleRecvState.setIndyFreq, 0, sizeof(bleRecvState.setIndyFreq));
	memset(bleRecvState.setIndyPower, 0, sizeof(bleRecvState.setIndyPower));
	bleRecvState.setMoreInfo = 0;
	bleState.enSend = 0;
	bleState.fre = 0;
	bleState.moreInfor = 0;
	bleState.power = 0;
	*/
	memset(&bleRecvState, 0, sizeof(struct BLERECVSTATE));
	memset(&bleState, 0, sizeof(struct BLESTATE));
	bleClearRecData();
}

// ----------------- 清除收到的数据 ----------------
void bleClearRecData()
{
	clear_uart3_data();
	return;
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
signed char ble_set_baud( unsigned char id )
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
	if( uart2_rec_data[2] == 0x01 ){
		OledWriteWordByHex(3, 0, uart2_rec_cnt);
		OledWriteMessage57("BLE set baud Success");
		return 1;	// success
	}
	else {
		OledWriteWordByHex(3, 0, uart2_rec_cnt);
		OledWriteMessage57("BLE set baud False");
		return -1;
	}
}


// ---------------- 查询波特率 -----------------
// 成功 返回 id
// 失败 返回 -1
// ---------------------------------------------
signed char ble_inquire_baud(void)
{
	clear_uart2_data();
	// inquire
	SendData2( 0x01 );
    SendData2( 0xfc );
    SendData2( 0x02 );
    SendData2( 0x00 );

	Delay50ms();

   	// check

	if( uart2_rec_cnt != 5 ){
		//OledClear();
		OledWriteWordByHex(3, 0, uart2_rec_cnt);
		OledWriteMessage57("BLE baud read False1");
		return -1;
	}
	
	if( uart2_rec_data[3] == 0x01 ){
		//OledClear();
		OledWriteMessage57("BLE baud read Success");
		return uart2_rec_data[4];	// success
	}
	else{
		//OledClear();
		OledWriteMessage57("BLE baud read False2");
		return -1; 
	}
}

void bleSendChar(char dat)
{
	SendData3(dat);
	SendData4(dat);
}

/*
蓝牙协议:

包头			设备号		命令号		包长度	状态标识 数据	 	校验位		包尾
1、	状态包
'h''m' 				5    	0  2 		 0  12 		0  [1字节DATA]	   1		'e''n' 'd'

2、	rfid
'h''m'				5 		0  3  		 0  23      0 [12字节DATA] 	   1		'e''n' 'd'

3、	TEMP
'h''m' 				5		0  4  		 0  13  	0 [2字节DATA] 	   1		'e''n' 'd'

4、	sensorCode
'h''m' 				5 		0  5  		 0  13      0 [2字节DATA]      1		'e''n' 'd'

5、	rssi
'h''m' 				5 		0  6  		 0  13  	0 [2字节DATA]      1		'e''n' 'd'

*/

// ----------------- 发送数传 --------------
void bleSendData(unsigned char cmdType, char *buf, unsigned char bufLen, unsigned char state)
{
	int i = 0;
	
	bleSendChar('h'); bleSendChar('m');	// 包头
	
	bleSendChar(5);	// 设备号
	
	bleSendChar(0); bleSendChar(cmdType); // 命令
	
	bleSendChar(0); bleSendChar(12 + bufLen);	// 包长度

	bleSendChar(state);	// 状态标志
	
	if(buf == NULL)
		bleSendChar(0);
	else
		for(i = 0; i < bufLen; i++){	// 命令
			bleSendChar(*(buf + i));
		}
	bleSendChar(1);	// 校验位
	bleSendChar('e');bleSendChar('n');bleSendChar('d');	// 包尾
}


// 解析蓝牙接收到的命令
unsigned char bleParseData(void)
{
	unsigned char rec = 0;
	int from = 0;	// 命令起始位置
	int to = 0;		// 命令结束位置
	int DataLen = 0;	// 包长度
	int cmdIndex = 0; 	// 命令号
	int cmdStart = 0;	// 命令开始
	int fredata = 0;
	int powdata = 0;
	if(uart3_rec_cnt == 0)
		return 0;
	for(; from < uart3_rec_cnt; from++){
		if(uart3_rec_data[from] != 'h' || uart3_rec_data[from + 1] != 'm')	// 包头
			continue;
		else{
			to = from + 2;
			for(; to + 2 < uart3_rec_cnt; to++){
				if(uart3_rec_data[to] != 'e' || uart3_rec_data[to + 1] != 'n' || uart3_rec_data[to + 2] != 'd')	// 包尾
					continue;
				else{	// 分析数据
					
					DataLen = from + 6;
					cmdIndex = from + 4;
					cmdStart = from + 7;
					if(uart3_rec_data[DataLen] != to - from + 3){	// 确定命令的长度格式是否正确
						bleRecvError(HAND);
					}
					else{
						bleRecvState.recState = SUC;
						switch (uart3_rec_data[cmdIndex])	// 判断命令类型
						{
							case HAND:	// 握手
								rec = 1;	// 数据正确
							//	bleSendData(HAND, NULL, 1, bleRecvState.recState);
								bleSendData(HAND, NULL, 1, bleRecvState.recState);
								beep100msBZ();
								break;
							case ENREAD:	// 允许单片机发送数据位
								rec = 1;	// 收到数据								
								if(uart3_rec_data[cmdStart] == 1 || uart3_rec_data[cmdStart] == 0){
									bleState.enSend = uart3_rec_data[cmdStart];
									bleSendData(SENDSTATE, &bleState.enSend, 1, bleRecvState.recState);		
								}
								else{
									bleRecvError(ENREAD);
								}
								break;
							case MOREINFO:	// 显示更多信息
								rec = 1;
								if(uart3_rec_data[cmdStart] == 0 || uart3_rec_data[cmdStart] == 1){
									bleState.moreInfor = uart3_rec_data[cmdStart];
									bleSendData(MOREINFO, &bleState.moreInfor, 1, bleRecvState.recState);
								}
								else{
									bleRecvError(MOREINFO);
								}
								break;
							case SETINDYFRE :	// 设置频率
								rec = 1;
								fredata = (uart3_rec_data[cmdStart + 2] << 8 | uart3_rec_data[cmdStart + 3]);
								
								if(fredata >= 820 && fredata <= 960){								
									bleState.fre[0] = uart3_rec_data[cmdStart];	// 通道
									bleState.fre[1] = uart3_rec_data[cmdStart + 1];		// 开关
									bleState.fre[2] = uart3_rec_data[cmdStart + 2];		// 频率
									bleState.fre[3] = uart3_rec_data[cmdStart + 3];
									indySetFre(bleState.fre[0],bleState.fre[1], (bleState.fre[2] << 8 | bleState.fre[3]) );
									bleSendData(SETINDYFRE, bleState.fre, 4, bleRecvState.recState);
								}
								else{
									bleRecvError(SETINDYFRE);
								}
								break;
							case SETINDYPOWER:	// 设置功率
								rec = 1;
								powdata = (uart3_rec_data[cmdStart] << 8 | uart3_rec_data[cmdStart + 1]);
								if(powdata > 0 && powdata <= 300){
									
									bleState.power[0] = uart3_rec_data[cmdStart];
									bleState.power[1] = uart3_rec_data[cmdStart + 1];
									indySetPower(bleState.power[0] << 8 | bleState.power[1]);
									bleSendData(SETINDYPOWER, bleState.power, 2, bleRecvState.recState);
								}
								else{
									bleRecvError(SETINDYPOWER);
								}
								break;
							case SETSTATUS:	// 进入设置界面
								rec = 1;
								bleSendData(SETINDYFRE, bleState.fre, 4, bleRecvState.recState);
								bleSendData(SETINDYPOWER, bleState.power, 2, bleRecvState.recState);
								break;
								
							default:	// 目前没有的命令								
								bleRecvError(BLANK);
								break;
						}
					}
				}
			}
			from = to + 3;	// 继续读取数据，判断后面是否还有完整的包
		}
	}
//	SendData4(rec);
	if(rec == 1)		// 如果只接受到一部分的数据，先不清除数据，等到下一次再重新分析
		bleClearRecData();
	return rec;	
}
/*
void bleDoCmd()
{
	switch(bleRecvState.cmd){
		case HAND :
			bleSendData(HAND, &bleRecvState.hand, 1, bleRecvState.recState);
			break;
		case ENREAD:
			bleState.enSend = bleRecvState.enSend;
			bleSendData(SENDSTATE, &bleRecvState.enSend, 1, bleRecvState.recState);			
			break;
		case MOREINFO:	// 显示更多信息
			bleState.moreInfor = bleRecvState.enMoreInfo;
			bleSendData(MOREINFO, &bleState.moreInfor, 1, bleRecvState.recState);
			break;
		case SETINDYFRE :	// 设置频率
		
			bleSendData(SETINDYFRE, bleRecvState.setIndyFreq, 4, bleRecvState.recState);
			break;
		case SETINDYPOWER:	// 设置功率
			bleSendData(SETINDYPOWER, bleRecvState.setIndyPower, 2, bleRecvState.recState);
			break;
		case SETSTATUS:	// 进入设置界面
			bleSendData(SETINDYFRE, bleState.fre, 4, bleRecvState.recState);
			bleSendData(SETINDYPOWER, bleState.power, 2, bleRecvState.recState);
			break;
		case ERR:
			bleSendData(bleRecvState.errNum, NULL, 0, bleRecvState.recState);
			break;
		default;
			break;
	}
	bleRecvState.cmd = BLANK;
	
	if(bleRecvState.hand == 1){	// 握手
		bleSendData(BLANK, &bleRecvState.hand, 1, bleRecvState.recState);
		bleRecvState.hand = 0;
	}

	if(bleRecvState.setEnSendState == 1){	// 状态设置
		bleState.enSend = bleRecvState.enSend;
		bleSendData(SENDSTATE, &bleRecvState.enSend, 1, bleRecvState.recState);
		bleRecvState.setEnSendState = 0;
	}

	if(bleRecvState.setMoreInfo == 1){	// 更多信息状态
		bleState.moreInfor = bleRecvState.enMoreInfo;
		bleSendData(MOREINFO, &bleState.moreInfor, 1, bleRecvState.recState);
		//bleRecvState.enMoreInfo = 0;
		bleRecvState.setMoreInfo = 0;
	}

	if(bleRecvState.inSetStatus == 1){		// 进入设置界面
	
		bleSendData(SETINDYFRE, bleState.fre, 4, bleRecvState.recState);
		bleSendData(SETINDYPOWER, bleState.power, 2, bleRecvState.recState);
		bleRecvState.inSetStatus = 0;
	}

	
	if(bleRecvState.setIndyFreq[3] > 0){	// 设置频率

		bleSendData(SETINDYFRE, bleRecvState.setIndyFreq, 4, bleRecvState.recState);
		memset(bleRecvState.setIndyFreq, 0, sizeof(bleRecvState.setIndyFreq));
	}

	if(bleRecvState.setIndyPower[1] > 0){	// 设置功率
	
		bleSendData(SETINDYPOWER, bleRecvState.setIndyPower, 2, bleRecvState.recState);
		bleRecvState.setIndyPower[0] = 0;
		bleRecvState.setIndyPower[1] = 0;
	}	

	if(bleRecvState.recState == ERR){	// 接受到的状态错误
		bleSendData(bleRecvState.errNum, NULL, 0, bleRecvState.recState);
		bleRecvState.recState = 0;
		bleRecvState.errNum = 0;
		return;
	}
	
	return;
}
*/

void bleRecvError(unsigned char errNum)
{	
	bleSendData(errNum, NULL, 0, ERR);
}

