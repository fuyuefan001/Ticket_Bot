
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "M8266HostIf.h"
#include "led.h"
#include "key.h"
#include "M8266WIFIDrv.h"
#include "M8266WIFI_ops.h"
#include "brd_cfg.h"
#include "timer.h"

#include "main.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "ltdc.h"
#include "sdram.h"
#include "w25qxx.h"
#include "nand.h"  
#include "mpu.h"
#include "ov5640.h"
#include "pcf8574.h"
#include "dcmi.h"
#include "sdmmc_sdcard.h"
#include "usmart.h"
#include "malloc.h"
#include "ff.h"
#include "exfuns.h"
#include "fontupd.h"
#include "text.h"
#include "piclib.h"	
#include "string.h"		
#include "math.h"

// /************************************************
//  ALIENTEK ??????STM32F7?????? ???46
//  ????????-HAL??????
//  ????????www.openedv.com
//  ????????http://eboard.taobao.com 
//  ????????????????"???????"???????STM32?????
//  ??????????????????????  
//  ???????????? @ALIENTEK
// ************************************************/

vu8 bmp_request=0;						
u8 ovx_mode=0;							
u16 curline=0;							
u16 yoffset=0;							

#define jpeg_buf_size   8*1024*1024		
#define jpeg_line_size	2*1024		

u32 *dcmi_line_buf[2];				
u32 *jpeg_data_buf;					

 volatile u32 jpeg_data_len=0; 		
 volatile u8 jpeg_data_ok=0;	
 nmea_msg gpsx; 											//GPS??
