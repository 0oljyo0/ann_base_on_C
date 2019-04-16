#ifndef __MYNEURE_H
#define __MYNEURE_H

//#include "stdafx.h"

typedef double(*Inputfun_def)(double *sift, double *weight, double b, int inputnumber);
typedef double(*Transferfun_def)(double sum);

typedef struct
{
	int input_num;               //��Ԫ�������ݸ���
	double b;                    //��Ԫƫ����
	double *input_sift;			 //��Ԫ�������ݻ���
	double *weight;			     //��Ԫ������������Ȩ��
	double *output;				 //��Ԫ������ݵ�ָ��
	double now_err;				 //���һ��ѵ���ó������ ��ѵ����صĲ���
	double old_err;			     //��һ��ѵ���ĳ������ ��ѵ����صĲ���

	Inputfun_def inputfun;       //��Ȩ��ͺ���
	Transferfun_def transferfun; //���亯��
}Neure_def;

typedef struct
{
	int layer_neure_num;      //����Ԫ��Ŀ
	int layer_input_num;      //���������ݵ���Ŀ �ͱ�����Ԫ���������ݵĸ�����ͬ
	int layer_output_num;     //��������ݵ���Ŀ һ��Ͳ���Ԫ������ͬ
	double *layer_input_data; //�������ݵĸ���

	Neure_def *neure;         //�����Ԫ���� �����������������Ԫ
}Neure_Network_Layer_def;

typedef struct
{
	int input_num;                          //�������������ݸ���
	int layer_num;                          //���������
	int output_num;                         //������������ݸ��� һ������������һ����Ԫ�ĸ�����ͬ
	double *input_sift;                     //�������ݻ���
	double *output;                         //������ݵ�ָ��

	Neure_Network_Layer_def *neure_layer;   //����������Ԫ������ ����ÿһ����Ԫ��
}Neure_Network_def;


void neure_init(Neure_def *neure, int input_data_num, Inputfun_def inputfunction, Transferfun_def transferfunction);  //��Ԫ��ʼ�� ����ռ� ��ʱû���ͷ�
void neure_network_layer_init(Neure_Network_Layer_def *neure_layer, int input_data_num, int layer_neure_num);         //������Ԫ��ʼ�� ��ʼ��������ĵ���
void neure_network_init(Neure_Network_def *neure_network, Neure_Network_Layer_def *neure_layer, int layer_num, int input_data_num); //��ʼ������ ��Ҫ�ǽ��������ϵ

void computed_neure_network_output(Neure_Network_def *neure_network, double *input_sift);    //��������������
void train_neure_network(Neure_Network_def *neure_network, double *sample, double *answer, int sample_num, long long train_num);

double simple_sum(double *sift, double *weight, double b, int input_data_num);   //��ͺ���


double step(double sum);  //���亯��

#endif