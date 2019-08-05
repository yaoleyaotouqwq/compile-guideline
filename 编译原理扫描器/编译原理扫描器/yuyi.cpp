#include "stdafx.h"

int value_curror = 0;

bool tran_error_flag = 0;//出错状态记录
int tran_error_curror = 0;//保存出错下标记录

int NXQ = 0;
int temp_value_id = 0;
const int Max_len = 100;

value_table value_tables[Max_len];
For_money for_money[Max_len];

int TransStatement(word_list *words, int &word_cursor, int &result_value);
bool TransStatement_table(word_list *words, int &word_cursor, int &result_value);

int Fing_value_index(std::string name)
{
	for (int temp = 0; temp < value_curror; temp++)
	{
		if (value_tables[temp].name == name)
		{
			return temp;
		}
	}
	return -1;
}

bool Bool_result(int op, int temp1, int temp2)
{
	if (op == 14)
	{
		return temp1 < temp2;
	}
	else if (op == 15)
	{
		return temp1 >temp2;
	}
	else if (op == 16)
	{
		return temp1 <= temp2;
	}
	else if (op == 17)
	{
		return temp1 >= temp2;
	}
	else
	{
		return temp1 != temp2;
	}
}

std::string findoperate(int op)
{
	if (op == 10)
	{
		return "=";
	}
	else if(op == 11)
	{
		return ":=";
	}
	else if (op == 12)
	{
		return "+";
	}
	else if (op == 13)
	{
		return "*";
	}
	else if (op == 14)
	{
		return "<";
	}
	else if (op == 15)
	{
		return ">";
	}
	else if (op == 16)
	{
		return "<=";
	}
	else if (op == 17)
	{
		return ">=";
	}
	else if (op == 18)
	{
		return "(";
	}
	else if (op == 19)
	{
		return ")";
	}
	else if (op == 20)
	{
		return "<>";
	}
	else if (op == 404)
	{
		return "NULL_Operater";
	}
	else if (op == 502)
	{
		return "Temp";
	}
	else if (op == 666)
	{
		return "Entry_Value";
	}
	else if (op == 777)
	{
		return "Get_Value";
	}
	else if (op == 888)
	{
		return "Const_Operate";
	}
	else if (op == 999)
	{
		return "JMP_Other";
	}

}

std::string findValue_name(int index)
{
	if(index < value_curror)
	{
		return value_tables[index].name;
	}
}

int findValue_value(int index)
{
	if (index < value_curror)
	{
		return value_tables[index].value;
	}
}

int newValue()
{
	if (value_curror < Max_len)
	{

		value_tables[value_curror].type = TEMP_TYPE;
		value_tables[value_curror].name = "temp_value" + std::to_string(temp_value_id);
		value_curror++;
		temp_value_id++;
		return value_curror - 1;
	}
	return -1;
}

int General_for_money(int op, Item arg1, Item arg2, int result)
{
	if (NXQ < Max_len)
	{
		for_money[NXQ].op = op;
		for_money[NXQ].arg1 = arg1;
		for_money[NXQ].arg2 = arg2;
		for_money[NXQ].result = result;
		NXQ++;
		return NXQ - 1;
	}

	return -1;
}

void updata_Value(int id, int value)
{
	value_tables[id].value = value;
}

int General_for_money(int op, int arg1, int arg2, int result)
{
	if (NXQ < Max_len)
	{
		for_money[NXQ].op = op;
		for_money[NXQ].arg1.type = NULL_TYPE;
		for_money[NXQ].arg1.value = 0;
		for_money[NXQ].arg2.type = NULL_TYPE;
		for_money[NXQ].arg2.value = 0;
		for_money[NXQ].result = result;
		NXQ++;
		return NXQ - 1;
	}
}

//用四元式序号回填以out为首的链，将每个四元式的Result域改为NXQ
void BackPut(int out, int nextindex)
{
	int index = out;
	while (index) {
		int temp = for_money[index].result;
		for_money[index].result = nextindex;
		index = temp;
	}
}

int Find_value(std::string name)
{
	int temp;
	for (temp = 0; temp < value_curror; temp++)
	{
		if (value_tables[temp].name == name)
		{
			return temp;
		}
	}
	return -1;
}

