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
	
	beep200msBZ();	// �ϵ�����

	UartInit();		// Uart1 (for indy)	//115200bps@22.1184MHz
	Uart2Init();	// Uart2 (for BT)		//115200bps@22.1184MHz
	Uart3Init(); 	//	115200bps@22.1184MHz
	Uart4Init();	// Uart4 (for debug) //115200bps@22.1184MHz
	
	batteryInit();	// ��ʼ����ԴоƬ
	
	InitOled();		// ��ʼ��OLED
	
	OledPowerUp();		// ����OLED

	ble_init();	// Ĭ��͸��ģʽ
	
	//t0IntInit();	// t0��ʼ��


	OledFlush((unsigned char *)HM);
	//OledWriteMessage57("ABCDEabcdefghijklmnopqrstuvwxyz BT TEST 0123456498751321");

	//OledWriteWordByHex57(3,0,'s');
	//OledWriteMessageByHex57("hello world\n");
	//OledWeakUpDownMode(9);	// ����˯��ģʽ
	/*
	while(1){
		Delay200ms();
		SendData(0xff);
		Delay200ms();
		SendData(0x00);
		i = 0;
		while(rec_num){
			SendData(rec_data[i]);
			i++;
			rec_num--;
		}
	}*/
	
	
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
	
	
	
	
	// ------------------- indy ----------------------------------
  //  WDT_CONTR |= 0X37;//ʹ�ܿ��Ź���128��Ƶ��ι��ʱ���Լ2.6S.
/*
	clear_uart2_data();
	clear_uart2_mes();
	clear_rec_data();
	clear();
	 //�Ķ�������Ԥ�衣Ԥ������йرմ���2�жϡ�Ԥ����ɺ��ٴ򿪡�
	index_send(1);
	
	Delay20ms();
 
	if(index_r_check(1) == 0)
	{
		WriteTestMessage57("connect OK ");
	}
	else
		WriteTestMessage57("connect error ");
	
	for(i = 1;i <= 33;i++)
	{
        clear_rec_data();
        index_send(i);
        Delay20ms();    
    }
	
	while(1)
		{	
		 	Delay100ms();
			keyScanResult = keyScan();
			
	    	clear_uart2_mes();
			
	        WriteMessageByHex57(rec_data);
	      
			for(send_times=0;send_times<5;send_times++)
			{
				clear_rec_data();
	            switch(keyScanResult)
	            {
	                case 1:  
	                    rec_flag=0;
	                    indy_readtemp();//temp
	                    break;

	                case 2: 
	                    rec_flag=0;
	                    indy_read_sensorcode();//sensor code
	                    break;

	                case 3: 
	                    rec_flag=0;
	                    indy_readrssi();//rssi
	                    break;

	               default:
	                    break;
	            }  
	            //beep = 0;
			}
	        
	        if(keyScanResult)
	        {
	            
	            if(rec_flag_cnt > 0)
	            {
	                rec_flag   = 1;
	                rec_flag_cnt = 0;
	            }
	 
	            if(rec_flag)
	    	    {

	                //SendData2(0xee);
	                uart2_sendMes();
	              //  clear_uart2_data();
	              //  clear_uart2_mes();
	            }
	            else
	            {
	                fail_cnt++;
	                if(fail_cnt >= 30)
	                    return 0;
	                uart2_body.cmd_type = 0;
	                //SendData2(0xcc);
	                uart2_sendMes();
	             //   clear_uart2_data();
	             //   clear_uart2_mes();
	            } 
	        }
        
		}
*/
	
	// ----------------------- ���� -----------------------------
//		Delay100ms();
//		keyScanResult = keyScan();
//		if(keyScanResult != S0_NO_INPUT)
//			System_Power_Vcc_ON_OFF = !System_Power_Vcc_ON_OFF;
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		
	// ------------------------ ����� -----------------------------
		
//		checkCharging();
//		Delay999ms();	
		
		
		//SendString4("Start 1.23 \n");
	
	// ---------------------------- ���� ---------------------------------
		
//		SendString2("BT test \n ");
//		Delay999ms();
//		BT_OPEN;
//		WEEK_UP_BT;
//		TRAN_MODE_BLE_IN;	// Ĭ��͸��ģʽ
//		
//		Delay999ms();
//		BT_CLOSE;
//		SLEEP_BT;
//		CMD_MODE_BLE_IN;		clear_uart2_data();
		
//		keyScanResult = keyScan();	// ɨ�谴��
//		switch(keyScanResult){	// ��������
//			case S0_NO_INPUT:
//				break;
//			case S1_TUCH:
//				if(uart2_rec_cnt > 0){
//				Delay10ms();
//				clear();
//				
//				SendString4(uart2_rec_data);
//				WriteTestMessage57(uart2_rec_data);
//				}
//				break;
//			case S2_KEYP:
//				CMD_MODE_BLE_IN;
//				break;	
//			case S3_KEYM:
//				TRAN_MODE_BLE_IN;	// Ĭ��͸��ģʽ
//				break;
//			case S4_MODE:
//				SendString2("BT test \n ");
//				break;
//			case S5_ENTER:
//				clear();
//				WriteTestMessage(" BT TEST ");
//				SendString4("Debug test \n ");
//				break;
//			default:
//				SendString4("error \n ");
//				break;
//		}
//				
//		if(uart2_rec_cnt > 0){
//			Delay10ms();
//			clear();
//			
//			SendString4(uart2_rec_data);
//			WriteMessageByHex57(uart2_rec_data);
//		}
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	// -----�������ģʽ(����) 5ms����һ��	10 * 0.488ms ----------------
//		P51  = 1;	// ���ڼ��CPUʹ����
//		InPowerDownMode;
//		P51 = 0;
	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	// ------------------------------ �¶� ���� --------------------------------	

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
//		WriteTestMessage57(tempStr);
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
	//------------------ ��ѯ�¼� -----------------------------------------
//		keyScanResult = keyScan();	// ɨ�谴��
//		switch(keyScanResult){	// ��������
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
	
	

//	}
		
	
	

//	P51 = 1;
	return 0;

}

