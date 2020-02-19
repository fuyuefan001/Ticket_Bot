#include "usart.h"
#include "ltdc.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//ï¿½ï¿½ï¿½Ê¹ï¿½ï¿½os,ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Í·ï¿½Ä¼ï¿½ï¿½ï¿½ï¿½ï¿½.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os Ê¹ï¿½ï¿½	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö»ï¿½ï¿½Ñ§Ï°Ê¹ï¿½Ã£ï¿½Î´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½É£ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Îºï¿½ï¿½ï¿½Í¾
//ALIENTEK STM32F7ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
//ï¿½ï¿½ï¿½ï¿½1ï¿½ï¿½Ê¼ï¿½ï¿½		   
//ï¿½ï¿½ï¿½ï¿½Ô­ï¿½ï¿½@ALIENTEK
//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì³:www.openedv.com
//ï¿½Þ¸ï¿½ï¿½ï¿½ï¿½ï¿½:2015/6/23
//ï¿½æ±¾ï¿½ï¿½V1.5
//ï¿½ï¿½È¨ï¿½ï¿½ï¿½Ð£ï¿½ï¿½ï¿½ï¿½ï¿½Ø¾ï¿½ï¿½ï¿?
//Copyright(C) ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ó¿Æ¼ï¿½ï¿½ï¿½ï¿½Þ¹ï¿½Ë¾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.0ï¿½Þ¸ï¿½Ëµï¿½ï¿½ 
////////////////////////////////////////////////////////////////////////////////// 	  
//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Â´ï¿½ï¿½ï¿½,Ö§ï¿½ï¿½printfï¿½ï¿½ï¿½ï¿½,ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÒªÑ¡ï¿½ï¿½use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//ï¿½ï¿½×¼ï¿½ï¿½ï¿½ï¿½Òªï¿½ï¿½Ö§ï¿½Öºï¿½ï¿½ï¿½                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//ï¿½ï¿½ï¿½ï¿½_sys_exit()ï¿½Ô±ï¿½ï¿½ï¿½Ê¹ï¿½Ã°ï¿½ï¿½ï¿½ï¿½ï¿½Ä£Ê½    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//ï¿½Ø¶ï¿½ï¿½ï¿½fputcï¿½ï¿½ï¿½ï¿½ 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->ISR&0X40)==0);//Ñ­ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½,Ö±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?   
	USART1->TDR=(u8)ch;      
	return ch;
}
#endif 

#if EN_USART1_RX   //ï¿½ï¿½ï¿½Ê¹ï¿½ï¿½ï¿½Ë½ï¿½ï¿½ï¿?
//ï¿½ï¿½ï¿½ï¿½1ï¿½Ð¶Ï·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
//×¢ï¿½ï¿½,ï¿½ï¿½È¡USARTx->SRï¿½Ü±ï¿½ï¿½ï¿½Äªï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä´ï¿½ï¿½ï¿?   	
u8 USART_RX_BUF[USART_REC_LEN];     //ï¿½ï¿½ï¿½Õ»ï¿½ï¿½ï¿½,ï¿½ï¿½ï¿½USART_REC_LENï¿½ï¿½ï¿½Ö½ï¿½.
//ï¿½ï¿½ï¿½ï¿½×´Ì¬
//bit15ï¿½ï¿½	ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½É±ï¿½Ö?
//bit14ï¿½ï¿½	ï¿½ï¿½ï¿½Õµï¿½0x0d
//bit13~0ï¿½ï¿½	ï¿½ï¿½ï¿½Õµï¿½ï¿½ï¿½ï¿½ï¿½Ð§ï¿½Ö½ï¿½ï¿½ï¿½Ä¿
u16 USART_RX_STA=0;       //ï¿½ï¿½ï¿½ï¿½×´Ì¬ï¿½ï¿½ï¿?	
u16 USART2_RX_STA=0;

