#include "stc15_global.h"
#include "beep.h"		
#include "keyScan.h"
#include "power.h"
#include "BLE.h"
#include "stdio.h"
#include "string.h"
#include "indy.h"

// struct RESULT result;

int main()
{
	unsigned char keyScanResult = 0;	// �������
	unsigned char i = 0;
	unsigned char j = 0;

	unsigned char batCnt = 0;
	int batteryValue = 0;
	
//	unsigned char id[12] = {1,2,3,4,5,6,7,8,9,'a','b','c'};
//	unsigned char temp[2] = {31, 23};
//	unsigned char sen[2] = {12, 13};
//	unsigned char rssi[2] = {13, 14};
		
//	unsigned char fail_cnt = 0;
	unsigned char send_times = 0;

	// enum BLE_CMD blecmd;
	beep200msBZ();	// �ϵ�����

	UartInit();		// Uart1 (for indy)	//115200bps@22.1184MHz
	Uart2Init();	// Uart2 	// 9600bps@22.1184MHz
	Uart3Init(); 	// Uart3 (for BT)	9600bps@22.1184MHz
	Uart4Init();	// Uart4 (for debug) //115200bps@22.1184MHz
	
	batteryInit();	// ��ʼ����ԴоƬ
		
	InitOled();		// ��ʼ��OLED
	
	OledPowerUp();		// ����OLED

	ble_init();	// Ĭ��͸��ģʽ
	//BLE_CMD_MODE_IN;
	
	//t0IntInit();	// t0��ʼ��
	// OledClear();
	//Delay999ms();
	OledFlush((unsigned char *)HM);


//	OledFlushBaseState();
//	OledBatteryPower(97);
//	OledBatteryState(3);
//	OledBtConnectState(1);
//	OledChargingState(1);

//	OledWriteWordByHex(1,0,sizeof(buf));
	//OledWeakUpDownMode(9);	// ����˯��ģʽ
	
	// ------------------- ���ȵ��� ---------------------------	
/*	
	while(1)
	{
		Delay100ms();
		checkCharging();
		keyScanResult = keyScan();
		if(keyScanResult != S0_NO_INPUT){
			//i += 5;
			//GetPwmBRT(i);
			OledWriteWordByHex(1, 0, OLED_BRT_DUTY);
			OledBrtAdjust(1);
			
		}
		if(OLED_BRT_DUTY == 90)
			OLED_BRT_DUTY = 5;		  
	}
*/

	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	
	// ------------------- indy ----------------------------------
  

	
	clear_rec_data();
	OledClear();
//	Delay20ms();
	OledWriteMessage57("connect... ");
	indyInitCh875();
	OledClear();
	//OledAssicForState(0,90,'*');	
	while(1)
		{	
			//indySetBD(1001, 1002, 1003, 1004, 0);
			// WDT_CONTR |= 0X37;//ʹ�ܿ��Ź���128��Ƶ��ι��ʱ���Լ2.6S.
			bleParseData();	// �������յ������ݽ��н���
			
			clear_mes_body();
		 	Delay50ms();
			//getBatteryPercent();
			//batteryScan();

			
			// ���͵�����Ϣ������
			batCnt++;
			if(batCnt >= 20){	// 50ms * 20 = 1s;����һ�ε���
				batCnt = 0;		
				if(bleState.moreInfor == 1){							
					 bleState.battery[0] = batteryCharge;	// ���״̬�����P07
					 bleState.battery[1] = getBatteryPercent();	// �õ������İٷֱ�
					bleSendData(BATTERY, bleState.battery, 2, SUC);
				}
			}
			
			// ����ɨ��
			keyScanResult = keyScan();
	 		
			if(keyScanResult == S0_NO_INPUT){ // ��һ��ʱ�����OLED
				if(i < 80){
					i++;
					continue;
				}
				else{
					i = 0;
					OledClear();
				}	
			}
			else if(keyScanResult == S4_MODE){	// ��
				OledPowerDown();
				death_ctron = 0;		
				System_Power_Vcc_ON_OFF = 0;
				}
			else if(keyScanResult == S5_ENTER){	// ��
				if(death_ctron == 0){
					System_Power_Vcc_ON_OFF = 1;
					death_ctron = 1;	
					ble_init();	// Ĭ��͸��ģʽ
					beep200msBZ();	// ������ʾ
					OledPowerUp();	
					OledClear();
					OledWriteMessage57("init... ");
					indyInitCh875();
					OledClear();	
				}
			}else{			
					//clear_rec_data();
					for(send_times = 0; send_times < 5; send_times++){
						switch(keyScanResult)
						{
								case S1_TUCH: 
									if(bleState.moreInfor == 1)
										indy_readall();		// ������
									else
										{
										indyGetBD();
										indy_readtemp();	//temp
										}
										break;

								case S2_KEYP: 
										indy_read_sensorcode();	//sensor code
										break;

								case S3_KEYM: 
										indy_readrssi();	//rssi
										break;

							 	default:
										break;
						}  
					}
				if(keyScanResult && rec_num == 0)
					OledAssicForState(0,90,'*');
				else
					OledAssicForState(0,90,' ');
			}	

	    }

	
	// ----------------------- ���� -----------------------------
/*	while(1){
		
		Delay100ms();
		keyScanResult = keyScan();
		
		if(keyScanResult != S0_NO_INPUT){
			if(System_Power_Vcc_ON_OFF == 1){				
				OledPowerDown();
				CS_OLED = 0;
				DC_OLED = 0;
				death_ctron = 0;
				WR_OLED = 0;
				RD_OLED = 0;
				System_Power_Vcc_ON_OFF = 0;
			}
			else{
				System_Power_Vcc_ON_OFF = 1;
				death_ctron = 1;
				OledPowerUp();				
			}
		}
	}	
*/
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		
	// ------------------------ ����� -----------------------------
		
//		checkCharging();
//		Delay999ms();	

	
	// ---------------------------- ���� ---------------------------------
	/*	
		
		Delay999ms();
		BT_OPEN;
		WEEK_UP_BT;
		BLE_TRAN_MODE_IN;	// Ĭ��͸��ģʽ
		
		Delay999ms();
		BT_CLOSE;
		SLEEP_BT;
		BLE_CMD_MODE_IN;		
		clear_uart2_data();
	*/	

	//SendString4("BT test \n ");
/*
	while(1){
		Delay100ms();
		if(uart3_rec_cnt > 0){
			SendBuf4(uart3_rec_data, uart3_rec_cnt);			
		}
		if( bleParseData() ){
			bleDoCmd();
		//	Delay100ms();
		}
		
		//P50 = 1;
		//P51 = 1;
		//Delay999ms();
		//P50 = 0;
		//P51 = 0;
		//Delay999ms();

//		bleSendData(BLANK, NULL, 1, 0);	
//		bleRecvError();
//		bleSendChar(0xff);
//		bleSendData(ID, id, 12, 0);
		
//		bleSendChar(0xff);
//		bleSendData(TEMP, temp, 2, 0);
//		temp[0]++;
//		temp[1]++;
//		bleSendChar(0xff);
//		bleSendData(SENSORCODE, sen, 2, 0);
//		sen[0]++;
//		sen[1]++;
//		bleSendChar(0xff);
//		bleSendData(RSSI, rssi, 2, 0);
//		rssi[0]++;
//		rssi[1]++;
//		bleSendChar(0xff);
//		Delay999ms();
		
		keyScanResult = keyScan();	// ɨ�谴��
		switch(keyScanResult){	// ������
			case S0_NO_INPUT:
				break;
			case S1_TUCH:
				bleSendData(ID, id, 12, 0);
				bleSendData(TEMP, temp, 2, 0);
				temp[0]++;
				temp[1]++;
			if(temp[0] > 45)
					temp[0] = 20;
			if(temp[1] > 99)
					temp[1] = 0;
				break;
			case S2_KEYP:
				BLE_CMD_MODE_IN;
				break;	
			case S3_KEYM:
				BLE_TRAN_MODE_IN;	// Ĭ��͸��ģʽ
				break;
			case S4_MODE:
				SendString3("BT test \n ");
				break;
			case S5_ENTER:
				OledClear();
				OledWriteMessage(" BT TEST ");
				SendString4("Debug test \n ");
				break;
			default:
				SendString4("error \n ");
				break;
		}
*/				
		/*
		if(uart3_rec_cnt > 0){
			Delay10ms();
			OledClear();
			
			SendString4(uart3_rec_data);
			OledWriteBufByHex57(uart3_rec_data, uart3_rec_cnt);
			bleClearRecData();
		}
		*/
	
//	}
	
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	// -----�������ģʽ(����) 5ms����һ��	10 * 0.488ms ----------------
//		P51  = 1;	// ���ڼ��CPUʹ����
//		InPowerDownMode;
//		P51 = 0;
	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	// ------------------------------ �¶� ���� --------------------------------	
/*	
		while(1){
			//batteryValue = batteryScan();
			//OledWriteDouble(1,0, batteryValue);
			//temp = tempScan();
			//OledWriteDouble(3,0, temp);

			Delay100ms();
			keyScanResult = keyScan();
			if(keyScanResult != S0_NO_INPUT){
				if(System_Power_Vcc_ON_OFF == 1){				
					OledPowerDown();
					System_Power_Vcc_ON_OFF = 0;
				}
				else{
					System_Power_Vcc_ON_OFF = 1;
					OledPowerUp();
					
				}
			}
		}
*/
//		memset(tempStr, 0, sizeof(tempStr));
//		
		
//
//		bat = getBatteryPercent();

//		OledWriteMessage57(tempStr);
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
	//------------------ ��ѯ�¼� -----------------------------------------
//		keyScanResult = keyScan();	// ɨ�谴��
//		switch(keyScanResult){	// ������
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
	
	// --------------------- ҵ�� ---------------------------------

	//OledWriteWordByHex(3,0,WIRC_H);
	//OledWriteWordByHex(3,16,WIRC_L);
	//WKTCL = 100;
	// WKTCH = 0;
	//WKTCH = 0x80;
	//WeakUpDownMode(20408);
/*
	while(1){
		Delay10ms();
		keyScanResult = keyScan();	// ɨ�谴��
		if(keyScanResult != S0_NO_INPUT){
			switch(i){
				case 0 :
					GetPwm4(1000);	// 1MHZ
					i = 1;
					break;
				case 1 :
					GetPwm4(100);	// 100KHZ
					i = 2;
					break;
				case 2 :
					GetPwm4(10);	// 10KHZ
					i = 3;
					break;
				case 3 :
					GetPwm4(10000);	// 10KHZ
					i = 0;
					break;	
				default:
					break;
			}

		}
*/

		//OledWriteMessage("Sleep");
		//InPowerDownMode;
		//OledWriteMessage("Weekup");
		//Delay999ms();
		// Delay999ms();
//		SendData2(0xea);
//		Delay20ms();
//		while(uart2_rec_cnt > 0){
			//SendString2(uart2_rec_data);
			//OledWriteAssic57(1, 0, 0xff);
//			OledWriteBufByHex57(uart2_rec_data, uart2_rec_cnt);
//			clear_uart2_data();
//		} 

//	}
//		
	
	

//	P51 = 1;
	return 0;

}
