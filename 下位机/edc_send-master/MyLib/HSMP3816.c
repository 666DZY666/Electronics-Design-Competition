#include "HSMP3816.h"
/*
    PA4 做输出端口

 */
void HSMP3816_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	  //使能PORTA通道时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //使能DAC通道时钟 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;				 // 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //模拟输入
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5)	;//PA.4 输出高
					
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //初始化DAC通道1
	DAC_Init(DAC_Channel_2,&DAC_InitType);	

	DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC1
	DAC_Cmd(DAC_Channel_2, ENABLE); 
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
	DAC_SetChannel2Data(DAC_Align_12b_R, 0);
}
/*
    IO:PA0
	实测最大为3100
    设置输出电压,最大为4096
*/
void HSMP3816_Control(u16 v){
	float temp=v;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12位右对齐数据格式设置DAC值
}
void JTB2_Control(u16 v){
	float temp=v;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel2Data(DAC_Align_12b_R,temp);//12位右对齐数据格式设置DAC值
}