//符号表添加变量
void entry_value(std::string name)
{
	if (Find_value(name) == -1)
	{
		value_tables[value_curror].type = VALUE_TYPE;
		value_tables[value_curror].name = name;
		value_curror++;
	}
}

//保存出错时的指针位置
void tran_error_curor_save(int word_cursor)
{
	if (tran_error_flag == 0)
	{
		tran_error_curror = word_cursor;
		tran_error_flag = 1;
	}
}

//输出错误信息
void tran_run_error(word_list *words, int word_cursor, int max_len)
{
	if (word_cursor > 0 && word_cursor < max_len - 1)
	{
		std::cout << "Error rows content:" <<
			words[word_cursor - 1].type << " " <<
			words[word_cursor].type << " " <<
			words[word_cursor + 1].type << std::endl;
	}
	else if (word_cursor == 0)
	{
		std::cout << "Error rows content:" <<
			words[word_cursor].type << " " <<
			words[word_cursor + 1].type << " " <<
			words[word_cursor + 2].type << std::endl;
	}
	else
	{
		std::cout << "Error rows content:" <<
			words[word_cursor - 2].type << " " <<
			words[word_cursor - 1].type << " " <<
			words[word_cursor].type << std::endl;
	}

	std::cout << "Please check your code!" << word_cursor + 1 << std::endl;
	std::cin.get();
	exit(0);
}

//<变量表>	→	ID|ID,<变量表>
int TransVariable_table(word_list *words, int &word_cursor, int &result_value)
{
	if (words[word_cursor].num == ID)
	{
		entry_value(words[word_cursor].type);
		++word_cursor;
		if (words[word_cursor].num == BEAN)
		{
			++word_cursor;
			if (TransVariable_table(words, word_cursor, result_value) == 1)
			{
				return 1;
			}
			else
			{
				tran_error_curor_save(word_cursor);
				std::cout << "Error! the mean not <变量表>→ID | ID, <变量表>" << std::endl;
				return 0;
			}
		}
		return 1;
	}
	return 2;//可以没有ID即判断为变量表为空
}

//<变量说明序列>	→	<变量表>:INTEGER; <变量说明序列公因式>|εand <变量说明序列公因式> → ε|<变量说明序列>
bool TransVariable_explorer_list(word_list *words, int &word_cursor, int &result_value)
{
	++word_cursor;
	int flag_temp = TransVariable_table(words, word_cursor, result_value);

	if (flag_temp == 0)
	{
		tran_error_curor_save(word_cursor);
		std::cout << "Error! the mean not <变量说明序列>	→	<变量表>:INTEGER; <变量说明序列公因式>|εand <变量说明序列公因式> → ε|<变量说明序列>" << std::endl;
		return 0;
	}

	else if (flag_temp == 1)
	{
		if (words[word_cursor].num == BUBBLE && words[word_cursor + 1].num == INTEGER && words[word_cursor + 2].num == SPLIT)
		{
			word_cursor += 2;
			if (words[word_cursor + 1].num == ID)
			{
				if (TransVariable_explorer_list(words, word_cursor, result_value))
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				++word_cursor;
				return 1;
			}
		}
		else
		{
			tran_error_curor_save(word_cursor);
			std::cout << "Error! the mean not <变量说明序列>	→	<变量表>:INTEGER; <变量说明序列公因式>|εand <变量说明序列公因式> → ε|<变量说明序列>" << std::endl;
			return 0;
		}
	}

	else//变量说明序列可以为空，但如果只是变量表为空，后面的integer还有则报错
	{
		return 1;
	}

}

//<变量说明>→VAR<变量说明序列>
bool TransVariable_explorer(word_list *words, int &word_cursor, int &result_value)
{
	if (words[word_cursor].num == VAR)
	{
		if (TransVariable_explorer_list(words, word_cursor, result_value))
		{
			return 1;
		}
		else
		{
			tran_error_curor_save(word_cursor);
			std::cout << "Error! the mean not <变量说明>→VAR<变量说明序列>" << std::endl;
			return 0;
		}
	}
	else
	{
		tran_error_curor_save(word_cursor);
		std::cout << "Error! the mean not <变量说明>→VAR<变量说明序列>" << std::endl;
		return 0;
	}
}

