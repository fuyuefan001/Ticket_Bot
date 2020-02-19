#include "myjpeg.h"
#include "ltdc.h"
#include "lcd.h"
#include "malloc.h"
#include "jpeg_utils.h"
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

JPEG_HandleTypeDef  JPEG_Handler;           //JPEG句柄
DMA_HandleTypeDef   JPEGDMAIN_Handler;      //JPEG数据输入DMA
DMA_HandleTypeDef   JPEGDMAOUT_Handler;     //JPEG数据输出DMA

_jpeg_dev jpeg_dev;     //管理JPEG的重要参数

JPEG_YCbCrToRGB_Convert_Function pConvert_Function;

u8 Out_DataBuffer[OUT_BUFFER_NUM][CHUNK_SIZE_OUT];  //输出缓冲区
u8 In_DataBuffer[OUT_BUFFER_NUM][CHUNK_SIZE_IN];    //输入缓冲区

extern u32 decodetime;

//显示JPEG图片
//x,y：图片左上角在LCD上的坐标
//JPEGFileName:要显示的图片文件路径
//mode:显示模式，0 根据指定位置显示,X和Y参数有效
//               1 居中显示，X和Y参数无效
void JPEG_Display(u16 x,u16 y,u8 *JPEGFileName,u8 mode)
{
    JPEG_ConfTypeDef JPEG_Info;
    u8 state=0;
    
    jpeg_dev.totalblock=0;          //总块数清零
    jpeg_dev.blockindex=0;          //初始化块索引为零
    jpeg_dev.decodend=0;            //解码状态标识清零，解码未完成
    jpeg_dev.FrameBufAddr=0;        //帧地址清零

    jpeg_dev.outbuf_readindex=0;    //输出缓冲区读位置清零
    jpeg_dev.outbuf_writeindex=0;   //输出缓冲区写位置清零
    jpeg_dev.output_paused=0;  

    jpeg_dev.inbuf_readindex=0;     //输入缓冲区读位置清零
    jpeg_dev.inbuf_writeindex=0;    //输入缓冲区写位置清零
    jpeg_dev.input_paused=0;      
    
    if(f_open(&jpeg_dev.jpegfile,(const TCHAR*)JPEGFileName,FA_READ)!=FR_OK)//打开图片
    {
        printf("图片打开失败\r\n");
    }
    jpeg_decode(JPEG_OUTPUT_DATA_BUFFER);
    do
    {
        SCB_CleanInvalidateDCache();
        jpegdata_input();
        state=jpegdata_output();
          
    }while(state==0);
    HAL_JPEG_GetInfo(&JPEG_Handler,&JPEG_Info);
    
    //图片尺寸小于LCD尺寸的时候才能居中显示
    if(mode&&(lcddev.width>=JPEG_Info.ImageWidth)&&(lcddev.height>=JPEG_Info.ImageHeight))
    {
        x=(lcddev.width-JPEG_Info.ImageWidth)/2;
        y=(lcddev.height-JPEG_Info.ImageHeight)/2;
    }
    printf("解码图片用时:%dms\r\n",decodetime/10);
    dma2d_datacopy((u32 *)JPEG_OUTPUT_DATA_BUFFER,(u32 *)LCD_FRAME_BUF_ADDR,x,y,lcddev.width,lcddev.height);
    f_close(&jpeg_dev.jpegfile);        //关闭文件 
}

//JPEG解码模块初始化
void JPEG_Init(void)
{ 
    u8 i=0;
       
    //输出数据缓冲区相关信息初始化
    for(i=0;i<OUT_BUFFER_NUM;i++)
    {
        jpeg_dev.OUT_Buffer[i].State=JPEG_BUFFER_EMPTY;
        jpeg_dev.OUT_Buffer[i].DataBufferSize=0;
        jpeg_dev.OUT_Buffer[i].DataBuffer=Out_DataBuffer[i];
    }
  
    //输入数据缓冲区相关信息初始化
    for(i=0;i<OUT_BUFFER_NUM;i++)
    {
        jpeg_dev.IN_Buffer[i].State=JPEG_BUFFER_EMPTY;
        jpeg_dev.IN_Buffer[i].DataBufferSize=0;
        jpeg_dev.IN_Buffer[i].DataBuffer=In_DataBuffer[i];
    }
    
    JPEG_InitColorTables();         //初始化JPEG的LUT表，YCbCr和RGB之间的转换
    JPEG_Handler.Instance=JPEG;
    HAL_JPEG_Init(&JPEG_Handler);   //初始化JPEG
}

