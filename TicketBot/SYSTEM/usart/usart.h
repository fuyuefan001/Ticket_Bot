#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	
#include "stdarg.h"	 	 	 	 
#include "string.h"	
#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.csom
//修改日期:2015/6/23
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.0修改说明 
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	#define USART2_MAX_RECV_LEN		800					//锟斤拷锟斤拷锟秸伙拷锟斤拷锟街斤拷锟斤拷
#define USART2_MAX_SEND_LEN		800					//锟斤拷锟斤拷突锟斤拷锟斤拷纸锟斤拷锟?
#define USART2_RX_EN 			1					//0,锟斤拷锟斤拷锟斤拷;1,锟斤拷锟斤拷.

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
extern UART_HandleTypeDef UART1_Handler; //UART句柄
extern UART_HandleTypeDef UART2_Handler; //UART句柄
#define RXBUFFERSIZE   1 //缓存大小
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL库USART接收Buffer
extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//锟斤拷锟秸伙拷锟斤拷,锟斤拷锟経SART2_MAX_RECV_LEN锟街斤拷
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//锟斤拷锟酵伙拷锟斤拷,锟斤拷锟経SART2_MAX_SEND_LEN锟街斤拷
extern u16 USART2_RX_STA;   						//锟斤拷锟斤拷锟斤拷锟斤拷状态
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
#endif
