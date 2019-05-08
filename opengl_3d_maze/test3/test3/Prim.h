#ifndef PRIM_H
#define PRIM_H

#include <stdio.h>
#include <stdlib.h>
#include<time.h>

#define maxlen 10000
typedef struct{
	int i;
	int j;
}SeqData;
typedef struct{
	SeqData data[maxlen];
	int top;
}SeqStack;
//a的行列数为偶数代表房间，b只用奇数代表wall
//a代表访问序列，b代表墙，c代表初始走廊
//b[i][j]=1或c[i][j]=1画走廊,不然画墙，遍历即可。
extern int a[100][100], b[100][100], c[100][100];
//定义size
extern int size;


int StackEmpty(SeqStack *S);
int StackFull(SeqStack *S);
SeqData GetTop(SeqStack *S);
void Push(SeqStack *S, int i, int j);
void Pop(SeqStack *S);
void PrimMaze();

#endif