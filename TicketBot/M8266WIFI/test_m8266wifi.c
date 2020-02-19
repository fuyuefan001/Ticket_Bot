/********************************************************************
 * test_m8266wifi.c
 * .Description
 *     Source file of M8266WIFI testing application 
 * .Copyright(c) Anylinkin Technology 2015.5-
 *     IoT@anylinkin.com
 *     http://www.anylinkin.com
 *     http://anylinkin.taobao.com
 *  Author
 *     wzuo
 *  Date
 *  Version
 ********************************************************************/
 
#include "stdio.h"
#include "brd_cfg.h"
#include "M8266WIFIDrv.h"
#include "M8266HostIf.h"
#include "M8266WIFI_ops.h"
#include "led.h"

void M8266WIFI_Test(void)
{
	 u16 i;
	 u16 status = 0;
	 u8  link_no=0;

 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	 Macro Defitions SETUP  SOCKET CONNECTIONS  (Chinese: 创建套接字的一些宏)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 
   /////////////
   ////	 Macro for Test Type(Chinese：测试类型宏定义)
	 #define TEST_M8266WIFI_TYPE    3	     //           1 = Repeative Sending, 2 = Repeative Reception, 3 = Echo  4 = multi-clients transimission test
	                                       // (Chinese: 1=模组向外不停地发送数据 2=模组不停地接收数据 3= 模组将接收到的数据发送给发送方 4=多客户端测试) 

	 /////////////
   ////	 Macros for Socket Connection Type (Chinese：套接字类型的宏定义) 
	 #define TEST_CONNECTION_TYPE   1	    //           0=WIFI module as UDP, 1=WIFI module as TCP Client, 2=WIFI module as TCP Server
	                                      // (Chinese: 0=WIFI模组做UDP, 1=WIFI模组做TCP客户端, 2=WIFI模组做TCP服务器
   /////////////
   ////	 Macros for Soket ip:port pairs  (Chinese：套接字的本地端口和目标地址目标端口的宏定义) 
   //local port	(Chinese：套接字的本地端口)
#if (TEST_CONNECTION_TYPE==1)         //// if module as TCP Client (Chinese:如果模组作为TCP客户端)
	 #define TEST_LOCAL_PORT  			0			//           local port=0 will generate a random local port each time fo connection. To avoid the rejection by TCP server due to repeative connection with the same ip:port
   	                                    // (Chinese: 当local port传递的参数为0时，本地端口会随机产生。这一点对于模组做客户端反复连接服务器时很有用。因为随机产生的端口每次会不一样，从而避免连续两次采用同样的地址和端口链接时被服务器拒绝。
#elif (TEST_CONNECTION_TYPE==0) || (TEST_CONNECTION_TYPE==2) //// if module as UDP or TCP Server (Chinese:如果模组作为UDP或TCP服务器)
   #define TEST_LOCAL_PORT  			4321  //           a local port should be specified //(Chinese:如果模组作为UDP或TCP服务器，则需要指定该套接字的本地端口)
#else
#error WRONG TEST_CONNECTION_TYPE defined !
#endif                                  // (Chinese: 如果模组作为TCP服务器或UDP，那么必须指定本地端口

   //local port	(Chinese：套接字的目标地址和目标端口)
#if (TEST_CONNECTION_TYPE==0)        //// if module as UDP (Chinese:如果模组作为UDP，则可以指定目标地址和端口，也可以随便填充，在发送数据时再设置或更改)
   #define TEST_REMOTE_ADDR    		"192.168.4.2"
   #define TEST_REMOTE_PORT  	    1234
#elif (TEST_CONNECTION_TYPE==1)      //// if module as TCP Client (Chinese:如果模组作为TCP客户端，当然必须指定目标地址和目标端口，即模组所要去连接的TCP服务器的地址和端口)
   #define TEST_REMOTE_ADDR    	 	"192.168.137.1"  // "www.baidu.com"
   #define TEST_REMOTE_PORT  	    4321						// 80
#elif (TEST_CONNECTION_TYPE==2)     //// if module as TCP Server (Chinese:如果模组作为TCP服务器)
		#define TEST_REMOTE_ADDR      "1.1.1.1" // no need remote ip and port upon setup connection. below values just for filling and any value would be ok. 
    #define TEST_REMOTE_PORT  	  1234  	 //(Chinese: 创建TCP服务器时，不需要指定目标地址和端口，这里的数据只是一个格式填充，随便填写。
#else
#error WRONG TEST_CONNECTION_TYPE defined !
#endif			 


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	 Setup Connection and Config connection upon neccessary (Chinese: 创建套接字，以及必要时对套接字的一些配置)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////
//step 0: config tcp windows number (Chinese: 步骤0：如果是TCP类型的套接字，可以配置调整窗口参数）
#if ( 1 && ((TEST_CONNECTION_TYPE==1)||(TEST_CONNECTION_TYPE==2)) ) //If you hope to change TCP Windows, please change '0' to '1' in the #if clause before setup the connection
																																		//(Chinese: 如果想改变套接字的窗口数，可以将#if语句中的0改成1，这个配置需要在创建套接字之前执行)
// u8 M8266WIFI_SPI_Config_Tcp_Window_num(u8 link_no, u8 tcp_wnd_num, u16* status)
  while(M8266WIFI_SPI_Config_Tcp_Window_num(link_no, 4, &status)==0)
  {

     	  LED0(0);
	  LED1(0);
	   
	}
#endif

///////	
//step 1: setup connection (Chinese: 步骤1：创建套接字连接）

  //  u8 M8266WIFI_SPI_Setup_Connection(u8 tcp_udp, u16 local_port, char remote_addr, u16 remote_port, u8 link_no, u8 timeout_in_s, u16* status);
	if(M8266WIFI_SPI_Setup_Connection(TEST_CONNECTION_TYPE, TEST_LOCAL_PORT, TEST_REMOTE_ADDR, TEST_REMOTE_PORT, link_no, 20, &status)==0)
	{		
		 while(1)
		 {
	  LED0(0);
	  LED1(0);
		 }
	}
	else  // else: setup connection successfully, we could config it (Chinese: 创建套接字成功，就可以配置套接字）
	{
#if (0 && (TEST_CONNECTION_TYPE == 0) )  		//// If UDP, then the module could join a multicust group. If you hope to use multicust, Change first '0' to '1'  in the #if clause
		                                        //   (Chinese: 如果这个套接字是UDP，那么可以配置成组播模式。如果需要配置成组播，可以将#if语句中的第一个'0'改成'1')
			//u8 M8266WIFI_SPI_Set_Multicuast_Group(u8 join_not_leave, char multicust_group_ip[15+1], u16* status)
		 if(M8266WIFI_SPI_Op_Multicuast_Group(0, "224.6.6.6", &status)==0)
     {
		   while(1)
		   {
      	  LED0(0);
	  LED1(0);			 
		   }
     }
     else

#elif (TEST_CONNECTION_TYPE == 2)          //// If TCP server, then tcp server auto disconnection timeout, and max clients allowed could be set	
		                                        //  (Chinese: 如果TCP服务器，那么可以设置这个TCP服务器(因长时间无通信而)断开客户端的超时时间)
#if 1
		 //u8 M8266WIFI_SPI_Set_TcpServer_Auto_Discon_Timeout(u8 link_no, u16 timeout_in_s, u16* status)	
		 if( M8266WIFI_SPI_Set_TcpServer_Auto_Discon_Timeout(link_no, 120, &status) == 0)
     {
		   while(1)
		   {
     	  LED0(0);
	  LED1(0);		 
		   }
     }
#endif		 
#if 0
  		   //u8 M8266WIFI_SPI_Config_Max_Clients_Allowed_To_A_Tcp_Server(u8 server_link_no, u8 max_allowed, u16* status);
     else if( M8266WIFI_SPI_Config_Max_Clients_Allowed_To_A_Tcp_Server(link_no, 5, &status)==0)
		 {
		   while(1)
		   {
       #ifdef USE_LED_AND_KEY_FOR_TEST	// led flash in 1Hz when error
	        LED_set(0, 0); LED_set(1, 0); M8266WIFI_Module_delay_ms(1000);
	        LED_set(0, 1); LED_set(1, 1); M8266WIFI_Module_delay_ms(1000);
       #endif		 
		   }
		 }
#endif		 
		 else
#endif
		 //Setup Connection successfully (Chinese: 设置套接字链接成功)
     {
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
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	 Communication Test (Chinese: WIFI套接字的数据收发通信测试)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if (TEST_M8266WIFI_TYPE==3)  // Echo test: to receive data from remote and then echo back to remote (Chinese: 收发测试，模组将接收到的数据立刻返回给发送方)
{
#define  RECV_DATA_MAX_SIZE  128   

   u8  RecvData[RECV_DATA_MAX_SIZE];   // make sure the stack size is more than RECV_DATA_MAX_SIZE to avoid stack leak (Chinese: 这里有大数组。请确保单片机的堆栈足够大，至少不少于数据和变量的总长度和，否则单片机程序可能会出现溢出错误)
   u16 received = 0;
	 u16 sent;
	
	 for(i=0; i<RECV_DATA_MAX_SIZE; i++) RecvData[i]=i; 

   link_no = 0;
   sent = M8266WIFI_SPI_Send_Data(RecvData, 128, link_no, &status);

   while(1)
	 {
			if(M8266WIFI_SPI_Has_DataReceived()) //if received data (Chinese: 如果接收到数据)
			{
				//Receive the data (Chinese: 单片机接收数据)
				//u16 M8266WIFI_SPI_RecvData(u8 data[], u16 max_len, uint16_t max_wait_in_ms, u8* link_no, u16* status);
	      received = M8266WIFI_SPI_RecvData(RecvData, RECV_DATA_MAX_SIZE, 5*1000, &link_no, &status);
				
				if(received!=0) //if received data length is not 0  (Chinese: 如果单片机的确接收到了数据，即长度不等于0)
				{
					u16 tcp_packet_size = 1024;
          u16 loops     = 0;
		      u16 max_loops = 5000;
			    u32 len       = received; 
		     
          for(sent=0, loops=0; (sent<len)&&(loops<=max_loops); loops++)
          {		
				    sent += M8266WIFI_SPI_Send_Data(RecvData+sent, ((len-sent)>tcp_packet_size)?tcp_packet_size:(len-sent), link_no, &status);
            if(sent>=len)  break;
			      if((status&0xFF) == 0x00)
			      {
							 loops = 0;
			      }
		        else
			      {
				      if(   ((status&0xFF) == 0x14)      // 0x14 = connection of link_no not present (Chinese: 该套接字不存在)
                 || ((status&0xFF) == 0x15) )    // 0x15 = connection of link_no closed(Chinese: 该套接字已经关闭或断开)
	            {
								 M8266HostIf_delay_us(99);
          			 //need to re-establish the socket connection (Chinese: 需要重建建立套接字)
	            }
							else if( (status&0xFF) == 0x18 )        // 0x18 = TCP server in listening states and no tcp clients have connected. (Chinese: 这个TCP服务器还没有客户端连接着它)
			        {
				         M8266HostIf_delay_us(100);
			        }
	            else
	            {
	               M8266HostIf_delay_us(250);
	            }
			      }
          } // end of for(...
		    } // end of if(received!=0)
			}
		} // end of while(1)
}	 
 


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#else
#error NOT Supported Test Type! should be 1~4!		
#endif

} // end of M8266WIFI_Test
