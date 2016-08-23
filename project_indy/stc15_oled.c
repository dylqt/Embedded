#include "stc15_oled.h"

void InitOled()
{
	//	6.1  CS# 	Ƭѡ  ���� ǿ�����
	//	6.2  D/C#  ���ݻ����� ѡ��	  ���� ǿ�����
	//	6.3  E/RD#	��дʹ�� ���	  ���� ǿ�����
	//	6.4  R/W#	��дѡ�� �� д	  ���� ǿ�����
	P2M1 = 0x00;
	P2M0 = 0xff;	

// Ӳ���õ�	
//	BS1_OLED = 0; 
//	BS2_OLED = 0;	//BS1 BS2  ���� ǿ��� �������� ��ѡ�� 80xx ���ߡ�  00 ѡ��spi
	
	//6.5  D0~D7	��������   д  ���� ǿ����������� ������������衣
	//P7M1 &= ~0x80;	// D0 P7.7
	//P7M0 |= 0x80;
	//P4M1 &= ~0x30;	// D1 P4.5
	//P4M0 |= 0x30;	// D2 P4.6
	//P0M1 &= ~0x1F;// D3 P0.0
	//P0M0 |= 0x1F;// D4 P0.1
	// D5 P0.2
	// D6 P0.3
	// D7 P0.4

	//P2M1 &= ~0x80;	// SDIN P2.7
	//P2M0 |= 0x80;

	P7M1 &= ~0x10;	// SCLK P7.4
	P7M0 |= 0x10;

}

// ------------------------------ģ�� SPI ----------------------------------------------
void uDelay(unsigned char t)
{   
	   while(t--);
}


void WRByte(unsigned char dat,unsigned char bIsCmd) //д��һ�ֽ�,column������
{
		unsigned char i=0x80;	
	   CS_OLED = 0;		// ChipSelected(); 
	   if(bIsCmd == 1) 
	   {
			DC_OLED = 0; 	// CmdInput();
	   }else	 
	   {
			DC_OLED = 1;	// DataInput();
	   }
	   for(;i>0;i>>=1) //OLED���ֽڵĸ�λD7����,����D0 -- SPIģ��
	   {
		   SCLK_OLED = 0; // SCLK.low();	
		   SDIN_OLED = i & dat; // SDIN.set(i&dat); 	   
		   uDelay(15);
		   SCLK_OLED = 1; // SCLK.high(); 	   
		   uDelay(15);					
	   }
	   SCLK_OLED = 0;
	   SDIN_OLED = 1;
	   uDelay(15);
		SCLK_OLED = 1; // SCLK.high(); 
		
		uDelay(15);
	   CS_OLED = 1; 	//ChipUnselected(); 
}		   

void WR1BData(unsigned char dat)
{ 		 
	  WRByte(dat, 0);
} 

void WR1BCmd(unsigned char cmd)
{
	  WRByte(cmd, 1);    
}  