u8 aRxBuffer[RXBUFFERSIZE];//HALï¿½ï¿½Ê¹ï¿½ÃµÄ´ï¿½ï¿½Ú½ï¿½ï¿½Õ»ï¿½ï¿½ï¿½
UART_HandleTypeDef UART1_Handler; //UARTï¿½ï¿½ï¿?
UART_HandleTypeDef UART2_Handler; //UART¾ä±ú
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	//·¢ËÍ»º³å,×î´óUSART2_MAX_SEND_LEN×Ö½Ú  	  
//´®¿Ú½ÓÊÕ»º´æÇø 	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//½ÓÊÕ»º³å,×î´óUSART2_MAX_RECV_LEN¸ö×Ö½Ú.
		
//ï¿½ï¿½Ê¼ï¿½ï¿½IO ï¿½ï¿½ï¿½ï¿½1 
//bound:ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

void uart_init(u32 bound)
{	
	//UART ï¿½ï¿½Ê¼ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	UART1_Handler.Instance=USART3;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //ï¿½Ö³ï¿½Îª8Î»ï¿½ï¿½ï¿½Ý¸ï¿½Ê½
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //Ò»ï¿½ï¿½Í£Ö¹Î»
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //ï¿½ï¿½ï¿½ï¿½Å¼Ð£ï¿½ï¿½Î»
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //ï¿½ï¿½Ó²ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //ï¿½Õ·ï¿½Ä£Ê½
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()ï¿½ï¿½Ê¹ï¿½ï¿½UART1
	
	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);
  
}

void usart2_init(u32 bound)
{	
	//UART ³õÊ¼»¯ÉèÖÃ
	UART2_Handler.Instance=USART2;					    //USART2
	UART2_Handler.Init.BaudRate=bound;				    //²¨ÌØÂÊ
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    //Ò»¸öÍ£Ö¹Î»
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    //ÎÞÆæÅ¼Ð£ÑéÎ»
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //ÎÞÓ²¼þÁ÷¿Ø
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		    //ÊÕ·¢Ä£Ê½
	HAL_UART_Init(&UART2_Handler);					    //HAL_UART_Init()»áÊ¹ÄÜUART1
}
//UARTï¿½×²ï¿½ï¿½Ê¼ï¿½ï¿½ï¿½ï¿½Ê±ï¿½ï¿½Ê¹ï¿½Ü£ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ã£ï¿½ï¿½Ð¶ï¿½ï¿½ï¿½ï¿½ï¿?
//ï¿½Ëºï¿½ï¿½ï¿½ï¿½á±»HAL_UART_Init()ï¿½ï¿½ï¿½ï¿½
//huart:ï¿½ï¿½ï¿½Ú¾ï¿½ï¿?

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIOï¿½Ë¿ï¿½ï¿½ï¿½ï¿½ï¿½
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART3)//ï¿½ï¿½ï¿½ï¿½Ç´ï¿½ï¿½ï¿?1ï¿½ï¿½ï¿½ï¿½ï¿½Ð´ï¿½ï¿½ï¿½1 MSPï¿½ï¿½Ê¼ï¿½ï¿½
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			//Ê¹ï¿½ï¿½GPIOAÊ±ï¿½ï¿½
		__HAL_RCC_USART3_CLK_ENABLE();			//Ê¹ï¿½ï¿½USART1Ê±ï¿½ï¿½
	
		GPIO_Initure.Pin=GPIO_PIN_10;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
		GPIO_Initure.Pull=GPIO_PULLUP;			//ï¿½ï¿½ï¿½ï¿½
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//ï¿½ï¿½ï¿½ï¿½
		GPIO_Initure.Alternate=GPIO_AF7_USART3;	//ï¿½ï¿½ï¿½ï¿½ÎªUSART1
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//ï¿½ï¿½Ê¼ï¿½ï¿½PA9

		GPIO_Initure.Pin=GPIO_PIN_11;			//PA10
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//ï¿½ï¿½Ê¼ï¿½ï¿½PA10
		
