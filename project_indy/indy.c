#include "indy.h"

char rec_flag  =   -1;     //接收到耳标信号标志。-1没接受到。0接受到
unsigned char  rec_flag_cnt = 0;   //接收到耳标信息计数。如果接收到一次，则认为读取正常。
xdata   UART2_BODY  mes_body;

uint16 TEMP2 = 1600;
uint16 TEMP1 = 1008;
uint16 CODE2 = 2619;
uint16 CODE1 = 2186;        
void write_mac_register(uint16 addr,uint16 w_data,uint16 w_data_l)
{

		SendData(0x01);
		SendData(0x00);
		SendData((uint8)(addr&0x00ff));
		SendData((uint8)(addr>>8));
		SendData((uint8)(w_data_l&0x00ff));
		SendData((uint8)(w_data_l>>8));
		SendData((uint8)(w_data&0x00ff));
		SendData((uint8)(w_data>>8));
}

void read_mac_register(uint16 addr)
{

		SendData(0x00);
		SendData(0x00);
		SendData((uint8)(addr&0x00ff));
		SendData((uint8)(addr>>8));
		SendData(0x00);
		SendData(0x00);
		SendData(0x00);
		SendData(0x00);
}

void indySendMes(void)
{
    char i;
    unsigned char sum = 0;
    char send_buf[32];

    send_buf[0] = mes_body.cmd_type;
    for(i = 0;i < EPC_CODE_NUM;i++)
    {
        send_buf[1 + i] = mes_body.epc_code[i];
    }
    for(i = 0;i < VAL_DATA_NUM;i++)
    {
        send_buf[13 + i] = mes_body.val_data[i];
    }

    for(i = 15;i < UART2_SEND_NUM;i++)
        send_buf[i] = RESERVED_DATA;

    for(i = 0;i < UART2_SEND_NUM;i++)
    {
        sum +=  send_buf[i];  
    }
    
    send_buf[UART2_SEND_NUM -1] = sum & 0xff;
	OledWriteBufByHex57(send_buf, UART2_SEND_NUM);
   // for(i = 0;i < UART2_SEND_NUM;i++)
   // {
   //     SendData2(send_buf[i]);
		
		//OledWriteWordByHex57(i / 16 + 1, i % 16, send_buf[i]);
   // }
     //OledWriteMessageByHex57(send_buf, UART2_SEND_NUM);
    
}

void clear_mes_body()
{
    char    i;

    mes_body.cmd_type =   0;
    for(i = 0;i <   EPC_CODE_NUM;i++)
    mes_body.epc_code[i] = 0;
    for(i = 0;i <   VAL_DATA_NUM;i++)
    mes_body.val_data[i] = 0;  
    for(i = 0;i < RESERVED_DATA;i++)
    mes_body.res[i] = 0;
    mes_body.check_sum = 0;
}

