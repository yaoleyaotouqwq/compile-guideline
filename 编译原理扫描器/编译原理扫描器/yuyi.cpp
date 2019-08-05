#include "stdafx.h"

int value_curror = 0;

bool tran_error_flag = 0;//����״̬��¼
int tran_error_curror = 0;//��������±��¼

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

//����Ԫʽ��Ż�����outΪ�׵�������ÿ����Ԫʽ��Result���ΪNXQ
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

//���ű���ӱ���
void entry_value(std::string name)
{
	if (Find_value(name) == -1)
	{
		value_tables[value_curror].type = VALUE_TYPE;
		value_tables[value_curror].name = name;
		value_curror++;
	}
}

//�������ʱ��ָ��λ��
void tran_error_curor_save(int word_cursor)
{
	if (tran_error_flag == 0)
	{
		tran_error_curror = word_cursor;
		tran_error_flag = 1;
	}
}

//���������Ϣ
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

//<������>	��	ID|ID,<������>
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
				std::cout << "Error! the mean not <������>��ID | ID, <������>" << std::endl;
				return 0;
			}
		}
		return 1;
	}
	return 2;//����û��ID���ж�Ϊ������Ϊ��
}

//<����˵������>	��	<������>:INTEGER; <����˵�����й���ʽ>|��and <����˵�����й���ʽ> �� ��|<����˵������>
bool TransVariable_explorer_list(word_list *words, int &word_cursor, int &result_value)
{
	++word_cursor;
	int flag_temp = TransVariable_table(words, word_cursor, result_value);

	if (flag_temp == 0)
	{
		tran_error_curor_save(word_cursor);
		std::cout << "Error! the mean not <����˵������>	��	<������>:INTEGER; <����˵�����й���ʽ>|��and <����˵�����й���ʽ> �� ��|<����˵������>" << std::endl;
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
			std::cout << "Error! the mean not <����˵������>	��	<������>:INTEGER; <����˵�����й���ʽ>|��and <����˵�����й���ʽ> �� ��|<����˵������>" << std::endl;
			return 0;
		}
	}

	else//����˵�����п���Ϊ�գ������ֻ�Ǳ�����Ϊ�գ������integer�����򱨴�
	{
		return 1;
	}

}

//<����˵��>��VAR<����˵������>
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
			std::cout << "Error! the mean not <����˵��>��VAR<����˵������>" << std::endl;
			return 0;
		}
	}
	else
	{
		tran_error_curor_save(word_cursor);
		std::cout << "Error! the mean not <����˵��>��VAR<����˵������>" << std::endl;
		return 0;
	}
}

//<��>	��	<��ʽ><���ʽ> and <���ʽ> �� �� | *<��> | /<��>
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
				General_for_money(MULTIPLY, item1, item2, id);//�����Ԫʽ

				type = INDEX_TYPE;
				result_value = id;
				return 1;
			}
			else
			{
				return 0;
			}
		}
		return 1;//����ʽ�Ƶ����յ����
	}
	else
	{
		tran_error_curor_save(word_cursor);
		std::cout << "Error! the mean not  <��>	��	<��ʽ><���ʽ> and <���ʽ> �� �� | *<��> | /<��>" << std::endl;
		return 0;
	}
}

//<�������ʽ>��<��><�������ʽ����ʽ> and <�������ʽ����ʽ> �� �� | +<�������ʽ>
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
				General_for_money(ADD, item1, item2, id);//�����Ԫʽ
				
				type = INDEX_TYPE;
				result_value = id;
				return 1;
			}
			else
			{
				return 0;
			}
			
		}
		return 1;//����ʽ�Ƶ����յ����
	}
	else
	{
		tran_error_curor_save(word_cursor);
		std::cout << "Error! the mean not  <�������ʽ>��<��><�������ʽ����ʽ> and <�������ʽ������> �� �� | +<�������ʽ>" << std::endl;
		return 0;
	}
}

//<��ֵ���> 	��	<����> :=  <�������ʽ>;
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
				std::cout << "Error! the mean not  <��ֵ���> ��<����> : = <�������ʽ>;" << std::endl;
				return 0;
			}

			//item1��ֵ����ʱ������ֵ����ֵ����
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
			std::cout << "Error! the mean not  <��ֵ���> ��<����> : = <�������ʽ>;" << std::endl;
			return 0;
		}

		
	}
	return 2;
}