#if EN_USART1_RX
		
		HAL_NVIC_EnableIRQ(USART3_IRQn);				//Ê¹ï¿½ï¿½USART1ï¿½Ð¶ï¿½Í¨ï¿½ï¿½
		HAL_NVIC_SetPriority(USART3_IRQn,0,0);			//ï¿½ï¿½Õ¼ï¿½ï¿½ï¿½È¼ï¿½3ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È¼ï¿½3
#endif	
	}
		if(huart->Instance==USART2)
	{
		  //GPIO¶Ë¿ÚÉèÖÃ
		GPIO_InitTypeDef GPIO_Initure;
	
		__HAL_RCC_GPIOA_CLK_ENABLE();			//Ê¹ÄÜGPIOBÊ±ÖÓ
		__HAL_RCC_USART2_CLK_ENABLE();			//Ê¹ÄÜUSART2Ê±ÖÓ
	
		GPIO_Initure.Pin=GPIO_PIN_3;			//PB10
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//¸´ÓÃÍÆÍìÊä³ö
		GPIO_Initure.Pull=GPIO_PULLUP;			//ÉÏÀ­
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//¸ßËÙ
		GPIO_Initure.Alternate=GPIO_AF7_USART2;	//¸´ÓÃÎªUSART2
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//³õÊ¼»¯PB10

		GPIO_Initure.Pin=GPIO_PIN_2;			//PB11
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//³õÊ¼»¯PB11
	
		// __HAL_UART_DISABLE_IT(huart,UART_IT_TC);
	//¿ªÆô½ÓÊÕÖÐ¶Ï
		HAL_NVIC_EnableIRQ(USART2_IRQn);				//Ê¹ÄÜUSART2ÖÐ¶Ï
		HAL_NVIC_SetPriority(USART2_IRQn,2,2);			//ÇÀÕ¼ÓÅÏÈ¼¶2£¬×ÓÓÅÏÈ¼¶3
				__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		
		 TIM7_Int_Init(1000-1,9000-1);		//100msÖÐ¶Ï
		USART2_RX_STA=0;		//ÇåÁã
		TIM7->CR1&=~(1<<0);        //¹Ø±Õ¶¨Ê±Æ÷7
	}

}

//´®¿Ú3,printf º¯Êý
//È·±£Ò»´Î·¢ËÍÊý¾Ý²»³¬¹ýUSART2_MAX_SEND_LEN×Ö½Ú
void u3_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);		//´Ë´Î·¢ËÍÊý¾ÝµÄ³¤¶È
	for(j=0;j<i;j++)							//Ñ­»··¢ËÍÊý¾Ý
	{
		while((USART2->ISR&0X40)==0);			//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   
		USART2->TDR=USART2_TX_BUF[j];  
	} 
}

void TIM_SetTIM3Compare4(u32 compare);
void TIM_SetTIM3Compare4(u32 compare);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    u8 len;	
	u16 times=0;
	char printxx[10];
Show_Str(30,50,200,16,"enter callback",16,0);	 		
	if(huart->Instance==USART3)//å¦‚æžœæ˜¯ä¸²å?1
	// {
// 		sprintf(printxx,"%x",USART_RX_STA);
// 		Show_Str(30,70,200,16,printxx,16,0);	
// 		if((USART_RX_STA&0x8000)==0)//æŽ¥æ”¶æœªå®Œæˆ?
// 		{
// 			//Show_Str(30,80,200,16,"notfinish",16,0);	
// 			if(USART_RX_STA&0x4000)//æŽ¥æ”¶åˆ°äº†0x0d
				
// 			{Show_Str(30,150,200,16,"0x0d",16,0);	
// 				if(aRxBuffer[0]!=0x0a){USART_RX_STA=0;Show_Str(30,90,200,16,"rcverr",16,0);	}//æŽ¥æ”¶é”™è¯¯,é‡æ–°å¼€å§?
// 				else {USART_RX_STA|=0x8000;Show_Str(30,100,200,16,"rcv",16,0);	}	//æŽ¥æ”¶å®Œæˆäº? 

