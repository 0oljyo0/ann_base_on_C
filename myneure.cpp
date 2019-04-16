//#include "stdafx.h"
#include "myneure.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define DEBUG

#ifdef DEBUG
#include <windows.h>
#include <conio.h>
#endif
//˵����������Ԫ�ĳ�ʼ��   ͬʱ��Ȩ�ط���ռ�
//neure ������ʼ������Ԫ�ṹ
//input_data_num :��Ԫ�����������ݵĸ���
//inputfunction �����뺯�� ����Ȩ����õĺ���
//transferfunction �����亯�� �����ݸ�ʽ������ĺ���
void neure_init(Neure_def *neure, int input_data_num, Inputfun_def inputfunction, Transferfun_def transferfunction)
{
	int i = 0;
	neure->b = (rand() % 100000)*0.00001;                                        //��ʼ��ƫ����b
	neure->input_num = input_data_num;                                           //��ʼ��������������
	neure->input_sift = NULL;//(double *)malloc(input_data_num*sizeof(double));  //�����������ݵĿռ�
	neure->weight = (double *)malloc(input_data_num*sizeof(double));             //����Ȩ�صĿռ�

	for (i = 0; i < input_data_num; i++)									     //��ʼ��Ȩֵ
	{
		neure->weight[i] = (rand() % 100000)*0.00001;                            //Ȩֵ��ʼ��Ϊ�����
	}

 	neure->transferfun = transferfunction;										 //���ô��亯��
	neure->inputfun = inputfunction;										     //���ü�Ȩ��͵ĺ���
}

//˵����������ĳһ��ĳ�ʼ��  ��ÿһ����������ͬһ���ռ�
//neure_layer������ʼ�����񾭲�ṹ ������
//input_data_num���񾭲��������ݵĸ��� һ��ͱ�����Ԫ���������ݸ�����ͬ
//layer_neure_num��������Ԫ����
void neure_network_layer_init(Neure_Network_Layer_def *neure_layer, int input_data_num, int layer_neure_num)
{
	int i;
	neure_layer->layer_input_num = input_data_num;
	neure_layer->layer_neure_num = layer_neure_num;
	neure_layer->layer_input_data = (double *)malloc(input_data_num*sizeof(double));           //���뱾������Ŀռ� ����ÿ����Ԫ����

	for (i = 0; i < layer_neure_num; i++)                                                      //ѭ����ʼ�����ÿһ����Ԫ
	{
		neure_init(&neure_layer->neure[i], neure_layer->layer_input_num, simple_sum, step);    //��Ԫ��ʼ��
		neure_layer->neure[i].input_sift = neure_layer->layer_input_data;                      //�������ͬһ���ռ�
	}
}

//˵�������������ʼ�� ��Ҫ�ǹ����������� ��ǰ�����ָ����Ϻ������� ����ǰ����������������Ͽ�ʼ������ ����Ҫ����ǰ���������ֵ�����
//neure_network������ʼ����������ṹ
//neure_layer�����ڹ�������ĸ����񾭲�����
//layer_num��������Ĳ��� ��������ݲ���һ��
//input_data_num����������������ݸ���
void neure_network_init(Neure_Network_def *neure_network, Neure_Network_Layer_def *neure_layer, int layer_num, int input_data_num)
{
	int i, j, k;
	neure_network->neure_layer = neure_layer;
	neure_network->input_num = input_data_num;
	neure_network->input_sift = neure_network->neure_layer[0].neure[0].input_sift;
	neure_network->layer_num = layer_num;

	//��ÿһ��ǰ���ҹ� ��������ָ��ͬһ���ռ�
	for (i = 0; i < layer_num-1; i++)                                           //����ÿһ����� ��Ϊ3�������� �����������
	{
		for (j = 0; j < neure_network->neure_layer[i+1].layer_neure_num; j++)   //����ÿһ�������Ԫ
		{
			for (k = 0; k < neure_network->neure_layer[i].layer_neure_num; k++) //����ÿһ��ǰ����Ԫ
			{
				neure_network->neure_layer[i].neure[k].output = &neure_network->neure_layer[i + 1].neure[j].input_sift[k]; //��ǰ��������ַ��Ϊ���������ַ ��ȥ����ʱ��Ƶ����ֵ����
			}
		}
	}
	for (i = 0; i < neure_network->neure_layer[neure_network->layer_num - 1].layer_neure_num; i++) //���������ÿһ����Ԫ ���������Ԫ��������Ŀռ�
	{
		neure_network->neure_layer[neure_network->layer_num - 1].neure[i].output = (double *)malloc(sizeof(double));//�������һ������û�п��Թҵĺ�� ����ֱ�ӷ���ռ� �൱��һ������
	}
}