//<��ϵ���ʽ>��<�������ʽ> <��ϵ��> <�������ʽ>
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
				
				true_out = General_for_money(op, item1, item2, 0);//�����
				false_out = General_for_money(JMP, 0, 0, 0);//�ٳ���
				return 1;
			}
			else
			{
				tran_error_curor_save(word_cursor);
				std::cout << "Error! the mean not  <��ϵ���ʽ>��<�������ʽ> <��ϵ��> <�������ʽ>,the last <�������ʽ> is mistake." << std::endl;
				return 0;
			}
		}
		else
		{
			tran_error_curor_save(word_cursor);
			std::cout << "Error! the mean not  <��ϵ���ʽ>��<�������ʽ> <��ϵ��> <�������ʽ>,<��ϵ��> is mistake." << std::endl;
			return 0;
		}
	}
	else
	{
		std::cout << "Error! the mean not  <��ϵ���ʽ>��<�������ʽ> <��ϵ��> <�������ʽ>" << std::endl;

		return 0;
	}
}

//<�������>	��	IF <��ϵ���ʽ> THEN <���><������乫��ʽ> and <������乫��ʽ> �� �� | ELSE<���>
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
				BackPut(true_out, NXQ);//���������
				if (TransStatement(words, word_cursor, result_value))//����if��Ϊ�����
				{
					++word_cursor;
					if (words[word_cursor].num == ELSE)
					{
						loop = General_for_money(JMP, 0, 0, 0);
						BackPut(false_out,NXQ);//����ٳ���

						++word_cursor;
						if (TransStatement(words, word_cursor, result_value))
						{
							BackPut(loop, NXQ);//�����������
							return 1;
						}
						else
						{
							tran_error_curor_save(word_cursor);
							std::cout << "Error! the mean not <���>��<��ֵ���> | <�������> | <while���> " << std::endl;
							return 0;
						}
					}
					else
					{
						tran_error_curor_save(word_cursor);
						std::cout << "Error! the mean not  <�������>	��	IF <��ϵ���ʽ> THEN <���><������乫��ʽ> and <������乫����> �� �� | ELSE<���>" << std::endl;
						return 0;
					}
				}
				else
				{
					tran_error_curor_save(word_cursor);
					std::cout << "Error! the mean not <���>��<��ֵ���> | <�������> | <while���> " << std::endl;
					return 0;
				}
			}
			else
			{
				tran_error_curor_save(word_cursor);
				std::cout << "Error! the mean not  <�������>	��	IF <��ϵ���ʽ> THEN <���><������乫��ʽ> and <������乫����> �� �� | ELSE<���>" << std::endl;
				return 0;
			}
		}
		else
		{
			tran_error_curor_save(word_cursor);
			std::cout << "Error! the mean not  <�������>	��	IF <��ϵ���ʽ> THEN <���><������乫��ʽ> and <������乫����> �� �� | ELSE<���>" << std::endl;
			return 0;
		}
	}
	return 2;//������乫��ʽΪ�յ����

}

//<while���>��	WHILE <��ϵ���ʽ> DO <���>END;
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
						General_for_money(JMP, 0, 0, loop);//��������ת�ع�ϵ���ʽ
						BackPut(false_out, NXQ);//��д�ٳ���

						return 1;
					}
					else
					{
						tran_error_curor_save(word_cursor);
						std::cout << "Error! the mean not <while���>��	WHILE <��ϵ���ʽ> DO <���>END;" << std::endl;
						return 0;
					}
				}
				else
				{
					tran_error_curor_save(word_cursor);
					std::cout << "Error! the mean not <���>��<��ֵ���> | <�������> | <while���> " << std::endl;
					return 0;
				}
			}
			else
			{
				tran_error_curor_save(word_cursor);
				std::cout << "Error! the mean not <while���>	��	WHILE <��ϵ���ʽ> DO <���>END;" << std::endl;
				return 0;
			}
		}
		else
		{
			tran_error_curor_save(word_cursor);
			std::cout << "Error! the mean not <while���>	��	WHILE <��ϵ���ʽ> DO <���>END;" << std::endl;
			return 0;
		}
	}
	return 2;
}