void process_packets()
{
	double output = 0;
	uint16 input = 0;
	uint16 i;
	uint8 j = 1;
	uint16 data_len=0;
	uint16 cmd_type=0xffff;
	uint16 cmd_addr_fir=0;      //命令地址
	uint16 data_addr_fir=0;     //8个字节的数据包后的地址
	uint16 data_addr_end=0;     //数据包结束后的地址。
	uint16 epc_data_addr = 0;   //epc数据的起始地址
	uint16 sensor_data_addr = 0;   //sensor_code数据起始地址

	uint8 check_data=0;	// 判断数据是否全为0; 如果全为0则不显示
	bit check_con1 = 0;
	bit check_con2 = 0;

	
	unsigned char bleVal[2] = {0};
    uint8   pkt_flag = 0;

	while(1)
	{
        
		if((rec_data[0+cmd_addr_fir]==0x01) )// && (rec_data[cmd_addr_fir+1]==0x00)
		{	
            //SendData2(0x77);
            pkt_flag = rec_data[1 + cmd_addr_fir];
			cmd_type = (rec_data[3+cmd_addr_fir]<<8) + rec_data[2+cmd_addr_fir];
		//	cmd_type = (rec_data[3+16]) + rec_data[2+16];
			data_len = (rec_data[5+cmd_addr_fir]<<8) + rec_data[4+cmd_addr_fir];
			data_addr_fir = 8 + cmd_addr_fir;
			data_addr_end = data_addr_fir + data_len*4 ; 
			cmd_addr_fir = (data_len+2)*4+cmd_addr_fir;
		//	cmd_addr_fir += 16;

		}
		else
		{	
	
			break;
		}
 
		if(cmd_type == 0x0005)
		{ 
			epc_data_addr = data_addr_fir+14;
			check_data = 0;
            for(i = 0;i < 12;i++)
            { 
                //epc_code[i] = rec_data[epc_data_addr + i];//epc
                mes_body.epc_code[i] = rec_data[epc_data_addr + i];//epc

				check_data |= mes_body.epc_code[i];	// 如果全为0则不显示
            } 
			switch(mes_body.epc_code[1]){
			case 0x8a:
				TEMP2 = 1252;
				CODE2 = 2423;
				TEMP1 = 1133;
				CODE1 = 2340;
				break;
			 case 0x15:
				TEMP2 = 1290;
				CODE2 = 2514;
				TEMP1 = 1123;
				CODE1 = 2368;
				break;			
			case 0x8b:
				TEMP2 = 1257;
				CODE2 = 2388;
				TEMP1 = 1171;
				CODE1 = 2321;
				break;
			case 0x8c:
				TEMP2 = 1235;
				CODE2 = 2403;
				TEMP1 = 1175;
				CODE1 = 2352;
				break;
			case 0x5f:
				TEMP2 = 1263;
				CODE2 = 2409;
				TEMP1 = 1113;
				CODE1 = 2317;
				break;
			case 0x64:
				TEMP2 = 1258;
				CODE2 = 2442;
				TEMP1 = 1115;
				CODE1 = 2337;	
				break;
			default:
				break;
			}

			
			OledAssicForState(0,0, 'i');
			OledAssicForState(0,6, 'd');
			OledAssicForState(0,12, ':');
	

			 if(check_data != 0){
			 	OledWriteBufByHex57index(1, 0, mes_body.epc_code, 12);

			 	if(bleState.enSend == ENABLE)
			 		bleSendData(ID, mes_body.epc_code, 12, SUC);
			 }
		}
	    if(cmd_type == 0x0006)
	    {
      	
            if(pkt_flag == 0x00)	
            {    
            
                if(rec_data[data_addr_fir + 4] == 0xc2)
                {
                    sensor_data_addr = data_addr_fir + 12;
                    

                    switch(mes_body.cmd_type)
    			    {
    					case 1:	// 温度
							check_data = 0;
							lowBattery50msBZ();	// 读到温度提示
    						for(i=0; i<2; i++)
    					    {
                                mes_body.val_data[i] = rec_data[sensor_data_addr +  i];
    							check_data |= mes_body.val_data[i];
    					    }
							if(check_data == 0)
								break;
							
							input |= mes_body.val_data[0] << 8;
							input |= mes_body.val_data[1];

							if(( (input < CODE1) && (CODE1 - input > 100) ) || ( (input > CODE2) && (input - CODE2 > 100) ))
								break;	// 当温度离开温度区间10度时认为是错误数据
							output = (1.0 * (TEMP2 - TEMP1) / (CODE2 - CODE1) * ( input - CODE1) + TEMP1 - 800) / 10.0;

							// oled显示							
							OledWriteWordByHex57(4, 52, mes_body.val_data[0]);
							OledWriteWordByHex57(4, 68,mes_body.val_data[1]);
							OledWriteBufByAssic57(3, 0, "temp:", 5);
							OledWriteDouble(4, 0, output);
							OledWriteTempSymbol(4,30);
							
							// 蓝牙传输
							if(bleState.enSend == ENABLE){
			 					//bleSendData(TEMP, mes_body.val_data, 2, bleRecvFlag.recState);
			 					bleVal[0] = (unsigned char) output;
								bleVal[1] = (output - (double)bleVal[0]) * 100;
			 					bleSendData(TEMP, bleVal, 2, SUC);
								
								//if(bleState.moreInfor == 1){
									bleSendData(TEMPCODE, mes_body.val_data, 2, SUC);
								//}	
							}
							
    						break;
    					case 2:	// sensor code
    					
							check_data = 0;
    						for(i=0; i<2; i++)
    					    {
    							mes_body.val_data[i] = rec_data[sensor_data_addr +  i];
    							check_data |= mes_body.val_data[i];
    					    }
							if(check_data == 0)
								break;
							if(bleState.enSend == ENABLE)
			 					bleSendData(SENSORCODE, mes_body.val_data, 2, SUC);
							
    						break;
    					case 3:	// rssi
                            mes_body.val_data[0] = 0;
                            mes_body.val_data[1] = rec_data[sensor_data_addr +  1];
					
							
							if(bleState.enSend == ENABLE)
			 					bleSendData(RSSI, mes_body.val_data, 2, SUC);
    					
    						break;
    					default : 
                            rec_flag    =   READ_NO_TAG;
                            rec_flag_cnt = 0;
                            break;
    				}      
                }
            	 else{
				}   
				
				
            }
			else if(pkt_flag == 0x80)	// 温度校准数据
                {
                	click10msBZ(); // 读到温度提示
                	CODE1 = rec_data[data_addr_fir + 12] << 4 | ((rec_data[data_addr_fir + 13] & 0xf0) >> 4);
					TEMP1 = ((rec_data[data_addr_fir + 13] & 0x0f) << 7) | ((rec_data[data_addr_fir + 14] & 0xfe) >> 1);
					CODE2 = ((rec_data[data_addr_fir + 14] & 1) << 11) | (rec_data[data_addr_fir + 15] << 3) | ((rec_data[data_addr_fir + 16] & 0xE0) >> 5);
					TEMP2 = ((rec_data[data_addr_fir + 16] & 0x1f) << 6) | ((rec_data[data_addr_fir + 17] & 0xfc) >> 2);
                }
            else
            {
                //if(pkt_flag && 0x01)
                   // SendString2("MAC detected an err\n");
                //if(pkt_flag && 0x02)
                  //  SendString2("Tag did backscatter and err\n");
                // OledWriteWordByHex(1, 0, 0x33);
                break;
            }
        }
		
	} 
 
}
void indy_read(uint16 bank, uint16 addr, uint16 num)
{
    int rec_cnt = 0;
    //int j;
    //char i;
    
    write_mac_register(0x0a02,0x0000,bank);
	write_mac_register(0x0a03,0x0000,addr);
	write_mac_register(0x0a04,0x0000,num);
    write_mac_register(0x0a06,0x0000,0x0000);
	write_mac_register(0xf000,0x0000,0x0010);

	Delay20ms();

    /*
    SendData2(rec_num );
    rec_cnt = rec_num;
    SendData2(0x55);
    SendData2(rec_cnt);
    for(j = 0;j < rec_cnt;j++)
        SendData2(rec_data[j]);
        */
			
    if(rec_data[0]	== 0x01	)
    {	
        ES = 0;
        process_packets();	//处理数据
        ES = 1;
    }	
	//OledWriteMessage57(" indy_readsensorcode end ");
}
	
