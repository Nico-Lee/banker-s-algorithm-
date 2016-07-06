#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#define SIZE 11

int available[SIZE];//可利用资源矩阵
int claim[SIZE][SIZE];//所需资源最大量矩阵
int allocation[SIZE][SIZE];//已分配资源矩阵
int need[SIZE][SIZE];//需要资源矩阵
int request[SIZE][SIZE] = { 0 };//记录某个进程申请各个资源类中的资源实例的数量 
int finish[SIZE] = { 0 };//工作变量，用于判断进程是否已经执行过，初始状态全部为0，即未执行  
int p[SIZE];//记录序列执行顺序  
int ava;//记录系统有多少个资源类 
int process;//记录进程数量
int r;//记录当前要申请资源的进程的序号 
int key = 1;

void showdate();//显示资源矩阵函数
void allot();//分配函数
int init();//初始化各矩阵
int check();//检查是否为安全状态（重点）

void ava_xh()///输出资源序号 
{
	int k;
	for (k = 0; k < ava; k++)
	{
		printf("%c ", k + 65);
	}
	printf("\n");
}

int main(void)
{
	int t;
	char ch;
	while (1)//声明循环 
	{
		system("cls");
		t = init();
		if (t == 1)
		{
			break;
		}
	}
	do//资源申请循环 
	{
		allot();
		showdate();
		printf("还要申请对进程分配资源吗？(请按Y键):");
		scanf(" %c", &ch);
	} while (ch == 'y' || ch == 'Y');
	return 0;
}

int init()
{
	int i, j;
	printf("请输入资源类数量(1--%d)：", SIZE);
	while (1)
	{
		scanf("%d", &ava);
		if ((ava <= SIZE) && (ava >= 1))
		{
			break;
		}
		printf("输入错误，请重新输入：");
	}
	printf("请依次输入各资源类的个数（中间用空格分开）：\n");
	while (1)
	{
		ava_xh();
		for (i = 0; i < ava; i++)
		{
			scanf("%d", &available[i]);
		}
		for (i = 0; i < ava; i++)
		{
			if ((available[i] < 0) || (available[i] > 2147483647))
			{
				printf("有错误的输入,重新开始吧。\n");
				break;
			}
		}
		if (i == ava)
		{
			break;
		}
	}
	printf("请输入进程数量：", SIZE);
	while (1)
	{
		scanf("%d", &process);
		if ((process <= SIZE) && (process >= 1))
		{
			break;
		}
		printf("输入错误，请重新输入：");
	}

	printf("===============================================================\n");
	for (i = 0; i < process; i++)
	{
		//输入及检测进程所需各类资源的资源实例最大量
		printf("请输入进程P(%d)所需各类资源的资源最大量Max:\n", i);
		ava_xh();
		for (j = 0; j < ava; j++)
		{
			scanf("%d", &claim[i][j]);
		}
		for (j = 0; j < ava; j++)
		{
			if (claim[i][j] > available[j])
			{
				printf("有数据超过系统实例最大量，退出)。\n\n\n\n");
				system("pause");
				getchar();
				return 0;
			}
		}
		//输入及检测进程占有各资源类中资源实例的数量
		printf("请输入进程P(%d)已分配各类资源的数量Allocation:\n", i);
		ava_xh();
		for (j = 0; j < ava; j++)
		{
			scanf("%d", &allocation[i][j]);
		}
		for (j = 0; j < ava; j++)
		{
			if (claim[i][j] < allocation[i][j])
			{
				printf("有数据超过进程所需资源最大量。\n\n\n\n");
				system("pause");
				getchar();
				return 0;
			}
		}
		//输入进程还需各个资源类中资源实例的数量
		printf("下面是进程P(%d)还需各个资源类中资源的数量Need:\n", i);
		ava_xh();
		for (j = 0; j < ava; j++)
		{
			need[i][j] = claim[i][j] - allocation[i][j];
			printf("%d ", claim[i][j] - allocation[i][j]);
		}
		printf("\n===============================================================\n");
	}

	printf("\n下面是目前系统剩余的各个资源类的实例数量：\n");
	ava_xh();
	for (i = 0; i < ava; i++)
	{
		for (j = 0; j < process; j++)
		{
			available[i] = available[i] - allocation[j][i];
		}
		printf("%d ", available[i]);
	}
	printf("\n===============================================================\n");
	if (check() == 0)//安全检测 
	{
		printf("安全检测失败,可能发生死锁,数据重置\n");
		for (i = 0; i < ava; i++)//重置数据 
		{
			available[i] = available[i] + request[r][i];
			allocation[r][i] = allocation[r][i] - request[r][i];
			need[r][i] = need[r][i] + request[r][i];
		}
	}
	else
	{
		printf("\n进程顺利执行.\n\n");
	}
	showdate();
	return 1;
}

