// 编译原理扫描器.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

int rows = 1;
char token[30];
char nullarray[30];

int Max_len = 10;

int lookup(char *arrays)
{
	int c;
	if (!strcmp(arrays,"begin"))
	{
		c = 1;
	}
	else if (!strcmp(arrays, "end"))
	{
		c = 2;
	}
	else if (!strcmp(arrays, "var"))
	{
		c = 3;
	}
	else if (!strcmp(arrays, "integer"))
	{
		c = 4;
	}
	else if (!strcmp(arrays, "while"))
	{
		c = 5;
	}
	else if (!strcmp(arrays, "do"))
	{
		c = 6;
	}
	else if (!strcmp(arrays, "then"))
	{
		c = 7;
	}
	else if (!strcmp(arrays, "if"))
	{
		c = 8;
	}
	else if (!strcmp(arrays, "else"))
	{
		c = 9;
	}
	else
	{
		c = 0;
	}

	return c;
}

std::string relook_up(int c)
{
	char ch = c + '0';
	switch (ch)
	{
	case '1':
		return "begin";
	case '2':
		return "end";
	case '3':
		return"var";
	case '4':
		return"integer";
	case '5':
		return"while";
	case '6':
		return"do";
	case '7':
		return"then";
	case '8':
		return"if";
	case '9':
		return"else";
	default:
		return "error";
	}
}

void printout(int flag,char *strings,std::string name)
{
	char str[20];
	sprintf(str, "%d", flag);
	int temp,maxlen = strlen(str);
	for (temp = 0; temp < maxlen; ++temp)
	{
		strings[temp] = str[temp];
	}
	strings[temp] = '\0';
	std::cout << strings << " " << name << std::endl;
}

void call_error(FILE *fpoint,char ch)
{
	char temp_array[100];
	if (ch == '\n')
	{
		rows++;
		if (getc(fpoint) != EOF)
		{
			fseek(fpoint, -1, 1);
			std::cout << "the No " << rows << "rows:" << std::endl;
		}
		else
		{
			std::cout << "The program is End";
		}
	}

	if ( ch != ' ' && ch != '\n' && ch != '\t' )
	{
		if (ch == '/' && fgetc(fpoint) == '/')
		{
			fgets(temp_array, 100, fpoint);
			rows++;
			std::cout << "their is notes" << std::endl;
			std::cout << "the No " << rows << "rows:" << std::endl;
	
			return;
		}
		else if (ch == '{')
		{
			while ((ch = fgetc(fpoint)) != EOF)
			{
				if (ch == '\n')
				{
					rows++;
					if (getc(fpoint) != EOF)
					{
						fseek(fpoint, -1, 1);
						std::cout << "their is notes" << std::endl;
						std::cout << "the No " << rows << "rows:" << std::endl;
					}
					else
					{
						std::cout << "The program is End";
						std::cout << "Raise Error,please concert" << std::endl;
						std::cin.get();
						exit(0);
					}
				}
				else if (ch == '}')
				{
					std::cout << "their notes is end" << std::endl;
					return;
				}
			}
			std::cout << "The program is End" << std::endl;
			std::cout << "Raise Error,please concert" << std::endl;
			std::cin.get();
			exit(0);
		}
		else
		{
			std::cout << "Raise Error,please concert" << std::endl;
			exit(0);
		}
	}
}

int is_zh_ch(char p)
{

	if (~(p >> 8) == 0)  //将p字节进行移位运算，右移8位，这样，如果移位后是0，则说明原来的字节最高位为0，不是1那么也就不是汉字的一个字节。
	{
		return 0;  //代表不是汉字
	}

	return -1;
}