__align(4) u8 dtbuf[50];   								//?????
const u8*fixmode_tbl[4]={"Fail","Fail"," 2D "," 3D "};	//fix mode??? 			
void clrStr(u8* str,int size);
void M8266WIFI_Test(void);
void tcp_connect(){
		 u16 i;
	 u16 status = 0;
	 u8  link_no=0;
	 	 #define TEST_M8266WIFI_TYPE    3	     //           1 = Repeative Sending, 2 = Repeative Reception, 3 = Echo  4 = multi-clients transimission test
	                                       // (Chinese: 1=模组向外不停地发送数�? 2=模组不停地接收数�? 3= 模组将接收到的数据发送给发送方 4=多客户端测试) 

	 /////////////
   ////	 Macros for Socket Connection Type (Chinese：套接字类型的宏定义) 
	 #define TEST_CONNECTION_TYPE   1	    //           0=WIFI module as UDP, 1=WIFI module as TCP Client, 2=WIFI module as TCP Server
	                                      // (Chinese: 0=WIFI模组做UDP, 1=WIFI模组做TCP客户�?, 2=WIFI模组做TCP服务�?
   /////////////
   ////	 Macros for Soket ip:port pairs  (Chinese：套接字的本地端口和目标地址目标端口的宏定义) 
   //local port	(Chinese：套接字的本地端�?)
#if (TEST_CONNECTION_TYPE==1)         //// if module as TCP Client (Chinese:如果模组作为TCP客户�?)
	 #define TEST_LOCAL_PORT  			0			//           local port=0 will generate a random local port each time fo connection. To avoid the rejection by TCP server due to repeative connection with the same ip:port
   	                                    // (Chinese: 当local port传递的参数�?0时，本地端口会随机产生。这一点对于模组做客户端反复连接服务器时很有用。因为随机产生的端口每次会不一样，从而避免连续两次采用同样的地址和端口链接时被服务器拒绝�?
#elif (TEST_CONNECTION_TYPE==0) || (TEST_CONNECTION_TYPE==2) //// if module as UDP or TCP Server (Chinese:如果模组作为UDP或TCP服务�?)
   #define TEST_LOCAL_PORT  			4321  //           a local port should be specified //(Chinese:如果模组作为UDP或TCP服务器，则需要指定该套接字的本地端口)
#else
#error WRONG TEST_CONNECTION_TYPE defined !
#endif                                  // (Chinese: 如果模组作为TCP服务器或UDP，那么必须指定本地端�?

   //local port	(Chinese：套接字的目标地址和目标端�?)
#if (TEST_CONNECTION_TYPE==0)        //// if module as UDP (Chinese:如果模组作为UDP，则可以指定目标地址和端口，也可以随便填充，在发送数据时再设置或更改)
   #define TEST_REMOTE_ADDR    		"192.168.4.2"
   #define TEST_REMOTE_PORT  	    1234
#elif (TEST_CONNECTION_TYPE==1)      //// if module as TCP Client (Chinese:如果模组作为TCP客户端，当然必须指定目标地址和目标端口，即模组所要去连接的TCP服务器的地址和端�?)
   #define TEST_REMOTE_ADDR    	 	"192.168.137.1"  // "www.baidu.com"
   #define TEST_REMOTE_PORT  	    4321						// 80
#elif (TEST_CONNECTION_TYPE==2)     //// if module as TCP Server (Chinese:如果模组作为TCP服务�?)
		#define TEST_REMOTE_ADDR      "1.1.1.1" // no need remote ip and port upon setup connection. below values just for filling and any value would be ok. 
    #define TEST_REMOTE_PORT  	  1234  	 //(Chinese: 创建TCP服务器时，不需要指定目标地址和端口，这里的数据只是一个格式填充，随便填写�?
#else
#error WRONG TEST_CONNECTION_TYPE defined !
#endif			 
	while(M8266WIFI_SPI_Config_Tcp_Window_num(link_no, 4, &status)==0){
    	LED0(0);
		LED1(0);
	}	
	while(M8266WIFI_SPI_Setup_Connection(TEST_CONNECTION_TYPE, TEST_LOCAL_PORT, TEST_REMOTE_ADDR, TEST_REMOTE_PORT, link_no, 20, &status)==0){		
	  LED0(0);
	  LED1(0);
	}
	LED0_Toggle;LED1_Toggle;  M8266WIFI_Module_delay_ms(50);
	LED0_Toggle;LED1_Toggle;  M8266WIFI_Module_delay_ms(50);
	LED0_Toggle;LED1_Toggle;  M8266WIFI_Module_delay_ms(50);
	LED0_Toggle;LED1_Toggle;  M8266WIFI_Module_delay_ms(50);
	LED0_Toggle;LED1_Toggle;  M8266WIFI_Module_delay_ms(50);
	LED0_Toggle; LED1_Toggle; M8266WIFI_Module_delay_ms(50);	
	     	LED0_Toggle;LED1_Toggle;  M8266WIFI_Module_delay_ms(50);
	LED0_Toggle;LED1_Toggle;  M8266WIFI_Module_delay_ms(50);
	LED0_Toggle;LED1_Toggle;  M8266WIFI_Module_delay_ms(50);
	LED0_Toggle;LED1_Toggle;  M8266WIFI_Module_delay_ms(50);
	LED0_Toggle;LED1_Toggle;  M8266WIFI_Module_delay_ms(50);
	LED0_Toggle; LED1_Toggle; M8266WIFI_Module_delay_ms(50);	
}
void jpeg_data_process(void)
{
	u16 i;
	u16 rlen;			//??????????
	u32 *pbuf;
	curline=yoffset;	//??????λ
	if(ovx_mode&0X01)	//?????JPEG?????,???????????.
	{
		if(jpeg_data_ok==0)	//jpeg?????δ??????
		{
			DMA2_Stream1->CR&=~(1<<0);		//?????????
			while(DMA2_Stream1->CR&0X01);	//???DMA2_Stream1?????? 
			rlen=jpeg_line_size-DMA2_Stream1->NDTR;//?????????????	
			pbuf=jpeg_data_buf+jpeg_data_len;//??????Ч?????β,????????
			if(DMA2_Stream1->CR&(1<<19))for(i=0;i<rlen;i++)pbuf[i]=dcmi_line_buf[1][i];//???buf1????????????
			else for(i=0;i<rlen;i++)pbuf[i]=dcmi_line_buf[0][i];//???buf0???????????? 
			jpeg_data_len+=rlen;			//?????????
			jpeg_data_ok=1; 				//???JPEG??????????,???????????????
		}
		if(jpeg_data_ok==2)	//????ε?jpeg???????????????
		{
			DMA2_Stream1->NDTR=jpeg_line_size;//???????jpeg_buf_size*4???
			DMA2_Stream1->CR|=1<<0;			//???????
			jpeg_data_ok=0;					//???????δ???
			jpeg_data_len=0;				//??????????
		}
	}else
	{  
		if(bmp_request==1)	//??bmp????????,???DCMI
		{
			DCMI_Stop();	//??DCMI
			bmp_request=0;	//????????????.
		}
		LCD_SetCursor(0,0);  
		LCD_WriteRAM_Prepare();				//???д??GRAM  
	}  
} 
//jpeg?????????????
void jpeg_dcmi_rx_callback(void)
{  
	u16 i;
	u32 *pbuf;
	pbuf=jpeg_data_buf+jpeg_data_len;//??????Ч?????β
	if(DMA2_Stream1->CR&(1<<19))//buf0????,????????buf1
	{ 
		for(i=0;i<jpeg_line_size;i++)pbuf[i]=dcmi_line_buf[0][i];//???buf0?????????
		jpeg_data_len+=jpeg_line_size;//???
	}else //buf1????,????????buf0
	{
		for(i=0;i<jpeg_line_size;i++)pbuf[i]=dcmi_line_buf[1][i];//???buf1?????????
		jpeg_data_len+=jpeg_line_size;//??? 
	} 
} 