//<项>	→	<因式><项公因式> and <项公因式> → ε | *<项> | /<项>
bool TransItem(word_list *words, int &word_cursor, int &type, int &result_value)
{
	int id;
	int temp_type, temp_value;
	Item item1, item2;

	if (words[word_cursor].num == INT || words[word_cursor].num == ID)
	{
		if (words[word_cursor].num == ID)
		{
			if (Find_value(words[word_cursor].type) == -1)
			{
				std::cout << "Error! the Value " << words[word_cursor].type <<" not define." << std::endl;
				std::cin.get();
				exit(1);
			}
			type = INDEX_TYPE;

			result_value = Fing_value_index(words[word_cursor].type);
		}
		else
		{
			type = CONST_TYPE;
			result_value = stoi(words[word_cursor].type);
		}

		++word_cursor;
		if (words[word_cursor].num == MULTIPLY)
		{
			id = newValue();
			item1.type = type;
			item1.value = result_value;
			++word_cursor;
			if (TransItem(words, word_cursor, temp_type,temp_value))
			{
				item2.type = temp_type;
				item2.value = temp_value;
				General_for_money(MULTIPLY, item1, item2, id);//添加四元式

				type = INDEX_TYPE;
				result_value = id;
				return 1;
			}
			else
			{
				return 0;
			}
		}
		return 1;//公因式推导出空的情况
	}
	else
	{
		tran_error_curor_save(word_cursor);
		std::cout << "Error! the mean not  <项>	→	<因式><项公因式> and <项公因式> → ε | *<项> | /<项>" << std::endl;
		return 0;
	}
}

//<算术表达式>→<项><算术表达式公因式> and <算术表达式公因式> → ε | +<算术表达式>
bool TransCount(word_list *words, int &word_cursor, int &type ,int &result_value)
{
	int id;
	int temp_type,temp_value;
	Item item1, item2;

	if (TransItem(words, word_cursor, type ,result_value))
	{
		if (words[word_cursor].num == ADD)
		{
			id = newValue();
			item1.type = type;
			item1.value = result_value;
			++word_cursor;
			if (TransCount(words, word_cursor, temp_type, temp_value))
			{
				item2.type = temp_type;
				item2.value = temp_value;
				General_for_money(ADD, item1, item2, id);//添加四元式
				
				type = INDEX_TYPE;
				result_value = id;
				return 1;
			}
			else
			{
				return 0;
			}
			
		}
		return 1;//公因式推导出空的情况
	}
	else
	{
		tran_error_curor_save(word_cursor);
		std::cout << "Error! the mean not  <算术表达式>→<项><算术表达式公因式> and <算术表达式公因子> → ε | +<算术表达式>" << std::endl;
		return 0;
	}
}

//<赋值语句> 	→	<变量> :=  <算术表达式>;
int TransSetvalue_statement(word_list *words, int &word_cursor, int &result_value)
{
	int id;
	int type;
	Item item1, item2;

	if (words[word_cursor].num == ID)
	{
		id = Find_value(words[word_cursor].type);
		if (id == -1)
		{
			std::cout << "Error! the Value " << words[word_cursor].type << " not define." << std::endl;
			std::cin.get();
			exit(1);
		}
		++word_cursor;
		if (words[word_cursor].num == ASSIGNMENT)
		{
			++word_cursor;
			if (!TransCount(words, word_cursor, type,result_value) || words[word_cursor].num != SPLIT)
			{
				tran_error_curor_save(word_cursor);
				std::cout << "Error! the mean not  <赋值语句> →<变量> : = <算术表达式>;" << std::endl;
				return 0;
			}

			//item1的值由临时变量的值或常量值构成
			item1.type = type;
			item1.value = result_value;
			item2.type = 0;
			item2.value = 0;
			General_for_money(ASSIGNMENT, item1, item2, id);

			return 1;
		}
		else
		{
			tran_error_curor_save(word_cursor);
			std::cout << "Error! the mean not  <赋值语句> →<变量> : = <算术表达式>;" << std::endl;
			return 0;
		}

		
	}
	return 2;
}