// 		//////////////////////////////////
// {
//        if(USART_RX_STA&0x8000)
		{					   
			/////////////////////
			
			Show_Str(30,110,200,16,"enter function ",16,0);	
			len=USART_RX_STA&0x3fff;//å¾—åˆ°æ­¤æ¬¡æŽ¥æ”¶åˆ°çš„æ•°æ®é•¿åº¦
			if(aRxBuffer[0]=='0'){//ÃÂ£Â³Âµ
				Show_Str(30,130,200,16,"enter function 0",16,0);
			TIM_SetTIM3Compare3(150);	//ÃÃžÂ¸Ã„Â±ÃˆÂ½ÃÃ–ÂµÂ£Â¬ÃÃžÂ¸Ã„Ã•Â¼Â¿Ã•Â±Ãˆ
			TIM_SetTIM3Compare4(150);	
			}
		else if(aRxBuffer[0]=='1'){//Ã‡Â°Â½Ã¸
			Show_Str(30,150,200,16,"enter function 1",16,0);
			TIM_SetTIM3Compare3(155);	//ÃÃžÂ¸Ã„Â±ÃˆÂ½ÃÃ–ÂµÂ£Â¬ÃÃžÂ¸Ã„Ã•Â¼Â¿Ã•Â±Ãˆ
			TIM_SetTIM3Compare4(150);
			}
		else if(aRxBuffer[0]=='2'){//Ã‡Â°Ã—Ã³
			Show_Str(30,170,200,16,"enter function 2",16,0);
			TIM_SetTIM3Compare3(155);	//ÃÃžÂ¸Ã„Â±ÃˆÂ½ÃÃ–ÂµÂ£Â¬ÃÃžÂ¸Ã„Ã•Â¼Â¿Ã•Â±Ãˆ
			TIM_SetTIM3Compare4(190);
			}
		else if(aRxBuffer[0]=='3'){//Ã‡Â°Ã“Ã’
			Show_Str(30,190,200,16,"enter function 3",16,0);
			TIM_SetTIM3Compare3(155);	//ÃÃžÂ¸Ã„Â±ÃˆÂ½ÃÃ–ÂµÂ£Â¬ÃÃžÂ¸Ã„Ã•Â¼Â¿Ã•Â±Ãˆ
			TIM_SetTIM3Compare4(110);
			}
		else{//ÂºÃ³ÃÃ‹
			Show_Str(30,110,200,16,"enter function elsel",16,0);
			TIM_SetTIM3Compare3(150);	//ÃÃžÂ¸Ã„Â±ÃˆÂ½ÃÃ–ÂµÂ£Â¬ÃÃžÂ¸Ã„Ã•Â¼Â¿Ã•Â±Ãˆ
			TIM_SetTIM3Compare4(150);
				}
		
			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)(aRxBuffer),10,1000);	//å‘é€æŽ¥æ”¶åˆ°çš„æ•°æ?
			//while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//ç­‰å¾…å‘é€ç»“æ?
			USART_RX_STA=0;
			///////////////////////
		}
