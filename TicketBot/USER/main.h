/**
  ******************************************************************************
  * @file    Templates/Inc/main.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-April-2016
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#ifndef __GPS_H
#define __GPS_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEK STM32F103¿ª·¢°å
//ATK-S1216F8 GPSÄ£¿éÇý¶¯´úÂë	   
//ÕýµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//ÐÞ¸ÄÈÕÆÚ:2015/04/11
//°æ±¾£ºV1.0
//°æÈ¨ËùÓÐ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ¹ãÖÝÊÐÐÇÒíµç×Ó¿Æ¼¼ÓÐÏÞ¹«Ë¾ 2014-2024
//All rights reserved							  
////////////////////////////////////////////////////////////////////////////////// 	

//GPS NMEA-0183Ð­ÒéÖØÒª²ÎÊý½á¹¹Ìå¶¨Òå 
//ÎÀÐÇÐÅÏ¢
__packed typedef struct  
{										    
 	u8 num;		//ÎÀÐÇ±àºÅ
	u8 eledeg;	//ÎÀÐÇÑö½Ç
	u16 azideg;	//ÎÀÐÇ·½Î»½Ç
	u8 sn;		//ÐÅÔë±È		   
}nmea_slmsg; 
//±±¶· NMEA-0183Ð­ÒéÖØÒª²ÎÊý½á¹¹Ìå¶¨Òå 
//ÎÀÐÇÐÅÏ¢
__packed typedef struct  
{	
 	u8 beidou_num;		//ÎÀÐÇ±àºÅ
	u8 beidou_eledeg;	//ÎÀÐÇÑö½Ç
	u16 beidou_azideg;	//ÎÀÐÇ·½Î»½Ç
	u8 beidou_sn;		//ÐÅÔë±È		   
}beidou_nmea_slmsg; 

//UTCÊ±¼äÐÅÏ¢
__packed typedef struct  
{										    
 	u16 year;	//Äê·Ý
	u8 month;	//ÔÂ·Ý
	u8 date;	//ÈÕÆÚ
	u8 hour; 	//Ð¡Ê±
	u8 min; 	//·ÖÖÓ
	u8 sec; 	//ÃëÖÓ
}nmea_utc_time;   	   
//NMEA 0183 Ð­Òé½âÎöºóÊý¾Ý´æ·Å½á¹¹Ìå
__packed typedef struct  
{										    
 	u8 svnum;					//¿É¼ûGPSÎÀÐÇÊý
	u8 beidou_svnum;					//¿É¼û±±¶·ÎÀÐÇÊý
	nmea_slmsg slmsg[12];		//×î¶à12¿ÅGPSÎÀÐÇ
	beidou_nmea_slmsg beidou_slmsg[12];		//ÔÝÇÒËã×î¶à12¿Å±±¶·ÎÀÐÇ
	nmea_utc_time utc;			//UTCÊ±¼ä
	u32 latitude;				//Î³¶È ·ÖÀ©´ó100000±¶,Êµ¼ÊÒª³ýÒÔ100000
	u8 nshemi;					//±±Î³/ÄÏÎ³,N:±±Î³;S:ÄÏÎ³				  
	u32 longitude;			    //¾­¶È ·ÖÀ©´ó100000±¶,Êµ¼ÊÒª³ýÒÔ100000
	u8 ewhemi;					//¶«¾­/Î÷¾­,E:¶«¾­;W:Î÷¾­
	u8 gpssta;					//GPS×´Ì¬:0,Î´¶¨Î»;1,·Ç²î·Ö¶¨Î»;2,²î·Ö¶¨Î»;6,ÕýÔÚ¹ÀËã.				  
 	u8 posslnum;				//ÓÃÓÚ¶¨Î»µÄGPSÎÀÐÇÊý,0~12.
 	u8 possl[12];				//ÓÃÓÚ¶¨Î»µÄÎÀÐÇ±àºÅ
	u8 fixmode;					//¶¨Î»ÀàÐÍ:1,Ã»ÓÐ¶¨Î»;2,2D¶¨Î»;3,3D¶¨Î»
	u16 pdop;					//Î»ÖÃ¾«¶ÈÒò×Ó 0~500,¶ÔÓ¦Êµ¼ÊÖµ0~50.0
	u16 hdop;					//Ë®Æ½¾«¶ÈÒò×Ó 0~500,¶ÔÓ¦Êµ¼ÊÖµ0~50.0
	u16 vdop;					//´¹Ö±¾«¶ÈÒò×Ó 0~500,¶ÔÓ¦Êµ¼ÊÖµ0~50.0 

	int altitude;			 	//º£°Î¸ß¶È,·Å´óÁË10±¶,Êµ¼Ê³ýÒÔ10.µ¥Î»:0.1m	 
	u16 speed;					//µØÃæËÙÂÊ,·Å´óÁË1000±¶,Êµ¼Ê³ýÒÔ10.µ¥Î»:0.001¹«Àï/Ð¡Ê±	 
}nmea_msg;
 	////////////////////////////////////////////////////////////////////////////////////////////////////
//SkyTra S1216F8 ÅäÖÃ²¨ÌØÂÊ½á¹¹Ìå
__packed typedef struct
{
	u16 sos;            //Æô¶¯ÐòÁÐ£¬¹Ì¶¨Îª0XA0A1
	u16 PL;             //ÓÐÐ§Êý¾Ý³¤¶È0X0004£» 
	u8 id;             //ID£¬¹Ì¶¨Îª0X05
	u8 com_port;       //COM¿Ú£¬¹Ì¶¨Îª0X00£¬¼´COM1   
	u8 Baud_id;       //²¨ÌØÂÊ£¨0~8,4800,9600,19200,38400,57600,115200,230400,460800,921600£©
	u8 Attributes;     //ÅäÖÃÊý¾Ý±£´æÎ»ÖÃ ,0±£´æµ½SRAM£¬1±£´æµ½SRAM&FLASH£¬2ÁÙÊ±±£´æ
	u8 CS;             //Ð£ÑéÖµ
	u16 end;            //½áÊø·û:0X0D0A  
}SkyTra_baudrate;
//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 ÅäÖÃÊä³öÐÅÏ¢½á¹¹Ìå
__packed typedef struct
{
	u16 sos;            //Æô¶¯ÐòÁÐ£¬¹Ì¶¨Îª0XA0A1
	u16 PL;             //ÓÐÐ§Êý¾Ý³¤¶È0X0009£» 
	u8 id;             //ID£¬¹Ì¶¨Îª0X08
	u8 GGA;            //1~255£¨s£©,0:disable
	u8 GSA;            //1~255£¨s£©,0:disable
	u8 GSV;            //1~255£¨s£©,0:disable
	u8 GLL;            //1~255£¨s£©,0:disable
	u8 RMC;            //1~255£¨s£©,0:disable
	u8 VTG;            //1~255£¨s£©,0:disable
	u8 ZDA;            //1~255£¨s£©,0:disable
	u8 Attributes;     //ÅäÖÃÊý¾Ý±£´æÎ»ÖÃ ,0±£´æµ½SRAM£¬1±£´æµ½SRAM&FLASH£¬2ÁÙÊ±±£´æ
	u8 CS;             //Ð£ÑéÖµ
	u16 end;            //½áÊø·û:0X0D0A  
}SkyTra_outmsg;
//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 ÅäÖÃÎ»ÖÃ¸üÐÂÂÊ½á¹¹Ìå
__packed typedef struct
{
	u16 sos;            //Æô¶¯ÐòÁÐ£¬¹Ì¶¨Îª0XA0A1
	u16 PL;             //ÓÐÐ§Êý¾Ý³¤¶È0X0003£» 
	u8 id;             //ID£¬¹Ì¶¨Îª0X0E
	u8 rate;           //È¡Öµ·¶Î§:1, 2, 4, 5, 8, 10, 20, 25, 40, 50
	u8 Attributes;     //ÅäÖÃÊý¾Ý±£´æÎ»ÖÃ ,0±£´æµ½SRAM£¬1±£´æµ½SRAM&FLASH£¬2ÁÙÊ±±£´æ
	u8 CS;             //Ð£ÑéÖµ
	u16 end;            //½áÊø·û:0X0D0A  
}SkyTra_PosRate;
//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 ÅäÖÃÊä³öÂö³å(PPS)¿í¶È½á¹¹Ìå
__packed typedef struct
{
	u16 sos;            //Æô¶¯ÐòÁÐ£¬¹Ì¶¨Îª0XA0A1
	u16 PL;             //ÓÐÐ§Êý¾Ý³¤¶È0X0007£» 
	u8 id;             //ID£¬¹Ì¶¨Îª0X65
	u8 Sub_ID;         //0X01
	u32 width;        //1~100000(us)
	u8 Attributes;     //ÅäÖÃÊý¾Ý±£´æÎ»ÖÃ ,0±£´æµ½SRAM£¬1±£´æµ½SRAM&FLASH£¬2ÁÙÊ±±£´æ
	u8 CS;             //Ð£ÑéÖµ
	u16 end;            //½áÊø·û:0X0D0A 
}SkyTra_pps_width;
//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 ACK½á¹¹Ìå
__packed typedef struct
{
	u16 sos;            //Æô¶¯ÐòÁÐ£¬¹Ì¶¨Îª0XA0A1
	u16 PL;             //ÓÐÐ§Êý¾Ý³¤¶È0X0002£» 
	u8 id;             //ID£¬¹Ì¶¨Îª0X83
	u8 ACK_ID;         //ACK ID may further consist of message ID and message sub-ID which will become 3 bytes of ACK message
	u8 CS;             //Ð£ÑéÖµ
	u16 end;            //½áÊø·û 
}SkyTra_ACK;
//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 NACK½á¹¹Ìå
__packed typedef struct
{
	u16 sos;            //Æô¶¯ÐòÁÐ£¬¹Ì¶¨Îª0XA0A1
	u16 PL;             //ÓÐÐ§Êý¾Ý³¤¶È0X0002£» 
	u8 id;             //ID£¬¹Ì¶¨Îª0X84
	u8 NACK_ID;         //ACK ID may further consist of message ID and message sub-ID which will become 3 bytes of ACK message
	u8 CS;             //Ð£ÑéÖµ
	u16 end;            //½áÊø·û 
}SkyTra_NACK;


int NMEA_Str2num(u8 *buf,u8*dx);
void GPS_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GPGSV_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_BDGSV_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GNGGA_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GNGSA_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GNGSA_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GNRMC_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GNVTG_Analysis(nmea_msg *gpsx,u8 *buf);
u8 SkyTra_Cfg_Prt(u8 baud_id);
u8 SkyTra_Cfg_Tp(u32 width);
u8 SkyTra_Cfg_Rate(u8 Frep);
void SkyTra_Send_Date(u8* dbuf,u16 len);
#endif

 







#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