void scanner(word_list *words,int &max_len,FILE *fpoint)
{
	char ch;
	int i, c;
	int character_len = 0;

	ch = fgetc(fpoint);
	
	if (!is_zh_ch(ch))
	{
		std::cout << "Raise Error,please concert" << std::endl;
		std::cin.get();
		exit(0);
	}

	if (isalpha(ch))
	{
		token[0] = ch;
		ch = fgetc(fpoint);

		i = 1;
		character_len++;
		while (isalnum(ch))
		{
			token[i] = ch;
			i++;
			ch = fgetc(fpoint);
			character_len++;
			if (character_len > Max_len)
			{
				std::cout << "Raise Error,please concert" << std::endl;
				std::cin.get();
				exit(0);
			}
		}
		token[i] = '\0';
		fseek(fpoint, -1, 1);//回退一个字符
		c = lookup(token);
		if (c == 0)
		{
			words[max_len].type = token;
			printout(ID, token, words[max_len].type);
			words[max_len].num = ID;
			
			++max_len;
		}
		else
		{
			words[max_len].type = relook_up(c);
			printout(c, nullarray, words[max_len].type);
			words[max_len].num = c;
			++max_len;
		}
	}
	else
	{
		if (isdigit(ch))
		{
			token[0] = ch;
			ch = fgetc(fpoint);

			i = 1;
			character_len++;

			if (isalpha(ch))
			{
				std::cout << "Raise Error,please concert" << std::endl;
				std::cin.get();
				exit(0);
			}
			while (isdigit(ch))
			{
				token[i] = ch;
				i++;
				ch = fgetc(fpoint);
				if (character_len > Max_len)
				{
					std::cout << "Raise Error,please concert" << std::endl;
					std::cin.get();
					exit(0);
				}
			}
			token[i] = '\0';
			fseek(fpoint, -1, 1);

			words[max_len].type = token;
			printout(INT, token, words[max_len].type);
			words[max_len].num = INT;
			++max_len;
		}
		else
		{
			switch (ch)
			{
			case '<':
				ch = fgetc(fpoint);
				if (ch == '=')
				{
					words[max_len].type = "<=";
					printout(SMALLER_EQUAL, nullarray, words[max_len].type);
					words[max_len].num = SMALLER_EQUAL;
					++max_len;
				}
				else if (ch == '>')
				{
					words[max_len].type = "!=";
					printout(NOT_EQUAL, nullarray, words[max_len].type);
					words[max_len].num = NOT_EQUAL;
					++max_len;
				}
				else
				{
					fseek(fpoint, -1, 1);
					words[max_len].type = "<";
					printout(SMALLER, nullarray, words[max_len].type);
					words[max_len].num = SMALLER;
					++max_len;
				}
				break;
			case '=':
				words[max_len].type = "=";
				printout(EQUAL, nullarray, words[max_len].type);
				words[max_len].num = EQUAL;
				++max_len;
				break;
			case '>':
				ch = fgetc(fpoint);
				if (ch == '=')
				{
					words[max_len].type = ">=";
					printout(BIGER_EQUAL, nullarray, words[max_len].type);
					words[max_len].num = BIGER_EQUAL;
					++max_len;
				}
				else
				{
					fseek(fpoint, -1, 1);
					words[max_len].type = ">";
					printout(BIGER, nullarray, words[max_len].type);
					words[max_len].num = BIGER;
					++max_len;
				}
				break;
			case ':':
				ch = fgetc(fpoint);
				if (ch == '=')
				{
					words[max_len].type = ":=";
					printout(ASSIGNMENT, nullarray, words[max_len].type);
					words[max_len].num = ASSIGNMENT;
					++max_len;
				}
				else
				{
					fseek(fpoint, -1, 1);
					words[max_len].type = ":";
					printout(BUBBLE, nullarray, words[max_len].type);
					words[max_len].num = BUBBLE;
					++max_len;
				}
				break;
			case '+':
				words[max_len].type = "+";
				printout(ADD, nullarray, words[max_len].type);
				words[max_len].num = ADD;
				++max_len;
				break;
			case '*':
				words[max_len].type = "*";
				printout(MULTIPLY, nullarray, words[max_len].type);
				words[max_len].num = MULTIPLY;
				++max_len;
				break;
			case '(':
				words[max_len].type = "(";
				printout(LEFT_BRACKET, nullarray, words[max_len].type);
				words[max_len].num = LEFT_BRACKET;
				++max_len;
				break;
			case ')':
				words[max_len].type = ")";
				printout(RIGHT_BRACKET, nullarray, words[max_len].type);
				words[max_len].num = RIGHT_BRACKET;
				++max_len;
				break;
			case '.':
				words[max_len].type = ".";
				printout(POINT, nullarray, words[max_len].type);
				words[max_len].num = POINT;
				++max_len;
				break;
			case ',':
				words[max_len].type = ",";
				printout(BEAN, nullarray, words[max_len].type);
				words[max_len].num = BEAN;
				++max_len;
				break;
			case ';':
				words[max_len].type = ";";
				printout(SPLIT, nullarray, words[max_len].type);
				words[max_len].num = SPLIT;
				++max_len;
				break;
			default:call_error(fpoint,ch);
				break;
			}

			return;
		}
	}
}

int outthe_cifa_list(word_list *words)
{
	FILE *fpoint;
	int max_len = 0;
	std::cout << "Cifa_analysis Start at NO " << rows << "rows:" << std::endl;
	if (fpoint = fopen("代码.txt", "rt"))
	{
		while (fgetc(fpoint) != EOF)
		{
			fseek(fpoint, -1, 1);
			scanner(words,max_len,fpoint);
			
			std::cout << std::endl;
		}
		fclose(fpoint);
	}
	return max_len;
}

