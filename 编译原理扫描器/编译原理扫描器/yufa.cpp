#include "stdafx.h"

bool error_flag = 0;//����״̬��¼
int error_curror = 0;//��������±��¼

int isStatement(word_list *words, int &word_cursor);
bool isStatement_table(word_list *words, int &word_cursor);

//�������ʱ��ָ��λ��
void error_curor_save(int word_cursor)
{
	if (error_flag == 0)
	{
		error_curror = word_cursor;
		error_flag = 1;
	}
}

void run_error(word_list *words,int word_cursor,int max_len)
{
	if (word_cursor > 0 && word_cursor < max_len - 1 )
	{
		std::cout << "Error rows content:" <<
			words[word_cursor - 1].type << " " <<
			words[word_cursor].type << " " <<
			words[word_cursor + 1].type << std::endl;
	}
	else if(word_cursor == 0)
	{
		std::cout << "Error rows content:" <<
			words[word_cursor].type << " " <<
			words[word_cursor+1].type << " " <<
			words[word_cursor+2].type << std::endl;
	}
	else
	{
		std::cout << "Error rows content:" <<
			words[word_cursor-2].type << " " <<
			words[word_cursor-1].type << " " <<
			words[word_cursor].type << std::endl;
	}

	std::cout << "Please check your code!" << word_cursor+1 <<std::endl;
	std::cin.get();
	exit(0);
}

//<������>	��	ID|ID,<������>
int isVariable_table(word_list *words, int &word_cursor)
{
	if (words[word_cursor].num == ID)
	{
		++word_cursor;
		if (words[word_cursor].num == BEAN )
		{
			++word_cursor;
			if (isVariable_table(words, word_cursor) == 1)
			{
				return 1;
			}
			else
			{
				error_curor_save(word_cursor);
				std::cout << "Error! the mean not <������>��ID | ID, <������>" << std::endl; 
				return 0;
			}
		}
		return 1;
	}
	return 2;//����û��ID���ж�Ϊ������Ϊ��
}

