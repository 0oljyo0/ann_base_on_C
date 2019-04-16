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
//说明：单个神经元的初始化   同时给权重分配空间
//neure ：待初始化的神经元结构
//input_data_num :神经元接受输入数据的个数
//inputfunction ：输入函数 即加权求和用的函数
//transferfunction ：传输函数 即数据格式化输出的函数
void neure_init(Neure_def *neure, int input_data_num, Inputfun_def inputfunction, Transferfun_def transferfunction)
{
	int i = 0;
	neure->b = (rand() % 100000)*0.00001;                                        //初始化偏置量b
	neure->input_num = input_data_num;                                           //初始化输入数据数量
	neure->input_sift = NULL;//(double *)malloc(input_data_num*sizeof(double));  //分配输入数据的空间
	neure->weight = (double *)malloc(input_data_num*sizeof(double));             //分配权重的空间

	for (i = 0; i < input_data_num; i++)									     //初始化权值
	{
		neure->weight[i] = (rand() % 100000)*0.00001;                            //权值初始化为随机数
	}

 	neure->transferfun = transferfunction;										 //设置传输函数
	neure->inputfun = inputfunction;										     //设置加权求和的函数
}

//说明：网络中某一层的初始化  给每一层的输入挂上同一个空间
//neure_layer：待初始化的神经层结构 单层神经
//input_data_num：神经层输入数据的个数 一般和本层神经元的输入数据个数相同
//layer_neure_num：本层神经元个数
void neure_network_layer_init(Neure_Network_Layer_def *neure_layer, int input_data_num, int layer_neure_num)
{
	int i;
	neure_layer->layer_input_num = input_data_num;
	neure_layer->layer_neure_num = layer_neure_num;
	neure_layer->layer_input_data = (double *)malloc(input_data_num*sizeof(double));           //申请本层输入的空间 本层每个神经元公用

	for (i = 0; i < layer_neure_num; i++)                                                      //循环初始化层的每一个神经元
	{
		neure_init(&neure_layer->neure[i], neure_layer->layer_input_num, simple_sum, step);    //神经元初始化
		neure_layer->neure[i].input_sift = neure_layer->layer_input_data;                      //给输入挂同一个空间
	}
}

//说明：整个网络初始化 主要是构建整个网络 把前层输出指针挂上后层的输入 这样前层计算结束后可以马上开始计算后层 不需要进行前层计算结果赋值给后层
//neure_network：待初始化的神经网络结构
//neure_layer：用于构建网络的各个神经层数组
//layer_num：神经网络的层数 输入的数据不算一层
//input_data_num：神经网络输入的数据个数
void neure_network_init(Neure_Network_def *neure_network, Neure_Network_Layer_def *neure_layer, int layer_num, int input_data_num)
{
	int i, j, k;
	neure_network->neure_layer = neure_layer;
	neure_network->input_num = input_data_num;
	neure_network->input_sift = neure_network->neure_layer[0].neure[0].input_sift;
	neure_network->layer_num = layer_num;

	//给每一个前后层挂钩 即给两个指针同一个空间
	for (i = 0; i < layer_num-1; i++)                                           //遍历每一个间层 如为3层神经网络 则有两个间层
	{
		for (j = 0; j < neure_network->neure_layer[i+1].layer_neure_num; j++)   //遍历每一个后层神经元
		{
			for (k = 0; k < neure_network->neure_layer[i].layer_neure_num; k++) //遍历每一个前层神经元
			{
				neure_network->neure_layer[i].neure[k].output = &neure_network->neure_layer[i + 1].neure[j].input_sift[k]; //把前层的输出地址改为后层的输入地址 免去计算时的频繁赋值操作
			}
		}
	}
	for (i = 0; i < neure_network->neure_layer[neure_network->layer_num - 1].layer_neure_num; i++) //遍历最后层的每一个神经元 给输出层神经元分配输出的空间
	{
		neure_network->neure_layer[neure_network->layer_num - 1].neure[i].output = (double *)malloc(sizeof(double));//由于最后一层的输出没有可以挂的后层 所以直接分配空间 相当于一个变量
	}
}

