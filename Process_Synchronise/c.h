/*����ԭ�Ͷ��壺*/
void init();	//��ʼ��
void p(int s);	//p����
void v(int s);  //v����
void block(int s);	//��������
void wakeup(int s);	//���Ѳ���
void ProcessDispatch();	//����������
void Simulate_exe();	//ģ��ָ��ִ��
void print();			//��ӡ�������

/*��ʼ������init()
  ���룺
  �����
 */
void init()
{
	s1 = BUF;	//���ź�������ʼ��Ϊ��������ֵ
	s2 = 0;		//���ź�������ʼ��Ϊ0
	producer = (Node)malloc(sizeof(PCB));	//����һ�������߽�㣬����ʼ��
	strcpy(producer->name, "Producer");
	strcpy(producer->state, "Ready");
	strcpy(producer->reason, "null");
	producer->breakpoint = 0;
	producer->next = NULL;

	consumer = (Node)malloc(sizeof(PCB));	//����һ�������߽�㣬����ʼ��
	strcpy(consumer->name, "Consumer");
	strcpy(consumer->state, "Ready");
	strcpy(consumer->reason, "null");
	consumer->breakpoint = 0;
	consumer->next = NULL;

	ready = producer;		//��ʼ���������У�����������ǰ�����ѽ����ں�
	ready->next = consumer;
	consumer->next = NULL;
	
	block_s1 = NULL;		//��ʼ���������̣�Ϊ��
	block_s2 = NULL;

	PC = 0;					//���������
	consumer_cnt=0;			//�����߼�����
}

/*p��������p(int s)
  ���룺���̺�
  ��������������У������ź����ɹ������������ý���
*/
void p(int s)
{
	if (s == 1)					//p(s1)
	{
		s1--;
		if (s1 < 0)
			block(1);			//������������
		else
		{
			cout << "\ts1�ź�������ɹ���" << endl;
			ready->breakpoint = PC;		//����ϵ�
		}
	}
	else			//p(s2)
	{
		s2--;
		if (s2 < 0)
			block(2);			//�������ѽ���
		else
		{
			cout << "\ts2�ź�������ɹ���" << endl;
			ready->breakpoint = PC;		//����ϵ�
		}
	}
}

/*v��������v(int s)
  ���룺���̺�
  ���������һ����������
*/
void v(int s)
{
	if (s == 1)		//v(s1)
	{
		s1++;
		if (s1 <= 0)
			wakeup(1);			//������������
			ready->breakpoint = PC;		//����ϵ�
	}
	else			//v(s2)
	{
		s2++;
		if (s2 <= 0)
			wakeup(2);			//�������ѽ���
			ready->breakpoint = PC;		//����ϵ�
	}
}

/*��������block(int s)
  ���룺���̺�
  �����������ǰ����
*/
void block(int s)
{
	Node p;		//��������ָ��
	int num1 = 0;	//��¼���������еĽ�����
	int num2 = 0;
	if (s == 1)		//�����߽���
	{
		strcpy(producer->state, "Block");	//�޸�״̬Ϊ����
		strcpy(producer->reason, "s1");		//����ԭ��Ϊs1
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
		printf("\tproducer�����߽��������ˣ�\n");
		ready->breakpoint = PC;	//����ϵ�
		ready = ready->next;	//�ھ���������ȥ����ָ����һ��
		num1++;
	}
	else		//�����߽���
	{
		strcpy(consumer->state, "Block");	//�޸�״̬Ϊ����
		strcpy(consumer->reason, "s2");		//����ԭ��Ϊs1
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
		ready->breakpoint = PC;	//����ϵ�
		ready = ready->next;	//�ھ���������ȥ����ָ����һ��
		consumer->next = NULL;
		printf("\tconsumer�����߽��������ˣ�\n");
		num2++;
	}
	printf("\t�����������߽��̸���Ϊ��%d\n", num1);
	printf("\t�����������߽��̸���Ϊ��%d\n", num2);
}

