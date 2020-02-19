#ifndef __MYJPEG_H
#define __MYJPEG_H
#include "sys.h"
#include "ff.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//SDRAM驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/5/29
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

#define OUT_BUFFER_NUM          2
#define IN_BUFFERS_NUM          2

//JPEG数据缓冲结构体
typedef struct
{
    u8 State;
    u8 *DataBuffer;          //JPEG数据缓冲区
    u32 DataBufferSize;      //JPEG数据长度
}Data_Buffer;  

//JPEG相关参数结构体
typedef struct
{
    u8 decodend;            //解码完成标志，0:未完成,1:完成
    u8 outbuf_readindex;    //输出缓冲区读位置
    u8 outbuf_writeindex;   //输出缓冲区写位置
    u8 output_paused;       //输出处理暂停标志。1，暂停
    u8 inbuf_readindex;     //输入缓冲区读位置
    u8 inbuf_writeindex;    //输入缓冲区写位置
    u8 input_paused;        //输入处理暂停标志。1，暂停
    u32 FrameBufAddr;       //帧地址
    u32 blockindex;         //块索引
    u32 totalblock;         //总块数
    FIL jpegfile;           //JPEG图片文
    Data_Buffer OUT_Buffer[OUT_BUFFER_NUM]; //数据输出BUFFER    
    Data_Buffer IN_Buffer[IN_BUFFERS_NUM];  //数据输入BUFFER    
}_jpeg_dev;


#define CHUNK_SIZE_IN  ((uint32_t)(4096)) 
#define CHUNK_SIZE_OUT ((uint32_t)(768*4)) 

#define JPEG_BUFFER_EMPTY 0
#define JPEG_BUFFER_FULL  1

#define JPEG_OUTPUT_DATA_BUFFER  0xC0200000 

extern JPEG_HandleTypeDef JPEG_Handler;//JPEG句柄

void JPEG_Init(void);
void JPEG_Display(u16 x,u16 y,u8 *JPEGFileName,u8 mode);
u8 jpeg_decode(u32 destaddr);
u32 jpegdata_output(void);
void jpegdata_input(void);
void dma2d_datacopy(u32 *srcbuf,u32 *dstbuf,u16 x,u16 y,u16 xsize,u16 ysize);
#endif