void OledPowerUp(void)
{	
	int i = 0;
	RES_OLED = 0;
	
	 for(;i < 30000;i++);	 //��ʱ��֤�ɿ���ʾ 	
	VDDB_OLED = 1;
	Delay100ms();
	RES_OLED = 1;
	Delay3us();
	Delay3us();
	
	 WR1BCmd(0xae);		//0xAE �ر���ʾ		 
	 //SetMemoryAddressMode(2); 		 //0x20(0x02Ĭ��),��ַ����ģʽ,ҳģʽ��PAGE���Զ���0,ModeSelect://0, Horizontal Addressing Mode ;//1, Vertical Addressing Mode; //2,Page Addressing Mode (default)
	 //SetPageAddressRange(0);
	 //SetColumnAddressRange(0,127);
	 
	 WR1BCmd(0xd5);  WR1BCmd(0x80);		// SetClockDivideRatio(0x80); 		 //0xD5(0x80) Set Clock as 100 Frames/Sec
	 
	 WR1BCmd(0xa8);  WR1BCmd(0x27);		// SetMultiplexRatio(MAX_ROW-1);	 //0xA8(64d=0x3F)��������Ҫ��ʾ������ //PG9639TLBE(SSD1306)	 0xA8(0x27=39D)   
	 WR1BCmd( 0xd3 );	WR1BCmd(0x00);	//SetDisplayVerticalOffset(0);	   //0xD3(00)���������
	 WR1BCmd(0x40);					//SetDisplayStartLine(0);				 //0x40(0),��ƫ��
	
	 WR1BCmd(0x8d);  WR1BCmd(0x14);     //EnableChargePump(true);		//0x8D(0x14),�����ڲ���ѹ�� Enable Embedded DC/DC Converter 
	 WR1BCmd(0xa1); 
	 WR1BCmd(0xc8); 

	 WR1BCmd(0xda);  WR1BCmd(0x12);		// SetCOMPinsHardwareConfig(0x12); 	//0xDA(0x12)//?p31*config(0x32,0x02,0x12,0x22
	 WR1BCmd(0x81);  WR1BCmd(0xaf);		// SetBrightness(m_brightness);		 //0x81(0xAF)���ȼ�Contrast Control,Brightness:0x00(dark)~0xff(light)
	 WR1BCmd(0xd9);  WR1BCmd(0x25); 	// SetPreChargePeriod(0x25);			 //0xD9(0x25)//?p32*Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	 WR1BCmd(0xdb);  WR1BCmd(0x20);		//SetVcomhDeselectLevel(0x20);	 //0xDB(0x20)//00h	~ 0.65 x Vcc  //20h  ~ 0.77 x Vcc (RESET) //30h  ~ 0.83 x Vcc
	 WR1BCmd(0xa4);					// SwitchEntireDisplayOn(true);	 //0xA4 or 0xA5 ,falseȫ����ʾ
	 WR1BCmd(0xa6);					// SwitchInverseDisplay(false);	 //0xA6 or 0xA7 ������ʾ
	 WR1BCmd(0xaf);					// SwitchDisplayOn(true); 				 //0xAF  ������ʾ	 

	
	//flush((unsigned char *)HM);//����
	//OledClear();
	
}

void OledPowerDown()
{
	WR1BCmd(0xae);	// Set Display Off 0xAE
	WR1BCmd(0x8d);	// Set Charge Pump 0x8D, 0x10
	WR1BCmd(0x10);
	Delay100ms();	// Power Stabilized (100ms Delay Recommended)
	VDDB_OLED = 0;	//Power down VDDB (50ms Delay Recommended)
	Delay50ms();
		//Power down VDD

}

void OledEnteringSleepMode()
{
	WR1BCmd(0xae);	// Set Display Off 0xAE
	WR1BCmd(0x8d);	// Set Charge Pump 0x8D, 0x10
	WR1BCmd(0x10);
	VDDB_OLED = 0;	// Power down VDDB
}

void OledExitingSleepMode()
{
	VDDB_OLED = 1;	// Power up VDDB (100ms Delay Recommended)
	WR1BCmd(0x8d);	// Set Charge Pump 0x8D, 0x14
	WR1BCmd(0x14);
	WR1BCmd(0xaf);	// Set Display On 0xAF
}


char SetStartPage(uint8_t page)	 //ҳģʽ��,������ʼҳ
{
	if(page > 4){
		WR1BCmd( 0xB0 | (4 & 0x0F) );
		return 1;
	}
	WR1BCmd( 0xB0 | ((page) & 0x0F) );
	return 0;
}

char SetStartColumn(uint8_t ColumnAddress)   //ҳģʽ��,������ʼ��
{
	if(ColumnAddress > MAX_COLUMN){
		return 1;
	}
	//SH1106,X��ƫ��-2,ssd1306 x��ƫ��-32
	ColumnAddress += 32;

	WR1BCmd(0x0f &  ColumnAddress ); 			   //Set Lower Column (00H - 0FH) 
	WR1BCmd(0x0f & (ColumnAddress >> 4) | 0x10 ); 		//Set Higher Column	(10H - 1FH)
		return 0;
}


