// neure.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "myneure.h"
#include<stdio.h>
#include<stdlib.h>

#define LAYER_NUM                2           //�������������
#define NATEWORK_INPUT_DATA_NUM  2           //��������һ�����������ݵĸ���
#define NATEWORK_OUTPUT_DATA_NUM 1           //��������һ������������ݸ��� һ����������һ�����Ԫ������ͬ
#define SAMPLE_NUM               4           //��������ѵ������������
int layer_neure_num[LAYER_NUM] = { 2, 1 };    //����������������Ԫ����


int main(int argc, int* argv[])
{
	int i, j, k;

	Neure_def *layer[LAYER_NUM];

	Neure_Network_Layer_def neurelayer[LAYER_NUM];                           //������Ԫ��ṹ
	Neure_Network_def neure_network;                                         //����������ṹ

	double data[SAMPLE_NUM][NATEWORK_INPUT_DATA_NUM] = { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } };          //���ڲ��Ե�����
	double answer[SAMPLE_NUM] = { 0, 1, 1, 0 };
	double result[2];

	/*�����ʼ����ʼ*/
	for (i = 0; i < LAYER_NUM; i++)
	{
		layer[i] = (Neure_def *)malloc(layer_neure_num[i] * sizeof(Neure_def));//�������ø�ÿ����Ԫ�����ռ� �������ڳ�ʼ�������Ԫ����
		neurelayer[i].neure = layer[i];//�ѷ���õ���Ԫ���鸳����Ԫ��ṹ ����Ԫ�ṹ������������Ԫ��ṹ

		if (i == 0)                    //�����ǰ��ʼ�����������
			neure_network_layer_init(&neurelayer[i], NATEWORK_INPUT_DATA_NUM, layer_neure_num[i]);
		else                           //�����ǰ��ʼ���ķ������
			neure_network_layer_init(&neurelayer[i], layer_neure_num[i - 1], layer_neure_num[i]);
	}
	neure_network_init(&neure_network, neurelayer, LAYER_NUM, NATEWORK_INPUT_DATA_NUM);              //����������ṹ ��Ҫ�ǽ����������ӹ�ϵ
	/*�����ʼ�����*/
	train_neure_network(&neure_network, (double *)data, (double *)answer, SAMPLE_NUM, 1000000);      //ѵ��

	printf("\n\nѵ������ ��ʼ����\n");
	computed_neure_network_output(&neure_network, data[0]);
	result[0] = *layer[LAYER_NUM - 1]->output;
	printf("��%d��  %f  \n", 0, result[0]);

	computed_neure_network_output(&neure_network, data[1]);
	result[0] = *layer[LAYER_NUM - 1]->output;
	printf("��%d��  %f  \n", 1, result[0]);

	computed_neure_network_output(&neure_network, data[2]);
	result[0] = *layer[LAYER_NUM - 1]->output;
	printf("��%d��  %f  \n", 2, result[0]);

	computed_neure_network_output(&neure_network, data[3]);
	result[0] = *layer[LAYER_NUM - 1]->output;
	printf("��%d��  %f  \n", 3, result[0]);

	printf("\a\a\a");
	return 0;
}

