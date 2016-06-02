#include "stc15_flash.h"

//-----------------------------------------------

#define CMD_IDLE    0               //空闲模式
#define CMD_READ    1               //IAP字节读命令
#define CMD_PROGRAM 2               //IAP字节编程命令
#define CMD_ERASE   3               //IAP扇区擦除命令


//#define ENABLE_IAP 0x80           //if SYSCLK<30MHz
#define ENABLE_IAP 0x81           //if SYSCLK<24MHz
//#define ENABLE_IAP  0x82            //if SYSCLK<20MHz
//#define ENABLE_IAP 0x83           //if SYSCLK<12MHz
//#define ENABLE_IAP 0x84           //if SYSCLK<6MHz
//#define ENABLE_IAP 0x85           //if SYSCLK<3MHz
//#define ENABLE_IAP 0x86           //if SYSCLK<2MHz
//#define ENABLE_IAP 0x87           //if SYSCLK<1MHz

//测试地址
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
#define USED_BYTE_QTY_IN_ONE_SECTOR     32      //由于STC15F104W只有128字节RAM,所有缓冲区不可定义太大
//#define USED_BYTE_QTY_IN_ONE_SECTOR   64

#define DEBUG_Data_Memory_Begin_Sector_addr     0x0400

BYTE idata protect_buffer[USED_BYTE_QTY_IN_ONE_SECTOR];

/*----------------------------
关闭IAP
----------------------------*/
void IapIdle()
{
    IAP_CONTR = 0;                  //关闭IAP功能
    IAP_CMD = 0;                    //清除命令寄存器
    IAP_TRIG = 0;                   //清除触发寄存器
    IAP_ADDRH = 0x80;               //将地址设置到非IAP区域
    IAP_ADDRL = 0;
}

/*----------------------------
从ISP/IAP/EEPROM区域读取一字节
----------------------------*/
BYTE IapReadByte(WORD addr)
{
    BYTE dat;                       //数据缓冲区

    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_READ;             //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成
    dat = IAP_DATA;                 //读ISP/IAP/EEPROM数据
    IapIdle();                      //关闭IAP功能

    return dat;                     //返回
}

/*----------------------------
写一字节数据到ISP/IAP/EEPROM区域
----------------------------*/
void IapProgramByte(WORD addr, BYTE dat)
{
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_PROGRAM;          //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_DATA = dat;                 //写ISP/IAP/EEPROM数据
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成
    IapIdle();
}

/*----------------------------
扇区擦除
----------------------------*/
void IapEraseSector(WORD addr)
{
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_ERASE;            //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成
    IapIdle();
}

/* 写数据进 数据Flash存储器, 只在同一个扇区内写，不保留原有数据 */
/* begin_addr,被写数据Flash开始地址；counter,连续写多少个字节； array[]，数据来源   */
BYTE sequential_write_flash_in_one_sector(WORD begin_addr, WORD counter, BYTE array[])
{
    WORD i = 0;
    WORD in_sector_begin_addr = 0;
    WORD sector_addr = 0;

    /* 判是否是有效范围,此函数不允许跨扇区操作 */
    if(counter > USED_BYTE_QTY_IN_ONE_SECTOR)
        return  ERROR;
    in_sector_begin_addr = begin_addr & 0x01ff;
    if((in_sector_begin_addr + counter) > USED_BYTE_QTY_IN_ONE_SECTOR)
        return ERROR;

    /* 擦除 要修改/写入 的扇区 */
    IapEraseSector(begin_addr);

    for(i=0; i<counter; i++)
    {
        /* 写一个字节 */
        IapProgramByte(begin_addr, array[i]);
        /*  比较对错 */
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

/* 写数据进数据Flash存储器(EEPROM), 只在同一个扇区内写，保留同一扇区中不需修改的数据    */
/* begin_addr,被写数据Flash开始地址；counter,连续写多少个字节； array[]，数据来源       */
BYTE write_flash_with_protect_in_one_sector(WORD begin_addr, WORD counter, BYTE array[])
{
    WORD i = 0;
    WORD in_sector_begin_addr = 0;
    WORD sector_addr = 0;
    WORD byte_addr = 0;

    /* 判是否是有效范围,此函数不允许跨扇区操作 */
    if(counter > USED_BYTE_QTY_IN_ONE_SECTOR)
        return ERROR;
    in_sector_begin_addr = begin_addr & 0x01ff;
    /* 假定从扇区的第0个字节开始，到USED_BYTE_QTY_IN_ONE_SECTOR-1个字节结束,后面部分不用,程序易编写 */
    if((in_sector_begin_addr + counter) > USED_BYTE_QTY_IN_ONE_SECTOR)
        return ERROR;

    /* 将该扇区数据 0 - (USED_BYTE_QTY_IN_ONE_SECTOR-1) 字节数据读入缓冲区保护 */
    sector_addr = (begin_addr & 0xfe00);
    byte_addr = sector_addr;

    for(i = 0; i < USED_BYTE_QTY_IN_ONE_SECTOR; i++)
    {
        protect_buffer[i] = IapReadByte(byte_addr++);
    }

    /* 将要写入的数据写入保护缓冲区的相应区域,其余部分保留 */
    for(i = 0; i < counter; i++)
    {
        protect_buffer[in_sector_begin_addr++] = array[i];
    }

    /* 擦除 要修改/写入 的扇区 */
    IapEraseSector(sector_addr);

    /* 将保护缓冲区的数据写入 Data Flash, EEPROM */
    byte_addr = sector_addr;
    for(i = 0; i< USED_BYTE_QTY_IN_ONE_SECTOR; i++)
    {
        /* 写一个字节 */
        IapProgramByte(byte_addr, protect_buffer[i]);
        /*  比较对错 */
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