/*
	���� : ����
*/
void OledClear(void)		//�˰汾Ϊline mode
{
	unsigned char page = 0;	  
	uint8_t column = 0;
    for(;page < MAX_PAGE; page++)
    {	
    	column = 0;
        SetStartPage(page);		//SetPageAddress(page); 
        SetStartColumn(0);		//SetColumnAddress(0);
        for(;column < MAX_COLUMN; column++)
        {
              WR1BData(0x00);
        }
    }	
}

/*
	���� : ˢ����������
	���� : ���������λֵ
*/
char OledFlush(unsigned char *buf)
{
	unsigned char page = 0;	  
	uint8_t column = 0;
	unsigned int n = 0;

	if(buf == 0)
		return 1;
    for(;page < MAX_PAGE; page++)
    {	
    	column = 0;
        SetStartPage(page);		//SetPageAddress(page); 
        SetStartColumn(0);		//SetColumnAddress(0);
        for(;column < MAX_COLUMN; column++)
        {
            WR1BData(buf[n++]);
        }
    }
	return 0;	
}


/*
	���� : ˢ��ĳһ��
	���� : ĳһ�п�ʼλ�ã�������
*/
char OledFlushPage(unsigned char Page, unsigned char Col,unsigned char *buf, unsigned char bufNum)
{	 
	uint8_t column = 0;
	
	if(Page > 4 || Col > MAX_COLUMN || buf == NULL || Col + bufNum > MAX_COLUMN)
		return 1;
	
    SetStartPage(Page);		//SetPageAddress(page); 
    SetStartColumn(Col);		//SetColumnAddress(0);
    
    for(;column < bufNum && Col + column < MAX_COLUMN; column++)
    {
        WR1BData(buf[column]);
    }	

	return 0;
}


/*
	���� : ���ASSIC������ݣ�������24���ַ���ֻ�������ĸ�����֣������ַ�����ʾΪ�ո�
	���� : �ַ���
*/
void OledWriteMessage(char *str)
{
	unsigned char charNum = 0;
	unsigned char i = 0;
	
	while(*str){

		if(charNum < 12){			//��һ����
			SetStartPage(1);					//SetPageAddress(page); 
        	SetStartColumn(charNum * 8);		//SetColumnAddress(0);

        }
        else if(charNum < 24){
			SetStartPage(3);					//SetPageAddress(page); 
			SetStartColumn((charNum - 12) * 8);		//SetColumnAddress(0);

		}
		else
			return;
	
    	if(*str >= '0' && *str <= '9'){		// ���������
			for(i = 0; i < 8; i++){
				WR1BData(Num[*str - '0'][i]);
			}
    	}
		else if(*str >= 'a' && *str <= 'z'){	// a - z
			for(i = 0; i < 8; i++){
				WR1BData(LETTER[*str - 'a'][i]);
			}
		}
		else if(*str >= 'A' && *str <= 'Z'){	// A - Z
			for(i = 0; i < 8; i++){
				WR1BData(LETTER[*str - 'A' + 26][i]);
			}
		}
		else{							// ���඼Ϊ�ո�
			for(i = 0; i < 8; i++){
				WR1BData(0x00);
			}
		}
		if(charNum < 12){
			SetStartPage(2);					//SetPageAddress(page); 
        	SetStartColumn(charNum * 8);		//SetColumnAddress(0);
		}
		else if(charNum < 24){
			SetStartPage(4);					//SetPageAddress(page); 
			SetStartColumn((charNum - 12) * 8);		//SetColumnAddress(0);

		}
		else return;
		
    	if(*str >= '0' && *str <= '9'){		// ���������
			for(i = 0; i < 8; i++){
				WR1BData(Num[*str - '0'][i + 8]);
			}
    	}
		else if(*str >= 'a' && *str <= 'z'){	// a - z
			for(i = 0; i < 8; i++){
				WR1BData(LETTER[*str - 'a'][i + 8]);
			}
		}
		else if(*str >= 'A' && *str <= 'Z'){	// A - Z
			for(i = 0; i < 8; i++){
				WR1BData(LETTER[*str - 'A' + 26][i + 8]);
			}
		}
		else{							// ���඼Ϊ�ո�
			for(i = 0; i < 8; i++){
				WR1BData(0x00);
			}
		}
		charNum++;
		str++;
	}
}

