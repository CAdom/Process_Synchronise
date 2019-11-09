/*定义数据结构和全局变量*/
typedef struct PCB {
	char name[10];	 //进程名
	char state[10];	 //运行状态
	char reason[10]; //阻塞原因
	int breakpoint;	 //断点
	struct PCB *next;	//阻塞时的顺序
}PCB,*Node;

int s1, s2;		//信号量
Node producer;	//生产者
Node consumer;	//消费者
Node ready;		//就绪链队列
Node block_s1;	//s1阻塞链队列
Node block_s2;	//s2阻塞链队列

char buffer[BUF];//缓冲池
char str[MAX];	//输入的字符串
int sp=0;		//字符串指针
int length;		//字符串长度
int in = 0;		//生产者指针
int out = 0;	//消费者指针
int pro_queue[MAX];//生产者记录c
int p1=0;			//生产者记录指针
int con_queue[MAX];//消费者记录
int c1=0;			//消费者记录指针
int PC;			//程序计数器
int consumer_cnt;	//消费者计数器
int str_cnt;		//字符计数器
char temp;		//暂存消费者字符