//<����˵������>	��	<������>:INTEGER; <����˵�����й���ʽ>|��and <����˵�����й���ʽ> �� ��|<����˵������>
bool isVariable_explorer_list(word_list *words,int &word_cursor)
{
	++word_cursor;
	int flag_temp = isVariable_table(words, word_cursor);

	if (flag_temp == 0)
	{
		error_curor_save(word_cursor);
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
				if (isVariable_explorer_list(words, word_cursor))
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
			error_curor_save(word_cursor);
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
bool isVariable_explorer(word_list *words,int &word_cursor)
{
	if (words[word_cursor].num == VAR )
	{
		if (isVariable_explorer_list(words, word_cursor))
		{
			return 1;
		}
		else
		{
			error_curor_save(word_cursor);
			std::cout << "Error! the mean not <����˵��>��VAR<����˵������>" << std::endl;
			return 0;
		}
	}
	else
	{
		error_curor_save(word_cursor);
		std::cout << "Error! the mean not <����˵��>��VAR<����˵������>" << std::endl;
		return 0;
	}
}

//<��>	��	<��ʽ><���ʽ> and <���ʽ> �� �� | *<��> | /<��>
bool isItem(word_list *words, int &word_cursor)
{
	if (words[word_cursor].num == INT || words[word_cursor].num == ID)
	{
		++word_cursor;
		if (words[word_cursor].num == MULTIPLY)
		{
			++word_cursor;
			if (isItem(words, word_cursor))
			{
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
		error_curor_save(word_cursor);
		std::cout << "Error! the mean not  <��>	��	<��ʽ><���ʽ> and <���ʽ> �� �� | *<��> | /<��>" << std::endl;
		return 0;
	}
}

//<�������ʽ>��<��><�������ʽ����ʽ> and <�������ʽ����ʽ> �� �� | +<�������ʽ>
bool isCount(word_list *words, int &word_cursor)
{
	if (isItem(words, word_cursor))
	{
		if (words[word_cursor].num == ADD )
		{
			++word_cursor;
			if (isCount(words, word_cursor))
			{
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
		error_curor_save(word_cursor);
		std::cout << "Error! the mean not  <�������ʽ>��<��><�������ʽ����ʽ> and <�������ʽ������> �� �� | +<�������ʽ>" << std::endl;
		return 0;
	}
}

//<��ֵ���> 	��	<����> :=  <�������ʽ>;
int isSetvalue_statement(word_list *words, int &word_cursor)
{
	if (words[word_cursor].num == ID )
	{
		++word_cursor;
		if (words[word_cursor].num == ASSIGNMENT)
		{
			++word_cursor;
			if (!isCount(words, word_cursor) || words[word_cursor].num != SPLIT)
			{
				error_curor_save(word_cursor);
				std::cout << "Error! the mean not  <��ֵ���> ��<����> : = <�������ʽ>;" << std::endl;
				return 0;
			}
			return 1;
		}
		else
		{
			error_curor_save(word_cursor);
			std::cout << "Error! the mean not  <��ֵ���> ��<����> : = <�������ʽ>;" << std::endl; 
			return 0;
		}
	}
	return 2;
}

//<��ϵ���ʽ>��<�������ʽ> <��ϵ��> <�������ʽ>
bool isRelation(word_list *words, int &word_cursor)
{
	if (isCount(words,word_cursor))
	{
		if (words[word_cursor].num == SMALLER || words[word_cursor].num == BIGER || words[word_cursor].num == SMALLER_EQUAL || words[word_cursor].num == BIGER_EQUAL || words[word_cursor].num == NOT_EQUAL)
		{
			++word_cursor;
			if (isCount(words, word_cursor))
			{
				return 1;
			}
			else
			{
				error_curor_save(word_cursor);
				std::cout << "Error! the mean not  <��ϵ���ʽ>��<�������ʽ> <��ϵ��> <�������ʽ>,the last <�������ʽ> is mistake." << std::endl;
				return 0;
			}
		}
		else
		{
			error_curor_save(word_cursor);
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
int isCondition_statement(word_list *words, int &word_cursor)
{
	if (words[word_cursor].num == IF)
	{
		++word_cursor;
		if (isRelation(words, word_cursor))
		{
			if (words[word_cursor].num == THEN)
			{	
				++word_cursor;
				if (isStatement(words, word_cursor))//����if��Ϊ�����
				{
					++word_cursor;
					if (words[word_cursor].num == ELSE)
					{
						++word_cursor;
						if (isStatement(words, word_cursor))
						{
							return 1;
						}
						else
						{
							error_curor_save(word_cursor);
							std::cout << "Error! the mean not <���>��<��ֵ���> | <�������> | <while���> " << std::endl;
							return 0;
						}
					}
					else
					{
						error_curor_save(word_cursor);
						std::cout << "Error! the mean not  <�������>	��	IF <��ϵ���ʽ> THEN <���><������乫��ʽ> and <������乫����> �� �� | ELSE<���>" << std::endl;
						return 0;
					}
				}
				else
				{
					error_curor_save(word_cursor);
					std::cout << "Error! the mean not <���>��<��ֵ���> | <�������> | <while���> "<<std::endl;
					return 0;
				}
			}
			else
			{
				error_curor_save(word_cursor);
				std::cout << "Error! the mean not  <�������>	��	IF <��ϵ���ʽ> THEN <���><������乫��ʽ> and <������乫����> �� �� | ELSE<���>" << std::endl;
				return 0;
			}
		}
		else
		{
			error_curor_save(word_cursor);
			std::cout << "Error! the mean not  <�������>	��	IF <��ϵ���ʽ> THEN <���><������乫��ʽ> and <������乫����> �� �� | ELSE<���>" << std::endl;
			return 0;
		}
	}
	return 2;//������乫��ʽΪ�յ����
	
}

//<while���>��	WHILE <��ϵ���ʽ> DO <���>END;
int isWhile_statement(word_list *words, int &word_cursor)
{
	if (words[word_cursor].num == WHILE)
	{
		++word_cursor;
		if (isRelation(words, word_cursor))
		{
			if (words[word_cursor].num == DO)
			{
				if (isStatement_table(words, word_cursor))
				{
					if (words[word_cursor].num == END && words[word_cursor+1].num == SPLIT)
					{
						++word_cursor;
						return 1;
					}
					else
					{
						error_curor_save(word_cursor);
						std::cout << "Error! the mean not <while���>��	WHILE <��ϵ���ʽ> DO <���>END;" << std::endl;
						return 0;
					}
				}
				else
				{
					error_curor_save(word_cursor);
					std::cout << "Error! the mean not <���>��<��ֵ���> | <�������> | <while���> " << std::endl;
					return 0;
				}
			}
			else
			{
				error_curor_save(word_cursor);
				std::cout << "Error! the mean not <while���>	��	WHILE <��ϵ���ʽ> DO <���>END;" << std::endl;
				return 0;
			}
		}
		else
		{
			error_curor_save(word_cursor);
			std::cout << "Error! the mean not <while���>	��	WHILE <��ϵ���ʽ> DO <���>END;" << std::endl;
			return 0;
		}
	}
	return 2;
}

//<���>	��	<��ֵ���>|<�������>|<while���>
int isStatement(word_list *words, int &word_cursor)
{
	int flag_setvalue = isSetvalue_statement(words, word_cursor);
	int flag_condition = isCondition_statement(words, word_cursor);
	int flag_while = isWhile_statement(words, word_cursor);

	if (flag_setvalue == 0)
	{
		std::cout << "Error! the mean not  <���>	��	<��ֵ���>|<�������>|<while���>" << std::endl;
		return 0;
	}
	else if(flag_setvalue == 1)
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
bool isStatement_table(word_list *words, int &word_cursor)
{
	++word_cursor;
	int flag_temp = isStatement(words, word_cursor);

	if (flag_temp == 0)
	{
		error_curor_save(word_cursor);
		std::cout << "Error! the mean not   <����>��<���><��乫����>|��  and  <��乫����> �� �� | <����>" << std::endl;
		return 0;
	}
	else if (flag_temp == 1)
	{

		if (isStatement_table(words, word_cursor))
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
bool isProgram(word_list *words, int &word_cursor)
{
	if (isVariable_explorer(words, word_cursor))
	{
		if (words[word_cursor].num == BEGIN )
		{
			if (isStatement_table(words, word_cursor))
			{
				if (words[word_cursor].num == END && words[word_cursor + 1].num == POINT)
				{
					++word_cursor;
					return 1;
				}
				else
				{
					error_curor_save(word_cursor);
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
			error_curor_save(word_cursor);
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

void outthe_yufa_list(word_list *words,int max_len)
{
	int word_cursor=0;
	std::cout << "Yufa_analysis Start:" << std::endl;
	if (isProgram(words, word_cursor))
	{
		if (word_cursor == max_len - 1)
		{
			std::cout << std::endl;
			std::cout << "Result is 0 Error 0 Warning in your program." << std::endl;
		}
	}
	else
	{
		run_error(words, error_curror, max_len);
	}
}