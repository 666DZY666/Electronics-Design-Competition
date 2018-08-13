#include "chinese.h"
#include "lcd.h"	   
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板——扩展实验
//自定义汉字显示  代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/7/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
			    
//在LCD上的（x，y）处画点
//color：点的颜色
void LCD_Draw_Point(u16 x,u16 y,u16 color)
{
	u16 temp;
	temp=POINT_COLOR;
	POINT_COLOR=color;
  LCD_DrawPoint(x,y);
	POINT_COLOR=temp;
}
//在指定位置 显示1个16*16的汉字
//(x,y):汉字显示的位置
//index:tfont数组里面的第几个汉字
//color:这个汉字的颜色
void LCD_ShowOne(u16 x,u16 y,u8* tfont,u16 pcolor,u16 bcolor)
{   			    
	u8 temp,t,t1;
	u16 y0=y;				   
	for(t=0;t<32;t++)//每个16*16的汉字点阵 有32个字节
	{   
			temp=tfont[t];      //前16个字节
	//	else temp=tfont[t-16];    //后16个字节	                          
		 for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)
				LCD_Draw_Point(x,y,pcolor);//画实心点
			else 
				LCD_Draw_Point(x,y,bcolor);   //画空白点（使用背景色）
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}          
}

/*
//在指定位置 显示1个24*24的汉字
//(x,y):汉字显示的位置
//index:tfont数组里面的第几个汉字
//color:这个汉字的颜色
void Test_Show_CH_Font24(u16 x,u16 y,u8 index,u16 color)
{   			    
	u8 temp,t,t1;
	u16 y0=y;				   
    for(t=0;t<72;t++)//每个24*24的汉字点阵 有72个字节
    {   
		if(t<24)temp=tfont24[index*3][t];           //前24个字节
		else if(t<48)temp=tfont24[index*3+1][t-24]; //中24个字节	                          
        else temp=tfont24[index*3+2][t-48];         //后24个字节
	    for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_Draw_Point(x,y,color);//画实心点
			else LCD_Draw_Point(x,y,BACK_COLOR);   //画空白点（使用背景色）
			temp<<=1;
			y++;
			if((y-y0)==24)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
} */

//显示任意个汉字
void LCD_ShowFont(u16 x,u16 y,u8 tfont[][32],u8 len,u16 pcolor,u16 bcolor)
{
	u8 t;
	for(t=0;t<len;t++)//len个汉字
	{
		LCD_ShowOne(x,y,*(tfont+t),pcolor,bcolor);
		x+=16;
	}
}



//显示一副BMP图片
void LCD_ShowBmp(u16 x,u16 y,u16 bmp_length,u16 bmp_height,u16* buf)
{
	u16 i;
	u16 j;
	
	for(j=0;j<bmp_height;j++)
	{
		LCD_SetCursor(x,y);    		//设置光标位置 
		LCD_WriteRAM_Prepare();     			//开始写入GRAM
		for(i=0;i<bmp_length;i++)
		{
			LCD_WR_DATA(*buf++);
		}
		y++;
//		buf+=length_null;//???????BMP
	}
} 




