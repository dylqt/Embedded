#include "keyScan.h"


unsigned char mode_flag_t = 0;

unsigned char keyScan(void)
{
	unsigned char keyValue = 0;
	unsigned char keyValue1 = 0;
	unsigned char keyValue2 = 0;
	keyValue = read_adc256(3);

	if(keyValue >= 205){
		return S0_NO_INPUT;
	}
	else{
		Delay10ms();
		keyValue1 = read_adc256(3);
		Delay10ms();
		keyValue2 = read_adc256(3);
		keyValue = (keyValue1 + keyValue2) / 2;

		while(1)	// �ȴ������ſ�
		{	
			Delay10ms();
			if(read_adc256(3) > 205)
				break;
			else
				SendData(read_adc256(3));
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
	keyValue = read_adc256(3);
		if(keyValue >= 205)		//���û�а������£������ȴ�
			{
				Delay10ms();	// ÿ10ms���һ��
				SendString("NO : ");
				SendData(0xff);
				SendData(keyValue);
	
				SendString4("NO : ");
				SendData(keyValue);
				PWMCR &= ~0x20;					//�ر�PWM7�ź����

				return S0_NO_INPUT;
			}
		else{
			Delay50ms();
			keyValue1 = read_adc256(3);
			Delay10ms();
			keyValue2 = read_adc256(3);	

			keyValue = (keyValue1 + keyValue2) / 2;
		
			if(keyValue < 18){	    // S1 : tuch	0
				SendString("S1 : ");
				SendData(0xff);
				SendData(keyValue);
	
				SendString4("S1 : ");
				SendData(keyValue);
	
				GetPwmBZ(1, 60);
				
				return S1_TUCH;
			}else if(keyValue < 59){	// S2 : key+ 	36
				SendString("S2 : ");
				SendData(0xff);
				SendData(keyValue);
	
				SendString4("S1 : ");
				SendData(keyValue);
	
				GetPwmBZ(2, 60);
	
				return S2_KEYP;
			}else if(keyValue < 105){ // S3 : key-	82
				SendString("S3 : ");
				SendData(0xff);
				SendData(keyValue);
				
				SendString4("S3 : ");
				SendData(keyValue);
	
				GetPwmBZ(3, 60);
	
				return S3_KEYM;
			}else if(keyValue < 155){ // S4 : Mode	126
				SendString("S4 : ");
				SendData(0xff);
				SendData(keyValue);
	
				SendString4("S3 : ");
				SendData(keyValue);
	
				GetPwmBZ(4, 60);
	
				return S4_MODE;
			}else if(keyValue < 205){ // S5 : Enter	185
				SendString("S5 : ");
				SendData(0xff);
				SendData(keyValue);
	
				SendString4("S3 : ");
				SendData(keyValue);
				GetPwmBZ(0, 60);	// if  keyValue > 205 means no Key input 
				
				return S5_ENTER;
			}
			else
				return S0_NO_INPUT;
		}
}



void pageScan(unsigned char key)
{
	unsigned dealkey = key;
START:
	switch(mode_flag_t)
	{
		case 0 :	// ���˵�
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
			

//--------------- ��һҳ -------------------------------			

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


// --------------TEST��MODE -------------------------------------------

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
					System_Power_Vcc_ON_OFF = !System_Power_Vcc_ON_OFF; 	//��һ�� key+  ȡ��  SystemPowee_Vcc_ON_OFF��ƽ��
					if(System_Power_Vcc_ON_OFF == 1){	//��SystemPowee_Vcc_ON_OFFΪ�ߵ�ƽʱ ���ӳ�100ms �� PG ��
						Delay100ms();
						if(PG == 1){
							OledClear();
							OledWriteMessage(" U14 power good ");
							SendString4(" U14 power good \n ");	//��Ϊ�ߵ�ƽ DB����� "U14 power good "; 
						}
						else{
							OledClear();
							OledWriteMessage(" U14 power NG OCP OTP ");
							SendString4(" U14 power NG OCP OTP \n ");  //��Ϊ�͵�ƽ��OLED��ʾNG������ DB����� "U14 power NG   OCP   OTP ";
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
					OledWriteMessage(" BATTERY��MODE IN ");
					SendString4(" BATTERY��MODE IN \n ");
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