//RGB???????????????
void rgblcd_dcmi_rx_callback(void)
{  
	u16 *pbuf;
	if(DMA2_Stream1->CR&(1<<19))//DMA???buf1,???buf0
	{ 
		pbuf=(u16*)dcmi_line_buf[0]; 
	}else 						//DMA???buf0,???buf1
	{
		pbuf=(u16*)dcmi_line_buf[1]; 
	} 	
	LTDC_Color_Fill(0,curline,lcddev.width-1,curline,pbuf);//DM2D??? 
	if(curline<lcddev.height)curline++;
	if(bmp_request==1&&curline==(lcddev.height-1))//??bmp????????,???DCMI
	{
		DCMI_Stop();	//??DCMI
		bmp_request=0;	//????????????.
	}
}

//?л??OV5640??
void sw_ov5640_mode(void)
{ 
	GPIO_InitTypeDef GPIO_Initure;
 	OV5640_WR_Reg(0X3017,0XFF);	//????OV5650???(???????????)
	OV5640_WR_Reg(0X3018,0XFF); 

	//GPIOC8/9/11?л?? DCMI???
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11;  
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //??????
    GPIO_Initure.Pull=GPIO_PULLUP;              //????
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;         //????
    GPIO_Initure.Alternate=GPIO_AF13_DCMI;      //?????DCMI   
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);         //?????   
} 
//?л??SD????
void sw_sdcard_mode(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	OV5640_WR_Reg(0X3017,0X00);	//???OV5640??????(?????SD?????)
	OV5640_WR_Reg(0X3018,0X00);  
	
 	//GPIOC8/9/11?л?? SDIO???
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11;  
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //??????
    GPIO_Initure.Pull=GPIO_PULLUP;              //????
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;         //????
    GPIO_Initure.Alternate=GPIO_AF12_SDMMC1;    //?????SDIO  
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);   
}
//??????????????????
//mode:0,????.bmp???;1,????.jpg???.
//bmp????:????"0:PHOTO/PIC13141.bmp"???????
//jpg????:????"0:PHOTO/PIC13141.jpg"???????
void camera_new_pathname(u8 *pname,u8 mode)
{	 
	u8 res;					 
	u16 index=0;
	while(index<0XFFFF)
	{
		if(mode==0)sprintf((char*)pname,"0:PHOTO/PIC%05d.bmp",index);
		else sprintf((char*)pname,"0:PHOTO/PIC%05d.jpg",index);
		res=f_open(ftemp,(const TCHAR*)pname,FA_READ);//???????????
		if(res==FR_NO_FILE)break;		//?????????????=?????????????.
		index++;
	}
}  
//OV5640????jpg??
//?????:0,???
//    ????,???????
u8 ov5640_jpg_photo(u8 *pname)
{
	
	FIL* f_jpg; 
	u8 res=0,headok=0;
	u32 bwr;
	u32 i,jpgstart,jpglen;
	u8* pbuf;
	u8 printbuf[40];
	clrStr(printbuf,40);
	f_jpg=(FIL *)mymalloc(SRAMIN,sizeof(FIL));	//开辟FIL字节的内存区�? 
	if(f_jpg==NULL){
		HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("malloc fail\r\n"),10,1000);	//发送接收到的数�?
		while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结�?
	USART_RX_STA=0;
		return 0XFF;				//内存申请失败.
		
	}
	ovx_mode=1;
	jpeg_data_ok=0;
	sw_ov5640_mode();						//切换为OV5640模式 
	OV5640_JPEG_Mode();						//JPEG模式  
	OV5640_OutSize_Set(16,4,2592,1944);		//设置输出尺寸(500W)  
	dcmi_rx_callback=jpeg_dcmi_rx_callback;	//JPEG接收数据回调函数
	DCMI_DMA_Init((u32)dcmi_line_buf[0],(u32)dcmi_line_buf[1],jpeg_line_size,DMA_MDATAALIGN_WORD,DMA_MINC_ENABLE);//DCMI DMA配置    
	DCMI_Start(); 			//启动传输 
	while(jpeg_data_ok!=1);	//等待第一帧图片采集完
	jpeg_data_ok=2;			//忽略本帧图片,启动下一帧采�? 
	while(jpeg_data_ok!=1);	//等待第二帧图片采集完,第二�?,才保存到SD卡去. 
	DCMI_Stop(); 			//停止DMA搬运
	ovx_mode=0; 
	HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("sd mode\r\n"),10,1000);	//发送接收到的数�?
