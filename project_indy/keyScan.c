#include "keyScan.h"


unsigned char mode_flag_t = 0;

unsigned char keyScan(void)
{
	unsigned char keyValue = 0;
	unsigned char keyValue1 = 0;
	unsigned char keyValue2 = 0;
	keyValue = getKeyAdc();

	if(keyValue >= 205){
		return S0_NO_INPUT;
	}
	else{
		Delay10ms();
		keyValue1 = getKeyAdc();
		Delay10ms();
		keyValue2 = getKeyAdc();
		keyValue = (keyValue1 < keyValue2) ? keyValue1 : keyValue2;
		if(keyValue >= 18){	// 只有TUCH键时支持连续触发
			while(1)	// 等待按键放开
			{	
				Delay10ms();
				if(getKeyAdc() > 205)
					break;
			}
		}
	}
	
	if(keyValue < 18){	    // S1 : tuch	0
		click10msBZ();	
		return S1_TUCH;
	}else if(keyValue < 59){	// S2 : key+ 	36
		click10msBZ();	
		return S2_KEYP;
	}else if(keyValue < 105){ // S3 : key-	82
		click10msBZ();	
		return S3_KEYM;
	}else if(keyValue < 155){ // S4 : Mode	126
		click10msBZ();	
		return S4_MODE;
	}else if(keyValue < 205){ // S5 : Enter	185			 
		click10msBZ();	
		return S5_ENTER;
	}
	else			// if  keyValue > 205 means no Key input
		return S0_NO_INPUT;
	
}


unsigned char keyScanTest(void)
{
	unsigned char keyValue = 0;
	unsigned char keyValue1 = 0;
	unsigned char keyValue2 = 0;
	keyValue = getKeyAdc();
	if(keyValue > 205)
		return S0_NO_INPUT;
	keyValue = 0;
	Delay10ms();
	keyValue1 = getKeyAdc();
	Delay10ms();
	keyValue2 = getKeyAdc();
	keyValue = (keyValue1 < keyValue2) ? keyValue1 : keyValue2;
	
	
//	keyValue = (keyValue1 + keyValue2) / 2;

//	OledWriteDouble(1,1, keyValue);
	
	if(keyValue < 30){	    // S1 : tuch	0
		click10msBZ();
		
		return S1_TUCH;
	}else if(keyValue < 59){	// S2 : key+ 	36
		click10msBZ();

		return S2_KEYP;
	}else if(keyValue < 115){ // S3 : key-	82
		click10msBZ();

		return S3_KEYM;
	}else if(keyValue < 165){ // S4 : Mode	126
		click10msBZ();

		return S4_MODE;
	}else if(keyValue < 205){ // S5 : Enter	185
		click10msBZ();	// if  keyValue > 205 means no Key input 
		
		return S5_ENTER;
	}
	else
		return S0_NO_INPUT;
	
}



