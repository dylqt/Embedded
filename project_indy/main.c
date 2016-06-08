#include "stc15_global.h"
#include "beep.h"		
#include "keyScan.h"
#include "power.h"
#include "ble.h"
#include "stdio.h"
#include "string.h"
#include "indy.h"

struct RESULT result;

int main()
{
	unsigned char keyScanResult = 0;
	unsigned char i = 0;
	unsigned char j = 0;
	double temp = 0;
	unsigned char bat = 0;
	// unsigned char tempStr[60];
	
	unsigned char fail_cnt = 0;
	unsigned char send_times = 0;
	unsigned char rec_flag = 0;
	unsigned char rec_flag_cnt = 0;
	
	beep200msBZ();	// 上电提醒

	UartInit();		// Uart1 (for indy)	//115200bps@22.1184MHz
	Uart2Init();	// Uart2 (for BT)		// 9600bps@22.1184MHz
	Uart3Init(); 	//	115200bps@22.1184MHz
	Uart4Init();	// Uart4 (for debug) //115200bps@22.1184MHz
	
	batteryInit();	// 初始化电源芯片
	
	InitOled();		// 初始化OLED
	
	OledPowerUp();		// 开启OLED

	ble_init();	// 默认透传模式
	BLE_CMD_MODE_IN;
	
	//t0IntInit();	// t0初始化

	//Delay999ms();
	// OledFlush((unsigned char *)HM);
	OledFlushBaseState();
	OledBatteryPower(97);
	OledBatteryState(3);
	OledBtConnectState(1);
	OledChargingState(1);
//	OledWriteWordByHex(1,0,sizeof(buf));
	//OledWeakUpDownMode(9);	// 启动睡眠模式
	
//	while(1){	
//		Delay999ms();
		//ble_inquire_baud();
//		ble_set_baud(2);
		//SendString("hello \n ");
		//SendString4("hello \n");
//		i = 0;
//		j = 0;
//		while(rec_num){
//			SendData(rec_data[i]);			
//			i++;
//			rec_num--;
//		}
//		while(uart4_rec_cnt){
//			SendData4(uart4_rec_data[j]);
//			j++;
//			uart4_rec_cnt--;
//		}
//	}
	
	/*
	while(1)
	{
		Delay100ms();
		keyScanResult = keyScan();
		switch(keyScanResult)
		{
			case S1_TUCH:  
				//OledExitingSleepMode();
				OledWriteMessage57("exiting sleep mode");
				break;
			case S2_KEYP: 
				break;
			case S3_KEYM: 				
				OledWriteMessage57("entering sleep mode" );
				//OledEnteringSleepMode();
				break;
			

		 default:
					break;
		}  
	}
	*/
	
	
	
	// ------------------- indy ----------------------------------
  //  WDT_CONTR |= 0X37;//使能看门狗。128分频，喂狗时间大约2.6S.
//	
//	clear_uart2_data();
//	clear_uart2_mes();
//	clear_rec_data();
//	OledClear();
//	 //阅读器参数预设。预设过程中关闭串口2中断。预设完成后再打开。
//	index_send(1);
//	
//	Delay20ms();
// 
//	if(index_r_check(1) == 0)
//	{
//		OledWriteMessage57("connect OK ");
//	}
//	else
//		OledWriteMessage57("connect error ");

//	Delay999ms();
//	for(i = 1;i <= 33;i++)
//	{
//        clear_rec_data();
//        index_send(i);
//        Delay20ms();    
//    }
//	
//	while(1)
//		{	
//		 	// Delay100ms();
//			//keyScanResult = keyScan();
//			
//	    // clear_uart2_mes();
//	    	Delay999ms();
//			keyScanResult = S3_KEYM;
//	    	OledClear();
//	    	clear_rec_data();  
//			for(send_times=0;send_times<5;send_times++)
//			{
//				
//	            switch(keyScanResult)
//	            {
//	                case S1_TUCH:  
//	                    rec_flag=0;
//	                    indy_readtemp();	//temp
//	                    break;

//	                case S2_KEYP: 
//	                    rec_flag=0;
//	                    indy_read_sensorcode();	//sensor code
//	                    break;

//	                case S3_KEYM: 
//	                    rec_flag=0;
//	                    indy_readrssi();	//rssi
//	                    break;

//	               default:
//	                    break;
//	            }  
//	            //beep = 0;
//			}
//			/*
//			if(rec_num > 0){
//				Delay999ms();
//	      		OledWriteMessageByHex57(rec_data, rec_num);
//				beep200msBZ();
//			}
//			else
//				OledWriteMessage57("NO MESSAGE");
//				*/
//		
//	        if(keyScanResult)
//	        {
//	            
//	            if(rec_flag_cnt > 0)
//	            {
//	                rec_flag   = 1;
//	                rec_flag_cnt = 0;
//	            }
//	 
//	            if(rec_flag)
//	    	    {

//	                //SendData2(0xee);
//	                uart2_sendMes();
//	              //  clear_uart2_data();
//	              //  clear_uart2_mes();
//	            }
//	            else
//	            {
//	                fail_cnt++;
//	                if(fail_cnt >= 30)
//	                    return 0;
//	                uart2_body.cmd_type = 0;
//	                //SendData2(0xcc);
//	                uart2_sendMes();
//	             //   clear_uart2_data();
//	             //   clear_uart2_mes();
//	            } 
//	        }
//	    }
	

	
	// ----------------------- 按键 -----------------------------
//		Delay100ms();
//		keyScanResult = keyScan();
//		if(keyScanResult != S0_NO_INPUT)
//			System_Power_Vcc_ON_OFF = !System_Power_Vcc_ON_OFF;
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		
	// ------------------------ 充电检测 -----------------------------
		
//		checkCharging();
//		Delay999ms();	
		
		
		//SendString4("Start 1.23 \n");
	
	// ---------------------------- 蓝牙 ---------------------------------
		
//		SendString2("BT test \n ");
//		Delay999ms();
//		BT_OPEN;
//		WEEK_UP_BT;
//		TRAN_MODE_BLE_IN;	// 默认透传模式
//		
//		Delay999ms();
//		BT_CLOSE;
//		SLEEP_BT;
//		CMD_MODE_BLE_IN;		clear_uart2_data();
		
//		keyScanResult = keyScan();	// 扫描按键
//		switch(keyScanResult){	// 处理按键
//			case S0_NO_INPUT:
//				break;
//			case S1_TUCH:
//				if(uart2_rec_cnt > 0){
//				Delay10ms();
//				OledClear();
//				
//				SendString4(uart2_rec_data);
//				OledWriteMessage57(uart2_rec_data);
//				}
//				break;
//			case S2_KEYP:
//				CMD_MODE_BLE_IN;
//				break;	
//			case S3_KEYM:
//				TRAN_MODE_BLE_IN;	// 默认透传模式
//				break;
//			case S4_MODE:
//				SendString2("BT test \n ");
//				break;
//			case S5_ENTER:
//				OledClear();
//				OledWriteMessage(" BT TEST ");
//				SendString4("Debug test \n ");
//				break;
//			default:
//				SendString4("error \n ");
//				break;
//		}
//				
//		if(uart2_rec_cnt > 0){
//			Delay10ms();
//			OledClear();
//			
//			SendString4(uart2_rec_data);
//			OledWriteMessageByHex57(uart2_rec_data);
//		}
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	// -----进入掉电模式(休眠) 5ms唤醒一次	10 * 0.488ms ----------------
//		P51  = 1;	// 用于检测CPU使用率
//		InPowerDownMode;
//		P51 = 0;
	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	// ------------------------------ 温度 电量 --------------------------------	

//		Delay999ms();SendData(0xff);Delay999ms();SendData(0x00);
//		
//		memset(tempStr, 0, sizeof(tempStr));
//		
//		temp = tempScan();
//			
//		bat = getBatteryPercent();
//		sprintf(tempStr, "temp = %.2f    BAT = %d%%", temp, (int)bat);
//		SendData4(0xcc);
//		SendData4(bat);
//		SendString4(tempStr);
//		OledWriteMessage57(tempStr);
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
	//------------------ 查询事件 -----------------------------------------
//		keyScanResult = keyScan();	// 扫描按键
//		switch(keyScanResult){	// 处理按键
//			case S0_NO_INPUT:
//				break;
//			case S1_TUCH:
//				SendString4("S1 \n ");
//				break;
//			case S2_KEYP:
//				SendString4("S2 \n ");
//				pageScan(keyScanResult);
//				break;
//			case S3_KEYM:
//				SendString4("S3 \n ");
//				pageScan(keyScanResult);	
//				break;
//			case S4_MODE:
//				
//				SendString4("S4 \n ");
//				mode_flag_t = 0;
//				pageScan(S0_NO_INPUT);
//				break;
//	
//			case S5_ENTER:
//				pageScan(keyScanResult);	
//				break;
//			default	:
//				SendString4("ERROR \n ");
//				//pageScan(keyScanResult, mode_flag_t);
//				break;
//				
//		}
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^								
	
	// --------------------- 业务 ---------------------------------

	OledWriteWordByHex(3,0,WIRC_H);
	OledWriteWordByHex(3,16,WIRC_L);
	//WKTCL = 100;
	// WKTCH = 0;
	//WKTCH = 0x80;
//	WeakUpDownMode(2000);
	while(1){
//		OledWriteMessage("Sleep");
//		
//		
//		InPowerDownMode;			
//		OledWriteMessage("Weekup");
//		Delay999ms();
		

	}
		
	
	

//	P51 = 1;
	return 0;

}