void indy_read_sensorcode()
{
    int rec_cnt = 0;
    //int j;
    //char i;
    clear_rec_data();
	clear_mes_body();
    mes_body.cmd_type = 2;
    write_mac_register(0x0a02,0x0000,0x0000);
	write_mac_register(0x0a03,0x0000,0x000C);
	write_mac_register(0x0a04,0x0000,0x0002);
    write_mac_register(0x0a06,0x0000,0x0000);
	write_mac_register(0xf000,0x0000,0x0010);

	Delay20ms();

			
    if(rec_data[0]	== 0x01	)
    {	
        ES = 0;
        process_packets();	//处理数据
        ES = 1;
    }	
	//OledWriteMessage57(" indy_readsensorcode end ");
}

void indy_readrssi()
{

    int rec_cnt = 0;
     //int j;
     //char i;
    clear_rec_data();
	clear_mes_body();
    mes_body.cmd_type = 3;
    write_mac_register(HST_TAGACC_BANK,0x0000,0x0000);
	write_mac_register(HST_TAGACC_PTR,0x0000,0x000d);
	write_mac_register(HST_TAGACC_CNT,0x0000,0x0002);
	write_mac_register(HST_TAGACC_ACCPWD,0x0000,0x0000);
	write_mac_register(HST_CMD,0x0000,8);
    Delay20ms();     
    if(rec_data[0]  == 0x01 )
    {   
        ES = 0;
        process_packets();   //处理数据
        ES = 1;
    }   

	// OledWriteMessage57(" indy_readrssi end ");
}



/*
读取耳标的EPC和温度
*/
void indy_readtemp()
{
	
    //int rec_cnt = 0;
   	// int i;
	// SendString2(" indy_readtemp start \r\n");
    //for(i = 0;i < 5;i++)
    //{
    clear_rec_data();
    clear_mes_body();
    mes_body.cmd_type = 1;
    write_mac_register(HST_TAGACC_BANK,0x0000,0x0000);
    write_mac_register(HST_TAGACC_PTR,0x0000,0x000e);
    write_mac_register(HST_TAGACC_CNT,0x0000,0x0002);
    write_mac_register(HST_TAGACC_ACCPWD,0x0000,0x0000);
    write_mac_register(HST_CMD,0x0000,7);
	Delay20ms();

    //}
    // clear_rec_data();
	// Delay100ms();

			
	if(rec_data[0]	== 0x01	)
	{	
		ES = 0;
		process_packets();	 //处理数据
		ES = 1;
	}	
   // SendString2(" indy_readtemp end \r\n");
   // OledWriteMessage57(" indy_readtemp end ");
}

void indy_readall()
{
	//unsigned char temp[2] = {1, 2};

	//bleSendData(SENSORCODE, temp, 2, SUC);
	//bleSendData(RSSI, temp, 2, SUC);
	indyGetBD();
	indy_readtemp();
	indy_read_sensorcode();
	indy_readrssi();
	return;
}

void index_send(char id)
{
    switch(id)
    {
        case 1:		// 	hand
            SendData(0xC0);
            SendData(0x06);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;
        case 2:
            SendData(0x40);
            SendData(0x03);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;
            
        case 3:
            SendData(0x00);
            SendData(0x00);
            SendData(0x01);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

        case 4:
            SendData(0x00);
            SendData(0x00);
            SendData(0x10);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

        case 5:
            SendData(0x00);
            SendData(0x00);
            SendData(0x05);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

        case 6:
            SendData(0x00);
            SendData(0x00);
            SendData(0x06);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

       case 7:
            SendData(0x00);
            SendData(0x00);
            SendData(0x10);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;
       case 8:
            SendData(0x00);
            SendData(0x00);
            SendData(0x11);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

        case 9:
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

        case 10:
            SendData(0x00);
            SendData(0x00);
            SendData(0x01);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

        case 11:
            SendData(0x00);
            SendData(0x00);
            SendData(0x02);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

        case 12:
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x03);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

        case 13:
            SendData(0x00);
            SendData(0x00);
            SendData(0x02);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

        case 14:
            SendData(0x00);
            SendData(0x00);
            SendData(0x61);
            SendData(0x1B);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

 // 1 - 14 :   connect
        case 15:
            SendData(0x01);
            SendData(0x00);
            SendData(0x01);
            SendData(0x07);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);

            SendData(0x00);
            SendData(0x00);
            SendData(0x05);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

        case 16:
            SendData(0x00);
            SendData(0x00);
            SendData(0x02);
            SendData(0x07);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

       case 17:
            SendData(0x01);
            SendData(0x00);
            SendData(0x02);
            SendData(0x07);
            SendData(0x01);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);

            SendData(0x01);
            SendData(0x00);
            SendData(0x01);
            SendData(0x07);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);

            SendData(0x00);
            SendData(0x00);
            SendData(0x05);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

        case 18:
            SendData(0x00);
            SendData(0x00);
            SendData(0x04);
            SendData(0x07);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

       case 19:
            SendData(0x00);
            SendData(0x00);
            SendData(0x05);
            SendData(0x07);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

        case 20:
            SendData(0x00);
            SendData(0x00);
            SendData(0x06);
            SendData(0x07);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

       case 21:
            SendData(0x00);
            SendData(0x00);
            SendData(0x07);
            SendData(0x07);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

       case 22:
            SendData(0x00);
            SendData(0x00);
            SendData(0x12);
            SendData(0x0b);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

       case 23:
            SendData(0x01);
            SendData(0x00);
            SendData(0x01);
            SendData(0x07);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);

            SendData(0x00);
            SendData(0x00);
            SendData(0x05);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

        case 24:
            /*
            SendData(0x01);
            SendData(0x00);
            SendData(0x04);
            SendData(0x07);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);

            SendData(0x01);
            SendData(0x00);
            SendData(0x05);
            SendData(0x07);
            SendData(0xD0);
            SendData(0x07);
            SendData(0x00);
            SendData(0x00);

            SendData(0x01);
            SendData(0x00);
            SendData(0x06);
            SendData(0x07);					// power
            SendData(0xDC);//220 --> 22DB  	//
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);

            SendData(0x01);
            SendData(0x00);
            SendData(0x07);
            SendData(0x07);
            SendData(0x00);
            SendData(0x20);
            SendData(0x00);
            SendData(0x00);

            SendData(0x01);
            SendData(0x00);
            SendData(0x12);
            SendData(0x0B);
            SendData(0xff);
            SendData(0xFF);
            SendData(0x0f);
            SendData(0x00);
            */
            break;

        ////870:01 00 0A 01 70 46 0D 00
            //880:01 00 0A 01 80 6D 0D 00
            //885:01 00 0A 01 08 81 0D 00

 // -------------------------  15 - 24 : com set power -------------------------
        case 25:
            SendData(0x01);
            SendData(0x00);
            SendData(0x0A);
            SendData(0x01);
            SendData(0xF8);
            SendData(0x59);//59 0D :875
            SendData(0x0d);
            SendData(0x00);

            SendData(0x00);
            SendData(0x00);
            SendData(0x05);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

        case 26:
            SendData(0x00);
            SendData(0x00);
            SendData(0x05);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

       case 27:
            SendData(0x00);
            SendData(0x00);
            SendData(0x06);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

       case 28:
            SendData(0x01);
            SendData(0x00);
            SendData(0x08);
            SendData(0x01);
            SendData(0x01);
            SendData(0x01);
            SendData(0x00);
            SendData(0x00);

            SendData(0x00);
            SendData(0x00);
            SendData(0x05);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

        case 29:
            SendData(0x00);
            SendData(0x00);
            SendData(0x05);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

       case 30:
            SendData(0x00);
            SendData(0x00);
            SendData(0x06);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

       case 31:
            SendData(0x01);
            SendData(0x00);
            SendData(0x00);
            SendData(0xf0);
            SendData(0x27);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;        
        case 32:
            SendData(0x00);
            SendData(0x00);
            SendData(0x05);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;

       case 33:
            SendData(0x00);
            SendData(0x00);
            SendData(0x06);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            SendData(0x00);
            break;