//JPEG底层驱动，时钟使能
//此函数会被HAL_JPEG_Init()调用
//hsdram:JPEG句柄
void HAL_JPEG_MspInit(JPEG_HandleTypeDef *hjpeg)
{
    __HAL_RCC_JPEG_CLK_ENABLE();            //使能JPEG时钟
    __DMA2_CLK_ENABLE();                    //使能DMA2时钟
    
    HAL_NVIC_SetPriority(JPEG_IRQn,1,0);    //配置JPEG中断，抢占优先级1，子优先级0
    HAL_NVIC_EnableIRQ(JPEG_IRQn);          //使能JPEG中断
    
    //配置数据输入DMA   
    JPEGDMAIN_Handler.Instance=DMA2_Stream0;                        //DMA2数据流0
    JPEGDMAIN_Handler.Init.Channel=DMA_CHANNEL_9;                   //通道9
    JPEGDMAIN_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;          //存储器到外设模式
    JPEGDMAIN_Handler.Init.PeriphInc=DMA_PINC_DISABLE;              //外设非增量模式
    JPEGDMAIN_Handler.Init.MemInc=DMA_MINC_ENABLE;                  //存储器增量模式
    JPEGDMAIN_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_WORD; //外设数据长度:32位
    JPEGDMAIN_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_WORD;    //存储器数据长度:32位
    JPEGDMAIN_Handler.Init.Mode=DMA_NORMAL;                         //普通模式
    JPEGDMAIN_Handler.Init.Priority=DMA_PRIORITY_HIGH;              //高优先级
    JPEGDMAIN_Handler.Init.FIFOMode=DMA_FIFOMODE_ENABLE;            //使能FIFO   
    JPEGDMAIN_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;   //使用全FIFO
    JPEGDMAIN_Handler.Init.MemBurst=DMA_MBURST_INC4;                //存储器4字节增量突发传输
    JPEGDMAIN_Handler.Init.PeriphBurst=DMA_PBURST_INC4;             //外设4字节增量突发传输
  
    __HAL_LINKDMA(hjpeg,hdmain,JPEGDMAIN_Handler);      //将数据输入DMA和JPEG的数据输入DMA连接起来
    HAL_DMA_DeInit(&JPEGDMAIN_Handler);            
    HAL_DMA_Init(&JPEGDMAIN_Handler);                   //初始化数据输入DMA

    //配置数据输出DMA  
    JPEGDMAOUT_Handler.Instance=DMA2_Stream1;                       //DMA2数据流1   
    JPEGDMAOUT_Handler.Init.Channel=DMA_CHANNEL_9;                  //通道9    
    JPEGDMAOUT_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;         //外设到存储器
    JPEGDMAOUT_Handler.Init.PeriphInc=DMA_PINC_DISABLE;             //外设非增量模式
    JPEGDMAOUT_Handler.Init.MemInc=DMA_MINC_ENABLE;                 //存储器增量模式
    JPEGDMAOUT_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_WORD;//外设数据长度:32位
    JPEGDMAOUT_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_WORD;   //存储器数据长度:32位
    JPEGDMAOUT_Handler.Init.Mode=DMA_NORMAL;                        //普通模式
    JPEGDMAOUT_Handler.Init.Priority=DMA_PRIORITY_VERY_HIGH;        //最高优先级
    JPEGDMAOUT_Handler.Init.FIFOMode=DMA_FIFOMODE_ENABLE;           //使能FIFO
    JPEGDMAOUT_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;  //使用全FIFO
    JPEGDMAOUT_Handler.Init.MemBurst=DMA_MBURST_INC4;               //存储器4字节增量突发传输
    JPEGDMAOUT_Handler.Init.PeriphBurst=DMA_PBURST_INC4;            //外设4字节增量突发传输

    __HAL_LINKDMA(hjpeg, hdmaout, JPEGDMAOUT_Handler);  //将数据输出DMA和JPEG的数据输出DMA连接起来
    HAL_DMA_DeInit(&JPEGDMAOUT_Handler);  
    HAL_DMA_Init(&JPEGDMAOUT_Handler);                  //初始化数据输出DMA
   
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn,2,0);//数据输入DMA中断优先级，抢占优先级2，子优先级0
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);      //使能JPEG的数据输入DMA中断
        
    HAL_NVIC_SetPriority(DMA2_Stream1_IRQn,2,0);//数据输出DMA中断优先级，抢占优先级2，子优先级0
    HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);      //使能JPEG的数据输出DMA中断
 
}

