/*
	自己写的一些小的工具
*/

#include "Tool.h"
/*
	函数功能：
				匹配两个字符串是否相等
*/
int match(u8 *a,u8 *b)
{
	for(; *a!='\0'||*b!='\0'; a++,b++)
		{
			if(*a!=*b) return 0;
		}
	return 1;
}
/*
	函数功能：
				将一个数组清零
*/
void clear(u8 a[])
{
	for(; *a!='\0'; a++)
		{
			*a=NULL;
		}
}
/*
	函数功能:
		将一个一维数组放到二维数组中,一维数组不同的部分用空格隔开
		buff:一维数组
		argv:处理后的二维数组
*/
int deal(u8 *buff,u8 argv[][10]){
	int p=0,j=0;
	if (strlen((const char*)buff) == 0) return 0;
	for(int i=0;i<strlen((const char*)buff);i++){
		if(buff[i]!=' '){
			argv[j][p]=buff[i];
			p++;
		}else{
			while(buff[i+1]==' ') i++; //处理多个空格的情况
			if(buff[i+1]=='\0')	return j+1;//处理以空格结束的情况
			j++; //挪到下一行
			p=0; //指向数组头
		}
	}
	return j+1;
}
