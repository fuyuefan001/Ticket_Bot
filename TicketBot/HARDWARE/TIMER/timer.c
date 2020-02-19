#include "timer.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//?????????,??????,??????????
//ALIENTEK STM32F7???
//?????????	   
//????@ALIENTEK
//????:www.openedv.com
//????:2015/11/27
//??:V1.0
//????,?????
//Copyright(C) ????????????? 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

TIM_HandleTypeDef TIM3_Handler;      //????? 
TIM_HandleTypeDef TIM2_Handler;      //????? 
extern u8 ov_frame;
extern volatile u16 jpeg_data_len;
extern volatile u8 jpeg_data_qinf;
//extern volatile u8 qinf;
//?????3?????
//arr:??????
//psc:??????
//???????????:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=???????,??:Mhz
//?????????3!(???3??APB1?,???HCLK/2)

TIM_HandleTypeDef TIM7_Handler;      //定时器句柄 
extern vu16 USART2_RX_STA;

//基本定时器7中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
TIM_HandleTypeDef TIM3_Handler;         //????? 
TIM_OC_InitTypeDef TIM3_CH3Handler;     //???3??4??
TIM_HandleTypeDef TIM3_Handler;         //????? 
TIM_OC_InitTypeDef TIM3_CH4Handler;     //???3??4??
void TIM7_Int_Init(u16 arr,u16 psc)
{
		TIM7_Handler.Instance=TIM7;                          //通用定时器3
    TIM7_Handler.Init.Prescaler=psc;                     //分频系数
    TIM7_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM7_Handler.Init.Period=arr;                        //自动装载值
    TIM7_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM7_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM7_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE									 
}
//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
// void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
// {
//     if(htim->Instance==TIM7)
// 	{
// 		__HAL_RCC_TIM7_CLK_ENABLE();            //使能TIM7时钟
// 		HAL_NVIC_SetPriority(TIM7_IRQn,0,1);    //设置中断优先级，抢占优先级0，子优先级1
// 		HAL_NVIC_EnableIRQ(TIM7_IRQn);          //开启ITM7中断   
// 	}
// }
//定时器7中断服务程序		    
void TIM7_IRQHandler(void)
{ 	    		    
	// HAL_TIM_IRQHandler(&TIM7_Handler);
		 USART2_RX_STA|=1<<15;	//标记接收完成
		 __HAL_TIM_CLEAR_FLAG(&TIM7_Handler,TIM_EventSource_Update );       //清除TIM7更新中断标志  
	 TIM7->CR1&=~(1<<0);     			//关闭定时器7     											 
} 
void TIM_SetTIM3Compare4(u32 compare)
{
	TIM3->CCR4=compare; 
}
void TIM_SetTIM3Compare3(u32 compare)
{
	TIM3->CCR3=compare; 
}
void TIM3_PWM_Init(u16 arr,u16 psc)
{ 
    TIM3_Handler.Instance=TIM3;            //???3
    TIM3_Handler.Init.Prescaler=psc;       //?????
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//??????
    TIM3_Handler.Init.Period=arr;          //??????
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM3_Handler);       //???PWM
    
    TIM3_CH3Handler.OCMode=TIM_OCMODE_PWM1; //????PWM1
    TIM3_CH3Handler.Pulse=arr/2;            //?????,?????????,
                                            //???????????????,?????50%
    TIM3_CH3Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //???????? 
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH3Handler,TIM_CHANNEL_3);//??TIM3??4
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_3);//??PWM??4
   TIM_SetTIM3Compare3(150); //?????,?????
   TIM_SetTIM3Compare4(150); 
}
void TIM3_PWM_Init1(u16 arr,u16 psc)
{ 
    TIM3_Handler.Instance=TIM3;            //???3
    TIM3_Handler.Init.Prescaler=psc;       //?????
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//??????
    TIM3_Handler.Init.Period=arr;          //??????
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM3_Handler);       //???PWM
    
    TIM3_CH4Handler.OCMode=TIM_OCMODE_PWM1; //????PWM1
    TIM3_CH4Handler.Pulse=arr/2;            //?????,?????????,
                                            //???????????????,?????50%
    TIM3_CH4Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //???????? 
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH4Handler,TIM_CHANNEL_4);//??TIM3??4
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_4);//??PWM??4
   TIM_SetTIM3Compare3(150); //?????,?????
   TIM_SetTIM3Compare4(150); 
}


void TIM3_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;                          //?????3
    TIM3_Handler.Init.Prescaler=psc;                     //??
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //?????
    TIM3_Handler.Init.Period=arr;                        //?????
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM3_Handler);
    
    //HAL_TIM_Base_Start_IT(&TIM3_Handler); //?????3????3??   
}

//???????,????,???????
//?????HAL_TIM_Base_Init()????
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	 if(htim->Instance==TIM5)
	{
    __HAL_RCC_TIM3_CLK_ENABLE();            //??TIM3??
    HAL_NVIC_SetPriority(TIM3_IRQn,10,3);    //???????,?????1,????3
    HAL_NVIC_EnableIRQ(TIM3_IRQn);          //??ITM3??  
}

	    if(htim->Instance==TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();            //??TIM2??
		HAL_NVIC_SetPriority(TIM5_IRQn,3,3);    //???????,?????1,????3
		HAL_NVIC_EnableIRQ(TIM5_IRQn);          //??ITM3??   
	}  
        if(htim->Instance==TIM7)
	{
		__HAL_RCC_TIM7_CLK_ENABLE();            //使能TIM7时钟
		HAL_NVIC_SetPriority(TIM7_IRQn,9,9);    //设置中断优先级，抢占优先级0，子优先级1
		HAL_NVIC_EnableIRQ(TIM7_IRQn);          //开启ITM7中断   
	}
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
 __HAL_RCC_TIM3_CLK_ENABLE();   //?????3
    __HAL_RCC_GPIOB_CLK_ENABLE();   //??GPIOB??
 
    GPIO_Initure.Pin=GPIO_PIN_0;            //PB1
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;   //??????
    GPIO_Initure.Pull=GPIO_PULLUP;          //??
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //??
 GPIO_Initure.Alternate=GPIO_AF2_TIM3; //PB1???TIM3_CH4
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);


//GPIO_InitTypeDef GPIO_Initure;
 __HAL_RCC_TIM3_CLK_ENABLE();   //?????3
    __HAL_RCC_GPIOB_CLK_ENABLE();   //??GPIOB??
 
    GPIO_Initure.Pin=GPIO_PIN_1;            //PB1
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;   //??????
    GPIO_Initure.Pull=GPIO_PULLUP;          //??
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //??
 GPIO_Initure.Alternate=GPIO_AF2_TIM3; //PB1???TIM3_CH4
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}
//???3??????
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}

//???3????????



void TIM2_Init(u16 arr,u16 psc)
{  
    TIM2_Handler.Instance=TIM5;                          //?????3
    TIM2_Handler.Init.Prescaler=psc;                     //??
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //?????
    TIM2_Handler.Init.Period=arr;                        //?????
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//??????
    HAL_TIM_Base_Init(&TIM2_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM2_Handler); //?????3????3????:TIM_IT_UPDATE    
}

//???????,????,???????
//?????HAL_TIM_Base_Init()????


//???3??????
void TIM5_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM2_Handler);
}

