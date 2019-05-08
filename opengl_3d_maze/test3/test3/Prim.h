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
//a��������Ϊż�������䣬bֻ����������wall
//a����������У�b����ǽ��c�����ʼ����
//b[i][j]=1��c[i][j]=1������,��Ȼ��ǽ���������ɡ�
extern int a[100][100], b[100][100], c[100][100];
//����size
extern int size;


int StackEmpty(SeqStack *S);
int StackFull(SeqStack *S);
SeqData GetTop(SeqStack *S);
void Push(SeqStack *S, int i, int j);
void Pop(SeqStack *S);
void PrimMaze();

#endif