//说明：计算输出结果 结果存放在输出层神经元中 所以本函数无返回值
//neure_network：待计算结果的神经网络
//input_sift：输入神经网络的特征值
void computed_neure_network_output(Neure_Network_def *neure_network, double *input_sift)
{
	int i, j;
	double temp = 0;

	for (j = 0; j < neure_network->layer_num;j++)
	{		
		for (i = 0; i < neure_network->neure_layer[j].layer_neure_num; i++)   //带入网络输入数据 计算第一层的输出值
		{
			if (j == 0)  //如果当前计算的层为输入层 则直接带入输入数据计算
			{
				neure_network->neure_layer[j].neure[i].input_sift = input_sift;
				temp = neure_network->neure_layer[j].neure[i].inputfun(neure_network->neure_layer[j].neure[i].input_sift, neure_network->neure_layer[j].neure[i].weight, neure_network->neure_layer[j].neure[i].b, neure_network->neure_layer[j].neure[i].input_num);
				*neure_network->neure_layer[j].neure[i].output = neure_network->neure_layer[j].neure[i].transferfun(temp);
			}
			else         //如果当前计算的层为非输出层 则不用带入数据 直接计算即可
			{
				temp = neure_network->neure_layer[j].neure[i].inputfun(neure_network->neure_layer[j].neure[i].input_sift, neure_network->neure_layer[j].neure[i].weight, neure_network->neure_layer[j].neure[i].b, neure_network->neure_layer[j].neure[i].input_num);
				*neure_network->neure_layer[j].neure[i].output = neure_network->neure_layer[j].neure[i].transferfun(temp);
			}
		}
	}
}

//说明：用于训练神经网络 调整神经网络的权值
//neure_network：参与训练的神经网络结构
//sample：用于训练的样本
//answer：给定的样本的正确输出
//sample_num：样本的组数（一组样本可为多个值，也可为单个值）
//train_num：给定的训练次数 达到次数就退出 即进度百分百时退出
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
	printf("当前训练进度：%%%d  当前误差：%f  当前学习率：%f", schedule, now_err_average, rate);
#endif
	for (l = 0; l < train_num; l++)        //循环训练train_num次
	{
		//printf("\r");
		//system("cls");
		for (m = 0; m < sample_num; m++)  //循环带入每一组训练用的数据组
		{
			computed_neure_network_output(neure_network, &sample[m*neure_network->input_num]); //计算网络输出
			for (k = 0; k < neure_network->neure_layer[neure_network->layer_num - 1].layer_neure_num;k++)
				result[k] = *neure_network->neure_layer[neure_network->layer_num - 1].neure[k].output; //获取网络输出结果
			//开始反向传递误差
			//printf("  调入第%d组数据: %f,%f  结果：%f\n", m, sample[m*neure_network->input_num], sample[m*neure_network->input_num + 1],result[0]);
			for (i = 0; i < neure_network->layer_num; i++)  //遍历每层
			{
				//printf("    第%d层:\n", neure_network->layer_num - i - 1);
				for (j = 0; j < neure_network->neure_layer[neure_network->layer_num - i - 1].layer_neure_num; j++) // 遍历每层的每一个神经元
				{
					//printf("       第%d个神经元:\n", j);
					if (i == 0)  //当前层为输出层时
					{
						neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].now_err = result[j] * (1 - result[j])*(answer[m*neure_network->neure_layer[neure_network->layer_num - 1].layer_neure_num+j] - result[j]);
						
						for (k = 0; k < neure_network->neure_layer[neure_network->layer_num - i - 1].layer_input_num; k++)
							neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].weight[k] += rate*neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].now_err*neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].input_sift[k];
						
						neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].b += rate*neure_network->neure_layer[neure_network->layer_num - i - 1].neure[j].now_err;
					}
					else        //当前层非输出层
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
		schedule = (int)(l * 100 / train_num);//计算进度

		printf("\r");
		printf("当前训练进度：%%%d  当前误差：%f  当前学习率：%f", schedule, now_err_average,rate);

		if (fabs(now_err_average) < 0.001)
		{
			printf("\n误差达到要求 终止训练\n");
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