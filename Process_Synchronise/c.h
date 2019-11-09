/*函数原型定义：*/
void init();	//初始化
void p(int s);	//p操作
void v(int s);  //v操作
void block(int s);	//阻塞操作
void wakeup(int s);	//唤醒操作
void ProcessDispatch();	//处理器调度
void Simulate_exe();	//模拟指令执行
void print();			//打印输出函数

/*初始化函数init()
  输入：
  输出：
 */
void init()
{
	s1 = BUF;	//空信号量，初始化为缓冲池最大值
	s2 = 0;		//满信号量，初始化为0
	producer = (Node)malloc(sizeof(PCB));	//创建一个生产者结点，并初始化
	strcpy(producer->name, "Producer");
	strcpy(producer->state, "Ready");
	strcpy(producer->reason, "null");
	producer->breakpoint = 0;
	producer->next = NULL;

	consumer = (Node)malloc(sizeof(PCB));	//创建一个消费者结点，并初始化
	strcpy(consumer->name, "Consumer");
	strcpy(consumer->state, "Ready");
	strcpy(consumer->reason, "null");
	consumer->breakpoint = 0;
	consumer->next = NULL;

	ready = producer;		//初始化就绪队列，生产进程在前，消费进程在后
	ready->next = consumer;
	consumer->next = NULL;
	
	block_s1 = NULL;		//初始化阻塞进程，为空
	block_s2 = NULL;

	PC = 0;					//程序计数器
	consumer_cnt=0;			//消费者计数器
}

/*p操作函数p(int s)
  输入：进程号
  输出：缓冲区空闲，申请信号量成功，否则阻塞该进程
*/
void p(int s)
{
	if (s == 1)					//p(s1)
	{
		s1--;
		if (s1 < 0)
			block(1);			//阻塞生产进程
		else
		{
			cout << "\ts1信号量申请成功！" << endl;
			ready->breakpoint = PC;		//保存断点
		}
	}
	else			//p(s2)
	{
		s2--;
		if (s2 < 0)
			block(2);			//阻塞消费进程
		else
		{
			cout << "\ts2信号量申请成功！" << endl;
			ready->breakpoint = PC;		//保存断点
		}
	}
}

/*v操作函数v(int s)
  输入：进程号
  输出：唤醒一个就绪进程
*/
void v(int s)
{
	if (s == 1)		//v(s1)
	{
		s1++;
		if (s1 <= 0)
			wakeup(1);			//唤醒生产进程
			ready->breakpoint = PC;		//保存断点
	}
	else			//v(s2)
	{
		s2++;
		if (s2 <= 0)
			wakeup(2);			//唤醒消费进程
			ready->breakpoint = PC;		//保存断点
	}
}

/*阻塞函数block(int s)
  输入：进程号
  输出：阻塞当前进程
*/
void block(int s)
{
	Node p;		//阻塞队列指针
	int num1 = 0;	//记录阻塞队列中的进程数
	int num2 = 0;
	if (s == 1)		//生产者进程
	{
		strcpy(producer->state, "Block");	//修改状态为阻塞
		strcpy(producer->reason, "s1");		//阻塞原因为s1
		p = block_s1;
		while (p)
		{
			num1++;
			p = p->next;
		}
		if (!block_s1)
			block_s1 = producer;
		else
			p = producer;
		producer->next = NULL;
		printf("\tproducer生产者进程阻塞了！\n");
		ready->breakpoint = PC;	//保存断点
		ready = ready->next;	//在就绪队列中去掉，指向下一个
		num1++;
	}
	else		//消费者进程
	{
		strcpy(consumer->state, "Block");	//修改状态为阻塞
		strcpy(consumer->reason, "s2");		//阻塞原因为s1
		p = block_s2;
		while (p)
		{
			num2++;
			p = p->next;
		}
		if (!block_s2)
			block_s2 = consumer;
		else
			p = consumer;
		ready->breakpoint = PC;	//保存断点
		ready = ready->next;	//在就绪队列中去掉，指向下一个
		consumer->next = NULL;
		printf("\tconsumer消费者进程阻塞了！\n");
		num2++;
	}
	printf("\t阻塞的生产者进程个数为：%d\n", num1);
	printf("\t阻塞的消费者进程个数为：%d\n", num2);
}

/*唤醒函数wakeup(int s)
  输入：进程号
  输出：唤醒当前进程
*/
void wakeup(int s)
{
	Node p;
	Node q = ready;
	if (s == 1)		//唤醒block_s1队首生产者进程
	{
		p = block_s1;
		block_s1 = block_s1->next;   //阻塞指针指向下一个阻塞进程
		strcpy(p->state, "Ready");
		strcpy(p->reason, "null");
		while (q)		//插入就绪队列
			q = q->next;
		q = p;
		p->next = NULL;
		printf("\tproducer生产者进程唤醒了！\n");
	}
	else       //唤醒block_s1队首消费者进程
	{
		p = block_s2;
		block_s2 = block_s2->next;   //阻塞指针指向下一个阻塞进程
		strcpy(p->state, "Ready");
		strcpy(p->reason, "null");
		while (q->next)		//插入就绪队列
			q = q->next;
		q ->next= p;
		p->next = NULL;
		printf("\tconsumer消费者进程唤醒了！\n");
	}
}