// -------------------- 25 - 33 : set frequent ----------------------
		case 34:
		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x01); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x70); SendData(0x0d); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x01); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x01); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x84); SendData(0x0d); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);
		
		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x02); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x01); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x98); SendData(0x0d); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);	

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x03); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x01); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0xac); SendData(0x0d); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x04); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x01); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0xc0); SendData(0x0d); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x05); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x01); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0xd4); SendData(0x0d); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);
		
		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x06); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x01); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0xe8); SendData(0x0d); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);
		
		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x07); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x01); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0xfc); SendData(0x0d); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);
																				  
		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x08); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x01); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x10); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);
		
		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x09); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x01); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x24); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);
																								  
		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x0a); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x01); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x38); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);
		
		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x0b); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x01); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x4c); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);
		
		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x01); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);
		
		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x0d); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);
		
		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x0e); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);
		
		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x0f); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);
		
		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x10); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x11); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x12); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x13); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x14); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x15); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x16); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x17); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x18); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x19); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x1a); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x1b); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x1c); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x1d); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x1e); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x1f); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x20); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x21); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x22); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x23); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x24); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x25); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x26); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x27); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x28); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x29); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x2a); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x2b); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x2c); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x2d); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x2e); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x2f); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x30); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);

		SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x31); SendData(0x00); SendData(0x00); SendData(0x00);
        SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);
    	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);
		SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);
        
		break;

		// ----------------------^^^^^^^^^ channel set  ^^^^^^^^^^^^^^--------------
		default:
            break;
    }

}

