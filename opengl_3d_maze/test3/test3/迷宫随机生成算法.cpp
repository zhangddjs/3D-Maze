/*
��1������ͼ��ʾΪһ��6x6���Թ����ȼ����Թ������е�ͨ·������ȫ��յģ���ɫ�ĸ��ӱ�ʾ����ͨ������ɫ�ĸ��ӱ�ʾǽ�ڻ����ϰ�����ͨ����

��2�����ѡ��һ����ɫ�ĸ�����Ϊ��ǰ���ڷ��ʵĸ��ӣ�ͬʱ�Ѹø��ӷ���һ���Ѿ����ʵ��б��С�

��3��ѭ�����²�����ֱ�����еĸ��Ӷ������ʵ���

1.�õ���ǰ���ʸ��ӵ����ܣ��������ң��ĸ��ӣ�����Щ���������ѡ��һ��û���ڷ����б��еĸ��ӣ�����ҵ�����Ѹø��Ӻ͵�ǰ���ʵĸ����м��ǽ��ͨ(��Ϊ0)���Ѹø�����Ϊ��ǰ���ʵĸ��ӣ�����������б�

2.�����Χ���еĸ��Ӷ��Ѿ����ʹ�������ѷ��ʵ��б��У����ѡȡһ����Ϊ��ǰ���ʵĸ��ӡ�*/

#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include "Prim.h"
//ջ

int StackEmpty(SeqStack *S)
{
	if (S->top >= 0)
		return 0;
	else
		return 1;
}
int StackFull(SeqStack *S)
{
	if (S->top < maxlen - 1)
		return 0;
	else
		return 1;
}
SeqData GetTop(SeqStack *S)
{
	if (!StackFull(S))
		return(S->data[S->top]);
	else
	{
		printf("StackFull!");
		exit(0);
	}
}
void Push(SeqStack *S, int i, int j)
{
	if (!StackFull(S))
	{
		S->top += 1;
		S->data[S->top].i = i;
		S->data[S->top].j = j;
	}
	else
	{
		printf("StackFull! %d", S->top);
		exit(0);
	}
}
void Pop(SeqStack *S)
{
	if (!StackEmpty(S))
		S->top--;
	else
	{
		printf("StackEmpty!");
		exit(0);
	}
}
int a[100][100], b[100][100], c[100][100];
int size;

void PrimMaze()
{
	SeqStack *S;
	S = (SeqStack *)malloc(sizeof(SeqStack));
	S->top = -1;
	srand(time(NULL));
	printf("�������Ѷȼ���(1/2/3):");
	scanf("%d", &size);
	size *= 10;
	//���巿��
	int i, j;
	for (i = 0; i < size; i+=2)
	{
		for (j = 0; j < size; j+=2)
		{
			c[i][j] = 1;
		}
	}
	//���������һ��a��
	i = rand() % 5 * 2;
	j = rand() % 5 * 2;
	a[i][j] = 1;
	Push(S, i, j);
	//����������� 1Ϊ�ϣ�2Ϊ�£� 3Ϊ�� 4Ϊ��
	int s = rand() % 4 + 1;
	//����ִ�в���flag1
	bool flag1;
	//ѭ������ÿ����
	for (int n = 0; n < size * size / 4 - 1; n++)
	{
		printf("%d\n", n);
		flag1 = false;
		switch (s)
		{
		case 1:
			//����2�񣬸ô���û���ʹ�����Ǹô����ʹ������м�ǽ�ڴ�ͨ���Ըõ����
			if (a[i - 2][j] == 0 && i != 0)
			{
				a[i - 2][j] = 1;
				b[i - 1][j] = 1;
				i = i - 2;
				s = rand() % 4 + 1;
				//��ջ
				Push(S, i, j);
				break;
			}
			flag1 = true;
			//�����ʹ��������
		case 2:
			if (a[i + 2][j] == 0 && i != size - 2)
			{
				a[i + 2][j] = 1;
				b[i + 1][j] = 1;
				i = i + 2;
				s = rand() % 4 + 1;
				Push(S, i, j);
				break;
			}
		case 3:
			if (a[i][j - 2] == 0 && j != 0)
			{
				a[i][j - 2] = 1;
				b[i][j - 1] = 1;
				j = j - 2;
				s = rand() % 4 + 1;
				Push(S, i, j);
				break;
			}
		case 4:
			if (a[i][j + 2] == 0 && j != size - 2)
			{
				a[i][j + 2] = 1;
				b[i][j + 1] = 1;
				j = j + 2;
				s = rand() % 4 + 1;
				Push(S, i, j);
				break;
			}
		default:
			//�ж�4�������Ƿ�ȫ�����ԣ�û�оʹ��Ϸ�����
			if (!flag1)
			{
				s = 1;
				n--;
				break;
			}
			//��Χû�пɷ��ʵĵ㣬����ѷ��ʵ��������ȡһ����������
			else
			{
				i = GetTop(S).i;
				j = GetTop(S).j;
				Pop(S);
				n--;
				break;
			}
		}
	}
// 	system("mode con cols=300");
// 	system("mode con lines=300");
	//��ӡ

	//�����
	b[size - 1][size - 2] = 1;
	c[size - 1][size - 2] = 1;
	b[size][size - 2] = 1;
	c[size][size - 2] = 1;
	b[size + 1][size - 2] = 1;
	c[size + 1][size - 2] = 1;
	b[size + 2][size - 2] = 1;
	c[size + 2][size - 2] = 1;
	for (i = 0; i < size; i++)
	{
		printf("��");
	};
	printf("��\n��");
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (b[i][j] == 0 && c[i][j] == 0)
			{
				printf("��");
			}
			else
			{
				printf("  ");
			}
		}
		if (i != size - 1)
		{
			printf("\n��");
		}
	}
	for (i = 0; i < size; i += 2)
	{
		printf("\n");
		for (j = 0; j < size; j += 2)
		{
			printf("%d", a[i][j]);
		}
	}
}