//JPEG解码
//DestAddress:目的地址
//返回值:0 成功，其他，失败
u8 jpeg_decode(u32 destaddr)
{
    u8 i,state=0;
    jpeg_dev.FrameBufAddr=destaddr;       
    //读取JPEG数据到输入buffer中
    for(i=0;i<IN_BUFFERS_NUM;i++)
    {
        if(f_read(&jpeg_dev.jpegfile,jpeg_dev.IN_Buffer[i].DataBuffer,CHUNK_SIZE_IN,(UINT*)(&jpeg_dev.IN_Buffer[i].DataBufferSize))==FR_OK)
        {
            jpeg_dev.IN_Buffer[i].State=JPEG_BUFFER_FULL;   //标记输入buffer已满
            state=0;
        }else state=1;      
    } 
    //解码JPEG(DMA方式)
    HAL_JPEG_Decode_DMA(&JPEG_Handler,jpeg_dev.IN_Buffer[0].DataBuffer,jpeg_dev.IN_Buffer[0].DataBufferSize,\
                        jpeg_dev.OUT_Buffer[0].DataBuffer,CHUNK_SIZE_OUT);
    return state;
}

//DMA2D数据拷贝
//srcbuf:源地址
//dstbuf:目的地址
//x:X坐标
//y:Y坐标
//xsize:X轴大小
//ysize:Y轴大小
void dma2d_datacopy(u32 *srcbuf,u32 *dstbuf,u16 x,u16 y,u16 xsize,u16 ysize)
{   
    u32 timeout=0; 
    u32 destina=(u32)dstbuf+(y*lcddev.width+x)*(lcdltdc.pixsize);
    u32 source=(u32)srcbuf;

    //配置DMA2D的模式
    DMA2D_Handler.Instance=DMA2D; 
    DMA2D_Handler.Init.Mode=DMA2D_M2M_PFC;                  //存储器到存储器并支持像素格式转换
    DMA2D_Handler.Init.ColorMode=LCD_PIXFORMAT;             //输出格式
    DMA2D_Handler.Init.OutputOffset=lcddev.width-xsize;     //输出偏移 
    DMA2D_Handler.Init.AlphaInverted=DMA2D_REGULAR_ALPHA;   
    DMA2D_Handler.Init.RedBlueSwap=DMA2D_RB_REGULAR;        
  
    //前景层配置
    DMA2D_Handler.LayerCfg[0].AlphaMode=DMA2D_REPLACE_ALPHA;        //允许修改前景层的ALPHA值
    DMA2D_Handler.LayerCfg[0].InputAlpha=0xFF;                      //输入ALPH值
    DMA2D_Handler.LayerCfg[0].InputColorMode=DMA2D_INPUT_ARGB8888;  //输入颜色模式
    DMA2D_Handler.LayerCfg[0].InputOffset=0;                        //输出偏移
    DMA2D_Handler.LayerCfg[0].RedBlueSwap=DMA2D_RB_REGULAR;
    DMA2D_Handler.LayerCfg[0].AlphaInverted=DMA2D_REGULAR_ALPHA; 
    
    HAL_DMA2D_Init(&DMA2D_Handler);                                 //初始化DMA2D
    HAL_DMA2D_ConfigLayer(&DMA2D_Handler,0);                        //配置层
    HAL_DMA2D_Start(&DMA2D_Handler,source,destina,xsize,ysize);     //开启DMA2D传输
    HAL_DMA2D_PollForTransfer(&DMA2D_Handler,5);                    //传输数据
    while((__HAL_DMA2D_GET_FLAG(&DMA2D_Handler,DMA2D_FLAG_TC)==0)&&(timeout<0X5000))//等待DMA2D完成
    {
        timeout++;
    }
    __HAL_DMA2D_CLEAR_FLAG(&DMA2D_Handler,DMA2D_FLAG_TC);       //清除传输完成标志  
}