char index_r_check(char id)
{
    char i;

     xdata char  index_r1_num = 36;
     xdata char   index_r1[]= {0x24 ,0x03 ,0x49 ,0x00 ,0x6D ,0x00 ,0x70 ,0x00 ,0x69 ,0x00 ,0x6E ,0x00 ,0x6A ,0x00 ,0x53 ,0x00 ,0x65 ,0x00 ,0x72 ,0x00 ,0x69 ,0x00 ,0x61 ,0x00 ,0x6C ,0x00 ,0x4E ,0x00 ,0x75 ,0x00 ,0x6D ,0x00 ,0x30 ,0x00 ,0x31 ,0x00};
     xdata char  index_r2_num = 8;
     xdata char  index_r2[]= {0x40 ,0x03 ,0xBF ,0xFC ,0xBF ,0xFC ,0xBF ,0xFC };
     xdata char  index_r3_num = 8;
     xdata char  index_r3[]= {0x00 ,0x00 ,0x01 ,0x00 ,0x12 ,0x00 ,0x21 ,0x00};
     xdata char  index_r4_num = 8;
     xdata char  index_r4[]= {0x00  ,0x00  ,0x10  ,0x00  ,0x00  ,0x00  ,0x00  ,0x00};
     xdata char  index_r5_num = 8;
     xdata char  index_r5[]= {0x00  ,0x00  ,0x05  ,0x00  ,0x00  ,0x00  ,0x00  ,0x00};
     
     xdata char  index_r6_num = 8;
     xdata char  index_r6[]= {0x00  ,0x00  ,0x06  ,0x00  ,0x00  ,0x00  ,0x00  ,0x00};
     
     xdata char  index_r7_num = 8;
     xdata char  index_r7[]= {0x00  ,0x00  ,0x10  ,0x00  ,0x00  ,0x00  ,0x00  ,0x00};
     
     xdata char  index_r8_num = 8;
     xdata char  index_r8[]= {0x00  ,0x00  ,0x11  ,0x00  ,0x88  ,0xE1  ,0xF3  ,0xC0};
     
     xdata char  index_r9_num = 8;
     xdata char  index_r9[]= {0x00  ,0x00  ,0x00  ,0x00  ,0xF0  ,0x00  ,0x06  ,0x02};
     
     xdata char  index_r10_num = 8;
     xdata char  index_r10[]= {0x00  ,0x00  ,0x01  ,0x00  ,0x12  ,0x00  ,0x21  ,0x00};
     
     xdata char  index_r11_num = 8;
     xdata char  index_r11[]= {0x00  ,0x00  ,0x02  ,0x00  ,0xA0  ,0x00  ,0xA0  ,0x02};
     
     xdata char  index_r12_num = 8;
     xdata char  index_r12[]= {0x00  ,0x00  ,0x00  ,0x03  ,0x00  ,0x00  ,0x00  ,0x00};
     
     xdata char  index_r13_num = 8;
     xdata char  index_r13[]= {0x00  ,0x00  ,0x02  ,0x00  ,0xA0  ,0x00  ,0xA0  ,0x02};
     
     xdata char  index_r14_num = 8;
     xdata char  index_r14[]= {0x00  ,0x00  ,0x61  ,0x1B  ,0x02  ,0x06  ,0x00  ,0x00};
     
     xdata char  index_r15_num = 8;
     xdata char  index_r15[]= {0x00 ,0x00 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
     
     xdata char  index_r16_num = 8;
     xdata char  index_r16[]= {0x00 ,0x00 ,0x02 ,0x07 ,0x01 ,0x00 ,0x00 ,0x00};
     
     xdata char  index_r17_num = 8;
     xdata char  index_r17[]= {0x00 ,0x00 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
     
     xdata char  index_r18_num = 8;
     xdata char  index_r18[]= {0x00 ,0x00 ,0x04 ,0x07 ,0x00 ,0x00 ,0x00 ,0x00};
     
     xdata char  index_r19_num = 8;
     xdata char  index_r19[]= {0x00 ,0x00 ,0x05 ,0x07 ,0xD0 ,0x07 ,0x00 ,0x00};
     
     xdata char  index_r20_num = 8;
     xdata char  index_r20[]= {0x00 ,0x00 ,0x06 ,0x07 ,0xDC ,0x00 ,0x00 ,0x00};
     
     xdata char  index_r21_num = 8;
     xdata char  index_r21[]= {0x00 ,0x00 ,0x07 ,0x07 ,0x00 ,0x20 ,0x00 ,0x00};
     
     xdata char  index_r22_num = 8;
     xdata char  index_r22[]= {0x00 ,0x00 ,0x12 ,0x0B ,0xFF ,0xFF ,0x0F ,0x00};
     
     xdata char  index_r23_num = 8;
     xdata char  index_r23[]= {0x00 ,0x00 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
     
     xdata char  index_r24_num = 0;
     xdata char  index_r24[]= {0};
     
     
     xdata char  index_r25_num = 8;
     xdata char  index_r25[]= {0x00 ,0x00 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
     xdata char  index_r26_num = 8;
     xdata char  index_r26[]= {0x00 ,0x00 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
     xdata char  index_r27_num = 8;
     xdata char  index_r27[]= {0x00 ,0x00 ,0x06 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
     xdata char  index_r28_num = 8;
     xdata char  index_r28[]= {0x00 ,0x00 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
     xdata char  index_r29_num = 8;
     xdata char  index_r29[]= {0x00 ,0x00 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
     xdata char  index_r30_num = 8;
     xdata char  index_r30[]= {0x00 ,0x00 ,0x06 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
     xdata char  index_r31_num = 32;
     xdata char  index_r31[]= {0x01 ,0x00 ,0x00 ,0x00 ,0x02 ,0x00 ,0x00 ,0x00 ,0x27 ,0x00 ,0x00 ,0x00 ,0x87 ,0x2E ,0x0D ,0x00 ,0x01 ,0x00 ,0x01 ,0x00 ,0x02 ,0x00 ,0x00 ,0x00 ,0x8F ,0x2E ,0x0D ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
     xdata char  index_r32_num = 8;
     xdata char  index_r32[]= {0x00 ,0x00 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
     xdata char  index_r33_num = 8;
     xdata char  index_r33[]= {0x00 ,0x00 ,0x06 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
     
     

    switch(id)
    {
        case 1:
            for(i = 0;i<index_r1_num;i++)
            {        
                if(rec_data[i] != index_r1[i])
                {
                    return id;
                }
            }
            break;
        case 2:
            for(i = 0;i<index_r2_num;i++)
            {
                if(rec_data[i] != index_r2[i])
                {
                    return id;
                }
            }
            break;
            case 3:
            for(i = 0;i<index_r3_num;i++)
            {
                if(rec_data[i] != index_r3[i])
                {
                    return id;
                }
            }
            break;
        case 4:
            for(i = 0;i<index_r4_num;i++)
            {
                if(rec_data[i] != index_r4[i])
                {
                    return id;
                }
            }
            break;
        case 5:
            for(i = 0;i<index_r5_num;i++)
            {
                if(rec_data[i] != index_r5[i])
                {
                    return id;
                }
            }
            break;
        case 6:
            for(i = 0;i<index_r6_num;i++)
            {
                if(rec_data[i] != index_r6[i])
                {
                    return id;
                }
            }
            break;

       case 7:
            for(i = 0;i<index_r7_num;i++)
            {
                if(rec_data[i] != index_r7[i])
                {
                    return id;
                }
            }
            break;
        case 8:
            for(i = 0;i<index_r8_num;i++)
            {
                if(rec_data[i] != index_r8[i])
                {
                    return id;
                }
            }
            break;
        case 9:
            for(i = 0;i<index_r9_num;i++)
            {
                if(rec_data[i] != index_r9[i])
                {
                    return id;
                }
            }
            break;
        case 10:
            for(i = 0;i<index_r10_num;i++)
            {
                if(rec_data[i] != index_r10[i])
                {
                    return id;
                }
            }
            break;
        case 11:
            for(i = 0;i<index_r11_num;i++)
            {
                if(rec_data[i] != index_r11[i])
                {
                    return id;
                }
            }
            break;
        case 12:
            //SendData2(0x11);
            //for(i = 0;i < index_r12_num;i++)
           //     SendData2(index_r12[i]);
            for(i = 0;i<index_r12_num;i++)
            {
                if(rec_data[i] != index_r12[i])
                {
                    return id;
                }
            }
            break;

       case 13:
            for(i = 0;i<index_r13_num;i++)
            {
                if(rec_data[i] != index_r13[i])
                {
                    return id;
                }
            }
            break;
        case 14:
            for(i = 0;i<index_r14_num;i++)
            {
                if(rec_data[i] != index_r14[i])
                {
                    return id;
                }
            }
            break;

       case 15:
            for(i = 0;i<index_r15_num;i++)
            {
                if(rec_data[i] != index_r15[i])
                {
                    return id;
                }
            }
            break;
        case 16:
            for(i = 0;i<index_r16_num;i++)
            {
                if(rec_data[i] != index_r16[i])
                {
                    return id;
                }
            }
            break;

       case 17:
            //SendData2(0xcc);
            //for(i = 0;i < index_r17_num;i++)
            //    SendData2(rec_data[i]);
            //for(i = 0;i < index_r17_num;i++)
           //     SendData2(index_r17[i]);
            for(i = 0;i<index_r17_num;i++)
            {
                if(rec_data[i] != index_r17[i])
                {
                    //SendData2(0xbb);
                   // SendData2(i);
                    return id;
                }
            }
            break;
        case 18:
            for(i = 0;i<index_r18_num;i++)
            {
                if(rec_data[i] != index_r18[i])
                {
                    return id;
                }
            }
            break;
        case 19:
            for(i = 0;i<index_r19_num;i++)
            {
                if(rec_data[i] != index_r19[i])
                {
                    return id;
                }
            }
            break;
        case 20:
            
            //for(i = 0;i<index_r20_num;i++)
            {
               // if(rec_data[i] != index_r20[i])
                {
                    return 0;
                }
            }
            break;
        case 21:
            for(i = 0;i<index_r21_num;i++)
            {
                if(rec_data[i] != index_r21[i])
                {
                    return id;
                }
            }
            break;
        case 22:
            for(i = 0;i<index_r22_num;i++)
            {
                if(rec_data[i] != index_r22[i])
                {

                    return id;
                }
            }
            break;

       case 23:
            for(i = 0;i<index_r23_num;i++)
            {
                if(rec_data[i] != index_r23[i])
                {
                    return id;
                }
            }
            break;
        case 24:
            for(i = 0;i<index_r24_num;i++)
            {
                if(rec_data[i] != index_r24[i])
                {
                    return id;
                }
            }
            break;

        
       case 25:
            for(i = 0;i<index_r25_num;i++)
            {
                if(rec_data[i] != index_r25[i])
                {
                    return id;
                }
            }
            break;
        case 26:
            for(i = 0;i<index_r26_num;i++)
            {
                if(rec_data[i] != index_r26[i])
                {
                    return id;
                }
            }
            break;

       case 27:
            for(i = 0;i<index_r27_num;i++)
            {
                if(rec_data[i] != index_r27[i])
                {
                    return id;
                }
            }
            break;
        case 28:
            for(i = 0;i<index_r28_num;i++)
            {
                if(rec_data[i] != index_r28[i])
                {
                    return id;
                }
            }
            break;
        case 29:
            for(i = 0;i<index_r29_num;i++)
            {
                if(rec_data[i] != index_r29[i])
                {
                    return id;
                }
            }
            break;
        case 30:
            for(i = 0;i<index_r30_num;i++)
            {
                if(rec_data[i] != index_r30[i])
                {
                    return id;
                }
            }
            break;
        case 31:
            //for(i = 0;i<index_r31_num;i++)
            {
               // if(rec_data[i] != index_r31[i])
                {
                    return 0;
                }
            }
            break;
        case 32:
            for(i = 0;i<index_r32_num;i++)
            {
                if(rec_data[i] != index_r32[i])
                {
                    return id;
                }
            }
            break;

       case 33:
            for(i = 0;i<index_r33_num;i++)
            {
                if(rec_data[i] != index_r33[i])
                {
                    return id;
                }
            }
            break;
 
        default :
            return 0;
            break;
         
    }
    

    return 0;
}


void indyInitCh875()
{
	
 // ------------------- connect -----------------------------------------------------
	SendData(0xC0);SendData(0x06);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
	Delay20ms();

	SendData(0x40);SendData(0x03);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
	Delay20ms();
		 
	SendData(0x00);SendData(0x00);SendData(0x01);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
	Delay20ms();
	
	SendData(0x00);SendData(0x00);SendData(0x10);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
	Delay20ms();
	
	SendData(0x00);SendData(0x00);SendData(0x05);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
	Delay20ms();
	
	SendData(0x00);SendData(0x00);SendData(0x06);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
	Delay20ms();
	
	SendData(0x00);SendData(0x00);SendData(0x10);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
	Delay20ms();

	SendData(0x00);SendData(0x00);SendData(0x11);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
	Delay20ms();
	
	SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
	Delay20ms();
	
	SendData(0x00);SendData(0x00);SendData(0x01);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
	Delay20ms();

	SendData(0x00);SendData(0x00);SendData(0x02);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
	Delay20ms();
	
	SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x03);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
	Delay20ms();
	
	SendData(0x00);SendData(0x00);SendData(0x02);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
	Delay20ms();
	
	SendData(0x00);SendData(0x00);SendData(0x61);SendData(0x1B);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
	Delay20ms();

// -------------------------- set power -------------------------------------

SendData(0x01);SendData(0x00);SendData(0x01);SendData(0x07);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
SendData(0x00);SendData(0x00);SendData(0x05);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
Delay20ms();

SendData(0x00);SendData(0x00);SendData(0x02);SendData(0x07);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
Delay20ms();

SendData(0x01);SendData(0x00);SendData(0x02);SendData(0x07);SendData(0x01);SendData(0x00);SendData(0x00);SendData(0x00);
SendData(0x01);SendData(0x00);SendData(0x01);SendData(0x07);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
SendData(0x00);SendData(0x00);SendData(0x05);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
Delay20ms();

SendData(0x00);SendData(0x00);SendData(0x04);SendData(0x07);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
Delay20ms();

SendData(0x00);SendData(0x00);SendData(0x05);SendData(0x07);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
Delay20ms();

SendData(0x00);SendData(0x00);SendData(0x06);SendData(0x07);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
Delay20ms();

SendData(0x00);SendData(0x00);SendData(0x07);SendData(0x07);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
Delay20ms();

SendData(0x00);SendData(0x00);SendData(0x12);SendData(0x0b);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
Delay20ms();

SendData(0x01);SendData(0x00);SendData(0x01);SendData(0x07);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
SendData(0x00);SendData(0x00);SendData(0x05);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
Delay20ms();

SendData(0x01);SendData(0x00);SendData(0x04);SendData(0x07);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
SendData(0x01);SendData(0x00);SendData(0x05);SendData(0x07);SendData(0xD0);SendData(0x07);SendData(0x00);SendData(0x00);Delay20ms();

SendData(0x01);SendData(0x00);SendData(0x06);SendData(0x07);				   // power
SendData(0x2c);SendData(0x01);		//300  30dbm
bleState.power[0] = 0x01;
bleState.power[1] = 0x2c;
SendData(0x00);SendData(0x00);Delay20ms();

SendData(0x01);SendData(0x00);SendData(0x07);SendData(0x07);SendData(0x00);SendData(0x20);SendData(0x00);SendData(0x00);Delay20ms();
SendData(0x01);SendData(0x00);SendData(0x12);SendData(0x0B);SendData(0xff);SendData(0xFF);SendData(0x0f);SendData(0x00);Delay20ms();



// -------------------------- set channel 875 --------------------------------
	
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x01); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
// 875M
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0xac); SendData(0x0d); SendData(0x18); SendData(0x00);Delay20ms();
// 845M
// SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x34); SendData(0x0d); SendData(0x18); SendData(0x00);Delay20ms();
bleState.fre[0] = 0;
bleState.fre[1] = 1;
bleState.fre[2] = (875 & 0xff00) >> 8;
bleState.fre[3] = 875 & 0xff;

SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
	
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x01); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x84); SendData(0x0d); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
			
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x02); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x98); SendData(0x0d); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
	
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x03); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0xac); SendData(0x0d); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x04); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0xc0); SendData(0x0d); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
	
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x05); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0xd4); SendData(0x0d); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
			
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x06); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0xe8); SendData(0x0d); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
			
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x07); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0xfc); SendData(0x0d); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
																					  
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x08); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x10); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
			
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x09); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x24); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
																									  
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x0a); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x38); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
			
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x0b); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x4c); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
			
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
			
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x0d); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
			
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x0e); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
			
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x0f); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
			
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x10); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
	
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x11); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
	
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x12); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
	
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x13); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
	
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x14); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
	
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x15); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
	
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x16); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
	
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x17); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
	
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x18); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
	
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x19); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
	
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x1a); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
	
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x1b); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
	
SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x1c); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x1d); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x1e); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x1f); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x20); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x21); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x22); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x23); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x24); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x25); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x26); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x27); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x28); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x29); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x2a); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x2b); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x2c); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x2d); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x2e); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x2f); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x30); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();

SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); SendData(0x31); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); SendData(0x00); SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0x60); SendData(0x0e); SendData(0x18); SendData(0x00);Delay20ms();
SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();


}

/*
	channel : 通道编号
	state : 打开或关闭； 0 关闭， 1 打开
	fre : 设置的频率
*/
void indySetFre(unsigned char channel, unsigned char state, unsigned int fre)
{
	unsigned char left = 0;
	unsigned char right = 0;
	int sum = 0;
	if(fre < 820)
		fre = 820;
	if(fre > 955)
		fre = 955;

	if(channel > 31)
		channel = 31;
	
	
	sum = fre * 4;
	left = (sum & 0xff00) >> 8;
	right = sum & 0xff;
	
	SendData(0x01); SendData(0x00); SendData(0x01); SendData(0x0c); 
	
	SendData(channel); 	// 设置通道

	SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
	SendData(0x01); SendData(0x00); SendData(0x02); SendData(0x0c); 

	SendData(state); 

	SendData(0x00); SendData(0x00); SendData(0x00);Delay20ms();
// 875M
//SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); SendData(0xac); SendData(0x0d); SendData(0x18); SendData(0x00);Delay20ms();
	SendData(0x01); SendData(0x00); SendData(0x03); SendData(0x0c); 

	SendData(right); SendData(left); 	// left|right = fr * 4
	bleState.fre[0] = channel;
	bleState.fre[1] = state;
	bleState.fre[2] = (fre & 0xff00) >> 8;
	bleState.fre[3] = fre & 0xff;

	SendData(0x18); SendData(0x00);Delay20ms();
	SendData(0x01); SendData(0x00); SendData(0x04); SendData(0x0c); SendData(0x00); SendData(0x01); SendData(0x07); SendData(0x14);Delay20ms();
}