//<关系表达式>→<算数表达式> <关系符> <算数表达式>
bool TransRelation(word_list *words, int &word_cursor,int &true_out, int &false_out)
{
	int src, dir;
	int type1, type2;
	int op;
	Item item1, item2;
	if (TransCount(words, word_cursor, type1,src))
	{
		if (words[word_cursor].num == SMALLER || words[word_cursor].num == BIGER || words[word_cursor].num == SMALLER_EQUAL || words[word_cursor].num == BIGER_EQUAL || words[word_cursor].num == NOT_EQUAL)
		{
			op = words[word_cursor].num;
			++word_cursor;
			if (TransCount(words, word_cursor, type2,dir))
			{
				item1.type = type1;
				item1.value = src;
				item2.type = type2;
				item2.value = dir;
				
				true_out = General_for_money(op, item1, item2, 0);//真出口
				false_out = General_for_money(JMP, 0, 0, 0);//假出口
				return 1;
			}
			else
			{
				tran_error_curor_save(word_cursor);
				std::cout << "Error! the mean not  <关系表达式>→<算数表达式> <关系符> <算数表达式>,the last <算数表达式> is mistake." << std::endl;
				return 0;
			}
		}
		else
		{
			tran_error_curor_save(word_cursor);
			std::cout << "Error! the mean not  <关系表达式>→<算数表达式> <关系符> <算数表达式>,<关系符> is mistake." << std::endl;
			return 0;
		}
	}
	else
	{
		std::cout << "Error! the mean not  <关系表达式>→<算数表达式> <关系符> <算数表达式>" << std::endl;

		return 0;
	}
}

//<条件语句>	→	IF <关系表达式> THEN <语句><条件语句公因式> and <条件语句公因式> → ε | ELSE<语句>
int TransCondition_statement(word_list *words, int &word_cursor, int &result_value)
{
	int true_out, false_out;
	int loop;
	
	if (words[word_cursor].num == IF)
	{
		++word_cursor;
		if (TransRelation(words, word_cursor,true_out,false_out))
		{
			if (words[word_cursor].num == THEN)
			{
				++word_cursor;
				BackPut(true_out, NXQ);//回填真出口
				if (TransStatement(words, word_cursor, result_value))//允许if下为空语句
				{
					++word_cursor;
					if (words[word_cursor].num == ELSE)
					{
						loop = General_for_money(JMP, 0, 0, 0);
						BackPut(false_out,NXQ);//回填假出口

						++word_cursor;
						if (TransStatement(words, word_cursor, result_value))
						{
							BackPut(loop, NXQ);//回填结束出口
							return 1;
						}
						else
						{
							tran_error_curor_save(word_cursor);
							std::cout << "Error! the mean not <语句>→<赋值语句> | <条件语句> | <while语句> " << std::endl;
							return 0;
						}
					}
					else
					{
						tran_error_curor_save(word_cursor);
						std::cout << "Error! the mean not  <条件语句>	→	IF <关系表达式> THEN <语句><条件语句公因式> and <条件语句公因子> → ε | ELSE<语句>" << std::endl;
						return 0;
					}
				}
				else
				{
					tran_error_curor_save(word_cursor);
					std::cout << "Error! the mean not <语句>→<赋值语句> | <条件语句> | <while语句> " << std::endl;
					return 0;
				}
			}
			else
			{
				tran_error_curor_save(word_cursor);
				std::cout << "Error! the mean not  <条件语句>	→	IF <关系表达式> THEN <语句><条件语句公因式> and <条件语句公因子> → ε | ELSE<语句>" << std::endl;
				return 0;
			}
		}
		else
		{
			tran_error_curor_save(word_cursor);
			std::cout << "Error! the mean not  <条件语句>	→	IF <关系表达式> THEN <语句><条件语句公因式> and <条件语句公因子> → ε | ELSE<语句>" << std::endl;
			return 0;
		}
	}
	return 2;//条件语句公因式为空的情况

}

