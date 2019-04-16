// neure.cpp : 定义控制台应用程序的入口点。
//

#include "myneure.h"
#include<stdio.h>
#include<stdlib.h>

#define LAYER_NUM                2           //定义神经网络层数
#define NATEWORK_INPUT_DATA_NUM  2           //定义网络一次性输入数据的个数
#define NATEWORK_OUTPUT_DATA_NUM 1           //定义网络一次性输出的数据个数 一般和网络最后一层的神经元个数相同
#define SAMPLE_NUM               4           //定义用于训练的样本组数
int layer_neure_num[LAYER_NUM] = { 2, 1 };    //定义神经网络各层的神经元个数


int main(int argc, int* argv[])
{
	int i, j, k;

	Neure_def *layer[LAYER_NUM];

	Neure_Network_Layer_def neurelayer[LAYER_NUM];                           //定义神经元层结构
	Neure_Network_def neure_network;                                         //定义神经网络结构

	double data[SAMPLE_NUM][NATEWORK_INPUT_DATA_NUM] = { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } };          //用于测试的数据
	double answer[SAMPLE_NUM] = { 0, 1, 1, 0 };
	double result[2];

	/*网络初始化开始*/
	for (i = 0; i < LAYER_NUM; i++)
	{
		layer[i] = (Neure_def *)malloc(layer_neure_num[i] * sizeof(Neure_def));//按照配置给每个神经元层分配空间 设置用于初始化层的神经元数组
		neurelayer[i].neure = layer[i];//把分配好的神经元数组赋给神经元层结构 用神经元结构数组来构建神经元层结构

		if (i == 0)                    //如果当前初始化的是输入层
			neure_network_layer_init(&neurelayer[i], NATEWORK_INPUT_DATA_NUM, layer_neure_num[i]);
		else                           //如果当前初始化的非输入层
			neure_network_layer_init(&neurelayer[i], layer_neure_num[i - 1], layer_neure_num[i]);
	}
	neure_network_init(&neure_network, neurelayer, LAYER_NUM, NATEWORK_INPUT_DATA_NUM);              //构建神经网络结构 主要是建立层间的连接关系
	/*网络初始化完成*/
	train_neure_network(&neure_network, (double *)data, (double *)answer, SAMPLE_NUM, 1000000);      //训练

	printf("\n\n训练结束 开始测试\n");
	computed_neure_network_output(&neure_network, data[0]);
	result[0] = *layer[LAYER_NUM - 1]->output;
	printf("第%d组  %f  \n", 0, result[0]);

	computed_neure_network_output(&neure_network, data[1]);
	result[0] = *layer[LAYER_NUM - 1]->output;
	printf("第%d组  %f  \n", 1, result[0]);

	computed_neure_network_output(&neure_network, data[2]);
	result[0] = *layer[LAYER_NUM - 1]->output;
	printf("第%d组  %f  \n", 2, result[0]);

	computed_neure_network_output(&neure_network, data[3]);
	result[0] = *layer[LAYER_NUM - 1]->output;
	printf("第%d组  %f  \n", 3, result[0]);

	printf("\a\a\a");
	return 0;
}

