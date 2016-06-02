#include "stc15_flash.h"

//-----------------------------------------------

#define CMD_IDLE    0               //����ģʽ
#define CMD_READ    1               //IAP�ֽڶ�����
#define CMD_PROGRAM 2               //IAP�ֽڱ������
#define CMD_ERASE   3               //IAP������������


//#define ENABLE_IAP 0x80           //if SYSCLK<30MHz
#define ENABLE_IAP 0x81           //if SYSCLK<24MHz
//#define ENABLE_IAP  0x82            //if SYSCLK<20MHz
//#define ENABLE_IAP 0x83           //if SYSCLK<12MHz
//#define ENABLE_IAP 0x84           //if SYSCLK<6MHz
//#define ENABLE_IAP 0x85           //if SYSCLK<3MHz
//#define ENABLE_IAP 0x86           //if SYSCLK<2MHz
//#define ENABLE_IAP 0x87           //if SYSCLK<1MHz

//���Ե�ַ
#define IAP_ADDRESS 0x0400


BYTE TBUF,RBUF;
BYTE TDAT,RDAT;
BYTE TCNT,RCNT;
BYTE TBIT,RBIT;
BOOL TING,RING;
BOOL TEND,REND;


//#define USED_BYTE_QTY_IN_ONE_SECTOR   1
//#define USED_BYTE_QTY_IN_ONE_SECTOR   2
//#define USED_BYTE_QTY_IN_ONE_SECTOR   4
//#define USED_BYTE_QTY_IN_ONE_SECTOR   8
//#define USED_BYTE_QTY_IN_ONE_SECTOR   16
#define USED_BYTE_QTY_IN_ONE_SECTOR     32      //����STC15F104Wֻ��128�ֽ�RAM,���л��������ɶ���̫��
//#define USED_BYTE_QTY_IN_ONE_SECTOR   64

#define DEBUG_Data_Memory_Begin_Sector_addr     0x0400

BYTE idata protect_buffer[USED_BYTE_QTY_IN_ONE_SECTOR];

/*----------------------------
�ر�IAP
----------------------------*/
void IapIdle()
{
    IAP_CONTR = 0;                  //�ر�IAP����
    IAP_CMD = 0;                    //�������Ĵ���
    IAP_TRIG = 0;                   //��������Ĵ���
    IAP_ADDRH = 0x80;               //����ַ���õ���IAP����
    IAP_ADDRL = 0;
}

/*----------------------------
��ISP/IAP/EEPROM�����ȡһ�ֽ�
----------------------------*/
BYTE IapReadByte(WORD addr)
{
    BYTE dat;                       //���ݻ�����

    IAP_CONTR = ENABLE_IAP;         //ʹ��IAP
    IAP_CMD = CMD_READ;             //����IAP����
    IAP_ADDRL = addr;               //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;          //����IAP�ߵ�ַ
    IAP_TRIG = 0x5a;                //д��������(0x5a)
    IAP_TRIG = 0xa5;                //д��������(0xa5)
    _nop_();                        //�ȴ�ISP/IAP/EEPROM�������
    dat = IAP_DATA;                 //��ISP/IAP/EEPROM����
    IapIdle();                      //�ر�IAP����

    return dat;                     //����
}

/*----------------------------
дһ�ֽ����ݵ�ISP/IAP/EEPROM����
----------------------------*/
void IapProgramByte(WORD addr, BYTE dat)
{
    IAP_CONTR = ENABLE_IAP;         //ʹ��IAP
    IAP_CMD = CMD_PROGRAM;          //����IAP����
    IAP_ADDRL = addr;               //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;          //����IAP�ߵ�ַ
    IAP_DATA = dat;                 //дISP/IAP/EEPROM����
    IAP_TRIG = 0x5a;                //д��������(0x5a)
    IAP_TRIG = 0xa5;                //д��������(0xa5)
    _nop_();                        //�ȴ�ISP/IAP/EEPROM�������
    IapIdle();
}