//JPEG输出处理，用来将JPEG解码器解码出来的YUV信号转换为RGB信号。
//返回值:0，成功；其他，失败
u32 jpegdata_output(void)
{
    u32 datacount;
  
    if(jpeg_dev.OUT_Buffer[jpeg_dev.outbuf_readindex].State==JPEG_BUFFER_FULL)    //输出缓冲区已满
    {  
        //完成YUV到RGB的转换
        jpeg_dev.blockindex+=pConvert_Function(  jpeg_dev.OUT_Buffer[jpeg_dev.outbuf_readindex].DataBuffer, //要转换的数据
                                            (uint8_t *)jpeg_dev.FrameBufAddr,                               //转换完成后的数据存储区
                                            jpeg_dev.blockindex,                                            //块索引        
                                            jpeg_dev.OUT_Buffer[jpeg_dev.outbuf_readindex].DataBufferSize,  //要转换的数据量
                                            &datacount);                                                    //转换完成的数据量       
    
        jpeg_dev.OUT_Buffer[jpeg_dev.outbuf_readindex].State=JPEG_BUFFER_EMPTY; //转换完成以后标记此输出缓冲区为空
        jpeg_dev.OUT_Buffer[jpeg_dev.outbuf_readindex].DataBufferSize=0;        //数据长度清零
    
        jpeg_dev.outbuf_readindex++;    //输出缓冲区索引加一
        if(jpeg_dev.outbuf_readindex>=OUT_BUFFER_NUM)
        {
            jpeg_dev.outbuf_readindex=0;
        }
    
        if(jpeg_dev.blockindex==jpeg_dev.totalblock)
        {
            return 1;
        }
    }
    else if((jpeg_dev.output_paused==1)&& 
          (jpeg_dev.outbuf_writeindex==jpeg_dev.outbuf_readindex)&&\
          (jpeg_dev.OUT_Buffer[jpeg_dev.outbuf_readindex].State==JPEG_BUFFER_EMPTY))
    {
        jpeg_dev.output_paused=0;
        HAL_JPEG_Resume(&JPEG_Handler,JPEG_PAUSE_RESUME_OUTPUT);  //恢复处理     
    }
    return 0;  
}

//JPEG输入处理
void jpegdata_input(void)
{
    if(jpeg_dev.IN_Buffer[jpeg_dev.inbuf_writeindex].State==JPEG_BUFFER_EMPTY)//当前输入缓冲区为空，读取数据
    {
        if(f_read(&jpeg_dev.jpegfile,jpeg_dev.IN_Buffer[jpeg_dev.inbuf_writeindex].DataBuffer,  //从SD卡中读取数据
                CHUNK_SIZE_IN,(UINT*)(&jpeg_dev.IN_Buffer[jpeg_dev.inbuf_writeindex].DataBufferSize))==FR_OK)
        {  
            jpeg_dev.IN_Buffer[jpeg_dev.inbuf_writeindex].State=JPEG_BUFFER_FULL;   //标记当前输入缓冲区已满
        }
        else printf("数据读取错误\r\n");
    
        //当输入处理处于暂停，并且输入缓冲区写位置等于读位置时应该恢复处理
        if((jpeg_dev.input_paused==1)&&(jpeg_dev.inbuf_writeindex==jpeg_dev.inbuf_readindex))
        {
            jpeg_dev.input_paused=0;
            //配置输入缓冲区
            HAL_JPEG_ConfigInputBuffer( &JPEG_Handler,                                                  //JPEG句柄
                                        jpeg_dev.IN_Buffer[jpeg_dev.inbuf_readindex].DataBuffer,        //输入缓冲区
                                        jpeg_dev.IN_Buffer[jpeg_dev.inbuf_readindex].DataBufferSize);   //输入缓冲区长度
            HAL_JPEG_Resume(&JPEG_Handler, JPEG_PAUSE_RESUME_INPUT); //恢复数据处理
        }
    
        jpeg_dev.inbuf_writeindex++;    //写位置加一
        if(jpeg_dev.inbuf_writeindex>=IN_BUFFERS_NUM)
        {
            jpeg_dev.inbuf_writeindex=0;
        }            
    }
}

