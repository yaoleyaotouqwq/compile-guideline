#ifndef YUYI_H
#define YUYI_H

#include "cifa.h"

typedef struct {
	int type;
	std::string name;
	int value;
}value_table;//�ַ���

typedef struct {
	int type; //��Ч�����ã�����
	int value;//�������͵�valueΪ��ֵ���ַ����б�����valueΪ�����ַ����±�
}Item;

typedef struct {
	int op;
	Item arg1;
	Item arg2;
	int result;
}For_money;

#define NULL_TYPE 404//��Ч�ֶ�
#define TEMP_TYPE 502//��ʱֵ
#define VALUE_TYPE 666//ע�����
#define INDEX_TYPE 777//���ñ���������ڵı���
#define CONST_TYPE 888//����ֵ
#define JMP 999 //��������ת

void outthe_yuyi_list(word_list *words, int max_len);

#endif