while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结�?
USART_RX_STA=0;

	sw_sdcard_mode();		//切换为SD卡模�?
	HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("open\r\n"),10,1000);	//发送接收到的数�?
while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结�?
USART_RX_STA=0;
	res=f_open(f_jpg,(const TCHAR*)pname,FA_WRITE|FA_CREATE_NEW);//模式0,或者尝试打开失败,则创建新文件
	
if(res!=0){
HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("open fail\r\n"),10,1000);	//发送接收到的数�?
while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结�?
USART_RX_STA=0;
}	
	if(res==0)
	{

		sprintf(printbuf,"jpeg data size:%d\r\n",jpeg_data_len*4);//串口打印JPEG文件大小
			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)(printbuf),40,1000);
				while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);
			clrStr(printbuf,40); 
		pbuf=(u8*)jpeg_data_buf;
		jpglen=0;	//设置jpg文件大小�?0
		headok=0;	//清除jpg头标�?
		for(i=0;i<jpeg_data_len*4;i++)//查找0XFF,0XD8�?0XFF,0XD9,获取jpg文件大小
		{
			if((pbuf[i]==0XFF)&&(pbuf[i+1]==0XD8))//找到FF D8
			{
				jpgstart=i;
				headok=1;	//标记找到jpg�?(FF D8)
			}
			if((pbuf[i]==0XFF)&&(pbuf[i+1]==0XD9)&&headok)//找到头以�?,再找FF D9
			{
				jpglen=i-jpgstart+2;
				break;
			}
		}
		if(jpglen)			//正常的jpeg数据 
		{
			sprintf(printbuf,"start fwrite\r\n");
			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)(printbuf),40,1000);
			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);
			clrStr(printbuf,40); 
			
			pbuf+=jpgstart;	//偏移�?0XFF,0XD8�?
			res=f_write(f_jpg,pbuf,jpglen,&bwr);
			if(bwr!=jpglen)res=0XFE; 
			sprintf(printbuf,"after fwrite\r\n");
			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)(printbuf),40,1000);
			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);
			clrStr(printbuf,40); 
		}else res=0XFD; 
	}
	jpeg_data_len=0;
	f_close(f_jpg); 
	sw_ov5640_mode();		//切换为OV5640模式
	OV5640_RGB565_Mode();	//RGB565模式  
	if(lcdltdc.pwidth!=0)	//RGB�?
	{
		dcmi_rx_callback=rgblcd_dcmi_rx_callback;//RGB屏接收数据回调函�?
		DCMI_DMA_Init((u32)dcmi_line_buf[0],(u32)dcmi_line_buf[1],lcddev.width/2,DMA_MDATAALIGN_HALFWORD,DMA_MINC_ENABLE);//DCMI DMA配置  
	}else					//MCU �?
	{
		DCMI_DMA_Init((u32)&LCD->LCD_RAM,0,1,DMA_MDATAALIGN_HALFWORD,DMA_MINC_DISABLE);			//DCMI DMA配置,MCU�?,竖屏
	}
	myfree(SRAMIN,f_jpg); 
	return res;
}  
int pwm_init(){
	return 0;
}
int duty_cycle(int x){
	
}
extern u8 ov_frame;
extern TIM_HandleTypeDef TIM3_Handler;
extern TIM_HandleTypeDef TIM2_Handler;
u8 sd_ok;
	u8 *pname;	