//处理器调度函数
void ProcessDispatch()		
{
	int rand_number;	 //产生随机数
	int num = 0;	     //就绪进程个数

	Node p = ready;		//就绪队列指针
	if (ready == NULL)	//无就绪进程，结束
		return;
	
	while (p!=NULL)		 //统计就绪进程个数
	{
		num++;
		p = p->next;
	}
	printf("\t就绪进程个数为：%d\n",num);

	time_t t;
	srand((unsigned) time(&t));
	rand_number = rand() % num;  //随机函数产生随机数
	if (rand_number == 1)
	{
		p = ready;
		ready = ready->next;
		ready->next = p;
		p->next = NULL;
		strcpy(ready->state, "Run");
		strcpy(ready->next->state, "Ready");
	}
	else
		strcpy(ready->state, "Run");
	PC = ready->breakpoint;
}

//模拟处理器指令执行函数
void Simulate_exe()
{
	if(strcmp(ready->name,"Producer")==0)	//当前进程为生产者进程
		switch (PC)
		{
		case 0:		//produce
			printf("\t生产者生产了字符%c\n", str[sp]);
			pro_queue[p1] = str[sp];	//添加到生产者记录中
			sp = (sp + 1) % length;
			PC++;
			ready->breakpoint = PC;		//保护断点
			break;
		case 1:		//p(s1)
			PC++;
			p(1);
			break;
		case 2:		//put
			buffer[in]= pro_queue[p1];		//将输入字符放到缓冲池
			cout <<"\t"<< buffer[in] << "字符进入缓冲池" << endl;
			p1++;
			in = (in + 1) % BUF;
			PC++;
			ready->breakpoint = PC;
			break;
		case 3:		//v(s2)
			PC++;
			printf("\t释放一个s2信号\n");
			v(2);
			break;
		case 4:		//goto 0
			printf("\t生产者进程goto 0\n");
			PC = 0;
			str_cnt--;		//剩余字符减一
			printf("\t剩余字符为%d个\n",str_cnt);
			ready->breakpoint = PC;

			if (str_cnt <= 0)		//生产结束
			{
				printf("\t生产者结束生产！\n");
				strcpy(producer->state, "Stop");
				strcpy(producer->reason, "null");
				ready->breakpoint = -1;
				ready = ready->next;	//在就绪队列中除去
			}
		}
	else									//当前进程为消费者进程
		switch (PC)			
		{
		case 0:		//p(s2)
			PC++;
			p(2);
			break;
		case 1:		//get
			printf("\t消费者取字符！\n");
			temp = buffer[out];
			out = (out + 1) % BUF;
			PC++;
			ready->breakpoint = PC;
			break;
		case 2:		//v(s1)
			PC++;
			printf("\t释放一个s1信号\n");
			v(1);
			break;
		case 3:		//consume
			printf("\t消费者消费了字符%c\n", temp);
			con_queue[c1] = temp;	//添加到消费者记录中
			c1++;
			consumer_cnt++;
			if (consumer_cnt >= length)
			{
				strcpy(consumer->state, "Stop");
				strcpy(consumer->reason, "null");		//?
				consumer->breakpoint = -1;
				return;
			}
			PC++;
			ready->breakpoint = PC;		//保护断点
			break;
		case 4:		//goto 0
			printf("\t消费者进程goto 0\n");
			PC = 0;
			ready->breakpoint = PC;
		}
}

//打印输出函数
void print()
{
	int i, j;
	printf("**********生产者消费者模拟**********\n");
	printf("模拟过程的字符串为： ");
	printf("%s\n", &str);

	printf("已经生产：");
	for (i = 0; i <= p1; i++)
		printf("%c", pro_queue[i]);
	printf("\n空缓存：");
	for (i = c1; i <= p1; i++)
		printf("%c", buffer[i]);
	printf("\n已经消费：");
	for (i = 0; i <= c1; i++)
		printf("%c", con_queue[i]);

	printf("\n\n**********进程控制块的信息**********\n\n");
	printf("进程名\t\t状态\t等待原因\t断点\n\n");
	printf("%s\t%s\t%s\t\t%d\n\n", producer->name, producer->state, producer->reason, producer->breakpoint);
	printf("%s\t%s\t%s\t\t%d\n\n", consumer->name, consumer->state, consumer->reason, consumer->breakpoint);
	printf("*********************************************\n");
	printf("1.continue  0.exit\n");
	scanf("%d", &j);
	if (j == 0)
	{
		exit(0);
	}
}