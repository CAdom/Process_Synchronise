#define _CRT_SECURE_NO_WARNINGS
#include"a.h"
#include"b.h"
#include"c.h"
void main()
{
	printf("����ͬ��---ģ������������������\n");
	printf("**********************************\n");
	printf("�������ַ�����\n");
	scanf("%s", str);		//str[MAX]����ַ���ģ���Ʒ
	length = strlen(str);
	str_cnt = length;		//������ַ�����

	init();		//��ʼ��
	while (consumer_cnt < length)		//�����������ַ�����
	{
		//system("cls");	//����
		printf("----------ģ��ָ�����г���----------\n");
		ProcessDispatch();	//���������ȳ���
		Simulate_exe();		//ģ��ָ�����г���
		print();			//���
	}
	printf("\n�������\n");
}