/*���Ѻ���wakeup(int s)
  ���룺���̺�
  ��������ѵ�ǰ����
*/
void wakeup(int s)
{
	Node p;
	Node q = ready;
	if (s == 1)		//����block_s1���������߽���
	{
		p = block_s1;
		block_s1 = block_s1->next;   //����ָ��ָ����һ����������
		strcpy(p->state, "Ready");
		strcpy(p->reason, "null");
		while (q)		//�����������
			q = q->next;
		q = p;
		p->next = NULL;
		printf("\tproducer�����߽��̻����ˣ�\n");
	}
	else       //����block_s1���������߽���
	{
		p = block_s2;
		block_s2 = block_s2->next;   //����ָ��ָ����һ����������
		strcpy(p->state, "Ready");
		strcpy(p->reason, "null");
		while (q->next)		//�����������
			q = q->next;
		q ->next= p;
		p->next = NULL;
		printf("\tconsumer�����߽��̻����ˣ�\n");
	}
}

//���������Ⱥ���
void ProcessDispatch()		
{
	int rand_number;	 //���������
	int num = 0;	     //�������̸���

	Node p = ready;		//��������ָ��
	if (ready == NULL)	//�޾������̣�����
		return;
	
	while (p!=NULL)		 //ͳ�ƾ������̸���
	{
		num++;
		p = p->next;
	}
	printf("\t�������̸���Ϊ��%d\n",num);

	time_t t;
	srand((unsigned) time(&t));
	rand_number = rand() % num;  //����������������
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

//ģ�⴦����ָ��ִ�к���
void Simulate_exe()
{
	if(strcmp(ready->name,"Producer")==0)	//��ǰ����Ϊ�����߽���
		switch (PC)
		{
		case 0:		//produce
			printf("\t�������������ַ�%c\n", str[sp]);
			pro_queue[p1] = str[sp];	//��ӵ������߼�¼��
			sp = (sp + 1) % length;
			PC++;
			ready->breakpoint = PC;		//�����ϵ�
			break;
		case 1:		//p(s1)
			PC++;
			p(1);
			break;
		case 2:		//put
			buffer[in]= pro_queue[p1];		//�������ַ��ŵ������
			cout <<"\t"<< buffer[in] << "�ַ����뻺���" << endl;
			p1++;
			in = (in + 1) % BUF;
			PC++;
			ready->breakpoint = PC;
			break;
		case 3:		//v(s2)
			PC++;
			printf("\t�ͷ�һ��s2�ź�\n");
			v(2);
			break;
		case 4:		//goto 0
			printf("\t�����߽���goto 0\n");
			PC = 0;
			str_cnt--;		//ʣ���ַ���һ
			printf("\tʣ���ַ�Ϊ%d��\n",str_cnt);
			ready->breakpoint = PC;

			if (str_cnt <= 0)		//��������
			{
				printf("\t�����߽���������\n");
				strcpy(producer->state, "Stop");
				strcpy(producer->reason, "null");
				ready->breakpoint = -1;
				ready = ready->next;	//�ھ��������г�ȥ
			}
		}
	else									//��ǰ����Ϊ�����߽���
		switch (PC)			
		{
		case 0:		//p(s2)
			PC++;
			p(2);
			break;
		case 1:		//get
			printf("\t������ȡ�ַ���\n");
			temp = buffer[out];
			out = (out + 1) % BUF;
			PC++;
			ready->breakpoint = PC;
			break;
		case 2:		//v(s1)
			PC++;
			printf("\t�ͷ�һ��s1�ź�\n");
			v(1);
			break;
		case 3:		//consume
			printf("\t�������������ַ�%c\n", temp);
			con_queue[c1] = temp;	//��ӵ������߼�¼��
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
			ready->breakpoint = PC;		//�����ϵ�
			break;
		case 4:		//goto 0
			printf("\t�����߽���goto 0\n");
			PC = 0;
			ready->breakpoint = PC;
		}
}

//��ӡ�������
void print()
{
	int i, j;
	printf("**********������������ģ��**********\n");
	printf("ģ����̵��ַ���Ϊ�� ");
	printf("%s\n", &str);

	printf("�Ѿ�������");
	for (i = 0; i <= p1; i++)
		printf("%c", pro_queue[i]);
	printf("\n�ջ��棺");
	for (i = c1; i <= p1; i++)
		printf("%c", buffer[i]);
	printf("\n�Ѿ����ѣ�");
	for (i = 0; i <= c1; i++)
		printf("%c", con_queue[i]);

	printf("\n\n**********���̿��ƿ����Ϣ**********\n\n");
	printf("������\t\t״̬\t�ȴ�ԭ��\t�ϵ�\n\n");
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