/*
	���� : ���һ���ַ���HEX��
	���� : Page �� Col ��ʾ��λ��
		Page : 1 �� 3 ; �ܹ�5�У���0��ʼ�� ��һ��ֻ��7λ��һ���ַ���Ҫ2��page������Ϊ1��3page��ʼ
		Col : ���8�ı��� ��Ϊÿ���ַ���Ҫ��λ16��
*/
char OledWriteWordByHex(unsigned char Page, unsigned char Col, unsigned char Data)
{
	unsigned char i = 0;
	unsigned char left = (Data >> 4);
	unsigned char right = (Data & 0x0f);
	
	if(Page > 4 || Col > MAX_COLUMN)
		return 1;
	
	SetStartPage(Page);					//SetPageAddress(page); 
    SetStartColumn(Col);
	for(i = 0; i < 8; i++){
		WR1BData(Num[left][i]);
	}
	SetStartPage(Page + 1);					//SetPageAddress(page); 
    SetStartColumn(Col);
	for(i = 0; i < 8; i++){
			WR1BData(Num[left][i + 8]);
	}

	SetStartPage(Page);					//SetPageAddress(page); 
  	SetStartColumn(Col + 8);
	for(i = 0; i < 8; i++){
		if(i != 7)
			WR1BData(Num[right][i]);
		else
			WR1BData(Num[right][i] | 1);
	}
	SetStartPage(Page + 1);					//SetPageAddress(page); 
    SetStartColumn(Col + 8);
	for(i = 0; i < 8; i++){
			WR1BData(Num[right][i + 8]);
	}
	return 0;
}


/*
	���� : ���HEX������ݣ�������24�����֣���12���ַ�
	���� : �������ݣ��Լ�����ĳ���
	��� : �����鳬��12���ַ�ʱ����ʾ12���ַ��������ش������
*/
char OledWriteBufByHex(unsigned char *Data, unsigned char Num)
{
	unsigned char dataNum = 0;
	if(Data == NULL)
		return 1;
	while(dataNum < Num){
		if(dataNum < 6){
			OledWriteWordByHex(1, dataNum * 16, Data[dataNum]);
		}
		else if(dataNum < 12)
			OledWriteWordByHex(3, (dataNum - 6) * 16, Data[dataNum]);
		else 
			return 1;
		++dataNum;
	}
	return 0;
	
}

/*
	���� : ʹ��5*7��ģ  ���һ���ַ���HEX��
	���� : Page �� Col ��ʾ��λ��
		Page : 
		Col : ���6�ı��� 
*/

char OledWriteWordByHex57(unsigned char Page, unsigned char Col, unsigned char Data)
{
	unsigned char i = 0;
	unsigned char left = (Data >> 4);
	unsigned char right = (Data & 0x0f);

	if(Page > 4 || Col > MAX_COLUMN)
		return 1;
	
	SetStartPage(Page);					//SetPageAddress(page); 
    SetStartColumn(Col);
	if(left >=0 && left <= 9){
		for(i = 0; i < 5; i++){
			WR1BData(Fonts[('0' - 32 + left) * 5 + i]);
		}
	}
	else if(left >= 10 && left <= 15){
		for(i = 0; i < 5; i++){
			WR1BData(Fonts[('A' - 32 + left - 10) * 5 + i]);
		}
	}
	else return 1;
		
	WR1BData(0x00);
	
//	SetStartPage(Page);					//SetPageAddress(page); 
// 	SetStartColumn(Col + 6);
	
	if(right >=0 && right <= 9){
		for(i = 0; i < 5; i++){
			WR1BData(Fonts[('0' - 32 + right) * 5 + i]);
		}
	}
	else if(right >= 10 && right <= 15){
		for(i = 0; i < 5; i++){
			WR1BData(Fonts[('A' - 32 + right - 10) * 5 + i]);
		}
	}
	else return 1;
	WR1BData(0x03);	
	return 0;
}