void pageScan(unsigned char key)
{
	unsigned dealkey = key;
START:
	switch(mode_flag_t)
	{
		case 0 :	// 主菜单
			switch(dealkey){
				case S2_KEYP:
					break;
				case S3_KEYM:
					break;
				case S5_ENTER:
					mode_flag_t = 5;
					dealkey = S0_NO_INPUT;
					goto START;
				default:
					OledClear();
					OledFlush((unsigned char *)HM);
					SendString4(" MAIN MENU \n ");
					break;
			}
			break;
			

//--------------- 第一页 -------------------------------			

		case 5 :	// UHF mode
			switch(dealkey){
				case S2_KEYP:
					mode_flag_t = 6;
					dealkey = S0_NO_INPUT;
					goto START;
				case S3_KEYM:
					mode_flag_t = 7;
					dealkey = S0_NO_INPUT;
					goto START;
				case S5_ENTER:
					break;
				default:
					OledClear();
					OledWriteMessage(" UHF MODE ");
					SendString4(" UHF MODE \n ");
					break;
			}
			break;
			
		case 6 :	// BT mode
			switch(dealkey){
				case S2_KEYP:
					mode_flag_t = 7;
					dealkey = S0_NO_INPUT;
					goto START;
				case S3_KEYM:
					mode_flag_t = 5;
					dealkey = S0_NO_INPUT;
					goto START;
				case S5_ENTER:
					break;
				default:
					OledClear();
					OledWriteMessage(" BT MODE ");
					SendString4(" BT MODE \n ");	
					break;
			}
			break;
			
		case 7 :	// Test mode
			switch(dealkey){
				case S2_KEYP:
					mode_flag_t = 5;
					dealkey = S0_NO_INPUT;
					goto START;
				case S3_KEYM:
					mode_flag_t = 6;
					dealkey = S0_NO_INPUT;
					goto START;
				case S5_ENTER:
					mode_flag_t = 10;
					dealkey = S0_NO_INPUT;
					goto START;				
				default:
					OledClear();
					OledWriteMessage(" TEST MODE ");
					SendString4(" TEST MODE \n ");	
					break;
			}
			break;


// --------------TEST　MODE -------------------------------------------

		case 10 :	// battery 
			switch(dealkey){
				case S2_KEYP:
					mode_flag_t = 11;
					dealkey = S0_NO_INPUT;
					goto START;
				case S3_KEYM:
					mode_flag_t = 13;
					dealkey = S0_NO_INPUT;
					goto START;
				case S5_ENTER:
					mode_flag_t = 15;
					dealkey = S0_NO_INPUT;
					goto START;
				default:
					OledClear();
					OledWriteMessage(" BATTERT MODE ");
					SendString4(" BATTERY MODE \n ");
					break;
			}
			break;
			
		case 11 :	// BZ 
			switch(dealkey){
				case S2_KEYP:
					mode_flag_t = 12;
					dealkey = S0_NO_INPUT;
					goto START;
				case S3_KEYM:
					mode_flag_t = 10;
					dealkey = S0_NO_INPUT;
					goto START;
				case S5_ENTER:
					mode_flag_t = 20;
					dealkey = S0_NO_INPUT;
					goto START;
				
				default:
					OledClear();
					OledWriteMessage(" BZ MODE ");
					SendString4(" BZ MODE \n ");
					break;
			}
			break;

		case 12 :	// OLED_timing 
			switch(dealkey){
				case S2_KEYP:
					mode_flag_t = 13;
					dealkey = S0_NO_INPUT;
					goto START;
				case S3_KEYM:
					mode_flag_t = 11;
					dealkey = S0_NO_INPUT;
					goto START;
				case S5_ENTER:
					mode_flag_t = 25;
					dealkey = S0_NO_INPUT;
					goto START;
				
				default:
					OledClear();
					OledWriteMessage(" OLED_timing set ");
					SendString4(" OLED_timing set \n ");
					break;
			}
			break;

		case 13 :	// OLED_BZ 
			switch(dealkey){
				case S2_KEYP:
					mode_flag_t = 10;
					dealkey = S0_NO_INPUT;
					goto START;
				case S3_KEYM:
					mode_flag_t = 12;
					dealkey = S0_NO_INPUT;
					goto START;
				case S5_ENTER:
					mode_flag_t = 30;
					dealkey = S0_NO_INPUT;
					goto START;
				
				default:
					OledClear();
					OledWriteMessage(" OLED_BZ set ");
					SendString4(" OLED_BZ set \n ");
					break;
			}
			break;

//----------------- BATTERY MODE ----------------------------------------------------

		case 15 :
			
			switch(dealkey){
				case S2_KEYP:
					SendString4(" !System_Power_Vcc_ON_OFF \n ");
					System_Power_Vcc_ON_OFF = !System_Power_Vcc_ON_OFF; 	//按一次 key+  取反  SystemPowee_Vcc_ON_OFF电平；
					if(System_Power_Vcc_ON_OFF == 1){	//若SystemPowee_Vcc_ON_OFF为高电平时 ；延迟100ms 读 PG ；
						Delay100ms();
						if(PG == 1){
							OledClear();
							OledWriteMessage(" U14 power good ");
							SendString4(" U14 power good \n ");	//若为高电平 DB口输出 "U14 power good "; 
						}
						else{
							OledClear();
							OledWriteMessage(" U14 power NG OCP OTP ");
							SendString4(" U14 power NG OCP OTP \n ");  //若为低电平；OLED显示NG，至少 DB口输出 "U14 power NG   OCP   OTP ";
						}
					}
					break;
				case S3_KEYM:
					OledClear();
					OledWriteMessage(" N VSEL ");
					SendString4(" !VSEL \n ");
					VSEL = !VSEL;
					break;
				case S5_ENTER:
					OledClear();
					OledWriteMessage(" N BYP ");
					SendString4(" !BYP \n ");
					BYP = !BYP;
					break;
				default:
					OledClear();
					OledWriteMessage(" BATTERY　MODE IN ");
					SendString4(" BATTERY　MODE IN \n ");
					break;
			}
			break;



//----------------- BZ MODE ----------------------------------------------------

		case 20 :
			OledClear();
			OledWriteMessage(" BZ MODE IN ");
			SendString4(" BZ MODE IN \n ");
			break;

// ------------------- OLED_timing set ---------------------------------

		case 25 :	// OLED_timing 
			switch(dealkey){
				case S2_KEYP:
					OledClear();
					OledWriteMessage(" OLED get reading timing ");
					SendString4(" OLED_timing get reading timing \n ");
					break;
				case S3_KEYM:
					OledClear();
					OledWriteMessage(" OLED get writing timing ");
					SendString4(" OLED_timing get writing timing \n ");
					break;
				case S5_ENTER:
					break;
				default:
					OledClear();
					OledWriteMessage(" OLED_timing set IN ");
					SendString4(" OLED_timing set IN \n ");
					break;
			}
			break;	


// ---------------- OLED_BZ set -------------------------------------
		case 30 :	// OLED_timing 
			switch(dealkey){
				case S2_KEYP:
					OledClear();
					OledWriteMessage(" OLED_BZ up ");
					SendString4(" OLED_BZ up \n ");
					break;
				case S3_KEYM:
					OledClear();
					OledWriteMessage(" OLED_BZ down ");
					SendString4(" OLED_BZ down \n ");
					break;
				case S5_ENTER:
					break;
				default:
					OledClear();
					OledWriteMessage(" OLED_BZ set IN ");
					SendString4(" OLED_BZ set IN \n ");
					break;
			}
			break;	

			
// --------------------------------------------------------
		default :
			OledClear();
			OledWriteMessage(" NOT SET ");
			SendString4(" NOT SET \n ");
			break;


	}
}

