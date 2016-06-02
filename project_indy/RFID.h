	typedef char           	INT8S;	/* Signed    8 bit quantity        */
	typedef unsigned char  	INT8U;   /* Unsigned  8 bit quantity        */
    typedef          int   	INT16S;  /* Signed   16 bit quantity        */
	typedef unsigned int   	INT16U;  /* Unsigned 16 bit quantity        */
	typedef          long  	INT32S;  /* Signed   32 bit quantity        */
	typedef unsigned long  	INT32U;  /* Unsigned 32 bit quantity        */
	typedef double		INT64S;		/* Double precision floating point */
	typedef double    INT64U;     /* Double precision floating point */
	typedef float    	BOOL32;     /* Single precision floating point */
	typedef float	   HANDLE32;
	typedef double   	HANDLE64;

/******************************************************************************
 * Name:  RFID_RFID_PACKET_COMMON - The common packet preamble that contains
 *       fields that are common to all packets.
 ******************************************************************************/
typedef struct hostpkt_cmn
{
    /* Packet specific version number                                         */
    INT8U   pkt_ver;
    /* Packet specific flags*/
    INT8U   flags;
    /* Packet type identifier                                                 */
    INT16U  pkt_type;
    /* Packet length indicator - number of 32-bit words that follow the common*/
    /* packet preamble (i.e., this struct)                                    */
    INT16U  pkt_len;
    /* Reserved for future use                                                */
    INT16U  res0;
} RFID_PACKET_COMMON;

/******************************************************************************
 * Name:  RFID_PACKET_COMMAND_BEGIN - The command-begin packet.
 ******************************************************************************/
typedef struct hostpkt_cmd_beg
{
  /* Common preamble - part of every packet!                                  */
  RFID_PACKET_COMMON    cmn;
  /* The command for which the packet sequence is in response to              */
  INT32U                command;
  /* Current millisecond timer/counter                                        */
  INT32U                ms_ctr;
} RFID_PACKET_COMMAND_BEGIN;


/******************************************************************************
 * Name:  RFID_PACKET_COMMAND_END - The command-end packet.
 ******************************************************************************/
typedef struct hostpkt_cmd_end
{
  /* Common preamble - part of every packet!                                  */
  RFID_PACKET_COMMON    cmn;
  /* Current millisecond timer/counter                                        */
  INT32U                ms_ctr;
  /* Command status indicator                                                 */
  INT32U                status;
} RFID_PACKET_COMMAND_END;


/******************************************************************************
 * Name:  RFID_PACKET_18K6C_INVENTORY - The ISO 18000-6C inventory packet.
 ******************************************************************************/
typedef struct hostpkt_18k6c_inv
{
  /* Common preamble - part of every packet!                                  */
  RFID_PACKET_COMMON    cmn;
  /* current millisecond timer/counter                                        */
  INT32U                ms_ctr;
  /* Receive Signal Strength Indicator - backscattered tab signal */
  /* amplitude.                                                               */
  INT8U                 nb_rssi;
  INT8U                 wb_rssi;
  INT16U                ana_ctrl1;
  /* Reserved                                                                 */
  INT16U                rssi;
  INT16U                res0;
  /* Variable length inventory data (i.e., PC, EPC, and CRC)                  */
  INT32U                inv_data[1];
} RFID_PACKET_18K6C_INVENTORY;

/******************************************************************************
 * Name:  RFID_PACKET_18K6C_TAG_ACCESS - The ISO 18000-6C tag-access packet.
 ******************************************************************************/
typedef struct hostpkt_18k6c_acc
{
  /* Common preamble - part of every packet!                                  */
  RFID_PACKET_COMMON    cmn;
  /* Current millisecond timer/counter                                        */
  INT32U                ms_ctr;
  /* ISO 18000-6C access command                                              */
  INT8U                 command;
  /* Backscatter error code from tag access                                   */
  INT8U                 tag_error_code;
  /* Protocol attributed error code from tag access                                */
  INT16U                prot_error_code;
  /* The number of words successfully written; Write and BlockWrite only      */
  INT16U                write_word_count;
  /* Reserved                                                                 */
  INT16U                res0;
  /* Variable length access data                                              */
  INT32U                acc_data[1];
} RFID_PACKET_18K6C_TAG_ACCESS;