// 设置功率
void indySetPower(unsigned int pow)
{
	unsigned char high8 = 0;
	unsigned char low8 = 0;
	
	if(pow > 300)
		pow = 300;
	
	//Power设定方式为DATA中第5, 6位，power * 10
	high8 = (pow & 0xff00 ) >> 8;
	low8 = pow & 0xff;
	
	
	SendData(0x01);SendData(0x00);SendData(0x04);SendData(0x07);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x05);SendData(0x07);SendData(0xD0);SendData(0x07);SendData(0x00);SendData(0x00);Delay20ms();
	
	SendData(0x01);SendData(0x00);SendData(0x06);SendData(0x07);				   // power
	SendData(low8);SendData(high8);		//300  30dbm
	bleState.power[0] = high8;
	bleState.power[1] = low8;
	SendData(0x00);SendData(0x00);Delay20ms();
	
	SendData(0x01);SendData(0x00);SendData(0x07);SendData(0x07);SendData(0x00);SendData(0x20);SendData(0x00);SendData(0x00);Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x12);SendData(0x0B);SendData(0xff);SendData(0xFF);SendData(0x0f);SendData(0x00);Delay20ms();
}


// 设置标定信息
void indySetBD(uint16 code1, uint16 temp1, uint16 code2, uint16 temp2, uint16 vl){
	unsigned char addr9[2] = {0};
	unsigned char addra[2] = {0};
	unsigned char addrb[2] = {0};

	addr9[0] = (code1 & 0x0ff0) >> 4;	// 地址9的高位
	addr9[1] = ((code1 & 0x0f) << 4) | ((temp1 & 0x0780) >> 7); 	// 地址9的低位
	addra[0] = ((temp1 & 0x7f) << 1) | ((code2 & 0x0800) >> 11);	// 地址a的高位
	addra[1] = (code2 & 0x07f8) >> 3;	// 地址a的低位
	addrb[0] = ((code2 & 0x07) << 5) | ((temp2 & 0x07c0) >> 6);	// 地址b的高位
	addrb[1] = ((temp2 & 0x3f) << 2) | (vl & 0x03);	// 地址b的低位

	SendData(0x01);SendData(0x00);SendData(0x03);SendData(0x0a);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x01);SendData(0x0a);SendData(0x07);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x02);SendData(0x0a);SendData(0x03);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x04);SendData(0x0a);SendData(0x01);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x08);SendData(0x0a);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();

	SendData(0x01);SendData(0x00);SendData(0x09);SendData(0x0a);SendData(addr9[1]);SendData(addr9[0]);SendData(0x09);SendData(0x00);Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x06);SendData(0x0a);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x00);SendData(0xf0);SendData(0x11);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
	
	SendData(0x01);SendData(0x00);SendData(0x03);SendData(0x0a);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x01);SendData(0x0a);SendData(0x07);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x02);SendData(0x0a);SendData(0x03);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x04);SendData(0x0a);SendData(0x01);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x08);SendData(0x0a);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();

	SendData(0x01);SendData(0x00);SendData(0x09);SendData(0x0a);SendData(addra[1]);SendData(addra[0]);SendData(0x0a);SendData(0x00);Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x06);SendData(0x0a);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x00);SendData(0xf0);SendData(0x11);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
	
	SendData(0x01);SendData(0x00);SendData(0x03);SendData(0x0a);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x01);SendData(0x0a);SendData(0x07);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x02);SendData(0x0a);SendData(0x03);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x04);SendData(0x0a);SendData(0x01);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x08);SendData(0x0a);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