/*
	���� : ʹ��5*7��ģ  ���HEX������ݣ�������64�����֣���32���ַ�
	���� : ��������
*/

char OledWriteBufByHex57index(unsigned char Page, unsigned char Col, unsigned char *str, unsigned char Num)
{
	unsigned char charNum = 0;
	unsigned char writeNum = 0;
	unsigned char i = 0;
	//clearPageCol(0, 0);
	
	if(Page < 1 || Page > 4 || charNum > MAX_COLUMN || str == NULL){
		OledAssicForState(0,0,127);
		return 1;
	}
	charNum = (Page - 1) * 8 + ( (Col % 12 == 0) ? Col : ( (Col / 12 + 1) * 12) );	
		
	while(writeNum < Num){
		if(charNum < 8){			//��һ����
			OledWriteWordByHex57(1, charNum * 12, *str);
        }
        else if(charNum < 16){
			OledWriteWordByHex57(2, (charNum - 8) * 12, *str);

		}
		else if(charNum < 24){
			OledWriteWordByHex57(3, (charNum - 16) * 12, *str);

		}
		else if(charNum < 32){
			OledWriteWordByHex57(4, (charNum - 24) * 12, *str);

		}
		else
			return 1;
		str++;
		charNum++;
		writeNum++;
		
	}

	return 0;
}

char OledWriteBufByHex57(unsigned char *str, unsigned char Num)
{
	if(str == NULL){
		OledAssicForState(0, 0, 127);
		return 1;
	}
	OledWriteBufByHex57index(1, 0, str, Num);
	return 0;
}





/*
	���� : ʹ��5*7��ģ ���һ���ַ���assic��
	���� : Page �� Col ��ʾ��λ��
		Page : 
		Col : ���6�ı��� 
		Data : ��ĸ
*/
char OledWriteAssic57(unsigned char Page, unsigned char Col,unsigned char Data)
{
		unsigned char i = 0;
		
		if(Page > 4 || Col > MAX_COLUMN)
			return 1;

		if(Data < 32  || Data > 127){
			Data = 127;
			SetStartPage(Page);					//SetPageAddress(page); 
    		SetStartColumn(Col);
			for(i = 0; i < 5; i++){
				WR1BData(Fonts[(Data - 32) * 5 + i]);
			}
			WR1BData(0x00);	
			return 1;
		}
		
		SetStartPage(Page);					//SetPageAddress(page); 
    	SetStartColumn(Col);
		for(i = 0; i < 5; i++){
			WR1BData(Fonts[(Data - 32) * 5 + i]);
		}
		WR1BData(0x00);	

		return 0;
}

/*
	���� : ����state ʹ��5*7��ģ ���һ���ַ���assic��	
	���� : Page �� Col ��ʾ��λ��
		Page : 
		Col : ���6�ı��� 
		Data : ��ĸ
*/
char OledAssicForState(unsigned char Page, unsigned char Col,unsigned char Data)
{
		unsigned char i = 0;
		
		if(Page > 4 || Col > MAX_COLUMN)
			return 1;
		if(Data < 32 || Data > 127){
			Data = 127;
			SetStartPage(Page);					//SetPageAddress(page); 
    		SetStartColumn(Col);
			for(i = 0; i < 5; i++){
				WR1BData(Fonts[(Data - 32) * 5 + i] << 1);
			}
			WR1BData(0x00);	
			return 1;
		}
		
		SetStartPage(Page);					//SetPageAddress(page); 
    	SetStartColumn(Col);
		for(i = 0; i < 5; i++){
			WR1BData(Fonts[(Data - 32) * 5 + i] << 1);
		}
		WR1BData(0x00);

		return 0;
}



