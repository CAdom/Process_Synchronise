/*�������ݽṹ��ȫ�ֱ���*/
typedef struct PCB {
	char name[10];	 //������
	char state[10];	 //����״̬
	char reason[10]; //����ԭ��
	int breakpoint;	 //�ϵ�
	struct PCB *next;	//����ʱ��˳��
}PCB,*Node;

int s1, s2;		//�ź���
Node producer;	//������
Node consumer;	//������
Node ready;		//����������
Node block_s1;	//s1����������
Node block_s2;	//s2����������

char buffer[BUF];//�����
char str[MAX];	//������ַ���
int sp=0;		//�ַ���ָ��
int length;		//�ַ�������
int in = 0;		//������ָ��
int out = 0;	//������ָ��
int pro_queue[MAX];//�����߼�¼c
int p1=0;			//�����߼�¼ָ��
int con_queue[MAX];//�����߼�¼
int c1=0;			//�����߼�¼ָ��
int PC;			//���������
int consumer_cnt;	//�����߼�����
int str_cnt;		//�ַ�������
char temp;		//�ݴ��������ַ�


