#include "stdafx.h"

bool error_flag = 0;//出错状态记录
int error_curror = 0;//保存出错下标记录

int isStatement(word_list *words, int &word_cursor);
bool isStatement_table(word_list *words, int &word_cursor);

//保存出错时的指针位置
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

//<变量表>	→	ID|ID,<变量表>
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
				std::cout << "Error! the mean not <变量表>→ID | ID, <变量表>" << std::endl; 
				return 0;
			}
		}
		return 1;
	}
	return 2;//可以没有ID即判断为变量表为空
}

//<变量说明序列>	→	<变量表>:INTEGER; <变量说明序列公因式>|εand <变量说明序列公因式> → ε|<变量说明序列>
bool isVariable_explorer_list(word_list *words,int &word_cursor)
{
	++word_cursor;
	int flag_temp = isVariable_table(words, word_cursor);

	if (flag_temp == 0)
	{
		error_curor_save(word_cursor);
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
			std::cout << "Error! the mean not <变量说明>→VAR<变量说明序列>" << std::endl;
			return 0;
		}
	}
	else
	{
		error_curor_save(word_cursor);
		std::cout << "Error! the mean not <变量说明>→VAR<变量说明序列>" << std::endl;
		return 0;
	}
}

//<项>	→	<因式><项公因式> and <项公因式> → ε | *<项> | /<项>
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
		return 1;//公因式推导出空的情况
	}
	else
	{
		error_curor_save(word_cursor);
		std::cout << "Error! the mean not  <项>	→	<因式><项公因式> and <项公因式> → ε | *<项> | /<项>" << std::endl;
		return 0;
	}
}

//<算术表达式>→<项><算术表达式公因式> and <算术表达式公因式> → ε | +<算术表达式>
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
		return 1;//公因式推导出空的情况
	}
	else
	{
		error_curor_save(word_cursor);
		std::cout << "Error! the mean not  <算术表达式>→<项><算术表达式公因式> and <算术表达式公因子> → ε | +<算术表达式>" << std::endl;
		return 0;
	}
}

//<赋值语句> 	→	<变量> :=  <算术表达式>;
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
				std::cout << "Error! the mean not  <赋值语句> →<变量> : = <算术表达式>;" << std::endl;
				return 0;
			}
			return 1;
		}
		else
		{
			error_curor_save(word_cursor);
			std::cout << "Error! the mean not  <赋值语句> →<变量> : = <算术表达式>;" << std::endl; 
			return 0;
		}
	}
	return 2;
}

//<关系表达式>→<算数表达式> <关系符> <算数表达式>
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
				std::cout << "Error! the mean not  <关系表达式>→<算数表达式> <关系符> <算数表达式>,the last <算数表达式> is mistake." << std::endl;
				return 0;
			}
		}
		else
		{
			error_curor_save(word_cursor);
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
				if (isStatement(words, word_cursor))//允许if下为空语句
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
							std::cout << "Error! the mean not <语句>→<赋值语句> | <条件语句> | <while语句> " << std::endl;
							return 0;
						}
					}
					else
					{
						error_curor_save(word_cursor);
						std::cout << "Error! the mean not  <条件语句>	→	IF <关系表达式> THEN <语句><条件语句公因式> and <条件语句公因子> → ε | ELSE<语句>" << std::endl;
						return 0;
					}
				}
				else
				{
					error_curor_save(word_cursor);
					std::cout << "Error! the mean not <语句>→<赋值语句> | <条件语句> | <while语句> "<<std::endl;
					return 0;
				}
			}
			else
			{
				error_curor_save(word_cursor);
				std::cout << "Error! the mean not  <条件语句>	→	IF <关系表达式> THEN <语句><条件语句公因式> and <条件语句公因子> → ε | ELSE<语句>" << std::endl;
				return 0;
			}
		}
		else
		{
			error_curor_save(word_cursor);
			std::cout << "Error! the mean not  <条件语句>	→	IF <关系表达式> THEN <语句><条件语句公因式> and <条件语句公因子> → ε | ELSE<语句>" << std::endl;
			return 0;
		}
	}
	return 2;//条件语句公因式为空的情况
	
}

//<while语句>→	WHILE <关系表达式> DO <语句>END;
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
						std::cout << "Error! the mean not <while语句>→	WHILE <关系表达式> DO <语句>END;" << std::endl;
						return 0;
					}
				}
				else
				{
					error_curor_save(word_cursor);
					std::cout << "Error! the mean not <语句>→<赋值语句> | <条件语句> | <while语句> " << std::endl;
					return 0;
				}
			}
			else
			{
				error_curor_save(word_cursor);
				std::cout << "Error! the mean not <while语句>	→	WHILE <关系表达式> DO <语句>END;" << std::endl;
				return 0;
			}
		}
		else
		{
			error_curor_save(word_cursor);
			std::cout << "Error! the mean not <while语句>	→	WHILE <关系表达式> DO <语句>END;" << std::endl;
			return 0;
		}
	}
	return 2;
}

//<语句>	→	<赋值语句>|<条件语句>|<while语句>
int isStatement(word_list *words, int &word_cursor)
{
	int flag_setvalue = isSetvalue_statement(words, word_cursor);
	int flag_condition = isCondition_statement(words, word_cursor);
	int flag_while = isWhile_statement(words, word_cursor);

	if (flag_setvalue == 0)
	{
		std::cout << "Error! the mean not  <语句>	→	<赋值语句>|<条件语句>|<while语句>" << std::endl;
		return 0;
	}
	else if(flag_setvalue == 1)
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
bool isStatement_table(word_list *words, int &word_cursor)
{
	++word_cursor;
	int flag_temp = isStatement(words, word_cursor);

	if (flag_temp == 0)
	{
		error_curor_save(word_cursor);
		std::cout << "Error! the mean not   <语句表>→<语句><语句公因子>|ε  and  <语句公因子> → ε | <语句表>" << std::endl;
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
	else//语句可以没有的情况和公因式推导出空的情况
	{
		return 1;
	}
}

//<程序>→<变量说明>BEGIN<语句表>END
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
			error_curor_save(word_cursor);
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