// 	}
// //////////////////////////////////////////////
// 			}
// 			else //è¿˜æ²¡æ”¶åˆ°0X0D
// 			{	
// 				if(aRxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
// 				else
// 				{
// 					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
// 					USART_RX_STA++;
// 					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//æŽ¥æ”¶æ•°æ®é”™è¯¯,é‡æ–°å¼€å§‹æŽ¥æ”?	  
// 				}		 
// 			}
// 		}

	// }
}
void USART2_IRQHandler(void)
{
	u8 res;	      //HAL_UART_IRQHandler(&UART2_Handler);
	u8 printbuf[40];
			// sprintf(printbuf,"USART2_IRQHandler\r\n");//ä¸²å£æ‰“å°JPEGæ–‡ä»¶å¤§å°
			// HAL_UART_Transmit(&UART1_Handler,(uint8_t*)(printbuf),40,1000);
			// 	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);
			// clrStr(printbuf,40); 
	if(__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_RXNE)!=RESET)//½ÓÊÕµ½Êý¾Ý
	{	 
//		HAL_UART_Receive(&UART3_Handler,&res,1,1000);
		res=USART2->RDR; 			 
		if((USART2_RX_STA&(1<<15))==0)//½ÓÊÕÍêµÄÒ»ÅúÊý¾Ý,»¹Ã»ÓÐ±»´¦Àí,Ôò²»ÔÙ½ÓÊÕÆäËûÊý¾Ý
		{ 
			if(USART2_RX_STA<USART2_MAX_RECV_LEN)	//»¹¿ÉÒÔ½ÓÊÕÊý¾Ý
			{
//				__HAL_TIM_SetCounter(&TIM7_Handler,0);	
				TIM7->CNT=0;         				//¼ÆÊýÆ÷Çå¿Õ	
				if(USART2_RX_STA==0) 				//Ê¹ÄÜ¶¨Ê±Æ÷7µÄÖÐ¶Ï 
				{
//					__HAL_RCC_TIM7_CLK_ENABLE();            //Ê¹ÄÜTIM7Ê±ÖÓ
					TIM7->CR1|=1<<0;     			//Ê¹ÄÜ¶¨Ê±Æ÷7
				}
				USART2_RX_BUF[USART2_RX_STA++]=res;	//¼ÇÂ¼½ÓÊÕµ½µÄÖµ	 
			}else 
			{
				USART2_RX_STA|=1<<15;				//Ç¿ÖÆ±ê¼Ç½ÓÊÕÍê³É
				__HAL_UART_CLEAR_IT(&UART2_Handler,UART_FLAG_RXNE);
			} 
		}
	} else{
		CLEAR_BIT(UART2_Handler.Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
	}
			// 	sprintf(printbuf,"USART2_IRQHandler\r\n");//ä¸²å£æ‰“å°JPEGæ–‡ä»¶å¤§å°
			// HAL_UART_Transmit(&UART1_Handler,(uint8_t*)(printbuf),40,1000);
			// 	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);
			// clrStr(printbuf,40); 
	 				 											 
}   
 



//ï¿½ï¿½ï¿½ï¿½1ï¿½Ð¶Ï·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
void USART3_IRQHandler(void)                	
{ 
	u32 timeout=0;
    u32 maxDelay=0x1FFFF;
#if SYSTEM_SUPPORT_OS	 	//ä½¿ç”¨OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART1_Handler);	//è°ƒç”¨HALåº“ä¸­æ–­å¤„ç†å…¬ç”¨å‡½æ•?
	
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler)!=HAL_UART_STATE_READY)//ç­‰å¾…å°±ç»ª
	{
        timeout++;////è¶…æ—¶å¤„ç†
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler,(u8 *)aRxBuffer, RXBUFFERSIZE)!=HAL_OK)//ä¸€æ¬¡å¤„ç†å®Œæˆä¹‹åŽï¼Œé‡æ–°å¼€å¯ä¸­æ–­å¹¶è®¾ç½®RxXferCountä¸?1
	{
        timeout++; //è¶…æ—¶å¤„ç†
        if(timeout>maxDelay) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//ä½¿ç”¨OS
	OSIntExit();  											 
#endif
} 
// void USART3_IRQHandler(void)                	
// { 
// 	u8 Res;
// #if SYSTEM_SUPPORT_OS	 	//Ê¹ï¿½ï¿½OS
// 	OSIntEnter();    
// #endif
// 	if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //ï¿½ï¿½ï¿½ï¿½ï¿½Ð¶ï¿½(ï¿½ï¿½ï¿½Õµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý±ï¿½ï¿½ï¿½ï¿½ï¿½0x0d 0x0aï¿½ï¿½Î²)
// 	{
//         HAL_UART_Receive(&UART1_Handler,&Res,1,1000); 
// 		if((USART_RX_STA&0x8000)==0)//ï¿½ï¿½ï¿½ï¿½Î´ï¿½ï¿½ï¿?
// 		{
// 			if(USART_RX_STA&0x4000)//ï¿½ï¿½ï¿½Õµï¿½ï¿½ï¿½0x0d
// 			{
// 				if(Res!=0x0a)USART_RX_STA=0;//ï¿½ï¿½ï¿½Õ´ï¿½ï¿½ï¿½,ï¿½ï¿½ï¿½Â¿ï¿½Ê¼
// 				else USART_RX_STA|=0x8000;	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿? 
// 			}
// 			else //ï¿½ï¿½Ã»ï¿½Õµï¿½0X0D
// 			{	
// 				if(Res==0x0d)USART_RX_STA|=0x4000;
// 				else
// 				{
// 					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
// 					USART_RX_STA++;
// 					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý´ï¿½ï¿½ï¿½,ï¿½ï¿½ï¿½Â¿ï¿½Ê¼ï¿½ï¿½ï¿½ï¿½	  
// 				}		 
// 			}
// 		}   		 
// 	}
// 	HAL_UART_IRQHandler(&UART1_Handler);	
// #if SYSTEM_SUPPORT_OS	 	//Ê¹ï¿½ï¿½OS
// 	OSIntExit();  											 
// #endif
// } 
#endif	