/*
	SendData(0x01);SendData(0x00);SendData(0x09);SendData(0x0a);SendData(addrb[1]);SendData(addrb[0]);SendData(0x0b);SendData(0x00);
	SendData(0x01);SendData(0x00);SendData(0x06);SendData(0x0a);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
	SendData(0x01);SendData(0x00);SendData(0x00);SendData(0xf0);SendData(0x11);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
*/
	SendData(0x01);SendData(0x00);SendData(0x09);SendData(0x0a);SendData(addrb[1]);SendData(addrb[0]);SendData(0x0B);SendData(0x00);Delay20ms();
	SendData(0xf8);SendData(0x01);SendData(0x00);SendData(0x06);SendData(0x0a);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
	SendData(0x00);SendData(0x00);SendData(0x08);SendData(0x0a);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x00);SendData(0xf0);SendData(0x11);SendData(0x00);SendData(0x00);SendData(0x00);Delay20ms();

}


// 读取标定信息
void indyGetBD(){
	clear_rec_data();

	SendData(0x01);SendData(0x00);SendData(0x11);SendData(0x09);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
	//Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x02);SendData(0x0a);SendData(0x03);SendData(0x00);SendData(0x00);SendData(0x00);
	//Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x03);SendData(0x0a);SendData(0x09);SendData(0x00);SendData(0x00);SendData(0x00);
	//Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x04);SendData(0x0a);SendData(0x03);SendData(0x00);SendData(0x00);SendData(0x00);
	//Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x06);SendData(0x0a);SendData(0x00);SendData(0x00);SendData(0x00);SendData(0x00);
	//Delay20ms();
	SendData(0x01);SendData(0x00);SendData(0x00);SendData(0xf0);SendData(0x10);SendData(0x00);SendData(0x00);SendData(0x00);
	Delay20ms();

	if(rec_data[0]	== 0x01	)
	{	       	
		Delay20ms();
		ES = 0;
		process_packets();	 //处理数据
		ES = 1;
	}	

	
}
