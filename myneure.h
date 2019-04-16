#ifndef __MYNEURE_H
#define __MYNEURE_H

//#include "stdafx.h"

typedef double(*Inputfun_def)(double *sift, double *weight, double b, int inputnumber);
typedef double(*Transferfun_def)(double sum);

typedef struct
{
	int input_num;               //神经元输入数据个数
	double b;                    //神经元偏置量
	double *input_sift;			 //神经元输入数据缓存
	double *weight;			     //神经元各个输入数据权重
	double *output;				 //神经元输出数据的指针
	double now_err;				 //最进一次训练得出的误差 和训练相关的参数
	double old_err;			     //上一次训练的出的误差 和训练相关的参数

	Inputfun_def inputfun;       //加权求和函数
	Transferfun_def transferfun; //传输函数
}Neure_def;

typedef struct
{
	int layer_neure_num;      //层神经元数目
	int layer_input_num;      //层输入数据的数目 和本层神经元的输入数据的个数相同
	int layer_output_num;     //层输出数据的数目 一般和层神经元个数相同
	double *layer_input_data; //输入数据的个数

	Neure_def *neure;         //层的神经元数组 包含整个层的所有神经元
}Neure_Network_Layer_def;

typedef struct
{
	int input_num;                          //神经网络输入数据个数
	int layer_num;                          //神经网络层数
	int output_num;                         //神经网络输出数据个数 一般和神经网络最后一层神经元的个数相同
	double *input_sift;                     //输入数据缓存
	double *output;                         //输出数据的指针

	Neure_Network_Layer_def *neure_layer;   //神经网络中神经元层数组 包含每一个神经元层
}Neure_Network_def;


void neure_init(Neure_def *neure, int input_data_num, Inputfun_def inputfunction, Transferfun_def transferfunction);  //神经元初始化 分配空间 暂时没有释放
void neure_network_layer_init(Neure_Network_Layer_def *neure_layer, int input_data_num, int layer_neure_num);         //调用神经元初始化 初始化神经网络的单层
void neure_network_init(Neure_Network_def *neure_network, Neure_Network_Layer_def *neure_layer, int layer_num, int input_data_num); //初始化网络 主要是建立层间联系

void computed_neure_network_output(Neure_Network_def *neure_network, double *input_sift);    //计算整个层的输出
void train_neure_network(Neure_Network_def *neure_network, double *sample, double *answer, int sample_num, long long train_num);

double simple_sum(double *sift, double *weight, double b, int input_data_num);   //求和函数


double step(double sum);  //传输函数

#endif