void allot()
{
	int i, j;

	printf("\n请输入当前要申请资源的进程的序号(0--%d):", process - 1);
	while (1)
	{
		scanf("%d", &r);
		if ((r <= process - 1) && (r >= 0))
		{
			break;
		}
		printf("输入错误，请重新输入：");
	}

	printf("请输入要申请的各个资源实例的数量：\n");
	ava_xh();
	for (j = 0; j < ava; j++)
	{
		scanf("%d", &request[r][j]);
	}
	for (i = 0; i < ava; i++)
	{
		if (request[r][i] > need[r][i])
		{
			printf("\n申请资源量超过所声明的最大资源需求量Max\n");
			return;
		}
	}
	for (i = 0; i < ava; i++)
	{
		if (request[r][i] > available[i])
		{
			printf("剩余资源实例不足，需要等待,重来一次.\n");
			return;
		}
	}

	for (i = 0; i < ava; i++)//假设分配 
	{
		available[i] = available[i] - request[r][i];
		allocation[r][i] = allocation[r][i] + request[r][i];
		need[r][i] = need[r][i] - request[r][i];
	}
	//int ret=check();
	if (check() == 0)//安全检测 
	{
		printf("安全检测失败,可能发生死锁,数据重置\n");
		for (i = 0; i < ava; i++)//重置数据 
		{
			available[i] = available[i] + request[r][i];
			allocation[r][i] = allocation[r][i] - request[r][i];
			need[r][i] = need[r][i] + request[r][i];
		}
	}
	else
	{
		int key = 0;
		for (j = 0; j < ava; j++)
		{
			if (need[r][j] == 0)
			{
				key++;
			}
		}
		if (key == ava)
		{
			for (j = 0; j < ava; j++)
			{
				available[j] += allocation[r][j];
				allocation[r][j] = 0;
			}


		}
	}printf("\n进程顺利执行.\n\n");
	return;
}

int check()
{
	int i, j, k, l = 0;
	int work[SIZE] = { 0 };//工作数组 
	for (i = 0; i < ava; i++)//初始化 
	{
		work[i] = available[i];
	}

	for (i = 0; i < process; i++) //初始化  
	{
		finish[i] = 0;
	}

	for (i = 0; i < process; i++)
	{
		for (j = 0; j < ava; j++)////寻找条件 Need[i]<=Work[i]  
		{
			if (need[i][j] > work[j])
			{
				break;
			}
		}
		if ((j == ava) && (finish[i] == 0))///寻找条件 Finish[i]=false ，每次从头开始找安全序列
		{
			finish[i] = 1;
			for (k = 0; k < ava; k++)
			{
				work[k] = work[k] + allocation[i][k];
			}
			p[l] = i;//记录安全序列 
			l++;
			//i = -1;///重置i，为了寻找安全序列  
		}
		else
		{
			continue;
		}
		if (l == process)//可以找到安全序列，输出并结束 
		{
			printf("\n系统安全!\n");
			printf("安全序列为：");
			for (k = 0; k < l; k++)
			{
				printf("P(%d)", p[k]);
				if (k != l - 1)
				{
					printf("-->");
				}
			}
			return 1;
		}
	}

	printf("\n系统不安全，不能满足你的要求！\n");
	return 0;
}

void showdate()//显示现在所有数据 
{
	int i, j;
	printf("当前所有数据！\n");
	printf("\nMax\n");
	printf("     ");
	ava_xh();
	for (i = 0; i < process; i++)
	{
		printf("P(%d) ", i);
		for (j = 0; j < ava; j++)
		{
			printf("%d ", claim[i][j]);
		}
		printf("\n");
	}

	printf("\nAllocation\n");
	printf("     ");
	ava_xh();
	for (i = 0; i < process; i++)
	{
		printf("P(%d) ", i);
		for (j = 0; j < ava; j++)
		{
			printf("%d ", allocation[i][j]);
		}
		printf("\n");
	}

	printf("\nNeed\n");
	printf("     ");
	ava_xh();
	for (i = 0; i < process; i++)
	{
		printf("P(%d) ", i);
		for (j = 0; j < ava; j++)
		{
			printf("%d ", need[i][j]);
		}
		printf("\n");
	}
	printf("\nAvailable\n");
	ava_xh();
	for (i = 0; i < ava; i++)
	{
		printf("%d ", available[i]);
	}
	printf("\n===============================================================\n");
	printf("\n\n\n");
	return;
}