u8 res,fac;	
 	u8 scale=1;	
	u16 outputheight=0;
	 u8 success = 0;	
void clrStr(u8* str,int size){
	int x;
	for(x=0;x<size;x++){
		str[x]='\0';
	}
}
void GPS_Analysis(nmea_msg *gpsx,u8 *buf);
u8 USART3_TX_BUF[USART2_MAX_RECV_LEN]; 
void Gps_Msg_Show(void)
{
 	float tp;		   
	POINT_COLOR=BLUE;  	 
	tp=gpsx.longitude;	   
	sprintf((char *)dtbuf,"Longitude:%.5f %1c   ",tp/=100000,gpsx.ewhemi);	//???????
				HAL_UART_Transmit(&UART1_Handler,(uint8_t*)(dtbuf),40,1000);	
				while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
				USART_RX_STA=0;		  

 	 	   
	tp=gpsx.latitude;	   
	sprintf((char *)dtbuf,"Latitude:%.5f %1c   ",tp/=100000,gpsx.nshemi);	//???????
 					HAL_UART_Transmit(&UART1_Handler,(uint8_t*)(dtbuf),40,1000);	
				while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
				USART_RX_STA=0;		   	 
	tp=gpsx.altitude;	   
 	sprintf((char *)dtbuf,"Altitude:%.1fm     ",tp/=10);	    			//???????
 					HAL_UART_Transmit(&UART1_Handler,(uint8_t*)(dtbuf),40,1000);	
				while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
				USART_RX_STA=0;		   			   
	tp=gpsx.speed;	   
 	sprintf((char *)dtbuf,"Speed:%.3fkm/h     ",tp/=1000);		    		//???????	 
 					HAL_UART_Transmit(&UART1_Handler,(uint8_t*)(dtbuf),40,1000);	
				while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
				USART_RX_STA=0;		  				    
	if(gpsx.fixmode<=3)														//????
	{  
		sprintf((char *)dtbuf,"Fix Mode:%s",fixmode_tbl[gpsx.fixmode]);	
	  LCD_ShowString(30,200,200,16,16,dtbuf);			   
	}	 	    
	sprintf((char *)dtbuf,"GPS+BD Valid satellite:%02d",gpsx.posslnum);	 		//?????GPS???
 	LCD_ShowString(30,220,200,16,16,dtbuf);	    
	sprintf((char *)dtbuf,"GPS Visible satellite:%02d",gpsx.svnum%100);	 		//??GPS???
 	LCD_ShowString(30,240,200,16,16,dtbuf);
	
	sprintf((char *)dtbuf,"BD Visible satellite:%02d",gpsx.beidou_svnum%100);	 		//???????
 	LCD_ShowString(30,260,200,16,16,dtbuf);
	
	sprintf((char *)dtbuf,"UTC Date:%04d/%02d/%02d   ",gpsx.utc.year,gpsx.utc.month,gpsx.utc.date);	//??UTC??
	LCD_ShowString(30,280,200,16,16,dtbuf);		    
	sprintf((char *)dtbuf,"UTC Time:%02d:%02d:%02d   ",gpsx.utc.hour,gpsx.utc.min,gpsx.utc.sec);	//??UTC??
  LCD_ShowString(30,300,200,16,16,dtbuf);		  
}   
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	u8 printbuf[40];
	u8 i;
	u8 rxlen;
	u16 x;
	clrStr(printbuf,40);

	
	if(htim==(&TIM2_Handler))
    {
//HAL_NVIC_EnableIRQ(TIM7_IRQn);
	while(1) 
	{		
		delay_ms(1);
		if(USART2_RX_STA&0X8000||x>500)		//���յ�һ��������
		{
			rxlen=USART2_RX_STA&0X7FFF;	//�õ����ݳ���
			for(i=0;i<rxlen;i++)USART3_TX_BUF[i]=USART2_RX_BUF[i];	   
 			USART2_RX_STA=0;		   	//������һ�ν���
			//USART3_TX_BUF[i]=0;			//�Զ����ӽ�����
			//HAL_UART_Transmit(&UART1_Handler,(uint8_t*)(USART2_RX_BUF),rxlen,1000);	
			
			GPS_Analysis(&gpsx,(u8*)USART2_TX_BUF);//�����ַ���
			if(x>500){
				HAL_UART_Transmit(&UART1_Handler,(uint8_t*)(USART2_RX_BUF),USART2_MAX_RECV_LEN,1000);
				Gps_Msg_Show();
				
				
			}	else{
				
				HAL_UART_Transmit(&UART1_Handler,(uint8_t*)(USART2_RX_BUF),rxlen,1000);	
			}			//��ʾ��Ϣ	
			break;
 		}
		 x++;

	}

		///HAL_NVIC_DisableIRQ(USART2_IRQn);				//ʹ��USART2�ж�
		HAL_NVIC_DisableIRQ(TIM7_IRQn);				//ʹ��USART2�ж�
		HAL_NVIC_DisableIRQ(TIM5_IRQn);
		HAL_NVIC_DisableIRQ(USART2_IRQn);
		if(sd_ok){    
				
			sw_sdcard_mode();
			OV5640_Focus_Single(); 
			camera_new_pathname(pname,1);
			//OV5640_Flash_Ctrl(1);
			res=ov5640_jpg_photo(pname);
			//HAL_NVIC_EnableIRQ(USART2_IRQn);
			HAL_NVIC_EnableIRQ(TIM7_IRQn);
			//OV5640_Flash_Ctrl(0);
			if(scale==0){
				fac=800/lcddev.height;//???????????
				OV5640_OutSize_Set((1280-fac*lcddev.width)/2,(800-fac*lcddev.height)/2,lcddev.width,lcddev.height); 	 
 			}else{
				OV5640_OutSize_Set(16,4,lcddev.width,outputheight);
 			}	
			// if(lcddev.height>800)OV5640_WR_Reg(0x3035,0X51);//?????????????????????
		
			if(res){
				
				HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("cannot write\r\n"),15,1000);	
				while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
				USART_RX_STA=0;
			}else {
				for(i=0;i<40;i++){
					printbuf[i]='\0';
				}
				sprintf(printbuf,"%s\r\n",pname);
				HAL_UART_Transmit(&UART1_Handler,(uint8_t*)(printbuf),40,1000);	
				while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
				USART_RX_STA=0;		  
				for(i=0;i<40;i++){
					printbuf[i]='\0';
				}
				PCF8574_WriteBit(BEEP_IO,0);	//????????????????????
				delay_ms(100);
				PCF8574_WriteBit(BEEP_IO,1);	//????????
			}  

			 //delay_ms(500);		
			//  DCMI_Start();	
			//  DCMI_Stop();
		}else{	
				//LCD_Clear(WHITE);
				HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("SD card err\r\n"),15,1000);	
				while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
				USART_RX_STA=0;	
		}
		// DCMI_Stop();
		// DCMI_Start();	
		HAL_NVIC_EnableIRQ(USART2_IRQn);				//ʹ��USART2�ж�
		HAL_NVIC_EnableIRQ(TIM7_IRQn);				//ʹ��USART2�ж�	
		HAL_NVIC_EnableIRQ(TIM5_IRQn);
	}

}

