#ifndef YUYI_H
#define YUYI_H

#include "cifa.h"

typedef struct {
	int type;
	std::string name;
	int value;
}value_table;//字符表

typedef struct {
	int type; //无效，引用，常数
	int value;//常量类型的value为真值，字符表中变量的value为所在字符表下标
}Item;

typedef struct {
	int op;
	Item arg1;
	Item arg2;
	int result;
}For_money;

#define NULL_TYPE 404//无效字段
#define TEMP_TYPE 502//临时值
#define VALUE_TYPE 666//注册变量
#define INDEX_TYPE 777//引用变量表里存在的变量
#define CONST_TYPE 888//常量值
#define JMP 999 //无条件跳转

void outthe_yuyi_list(word_list *words, int max_len);

#endif