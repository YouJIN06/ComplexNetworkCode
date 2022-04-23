/**********************************************************************

Name：随机网络SIS模型

**********************************************************************/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <omp.h>
#include <float.h>
using namespace std;

void Link(struct epi* pp);
void degreee(struct epi* pp);
void generateRandomNetwork();
void initial();
void spread();

int** adjacentMatrix;

const int NETWORK_SIZE = 10000, N = NETWORK_SIZE, step = 100;

const double PROBABILITY_OF_EAGE = 0.0004;//连接概率

const double bate = 0.25, miu = 0.2;//bate:感染率，miu:恢复率

int x[N];

struct epi
{
	double X[N]; //state:0=S,1=I
	int A[N][N]; //adjacentMatrix
	int w[N];    //degree

}epi1;

struct epi* p1 = &epi1;



int main()
{
	srand((unsigned)time(NULL));

	Link(p1);

	degreee(p1);

	initial();

	spread();

}

void initial()
{
	int i, j, max = 0, maxd;

	for (i = 0; i < N; i++)//所有节点设为S态
	{
		p1->X[i] = 0;

	}

	for (i = 0; i < N; i++)
	{
		if (p1->w[i] > max)
		{
			max = p1->w[i];
			maxd = i;

		}
	}
	p1->X[0] = 1;//度最大的设为I态

}

void spread()
{
	ofstream ofs;
	ofs.open("it.dat", ios::out);

	int i, j, t;
	double gamma, bata, m, sum, r;


	for (t = 0; t < step; t++)
	{
		sum = 0;

		for (i = 0; i < N; i++)
		{
			x[i] = p1->X[i];
		}
		for (i = 0; i < N; i++)
		{
			if (p1->X[i] == 0)
			{
				for (j = 0; j < N; j++)
				{
					if (p1->X[j] == 1 && p1->A[i][j] == 1)
					{
						r = rand() / (double)RAND_MAX;
						if (r < bate)
						{
							x[i] = 1;
						}
					}
				}
			}
			if (p1->X[i] == 1)
			{
				r = rand() / (double)RAND_MAX;
				if (r < miu)
				{
					x[i] = 0;//等于0为SIS模型，等于2为SIR模型
				}
			}
		}

		for (i = 0; i < N; i++)
		{
			p1->X[i] = x[i];

		}

		for (i = 0; i < N; i++)
		{
			if (p1->X[i] == 1)
			{
				sum++;
			}

		}
		cout << sum << endl;
		ofs << t << " " << sum / (double)N << endl;

	}



}

void Link(struct epi* pp)
{
	int i, j;


	generateRandomNetwork();

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			pp->A[i][j] = adjacentMatrix[i + 1][j + 1];
		}
	}

}

void degreee(struct epi* pp)
{
	int i, j, max, min;
	double ave, sum = 0;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			pp->w[i] = pp->w[i] + pp->A[i][j];
		}
		sum = sum + pp->w[i];
		//		printf("%d ",pp->w[i]);
	}
	ave = sum / N;
	printf("ave=%f\n", ave);

}

void generateRandomNetwork()
{
	if (!(adjacentMatrix = (int**)malloc(sizeof(int*) * (NETWORK_SIZE + 1))))
	{
		printf("adjacentMatrix** malloc error");
		exit(0);
	}
	int i;
	for (i = 1; i <= NETWORK_SIZE; i++)
	{
		if (!(adjacentMatrix[i] = (int*)malloc(sizeof(int) * (NETWORK_SIZE + 1))))
		{
			printf("adjacentMatrix[%d]* malloc error", i);
			exit(0);
		}
	}

	int j;
	for (i = 1; i <= NETWORK_SIZE; i++)
		for (j = i; j <= NETWORK_SIZE; j++)
			adjacentMatrix[i][j] = adjacentMatrix[j][i] = 0;
	int k;
	int count = 0;
	double probability = 0.0;
	for (i = 1; i <= NETWORK_SIZE; i++)
	{
		for (j = i + 1; j <= NETWORK_SIZE; j++)
		{
			probability = rand() / (double)RAND_MAX;
			if (probability < PROBABILITY_OF_EAGE)
			{
				count++;
				adjacentMatrix[i][j] = adjacentMatrix[j][i] = 1;
			}
		}
	}

}