void TIM2_Init(u16 arr,u16 psc);
int main(void)
{
	//u8 res,fac;							 
//	u8 *pname;					
	u8 key;							   
	u8 i;						 
	char printbuf[20];				 
// 	u8 scale=1;			
	u8 msgbuf[15];			
//	u16 outputheight=0;
//	 u8 success = 0;	
sd_ok=1;
    Cache_Enable();                 //??L1-Cache
    MPU_Memory_Protection();        //???????�??????
    Write_Through(); 
	HAL_Init();				        //?????HAL??
    Stm32_Clock_Init(432,25,2,9);   //???????,216Mhz 
    delay_init(216);                //????????
	uart_init(115200);
	 TIM3_PWM_Init(2000-1,1080-1);
 TIM3_PWM_Init1(2000-1,1080-1);
	//while(1);
	HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("init 1\r\n"),8,1000);	
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
	USART_RX_STA=0;	
	usart2_init(38400);  

	usmart_dev.init(108); 		    //?????USMART
//    LED_Init();                     //?????LED
//   //  KEY_Init();                     //?????????
 //TIM3_Init(2000,1080);

    SDRAM_Init();                   //?????SDRAM
    LCD_Init();                     //?????LCD
	// TIM2_Init(30000-1,10800-1);
	HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("init2\r\n"),8,1000);	
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
	USART_RX_STA=0;	
		HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("initxx\r\n"),8,1000);	
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
	USART_RX_STA=0;	



	



	PCF8574_Init();                 //?????PCF8574
    OV5640_Init();			    //?????OV5640
	sw_sdcard_mode();				//?????л??OV5640??
		HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("done 2\r\n"),8,1000);	
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
	USART_RX_STA=0;	
	//W25QXX_Init();					//?????W25Q256
 	my_mem_init(SRAMIN);			//????????????
	my_mem_init(SRAMEX);			//???????????
	my_mem_init(SRAMDTCM);			//?????TCM???? 
	
	HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("done x\r\n"),8,1000);	
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
	USART_RX_STA=0;	

  LCD_ShowString(30,200,200,16,16,"done X");		
	exfuns_init();					//?fatfs?????????????  
		HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("11111\r\n"),8,1000);	
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
	USART_RX_STA=0;	
 	f_mount(fs[0],"0:",1); 
	 	HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("22222\r\n"),8,1000);	
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
	USART_RX_STA=0;				//????SD??  
	res=f_mkdir("0:/PHOTO");		//????PHOTO?????


	HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("mkdir\r\n"),8,1000);	
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
	USART_RX_STA=0;
  LCD_ShowString(30,200,200,16,16,"mkdir");		
	POINT_COLOR=RED; 
	if(res!=FR_EXIST&&res!=FR_OK) 	//?????????
	{		
	///	while(1){
		 //LCD_ShowString(30,200,200,16,16,"SD ERROR");
	//	}
			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("SD ERR \r\n"),8,1000);	
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
	USART_RX_STA=0;					  
			  
		sd_ok=0;  	
	} 	
		if(SkyTra_Cfg_Rate(5)!=0)	//���ö�λ��Ϣ�����ٶ�Ϊ5Hz,˳���ж�GPSģ���Ƿ���λ. 
	{
   		HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("seting\r\n"),8,1000);	
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
	USART_RX_STA=0;	
		delay_ms(500);
		do
		{
			usart2_init(115200);			//��ʼ������3������Ϊ9600
//LCD_ShowString(30,120,200,16,16,"init �ɹ�");
	  	SkyTra_Cfg_Prt(3);			//��������ģ��Ĳ�����Ϊ38400
			usart2_init(38400);			//��ʼ������3������Ϊ38400
      key=SkyTra_Cfg_Tp(100000);	//�������Ϊ100ms
		}while(SkyTra_Cfg_Rate(15)!=0&&key!=0);//����SkyTraF8-BD�ĸ�������Ϊ5Hz
	  
	  
	  	HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("done 1\r\n"),8,1000);	
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
	USART_RX_STA=0;	
		delay_ms(500);
		//LCD_Fill(30,120,30+200,120+16,WHITE);//�����ʾ 
	}

			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("SDOK  \r\n"),8,1000);	
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
	USART_RX_STA=0;	
	
	dcmi_line_buf[0]=mymalloc(SRAMIN,jpeg_line_size*4);	//?jpeg dma???????????	
	dcmi_line_buf[1]=mymalloc(SRAMIN,jpeg_line_size*4);	//?jpeg dma???????????	
	jpeg_data_buf=mymalloc(SRAMEX,jpeg_buf_size);		//?jpeg??????????(???4MB)
 	pname=mymalloc(SRAMIN,30);//???·?????????????30?????????	 
 	while(pname==NULL||!dcmi_line_buf[0]||!dcmi_line_buf[1]||!jpeg_data_buf)	//?????????
 	{	    
HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("mem fail\r\n"),10,1000);	
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
	USART_RX_STA=0;
		delay_ms(200);				  
		LCD_Fill(30,190,240,146,WHITE);//??????	     
		delay_ms(200);				  
	}   
	while(OV5640_Init())//?????OV5640
	{
		HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("ov5640 fail  \r\n"),15,1000);	
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
	USART_RX_STA=0;
		delay_ms(200);
	    LCD_Fill(30,190,239,206,WHITE);
		delay_ms(200);
	}	
	
 	HAL_UART_Transmit(&UART1_Handler,(uint8_t*)("ov5640 ok  \r\n"),10,1000);	
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
	USART_RX_STA=0;
	//???????????
	OV5640_RGB565_Mode();	//RGB565?? 
	OV5640_Focus_Init(); 
	OV5640_Light_Mode(0);	//?????
	OV5640_Color_Saturation(3);//???????0
	OV5640_Brightness(4);	//????0
	OV5640_Contrast(3);		//????0
	OV5640_Sharpness(33);	//??????
	OV5640_Focus_Constant();//???????????
	DCMI_Init();			//DCMI????

	if(lcdltdc.pwidth!=0)	//RGB??
	{
		dcmi_rx_callback=rgblcd_dcmi_rx_callback;//RGB????????????????
		DCMI_DMA_Init((u32)dcmi_line_buf[0],(u32)dcmi_line_buf[1],lcddev.width/2,DMA_MDATAALIGN_HALFWORD,DMA_MINC_ENABLE);//DCMI DMA????  
	}else					//MCU ??
	{
		DCMI_DMA_Init((u32)&LCD->LCD_RAM,0,1,DMA_MDATAALIGN_HALFWORD,DMA_MINC_DISABLE);			//DCMI DMA????,MCU??,????
	}
	if(lcddev.height==1024)
	{
		yoffset=(lcddev.height-800)/2;
		outputheight=800;
		OV5640_WR_Reg(0x3035,0X51);//?????????????????????
	}else if(lcddev.height==1280)
	{
		yoffset=(lcddev.height-600)/2;
		outputheight=600;
		OV5640_WR_Reg(0x3035,0X51);//????????????????????? 
	}else 
	{
		yoffset=0;
		outputheight=lcddev.height;
	}
	curline=yoffset;		//??????λ
	OV5640_OutSize_Set(16,4,lcddev.width,outputheight);	//???????????
	DCMI_Start(); 			//????????
	LCD_Clear(BLACK);
	delay_ms(1000);

	 TIM2_Init(30000-1,10800-1);

		while(1);
	
}