/*
	���� : ʹ��5*7��ģ ���ASSIC������ݣ�������64���ַ�
	���� : �ַ���
*/
void OledWriteMessage57(char *str)
{
	unsigned char charNum = 0;
	unsigned char i = 0;
	unsigned char page = 0;
	
	while(*str){

		if(charNum < 16){			//��һ����
			OledWriteAssic57(1, charNum * 6, *str);
        }
        else if(charNum < 32){
			OledWriteAssic57(2, (charNum - 16) * 6, *str);

		}
		else if(charNum < 48){
			OledWriteAssic57(3, (charNum - 32) * 6, *str);

		}
		else if(charNum < 64){
			OledWriteAssic57(4, (charNum - 48) * 6, *str);

		}
		else
			break;
		str++;
		charNum++;
	}
}

/*
	���� : ���ĳ��ĳ�п�ʼ��ֵ
	���� : Page �� Col ��λ��
*/
char clearPageCol(unsigned char Page, unsigned char Col)
{
	unsigned char i = Col;

	if(Page > 4 || Col > MAX_COLUMN)
		return 1;
	SetStartPage(Page);					//SetPageAddress(page); 
    SetStartColumn(Col);
	for(; i < MAX_COLUMN; i++){
		WR1BData(0);
	}
	return 0;
}

/*
	���� : ʹ��5*7��ģ  ���assic�����ݣ�������64�����֣���32���ַ�
	���� : ��ʼҳ����ʼλ�ã���������
*/
char OledWriteBufByAssic57(unsigned char Page, unsigned char Col, unsigned char *arr, unsigned char Num)
{
	unsigned char charNum = 0;
	unsigned char writeNum = 0;
	unsigned char i = 0;

	if(Page < 1 || Page > 4 || charNum > MAX_COLUMN || arr == NULL){
		OledAssicForState(0,0,127);
		return 1;
	}
	charNum = (Page - 1) * 16 + ( (Col % 6 == 0) ? Col : ( (Col / 6 + 1) * 6) );
	while(writeNum < Num){
		if(charNum < 16){			//��һ����
			OledWriteAssic57(1, charNum * 6, *arr);
        }
        else if(charNum < 32){
			OledWriteAssic57(2, (charNum - 16) * 6, *arr);

		}
		else if(charNum < 48){
			OledWriteAssic57(3, (charNum - 32) * 6, *arr);

		}
		else if(charNum < 64){
			OledWriteAssic57(4, (charNum - 48) * 6, *arr);

		}
		else
			break;
		arr++;
		charNum++;
		writeNum++;
	}
	return 0;
}


unsigned char OledWriteDouble(unsigned char Page, unsigned char Col, double input)
{
	unsigned char inputLeft = (unsigned char)input;	// С���������ֵ
	unsigned char inputRight = (input - (double)inputLeft) * 100;	// С�����ұ���ֵ	������λ


	unsigned char numArr[64] = {0};	// �����������֣�Ĭ�����Ϊ64λ
	unsigned char i = 0;
 	unsigned char nb = 0;
	unsigned char output = 0;
	if(Page > 4 || Col > MAX_COLUMN)
		return 1;

	// �õ���������
	numArr[63] = inputRight % 10 + '0';
	numArr[62] = inputRight / 10 + '0';
	numArr[61] = '.';
	do{
		numArr[60 - nb] = inputLeft % 10 + '0';
		++nb;
		inputLeft /= 10;
	}while(inputLeft != 0);
	
	OledWriteBufByAssic57(Page, Col, numArr + 61 - nb, nb + 3);
	return 0;
	
	
}

char OledWriteTempSymbol(unsigned char Page, unsigned char Col)
{
	int i = 0;
	if(Page < 1 || Page > 4 || Col > 84){
		OledAssicForState(0,0,127);
		return 1;
	}
	
	SetStartPage(Page);					//SetPageAddress(page); 
    SetStartColumn(Col);
	
	for(; i < 16; i++){
		WR1BData(tempSymbol[i]);
	}
	return 0;
}