//˵�������������� ���������������Ԫ�� ���Ա������޷���ֵ
//neure_network������������������
//input_sift�����������������ֵ
void computed_neure_network_output(Neure_Network_def *neure_network, double *input_sift)
{
	int i, j;
	double temp = 0;

	for (j = 0; j < neure_network->layer_num;j++)
	{		
		for (i = 0; i < neure_network->neure_layer[j].layer_neure_num; i++)   //���������������� �����һ������ֵ
		{
			if (j == 0)  //�����ǰ����Ĳ�Ϊ����� ��ֱ�Ӵ����������ݼ���
			{
				neure_network->neure_layer[j].neure[i].input_sift = input_sift;
				temp = neure_network->neure_layer[j].neure[i].inputfun(neure_network->neure_layer[j].neure[i].input_sift, neure_network->neure_layer[j].neure[i].weight, neure_network->neure_layer[j].neure[i].b, neure_network->neure_layer[j].neure[i].input_num);
				*neure_network->neure_layer[j].neure[i].output = neure_network->neure_layer[j].neure[i].transferfun(temp);
			}
			else         //�����ǰ����Ĳ�Ϊ������� ���ô������� ֱ�Ӽ��㼴��
			{
				temp = neure_network->neure_layer[j].neure[i].inputfun(neure_network->neure_layer[j].neure[i].input_sift, neure_network->neure_layer[j].neure[i].weight, neure_network->neure_layer[j].neure[i].b, neure_network->neure_layer[j].neure[i].input_num);
				*neure_network->neure_layer[j].neure[i].output = neure_network->neure_layer[j].neure[i].transferfun(temp);
			}
		}
	}
}

//˵��������ѵ�������� �����������Ȩֵ
//neure_network������ѵ����������ṹ
//sample������ѵ��������
//answer����������������ȷ���
//sample_num��������������һ��������Ϊ���ֵ��Ҳ��Ϊ����ֵ��
//train_num��������ѵ������ �ﵽ�������˳� �����Ȱٷְ�ʱ�˳�
void train_neure_network(Neure_Network_def *neure_network,double *sample,double *answer,int sample_num,long long train_num)
{
	int i, j, k,l,m,n,o;
	double result[10] = {0};
	double rate = 0.2;
	double temp = 0;
	double now_err_average = 0;
	double old_err_average = 0;
	int schedule = 0;

	double test = 1.2;
	test = 1.2;
	now_err_average = 0;

#ifdef DEBUG
	printf("��ǰѵ�����ȣ�%%%d  ��ǰ��%f  ��ǰѧϰ�ʣ�%f", schedule, now_err_average, rate);
#endif
	for (l = 0; l < train_num; l++)        //ѭ��ѵ��train_num��
	{
		//printf("\r");
		//system("cls");
		for (m = 0; m < sample_num; m++)  //ѭ������ÿһ��ѵ���õ�������
		{
			computed_neure_network_output(neure_network, &sample[m*neure_network->input_num]); //�����������
			for (k = 0; k < neure_network->neure_layer[neure_network->layer_num - 1].layer_neure_num;k++)
				result[k] = *neure_network->neure_layer[neure_network->layer_num - 1].neure[k].output; //��ȡ����������
			//��ʼ���򴫵����
			//printf("  �����%d������: %f,%f  �����%f\n", m, sample[m*neure_network->input_num], sample[m*neure_network->input_num + 1],result[0]);
			for (i = 0; i < neure_network->layer_num; i++)  //����ÿ��
			{
				//printf("    ��%d��:\n", neure_network->layer_num - i - 1);
				for (j = 0; j < neure_network->neure_layer[neure_network->layer_num - i - 1].layer_neure_num; j++) // ����ÿ���ÿһ����Ԫ
				{
					//printf("       ��%d����Ԫ:\n", j);
					if (i == 0)  //��ǰ��Ϊ�����ʱ
					{
						neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].now_err = result[j] * (1 - result[j])*(answer[m*neure_network->neure_layer[neure_network->layer_num - 1].layer_neure_num+j] - result[j]);
						
						for (k = 0; k < neure_network->neure_layer[neure_network->layer_num - i - 1].layer_input_num; k++)
							neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].weight[k] += rate*neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].now_err*neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].input_sift[k];
						
						neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].b += rate*neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].now_err;
					}
					else        //��ǰ��������
					{
						for (k = 0; k < neure_network->neure_layer[neure_network->layer_num - i].layer_neure_num; k++)
							temp += neure_network->neure_layer[neure_network->layer_num - i].neure[k].now_err*neure_network->neure_layer[neure_network->layer_num - i].neure[k].weight[j];

						neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].old_err = neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].now_err;

						neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].now_err = neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].output[0] * (1 - neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].output[0])*temp;
						temp = 0;

						for (k = 0; k < neure_network->neure_layer[neure_network->layer_num - i - 1].layer_input_num; k++)
							neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].weight[k] += rate*neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].now_err * neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].input_sift[k];
						
						neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].b += rate*neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].now_err;
					}
				}
			}
		}

		old_err_average = now_err_average;
		now_err_average = 0;
		for (k = 0; k < neure_network->neure_layer[neure_network->layer_num - 1].layer_neure_num; k++)
			now_err_average += neure_network->neure_layer[neure_network->layer_num - 1].neure[k].now_err;
		now_err_average /= neure_network->neure_layer[neure_network->layer_num - 1].layer_neure_num;

#ifdef DEBUG
		schedule = (int)(l * 100 / train_num);//�������

		printf("\r");
		printf("��ǰѵ�����ȣ�%%%d  ��ǰ��%f  ��ǰѧϰ�ʣ�%f", schedule, now_err_average,rate);

		if (fabs(now_err_average) < 0.001)
		{
			printf("\n���ﵽҪ�� ��ֹѵ��\n");
			return;
		}
		//getchar();
#endif
	}
}

double simple_sum(double *sift, double *weight, double b, int input_data_num)
{
	double sum = 0;
	int i = 0;
	for (i = 0; i < input_data_num; i++)
	{
		sum += sift[i] * weight[i];
	}
	sum += b;
	return sum;
}

double step(double sum)
{
	//if (sum>0)
	//	return 1;
	//else
	//	return 0;
	return 1 / (1 + exp(-sum));
}