//JPEG图片信息读取回调函数
void HAL_JPEG_InfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo)
{
    //执行颜色转换
    if(JPEG_GetDecodeColorConvertFunc(pInfo,&pConvert_Function,&jpeg_dev.totalblock)!=HAL_OK)
    {
       printf("JPEG颜色转换失败!\r\n");
    }  
}

//获取JPEG数据回调函数
//hjpeg:JPEG句柄
//NbDecodedData:要读取的数据长度
void HAL_JPEG_GetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t NbDecodedData)
{
    if(NbDecodedData==jpeg_dev.IN_Buffer[jpeg_dev.inbuf_readindex].DataBufferSize)
    {  
        jpeg_dev.IN_Buffer[jpeg_dev.inbuf_readindex].State=JPEG_BUFFER_EMPTY;
        jpeg_dev.IN_Buffer[jpeg_dev.inbuf_readindex].DataBufferSize=0;
  
        jpeg_dev.inbuf_readindex++;
        if(jpeg_dev.inbuf_readindex>=IN_BUFFERS_NUM)
        {
            jpeg_dev.inbuf_readindex=0;        
        }
  
        if(jpeg_dev.IN_Buffer[jpeg_dev.inbuf_readindex].State==JPEG_BUFFER_EMPTY)
        {
            HAL_JPEG_Pause(hjpeg,JPEG_PAUSE_RESUME_INPUT); //暂停数据输入处理
            jpeg_dev.input_paused=1;                 
        }
        else
        {    
            HAL_JPEG_ConfigInputBuffer( hjpeg,
                                        jpeg_dev.IN_Buffer[jpeg_dev.inbuf_readindex].DataBuffer, 
                                        jpeg_dev.IN_Buffer[jpeg_dev.inbuf_readindex].DataBufferSize);    
        }
    }
    else
    {
        HAL_JPEG_ConfigInputBuffer( hjpeg,
                                    jpeg_dev.IN_Buffer[jpeg_dev.inbuf_readindex].DataBuffer+NbDecodedData, 
                                    jpeg_dev.IN_Buffer[jpeg_dev.inbuf_readindex].DataBufferSize-NbDecodedData);      
    }
}

//JPEG数据准备好
//hjpeg:JPEG句柄
//pDataOut:输出数据缓冲区
//OutDataLength:输出数据缓冲区长度
void HAL_JPEG_DataReadyCallback (JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t OutDataLength)
{
    jpeg_dev.OUT_Buffer[jpeg_dev.outbuf_writeindex].State=JPEG_BUFFER_FULL;
    jpeg_dev.OUT_Buffer[jpeg_dev.outbuf_writeindex].DataBufferSize=OutDataLength;
    
    jpeg_dev.outbuf_writeindex++;
    if(jpeg_dev.outbuf_writeindex>=OUT_BUFFER_NUM)
    {
        jpeg_dev.outbuf_writeindex=0;        
    }

    if(jpeg_dev.OUT_Buffer[jpeg_dev.outbuf_writeindex].State!=JPEG_BUFFER_EMPTY)
    {
        HAL_JPEG_Pause(hjpeg,JPEG_PAUSE_RESUME_OUTPUT); //暂停数据输出处理
        jpeg_dev.output_paused=1;
    }
    HAL_JPEG_ConfigOutputBuffer(hjpeg,
                                jpeg_dev.OUT_Buffer[jpeg_dev.outbuf_writeindex].DataBuffer,
                                CHUNK_SIZE_OUT); 
}

//JPEG解码完成
void HAL_JPEG_DecodeCpltCallback(JPEG_HandleTypeDef *hjpeg)
{
     jpeg_dev.decodend=1;   //JPEG解码完成
}

//JPEG中断服务函数
void JPEG_IRQHandler(void)
{
    HAL_JPEG_IRQHandler(&JPEG_Handler);
}

//DMA2数据流0中断服务函数
void DMA2_Stream0_IRQHandler(void)
{
    HAL_DMA_IRQHandler(JPEG_Handler.hdmain);
}

//DMA2数据流1中断服务函数
void DMA2_Stream1_IRQHandler(void)
{   
    HAL_DMA_IRQHandler(JPEG_Handler.hdmaout);
}