//<���>	��	<��ֵ���>|<�������>|<while���>
int TransStatement(word_list *words, int &word_cursor, int &result_value)
{
	int flag_setvalue = TransSetvalue_statement(words, word_cursor,result_value);
	int flag_condition = TransCondition_statement(words, word_cursor, result_value);
	int flag_while = TransWhile_statement(words, word_cursor, result_value);

	if (flag_setvalue == 0)
	{
		std::cout << "Error! the mean not  <���>	��	<��ֵ���>|<�������>|<while���>" << std::endl;
		return 0;
	}
	else if (flag_setvalue == 1)
	{
		return 1;
	}

	if (flag_condition == 0)
	{
		std::cout << "Error! the mean not  <���>	��	<��ֵ���>|<�������>|<while���>" << std::endl;
		return 0;
	}
	else if (flag_condition == 1)
	{
		return 1;
	}

	if (flag_while == 0)
	{
		std::cout << "Error! the mean not  <���>	��	<��ֵ���>|<�������>|<while���>" << std::endl;
		return 0;
	}
	else if (flag_while == 1)
	{
		return 1;
	}

	return 2;//�����������Ϊ��

}

//<����>��<���><��乫��ʽ>|��  and  <��乫��ʽ> �� �� | <����>
bool TransStatement_table(word_list *words, int &word_cursor, int &result_value)
{
	++word_cursor;
	int flag_temp = TransStatement(words, word_cursor, result_value);

	if (flag_temp == 0)
	{
		tran_error_curor_save(word_cursor);
		std::cout << "Error! the mean not   <����>��<���><��乫����>|��  and  <��乫����> �� �� | <����>" << std::endl;
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
	else//������û�е�����͹���ʽ�Ƶ����յ����
	{
		return 1;
	}
}

//<����>��<����˵��>BEGIN<����>END
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
					std::cout << "Error! the mean not  <����>��<����˵��>BEGIN<����>END." << std::endl;
					return 0;
				}
			}
			else
			{
				std::cout << "Error! the mean not  <����>��<����˵��>BEGIN<����>END." << std::endl;
				return 0;
			}

		}
		else
		{
			tran_error_curor_save(word_cursor);
			std::cout << "Error! the mean not  <����>��<����˵��>BEGIN<����>END." << std::endl;
			return 0;
		}
	}
	else
	{
		std::cout << "Error! the mean not  <����>��<����˵��>BEGIN<����>END." << std::endl;
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
				//�������ʽΪ�棬ͬ��
				if (Bool_result(for_money[temp_index].op,for_money[temp_index].arg1.value, for_money[temp_index].arg2.value))
				{
					temp_index = for_money[temp_index].result;
					temp_index--;//ѭ��������һ;
				}
				else
				{
					++temp_index;//����ٳ���
					temp_index = for_money[temp_index].result;
					temp_index--;//ѭ��������һ;
				}
			}
			else if (for_money[temp_index].arg1.type == INDEX_TYPE && for_money[temp_index].arg2.type == CONST_TYPE)
			{
				if (Bool_result(for_money[temp_index].op, findValue_value(for_money[temp_index].arg1.value), for_money[temp_index].arg2.value))
				{
					temp_index = for_money[temp_index].result;
					temp_index--;//ѭ��������һ;
				}
				else
				{
					++temp_index;//����ٳ���
					temp_index = for_money[temp_index].result;
					temp_index--;//ѭ��������һ;
				}
			}
			else if (for_money[temp_index].arg1.type == CONST_TYPE && for_money[temp_index].arg2.type == INDEX_TYPE)
			{
				if (Bool_result(for_money[temp_index].op, for_money[temp_index].arg1.value, findValue_value(for_money[temp_index].arg2.value)))
				{
					temp_index = for_money[temp_index].result;
					temp_index--;//ѭ��������һ;
				}
				else
				{
					++temp_index;//����ٳ���
					temp_index = for_money[temp_index].result;
					temp_index--;//ѭ��������һ;
				}
			}
			else if (for_money[temp_index].arg1.type == INDEX_TYPE && for_money[temp_index].arg2.type == INDEX_TYPE)
			{
				if (Bool_result(for_money[temp_index].op, findValue_value(for_money[temp_index].arg1.value), findValue_value(for_money[temp_index].arg2.value)))
				{
					temp_index = for_money[temp_index].result;
					temp_index--;//ѭ��������һ;
				}
				else
				{
					++temp_index;//����ٳ���
					temp_index = for_money[temp_index].result;
					temp_index--;//ѭ��������һ;
				}
			}
		}
		else if (for_money[temp_index].op == JMP)//��������ת
		{
			temp_index = for_money[temp_index].result;
			temp_index--;//ѭ��������һ;
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