/****************************************************************************************/
/****************************************************************************************/
/*************************ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Í¨ï¿½ï¿½ï¿½Ú»Øµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ð±ï¿½Ð´ï¿½Ð¶Ï¿ï¿½ï¿½ï¿½ï¿½ß¼ï¿?*********************/
/****************************************************************************************
***************************************************************************************************
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//ï¿½ï¿½ï¿½ï¿½Ç´ï¿½ï¿½ï¿?1
	{
		if((USART_RX_STA&0x8000)==0)//ï¿½ï¿½ï¿½ï¿½Î´ï¿½ï¿½ï¿?
		{
			if(USART_RX_STA&0x4000)//ï¿½ï¿½ï¿½Õµï¿½ï¿½ï¿½0x0d
			{
				if(aRxBuffer[0]!=0x0a)USART_RX_STA=0;//ï¿½ï¿½ï¿½Õ´ï¿½ï¿½ï¿½,ï¿½ï¿½ï¿½Â¿ï¿½Ê¼
				else USART_RX_STA|=0x8000;	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿? 
			}
			else //ï¿½ï¿½Ã»ï¿½Õµï¿½0X0D
			{	
				if(aRxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý´ï¿½ï¿½ï¿½,ï¿½ï¿½ï¿½Â¿ï¿½Ê¼ï¿½ï¿½ï¿½ï¿½	  
				}		 
			}
		}

	}
}
 
//ï¿½ï¿½ï¿½ï¿½1ï¿½Ð¶Ï·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
void USART1_IRQHandler(void)                	
{ 
	u32 timeout=0;
    u32 maxDelay=0x1FFFF;
#if SYSTEM_SUPPORT_OS	 	//Ê¹ï¿½ï¿½OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART1_Handler);	//ï¿½ï¿½ï¿½ï¿½HALï¿½ï¿½ï¿½Ð¶Ï´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ãºï¿½ï¿½ï¿½
	
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler)!=HAL_UART_STATE_READY)//ï¿½È´ï¿½ï¿½ï¿½ï¿½ï¿½
	{
        timeout++;////ï¿½ï¿½Ê±ï¿½ï¿½ï¿½ï¿½
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler,(u8 *)aRxBuffer, RXBUFFERSIZE)!=HAL_OK)//Ò»ï¿½Î´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö®ï¿½ï¿½ï¿½ï¿½ï¿½Â¿ï¿½ï¿½ï¿½ï¿½Ð¶Ï²ï¿½ï¿½ï¿½ï¿½ï¿½RxXferCountÎª1
	{
        timeout++; //ï¿½ï¿½Ê±ï¿½ï¿½ï¿½ï¿½
        if(timeout>maxDelay) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//Ê¹ï¿½ï¿½OS
	OSIntExit();  											 
#endif
} 
#endif	
*/



 




