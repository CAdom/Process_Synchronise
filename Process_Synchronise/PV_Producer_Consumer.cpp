#define _CRT_SECURE_NO_WARNINGS
#include"a.h"
#include"b.h"
#include"c.h"
void main()
{
	printf("进程同步---模拟生产者消费者问题\n");
	printf("**********************************\n");
	printf("请输入字符串：\n");
	scanf("%s", str);		//str[MAX]存放字符，模拟产品
	length = strlen(str);
	str_cnt = length;		//输入的字符个数

	init();		//初始化
	while (consumer_cnt < length)		//消费完所有字符结束
	{
		//system("cls");	//清屏
		printf("----------模拟指令运行程序----------\n");
		ProcessDispatch();	//处理器调度程序
		Simulate_exe();		//模拟指令运行程序
		print();			//输出
	}
	printf("\n程序结束\n");
}