//<while语句>→	WHILE <关系表达式> DO <语句>END;
int TransWhile_statement(word_list *words, int &word_cursor, int &result_value)
{
	int true_out, false_out;
	int loop;
	if (words[word_cursor].num == WHILE)
	{
		++word_cursor;
		loop = NXQ;
		if (TransRelation(words, word_cursor, true_out, false_out))
		{
			if (words[word_cursor].num == DO)
			{
				BackPut(true_out, NXQ);
				if (TransStatement_table(words, word_cursor, result_value))
				{
					if (words[word_cursor].num == END && words[word_cursor + 1].num == SPLIT)
					{
						++word_cursor;
						General_for_money(JMP, 0, 0, loop);//无条件跳转回关系表达式
						BackPut(false_out, NXQ);//填写假出口

						return 1;
					}
					else
					{
						tran_error_curor_save(word_cursor);
						std::cout << "Error! the mean not <while语句>→	WHILE <关系表达式> DO <语句>END;" << std::endl;
						return 0;
					}
				}
				else
				{
					tran_error_curor_save(word_cursor);
					std::cout << "Error! the mean not <语句>→<赋值语句> | <条件语句> | <while语句> " << std::endl;
					return 0;
				}
			}
			else
			{
				tran_error_curor_save(word_cursor);
				std::cout << "Error! the mean not <while语句>	→	WHILE <关系表达式> DO <语句>END;" << std::endl;
				return 0;
			}
		}
		else
		{
			tran_error_curor_save(word_cursor);
			std::cout << "Error! the mean not <while语句>	→	WHILE <关系表达式> DO <语句>END;" << std::endl;
			return 0;
		}
	}
	return 2;
}

//<语句>	→	<赋值语句>|<条件语句>|<while语句>
int TransStatement(word_list *words, int &word_cursor, int &result_value)
{
	int flag_setvalue = TransSetvalue_statement(words, word_cursor,result_value);
	int flag_condition = TransCondition_statement(words, word_cursor, result_value);
	int flag_while = TransWhile_statement(words, word_cursor, result_value);

	if (flag_setvalue == 0)
	{
		std::cout << "Error! the mean not  <语句>	→	<赋值语句>|<条件语句>|<while语句>" << std::endl;
		return 0;
	}
	else if (flag_setvalue == 1)
	{
		return 1;
	}

	if (flag_condition == 0)
	{
		std::cout << "Error! the mean not  <语句>	→	<赋值语句>|<条件语句>|<while语句>" << std::endl;
		return 0;
	}
	else if (flag_condition == 1)
	{
		return 1;
	}

	if (flag_while == 0)
	{
		std::cout << "Error! the mean not  <语句>	→	<赋值语句>|<条件语句>|<while语句>" << std::endl;
		return 0;
	}
	else if (flag_while == 1)
	{
		return 1;
	}

	return 2;//语句表的语句可以为空

}

