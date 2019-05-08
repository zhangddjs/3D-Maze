/*
（1）如上图所示为一个6x6的迷宫，先假设迷宫中所有的通路都是完全封闭的，黄色的格子表示可以通过，黑色的格子表示墙壁或者障碍不能通过。

（2）随机选择一个黄色的格子作为当前正在访问的格子，同时把该格子放入一个已经访问的列表中。

（3）循环以下操作，直到所有的格子都被访问到。

1.得到当前访问格子的四周（上下左右）的格子，在这些格子中随机选择一个没有在访问列表中的格子，如果找到，则把该格子和当前访问的格子中间的墙打通(置为0)，把该格子作为当前访问的格子，并放入访问列表。

2.如果周围所有的格子都已经访问过，则从已访问的列表中，随机选取一个作为当前访问的格子。*/

#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include "Prim.h"
//栈

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
	printf("请输入难度级别(1/2/3):");
	scanf("%d", &size);
	size *= 10;
	//定义房间
	int i, j;
	for (i = 0; i < size; i+=2)
	{
		for (j = 0; j < size; j+=2)
		{
			c[i][j] = 1;
		}
	}
	//定义随机的一个a点
	i = rand() % 5 * 2;
	j = rand() % 5 * 2;
	a[i][j] = 1;
	Push(S, i, j);
	//定义随机方向 1为上，2为下， 3为左， 4为右
	int s = rand() % 4 + 1;
	//定义执行操作flag1
	bool flag1;
	//循环访问每个点
	for (int n = 0; n < size * size / 4 - 1; n++)
	{
		printf("%d\n", n);
		flag1 = false;
		switch (s)
		{
		case 1:
			//向上2格，该处若没访问过，则记该处访问过，且中间墙壁打通，以该点继续
			if (a[i - 2][j] == 0 && i != 0)
			{
				a[i - 2][j] = 1;
				b[i - 1][j] = 1;
				i = i - 2;
				s = rand() % 4 + 1;
				//入栈
				Push(S, i, j);
				break;
			}
			flag1 = true;
			//若访问过，则继续
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
			//判断4个方向是否全部尝试，没有就从上方再试
			if (!flag1)
			{
				s = 1;
				n--;
				break;
			}
			//周围没有可访问的点，则从已访问点中随机抽取一个继续访问
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
	//打印

	//打开入口
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
		printf("■");
	};
	printf("■\n■");
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (b[i][j] == 0 && c[i][j] == 0)
			{
				printf("■");
			}
			else
			{
				printf("  ");
			}
		}
		if (i != size - 1)
		{
			printf("\n■");
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