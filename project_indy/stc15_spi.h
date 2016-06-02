#ifndef __STD15_SPI_H__
#define __STD15_SPI_H__

#include "IAP15.h"

#define SPI_S0          0x04
#define SPI_S1          0x08

#define SPIF            0x80                    //SPSTAT.7
#define WCOL            0x40                    //SPSTAT.6

#define SSIG            0x80                    //SPCTL.7
#define SPEN            0x40                    //SPCTL.6
#define DORD            0x20                    //SPCTL.5
#define MSTR            0x10                    //SPCTL.4
#define CPOL            0x08                    //SPCTL.3
#define CPHA            0x04                    //SPCTL.2
#define SPDHH           0x00                    //CPU_CLK/4
#define SPDH            0x01                    //CPU_CLK/16
#define SPDL            0x02                    //CPU_CLK/64
#define SPDLL           0x03                    //CPU_CLK/128

sbit SS             =   P1^2;                   //SPIµÄSS½Å

#define SetSpiInt {IE2 |= ESPI; EA = 1;}

#endif