//<语句表>→<语句><语句公因式>|ε  and  <语句公因式> → ε | <语句表>
bool TransStatement_table(word_list *words, int &word_cursor, int &result_value)
{
	++word_cursor;
	int flag_temp = TransStatement(words, word_cursor, result_value);

	if (flag_temp == 0)
	{
		tran_error_curor_save(word_cursor);
		std::cout << "Error! the mean not   <语句表>→<语句><语句公因子>|ε  and  <语句公因子> → ε | <语句表>" << std::endl;
		return 0;
	}
	else if (flag_temp == 1)
	{

		if (TransStatement_table(words, word_cursor, result_value))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else//语句可以没有的情况和公因式推导出空的情况
	{
		return 1;
	}
}

//<程序>→<变量说明>BEGIN<语句表>END
bool TransProgram(word_list *words, int &word_cursor , int &result_value)
{
	if (TransVariable_explorer(words, word_cursor, result_value))
	{
		if (words[word_cursor].num == BEGIN)
		{
			if (TransStatement_table(words, word_cursor, result_value))
			{
				if (words[word_cursor].num == END && words[word_cursor + 1].num == POINT)
				{
					++word_cursor;
					return 1;
				}
				else
				{
					tran_error_curor_save(word_cursor);
					std::cout << "Error! the mean not  <程序>→<变量说明>BEGIN<语句表>END." << std::endl;
					return 0;
				}
			}
			else
			{
				std::cout << "Error! the mean not  <程序>→<变量说明>BEGIN<语句表>END." << std::endl;
				return 0;
			}

		}
		else
		{
			tran_error_curor_save(word_cursor);
			std::cout << "Error! the mean not  <程序>→<变量说明>BEGIN<语句表>END." << std::endl;
			return 0;
		}
	}
	else
	{
		std::cout << "Error! the mean not  <程序>→<变量说明>BEGIN<语句表>END." << std::endl;
		return 0;
	}
}

void out_put_the_foryuan_and_symbol_table()
{
	for (int temp = 0; temp < NXQ; temp++)
	{
		std::cout << temp << " " << "(" << for_money[temp].op << "," << for_money[temp].arg1.value << "," << for_money[temp].arg2.value << "," << for_money[temp].result << ")" << "    ";
		if (for_money[temp].op == ASSIGNMENT)
		{
			if (for_money[temp].arg1.type == INDEX_TYPE)
			{
				std::cout << "(" << findoperate(for_money[temp].op) << "," << findValue_name(for_money[temp].arg1.value) << "," << for_money[temp].arg2.value << "," << findValue_name(for_money[temp].result) << ")" << std::endl;
			}
			else
			{
				std::cout << "(" << findoperate(for_money[temp].op) << "," << for_money[temp].arg1.value << "," << for_money[temp].arg2.value << "," << findValue_name(for_money[temp].result) << ")" << std::endl;
			}
		}
		else if (for_money[temp].op == JMP)
		{
			std::cout << "(" << findoperate(for_money[temp].op) << "," << for_money[temp].arg1.value << "," << for_money[temp].arg2.value << "," << for_money[temp].result << ")" << std::endl;
		}
		else if (for_money[temp].op == ADD || for_money[temp].op == MULTIPLY)
		{
			if (for_money[temp].arg1.type == CONST_TYPE && for_money[temp].arg2.type == CONST_TYPE)
			{
				std::cout << "(" << findoperate(for_money[temp].op) << "," << for_money[temp].arg1.value << "," << for_money[temp].arg2.value << "," << findValue_name(for_money[temp].result) << ")" << std::endl;
			}
			else if (for_money[temp].arg1.type == INDEX_TYPE && for_money[temp].arg2.type == CONST_TYPE)
			{
				std::cout << "(" << findoperate(for_money[temp].op) << "," << findValue_name(for_money[temp].arg1.value) << "," << for_money[temp].arg2.value << "," << findValue_name(for_money[temp].result) << ")" << std::endl;
			}
			else if (for_money[temp].arg1.type == CONST_TYPE && for_money[temp].arg2.type == INDEX_TYPE)
			{
				std::cout << "(" << findoperate(for_money[temp].op) << "," << for_money[temp].arg1.value << "," << findValue_name(for_money[temp].arg2.value) << "," << findValue_name(for_money[temp].result) << ")" << std::endl;
			}
			else if (for_money[temp].arg1.type == INDEX_TYPE && for_money[temp].arg2.type == INDEX_TYPE)
			{
				std::cout << "(" << findoperate(for_money[temp].op) << "," << findValue_name(for_money[temp].arg1.value) << "," << findValue_name(for_money[temp].arg2.value) << "," << findValue_name(for_money[temp].result) << ")" << std::endl;
			}
		}
		else if ((for_money[temp].op >= 14 && for_money[temp].op <= 17) || for_money[temp].op == 20)
		{
			if (for_money[temp].arg1.type == CONST_TYPE && for_money[temp].arg2.type == CONST_TYPE)
			{
				std::cout << "(" << findoperate(for_money[temp].op) << "," << for_money[temp].arg1.value << "," << for_money[temp].arg2.value << "," << for_money[temp].result << ")" << std::endl;
			}
			else if (for_money[temp].arg1.type == INDEX_TYPE && for_money[temp].arg2.type == CONST_TYPE)
			{
				std::cout << "(" << findoperate(for_money[temp].op) << "," << findValue_name(for_money[temp].arg1.value) << "," << for_money[temp].arg2.value << "," << for_money[temp].result << ")" << std::endl;
			}
			else if (for_money[temp].arg1.type == CONST_TYPE && for_money[temp].arg2.type == INDEX_TYPE)
			{
				std::cout << "(" << findoperate(for_money[temp].op) << "," << for_money[temp].arg1.value << "," << findValue_name(for_money[temp].arg2.value) << "," << for_money[temp].result << ")" << std::endl;
			}
			else if (for_money[temp].arg1.type == INDEX_TYPE && for_money[temp].arg2.type == INDEX_TYPE)
			{
				std::cout << "(" << findoperate(for_money[temp].op) << "," << findValue_name(for_money[temp].arg1.value) << "," << findValue_name(for_money[temp].arg2.value) << "," << for_money[temp].result << ")" << std::endl;
			}
		}
		else
		{
			std::cout << "(" << findoperate(for_money[temp].op) << "," << value_tables[for_money[temp].arg1.value].name << "," << for_money[temp].arg2.value << "," << for_money[temp].result << ")" << std::endl;
		}
	}
}

void run_the_For_yuan_formula()
{
	/*if (for_money[temp_index].arg1.type == CONST_TYPE && for_money[temp_index].arg2.type == CONST_TYPE)
	{
		updata_Value(for_money[temp_index].result, for_money[temp_index].arg1.value);
	}
	else if (for_money[temp_index].arg1.type == INDEX_TYPE && for_money[temp_index].arg2.type == CONST_TYPE)
	{
		updata_Value(for_money[temp_index].result, Fin_for_money[temp_index].arg1.value);
	}
	else if (for_money[temp_index].arg1.type == CONST_TYPE && for_money[temp_index].arg2.type == INDEX_TYPE)
	{
		updata_Value(for_money[temp_index].result, for_money[temp_index].arg1.value);
	}
	else if (for_money[temp_index].arg1.type == INDEX_TYPE && for_money[temp_index].arg2.type == INDEX_TYPE)
	{
		updata_Value(for_money[temp_index].result, for_money[temp_index].arg1.value);
	}*/

	int temp_index;

	for (temp_index = 0; temp_index < NXQ; ++temp_index)
	{
		if ((for_money[temp_index].op >= 14 && for_money[temp_index].op <= 17) || for_money[temp_index].op == 20)
		{
			if (for_money[temp_index].arg1.type == CONST_TYPE && for_money[temp_index].arg2.type == CONST_TYPE)
			{
				//布尔表达式为真，同下
				if (Bool_result(for_money[temp_index].op,for_money[temp_index].arg1.value, for_money[temp_index].arg2.value))
				{
					temp_index = for_money[temp_index].result;
					temp_index--;//循环会自增一;
				}
				else
				{
					++temp_index;//进入假出口
					temp_index = for_money[temp_index].result;
					temp_index--;//循环会自增一;
				}
			}
			else if (for_money[temp_index].arg1.type == INDEX_TYPE && for_money[temp_index].arg2.type == CONST_TYPE)
			{
				if (Bool_result(for_money[temp_index].op, findValue_value(for_money[temp_index].arg1.value), for_money[temp_index].arg2.value))
				{
					temp_index = for_money[temp_index].result;
					temp_index--;//循环会自增一;
				}
				else
				{
					++temp_index;//进入假出口
					temp_index = for_money[temp_index].result;
					temp_index--;//循环会自增一;
				}
			}
			else if (for_money[temp_index].arg1.type == CONST_TYPE && for_money[temp_index].arg2.type == INDEX_TYPE)
			{
				if (Bool_result(for_money[temp_index].op, for_money[temp_index].arg1.value, findValue_value(for_money[temp_index].arg2.value)))
				{
					temp_index = for_money[temp_index].result;
					temp_index--;//循环会自增一;
				}
				else
				{
					++temp_index;//进入假出口
					temp_index = for_money[temp_index].result;
					temp_index--;//循环会自增一;
				}
			}
			else if (for_money[temp_index].arg1.type == INDEX_TYPE && for_money[temp_index].arg2.type == INDEX_TYPE)
			{
				if (Bool_result(for_money[temp_index].op, findValue_value(for_money[temp_index].arg1.value), findValue_value(for_money[temp_index].arg2.value)))
				{
					temp_index = for_money[temp_index].result;
					temp_index--;//循环会自增一;
				}
				else
				{
					++temp_index;//进入假出口
					temp_index = for_money[temp_index].result;
					temp_index--;//循环会自增一;
				}
			}
		}
		else if (for_money[temp_index].op == JMP)//无条件跳转
		{
			temp_index = for_money[temp_index].result;
			temp_index--;//循环会自增一;
		}
		else if (for_money[temp_index].op == ADD)
		{
			if (for_money[temp_index].arg1.type == CONST_TYPE && for_money[temp_index].arg2.type == CONST_TYPE)
			{
				updata_Value(for_money[temp_index].result, for_money[temp_index].arg1.value + for_money[temp_index].arg2.value);
			}
			else if (for_money[temp_index].arg1.type == INDEX_TYPE && for_money[temp_index].arg2.type == CONST_TYPE)
			{
				updata_Value(for_money[temp_index].result, findValue_value(for_money[temp_index].arg1.value) + for_money[temp_index].arg2.value);
			}
			else if (for_money[temp_index].arg1.type == CONST_TYPE && for_money[temp_index].arg2.type == INDEX_TYPE)
			{
				updata_Value(for_money[temp_index].result, for_money[temp_index].arg1.value + findValue_value(for_money[temp_index].arg2.value));
			}
			else if (for_money[temp_index].arg1.type == INDEX_TYPE && for_money[temp_index].arg2.type == INDEX_TYPE)
			{
				updata_Value(for_money[temp_index].result, findValue_value(for_money[temp_index].arg1.value) + findValue_value(for_money[temp_index].arg2.value));
			}
		}
		else if (for_money[temp_index].op == MULTIPLY)
		{
			if (for_money[temp_index].arg1.type == CONST_TYPE && for_money[temp_index].arg2.type == CONST_TYPE)
			{
				updata_Value(for_money[temp_index].result, for_money[temp_index].arg1.value * for_money[temp_index].arg2.value);
			}
			else if (for_money[temp_index].arg1.type == INDEX_TYPE && for_money[temp_index].arg2.type == CONST_TYPE)
			{
				updata_Value(for_money[temp_index].result, findValue_value(for_money[temp_index].arg1.value) * for_money[temp_index].arg2.value);
			}
			else if (for_money[temp_index].arg1.type == CONST_TYPE && for_money[temp_index].arg2.type == INDEX_TYPE)
			{
				updata_Value(for_money[temp_index].result, for_money[temp_index].arg1.value * findValue_value(for_money[temp_index].arg2.value));
			}
			else if (for_money[temp_index].arg1.type == INDEX_TYPE && for_money[temp_index].arg2.type == INDEX_TYPE)
			{
				updata_Value(for_money[temp_index].result, findValue_value(for_money[temp_index].arg1.value) * findValue_value(for_money[temp_index].arg2.value));
			}
		}
		else if(for_money[temp_index].op == ASSIGNMENT)
		{
			if (for_money[temp_index].arg1.type == CONST_TYPE)
			{
				updata_Value(for_money[temp_index].result, for_money[temp_index].arg1.value);
			}
			else if (for_money[temp_index].arg1.type == INDEX_TYPE)
			{
				updata_Value(for_money[temp_index].result, findValue_value(for_money[temp_index].arg1.value));
			}
		}
	}
}

void out_the_value_table()
{
	int finish_the_bianyiyuanli;
	std::cout << "Name  Value" << std::endl;
	for (finish_the_bianyiyuanli = 0; finish_the_bianyiyuanli < value_curror; finish_the_bianyiyuanli++)
	{
		std::cout << value_tables[finish_the_bianyiyuanli].name << "  " << value_tables[finish_the_bianyiyuanli].value << std::endl;
	}
}

void outthe_yuyi_list(word_list *words, int max_len)
{
	int word_cursor = 0;
	int result_value = 0;

	std::cout << std::endl;
	std::cout << "Yuyi_analysis Start:" << std::endl;

	if (TransProgram(words, word_cursor,result_value))
	{
		if (word_cursor == max_len - 1)
		{
			std::cout << std::endl;
			std::cout << "List the For yuan formula:" << std::endl;
			std::cout << std::endl;

			out_put_the_foryuan_and_symbol_table();

			run_the_For_yuan_formula();

			std::cout << std::endl;
			std::cout << "Value tables show after For yuan formula Run:" << std::endl;
			std::cout << std::endl;

			out_the_value_table();

			std::cout << std::endl;
			std::cout << "Your program have been Analyzed." << std::endl;
		}
	}
	else
	{
		tran_run_error(words, tran_error_curror, max_len);
	}
}