/*----------------------------
��������
----------------------------*/
void IapEraseSector(WORD addr)
{
    IAP_CONTR = ENABLE_IAP;         //ʹ��IAP
    IAP_CMD = CMD_ERASE;            //����IAP����
    IAP_ADDRL = addr;               //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;          //����IAP�ߵ�ַ
    IAP_TRIG = 0x5a;                //д��������(0x5a)
    IAP_TRIG = 0xa5;                //д��������(0xa5)
    _nop_();                        //�ȴ�ISP/IAP/EEPROM�������
    IapIdle();
}

/* д���ݽ� ����Flash�洢��, ֻ��ͬһ��������д��������ԭ������ */
/* begin_addr,��д����Flash��ʼ��ַ��counter,����д���ٸ��ֽڣ� array[]��������Դ   */
BYTE sequential_write_flash_in_one_sector(WORD begin_addr, WORD counter, BYTE array[])
{
    WORD i = 0;
    WORD in_sector_begin_addr = 0;
    WORD sector_addr = 0;

    /* ���Ƿ�����Ч��Χ,�˺������������������ */
    if(counter > USED_BYTE_QTY_IN_ONE_SECTOR)
        return  ERROR;
    in_sector_begin_addr = begin_addr & 0x01ff;
    if((in_sector_begin_addr + counter) > USED_BYTE_QTY_IN_ONE_SECTOR)
        return ERROR;

    /* ���� Ҫ�޸�/д�� ������ */
    IapEraseSector(begin_addr);

    for(i=0; i<counter; i++)
    {
        /* дһ���ֽ� */
        IapProgramByte(begin_addr, array[i]);
        /*  �Ƚ϶Դ� */
        if (IapReadByte(begin_addr) != array[i])
        {
            IapIdle();
            return ERROR;
        }
        begin_addr++;
    }
    IapIdle();
    return  OK;
}

/* д���ݽ�����Flash�洢��(EEPROM), ֻ��ͬһ��������д������ͬһ�����в����޸ĵ�����    */
/* begin_addr,��д����Flash��ʼ��ַ��counter,����д���ٸ��ֽڣ� array[]��������Դ       */
BYTE write_flash_with_protect_in_one_sector(WORD begin_addr, WORD counter, BYTE array[])
{
    WORD i = 0;
    WORD in_sector_begin_addr = 0;
    WORD sector_addr = 0;
    WORD byte_addr = 0;

    /* ���Ƿ�����Ч��Χ,�˺������������������ */
    if(counter > USED_BYTE_QTY_IN_ONE_SECTOR)
        return ERROR;
    in_sector_begin_addr = begin_addr & 0x01ff;
    /* �ٶ��������ĵ�0���ֽڿ�ʼ����USED_BYTE_QTY_IN_ONE_SECTOR-1���ֽڽ���,���沿�ֲ���,�����ױ�д */
    if((in_sector_begin_addr + counter) > USED_BYTE_QTY_IN_ONE_SECTOR)
        return ERROR;

    /* ������������ 0 - (USED_BYTE_QTY_IN_ONE_SECTOR-1) �ֽ����ݶ��뻺�������� */
    sector_addr = (begin_addr & 0xfe00);
    byte_addr = sector_addr;

    for(i = 0; i < USED_BYTE_QTY_IN_ONE_SECTOR; i++)
    {
        protect_buffer[i] = IapReadByte(byte_addr++);
    }

    /* ��Ҫд�������д�뱣������������Ӧ����,���ಿ�ֱ��� */
    for(i = 0; i < counter; i++)
    {
        protect_buffer[in_sector_begin_addr++] = array[i];
    }

    /* ���� Ҫ�޸�/д�� ������ */
    IapEraseSector(sector_addr);

    /* ������������������д�� Data Flash, EEPROM */
    byte_addr = sector_addr;
    for(i = 0; i< USED_BYTE_QTY_IN_ONE_SECTOR; i++)
    {
        /* дһ���ֽ� */
        IapProgramByte(byte_addr, protect_buffer[i]);
        /*  �Ƚ϶Դ� */
        if (IapReadByte(begin_addr) != protect_buffer[i])
        {
            IapIdle();
            return ERROR;
        }
        byte_addr++;
    }
    IapIdle();
    return OK;
}
