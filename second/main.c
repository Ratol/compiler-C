#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
void compoundstatement();
void statement();
void expression();
void error(int x);
int search();
int search_();
int FLAG;
int main_flag = 0;
int return_=0;
enum {
	ifSY = 0, elseSY, forSY, whileSY, constSY, intSY, charSY, floatSY,
	returnSY, scanfSY, printfSY, voidSY, mainSY, IDSY,
	plusSY, minusSY, starSY, divSY,
	commaSY, semiSY, LPSY, RPSY, LbSY, RbSY, LBSY, RBSY,
	charC, stringC,
	integer, real,
	equ_SY, equSY, leqSY, lssSY, geqSY, gtrSY, neqSY
};
char *word[] =
{
	"ifSY","elseSY","forSY","whileSY","constSY","intSY","charSY","floatSY",
	"returnSY","scanfSY","printfSY","voidSY","mainSY","IDSY",
	"plusSY","minusSY","starSY","divSY",
	"commaSY","semiSY","LPSY","RPSY","LbSY","RbSY","LBSY","RBSY",
	"charC","stringC",
	"integer","real",
	"equ_SY","equSY","leqSY","lssSY","geqSY","gtrSY","neqSY"
};
typedef struct Tab
{
	char name[100];
	int object;
	char type[10];
	int decline;	//声明行
	int position;	//函数作用域
	int adr;	//相对地址，值
	int Para;	//形参数
	char el_type[10];	//数组元素类型
	int el_top;	//数组上界
	int func_size;	//	函数变量数量->函数大小
	int func_code_line;	//函数p-code代码起始行
}tab_;

typedef struct expression_
{
	int expression_type;		//char类型=0,int类型=1,float类型=2.
	char expression_Type[10];
}Expression_;

Expression_ Expression, Term, Factor, set;
typedef struct code_
{
	char order[5];
	int f;
	int x;
	int y;
}Code_;
Code_ Code[5000];

tab_ tab[1000];

int temp;	//	记录 函数名登录符号表时，此时符号表的下标，之后的操作需要用到它
			//	函数名被引用时，查找得到的下标。
char id[200];
int obj;
char typ[10];
int pos = 0;
int AD;
int ad = 3;
int Para_;
int location;
int cc = 0;    //所读字符的位置
int ll = 0;    //所读代码行的长度
int lnum = 0;    //所读代码所在行数
int Lmax = 500;
int flag = 1;
int tnum = 0;	//符号表下标
int rnum = 0;	//实常量表的下标

double rconst[100];
char c;    //当前所读的字符
char token[1000];	//全局变量，字符数组，存放单词的字符串。
char line[500];    //代码内容
char symbol[20];
int adr = 0;
int main_lc = 2;
int Var_num = 0;
char *pcode[] =
{
	"LDA","LOD","LDI","DIS","\0","\0","\0","\0",
	"FCT","INT","JMP","JPC","SWT","CAS","F1U","F2U",
	"F1D","F2D","MKS","CAL","IDX","IXX","LDB","CPB",
	"LDC","LDR","FLT","RED","WRS","WRW","WRU","HLT",
	"EXP","EXF","LDT","NOT","MUS","WRR","STO","EQR",
	"NER","LSR","LER","GTR","GER","EQL","NEQ","LSS",
	"LER","GRT","GEQ","ORR","ADD","SUB","ADR","SUR",
	"AND","MUL","DIV","MOD","MUR","DIR","RDL","WRL"
};
double S[5000];
int etp = 3;
int abp = 0;

typedef struct stab_
{
	char string_[1000];
	int snum_;
}Stab_;
Stab_ stab[100];
int snum = 0;
int lc = 2;
int code_lc = 0;
int etp_flag[100];
int etp_num = -1;
int return_flag[100] = { 0 };	//有返回值的时候该值为1，表示在运行栈里S[etp-2]=返回值，退栈时etp-2,否则etp-3
int return_num = 0;
FILE *fp1;
FILE *fp2;
int isSpace(char x)
{
	if (x == ' ')
		return 1;
	else
		return 0;
}
int isNewline(char x)
{
	if (x == '\n')
		return 1;
	else
		return 0;
}
int isTab(char x)
{
	if (x == '\t')
		return 1;
	else
		return 0;
}
int isLetter(char x)
{
	if (x <= 'z'&&x >= 'a' || x <= 'Z' && x >= 'A')
		return 1;
	else
		return 0;
}
int isNumber(char x)
{
	if (x <= '9' && x >= '0')
		return 1;
	else
		return 0;
}
int isUnderline(char x)
{
	if (x == '_')
		return 1;
	else
		return 0;
}
int isReserved(char x[])
{
	if (!strcmp(x, "if"))
		return 0;
	else if (!strcmp(x, "else"))
		return 1;
	else if (!strcmp(x, "for"))
		return 2;
	else if (!strcmp(x, "while"))
		return 3;
	else if (!strcmp(x, "const"))
		return 4;
	else if (!strcmp(x, "int"))
		return 5;
	else if (!strcmp(x, "char"))
		return 6;
	else if (!strcmp(x, "float"))
		return 7;
	else if (!strcmp(x, "return"))
		return  8;
	else if (!strcmp(x, "scanf"))
		return 9;
	else if (!strcmp(x, "printf"))
		return 10;
	else if (!strcmp(x, "void"))
		return 11;
	else if (!strcmp(x, "main"))
		return 12;
	else
		return 13;
}
int isEqu(char x)
{
	if (x == '=')
		return 1;
	else
		return 0;
}
int isPlus(char x)
{
	if (x == '+')
		return 1;
	else
		return 0;
}
int isMinus(char x)
{
	if (x == '-')
		return 1;
	else
		return 0;
}
int isStar(char x)
{
	if (x == '*')
		return 1;
	else
		return 0;
}
int isDiv(char x)
{
	if (x == '/')
		return 1;
	else
		return 0;
}
int isComma(char x)
{
	if (x == ',')
		return 1;
	else
		return 0;
}
int isSemi(char x)
{
	if (x == ';')
		return 1;
	else
		return 0;
}
int isLP(char x)
{
	if (x == '(')
		return 1;
	else
		return 0;
}
int isRP(char x)
{
	if (x == ')')
		return 1;
	else
		return 0;
}
int isLb(char x)
{
	if (x == '[')
		return 1;
	else
		return 0;
}
int isRb(char x)
{
	if (x == ']')
		return 1;
	else
		return 0;
}
int isLB(char x)
{
	if (x == '{')
		return 1;
	else
		return 0;
}
int isRB(char x)
{
	if (x == '}')
		return 1;
	else
		return 0;
}
int islss(char x)
{
	if (x == '<')
		return 1;
	else
		return 0;
}
int isgtr(char x)
{
	if (x == '>')
		return 1;
	else
		return 0;
}
int isneq(char x)
{
	if (x == '!')
		return 1;
	else
		return 0;
}
int isLL(char x)
{
	if (x == '"')
		return 1;
	else
		return 0;
}
int isL(char x)
{
	if (x == '\'')
		return 1;
	else
		return 0;
}
void getch()
{
	int i;
	if (cc == ll || c == '\n')    //如果字符读到行末
	{
		for (i = 0;i<Lmax;i++)    //将代码行（缓冲区）初始化
			line[i] = '\0';
		if (fgets(line, Lmax, fp1) != NULL)    // 读取下一行
		{
			cc = 0;
			ll = strlen(line);
			c = line[cc++];    //c是当前新读代码行的第一个字符
			while (c == '\n')    //如果这个字符是换行符的话，继续读取下行
			{
				if (fgets(line, Lmax, fp1) != NULL)
				{
					cc = 0;
					ll = strlen(line);
					c = line[cc++];
					lnum++;
				}
				else
				{
					flag = 0;
					break;
				}
			}
			lnum++;
		}
		else
		{
			flag = 0;
		}
	}
	else
		c = line[cc++];
}
void getsym()
{
	int i, point;
	for (i = 0;i<1000;i++)
		token[i] = '\0';
	i = 0;
	while (isSpace(c) || isNewline(c) || isTab(c))	//读取字符，跳过空格，换行和Tab。
	{
		getch();
		if (flag == 0)
			return;
	}
	if (isLetter(c) || isUnderline(c))	//判断字符是否是一个字母
	{
		while (isLetter(c) || isNumber(c) || isUnderline(c))
		{
			if (c <= 'Z'&&c >= 'A')
				c = c + 32;
			token[i++] = c;
			getch();
		}
		strcpy(symbol, word[isReserved(token)]);
	}
	else if (isNumber(c))	//判断字符是否是一个数字。
	{
		point = 0;
		token[i++] = c;
		getch();
		while (isNumber(c) || c == '.')
		{
			if (c == '.')
				point = 1;
			token[i++] = c;
			getch();
		}	//把一个字符退回到输入流ungetc()中。
		if (point == 0)
			strcpy(symbol, word[28]);
		//printf("整数 %d\n", atoi(token));
		else
			strcpy(symbol, word[29]);
		//printf("实数 %f\n", atof(token));			//把一段字符串转换为小数。
	}
	else if (isEqu(c))	//等号
	{
		token[i++] = c;
		getch();
		if (isEqu(c))
		{
			token[i++] = c;
			strcpy(symbol, word[30]);
			getch();
		}
		else
			strcpy(symbol, word[31]);
	}
	else if (islss(c))
	{
		token[i++] = c;
		getch();
		if (isEqu(c))
		{
			strcpy(symbol, word[32]);
			token[i++] = c;
			getch();
		}//小于等于
		else
			strcpy(symbol, word[33]);	//小于
	}
	else if (isgtr(c))
	{
		token[i++] = c;
		getch();
		if (isEqu(c))
		{
			strcpy(symbol, word[34]);
			token[i++] = c;
			getch();
		}//大于等于
		else
			strcpy(symbol, word[35]);	//大于
	}
	else if (isneq(c))
	{
		token[i++] = c;
		getch();
		if (isEqu(c))
		{
			strcpy(symbol, word[36]);
			token[i++] = c;
			getch();
		}
		else
			printf("error at:%d %c\n", lnum, c);
	}	  //不等于
	else if (isPlus(c))		//加号
	{
		token[i++] = c;
		strcpy(symbol, word[plusSY]);
		getch();
	}
	else if (isMinus(c))	//减号
	{
		token[i++] = c;
		strcpy(symbol, word[15]);
		getch();
	}
	else if (isStar(c))		//乘号
	{
		token[i++] = c;
		strcpy(symbol, word[16]);
		getch();
	}
	else if (isDiv(c))
	{
		token[i++] = c;
		strcpy(symbol, word[17]);	//除号
		getch();
	}
	else if (isComma(c))
	{
		token[i++] = c;
		strcpy(symbol, word[18]);	//逗号
		getch();
	}
	else if (isSemi(c))
	{
		token[i++] = c;
		strcpy(symbol, word[19]);		//分号
		getch();
	}
	else if (isLP(c))	//左括号
	{
		token[i++] = c;
		strcpy(symbol, word[20]);
		getch();
	}
	else if (isRP(c))	//右括号
	{
		token[i++] = c;
		strcpy(symbol, word[21]);
		getch();
	}
	else if (isLb(c))	//左中括号
	{
		token[i++] = c;
		strcpy(symbol, word[22]);
		getch();
	}
	else if (isRb(c))	//右中括号
	{
		token[i++] = c;
		strcpy(symbol, word[23]);
		getch();
	}
	else if (isLB(c))	//左花括号
	{
		token[i++] = c;
		strcpy(symbol, word[24]);
		getch();
	}
	else if (isRB(c))	//右花括号
	{
		token[i++] = c;
		strcpy(symbol, word[25]);
		getch();
	}
	else if (isL(c))
	{
		getch();
		if (isLetter(c) || isNumber(c) || isPlus(c) || isMinus(c) || isStar(c) || isDiv(c))
		{
			strcpy(symbol, word[26]);
			token[i++] = c;
		}
		else
			error(3);
		getch();
		if (c != 39)
		{
			error(4);
			getch();
		}
		else
			getch();
	}
	else if (isLL(c))
	{
		getch();
		while (c != 34)
		{
			if (c == 32 || c == 33 || c >= 35 && c <= 126)
			{
				token[i++] = c;
				getch();
			}
			else
			{
				error(3);
				printf("跳读不合法符号。\n");
				getch();
			}
		}
		if (c != 34)
		{
			error(5);	//缺少 "
			getch();
		}
		else {
			strcpy(symbol, word[27]);
			getch();
		}

	}
	else
	{
		error(3);
		getch();
	}
}
void error(int x)
{
	switch (x)
	{
	case 0:printf("error 错误行数：%d\n", lnum);
		break;
	case 1:printf("常量声明前后类型不一致 错误行数：%d\n", lnum);
		break;
	case 2:printf("不合法的单词类型 错误行数：%d\n", lnum);
		break;
	case 3:printf("不合法的符号 错误行数：%d\n", lnum);
		break;
	case 4:printf("缺少 \' 错误行数：%d\n", lnum);
		break;
	case 5:printf("缺少 \" 错误行数：%d\n", lnum);
		break;
	case 6:printf("标识符名称重复 错误行数：%d\n", lnum);
		break;
	case 7:printf("缺少等号  错误行数：%d\n", lnum);
		break;
	case 8:printf("标识符名称不合法  错误行数：%d\n", lnum);
		break;
	case 9:printf("常量定义中出现错误,未声明类型 错误行数：%d\n", lnum);
		break;
	case 10:printf("scanf语句内格式错误 错误行数：%d\n", lnum);
		break;
	case 11:printf("缺少; 错误行数：%d\n", lnum);
		break;
	case 12:printf("应该是( 错误行数：%d\n", lnum);
		break;
	case 13:printf("应该是) 错误行数：%d\n", lnum);
		break;
	case 14:printf("应该是{ 错误行数：%d\n", lnum);
		break;
	case 15:printf("应该是} 错误行数：%d\n", lnum);
		break;
	case 16:printf("标识符重复命名 错误行数：%d\n", lnum);
		break;
	case 17:printf("没有形参声明类型或者类型不合法 错误行数：%d\n", lnum);
		break;
	case 18:printf("缺少','逗号 错误行数：%d\n", lnum);
		break;
	case 19:printf("变量定义类型错误 错误行数：%d\n", lnum);
		break;
	case 20:printf("变量定义——类型错误 错误行数：%d\n", lnum);
		break;
	case 21:printf("应该是] 错误行数：%d\n", lnum);
		break;
	case 22:printf("声明数组内必须是无符号整数 错误行数：%d\n", lnum);
		break;
	case 23:printf("声明变量不能出现等号 错误行数：%d\n", lnum);
		break;
	case 24:printf("该标识符未命名 错误行数：%d\n", lnum);
		break;
	case 25:printf("值参数表数值不同 错误行数：%d\n", lnum);
		break;
	case 26:printf("值参数表内有数据类型不同 错误行数：%d\n", lnum);
		break;
	case 27:printf("数组内表达式数据类型必须是int 错误行数：%d\n", lnum);
		break;
	case 28:printf("赋值前后类型不一致 错误行数：%d\n", lnum);
		break;
	case 29:printf("输入内不能是函数名 错误行数：%d\n", lnum);
		break;
	case 30:printf("无返回值函数不能有返回值 错误行数：%d\n", lnum);
		break;
	case 31:printf("有返回值函数不能无返回值 错误行数：%d\n", lnum);
		break;
	default:printf("0");
	}
}
void skip()
{
	while (strcmp(symbol, "semiSY"))
		getsym();
}
void enter()
{
	strcpy(tab[tnum].name, id);
	tab[tnum].object = obj;
	strcpy(tab[tnum].type, typ);
	tab[tnum].decline = lnum;
	tab[tnum].position = pos;
	tab[tnum].adr = AD;
	tnum++;
}
int loc()
{
	int i, flag = 0;
	for (i = 0;i < tnum;i++)
	{
		if ((!strcmp(id, tab[i].name) && pos == tab[i].position) )	//同一个函数作用域pos相同的变量重名，或者全局变量里面的标识符和函数名重复
		{
			flag = 1;
			break;
		}
	}
	return flag;
}
void emit3(int f_, int x_, int y_)	//生成p-code代码
{
	strcpy(Code[lc].order, pcode[f_]);
	Code[lc].f = f_;
	Code[lc].x = x_;
	Code[lc].y = y_;
	lc++;
}
void emit2(int f_, int y_)
{
	strcpy(Code[lc].order, pcode[f_]);
	Code[lc].f = f_;
	Code[lc].y = y_;
	lc++;
}
void emit1(int f_)
{
	strcpy(Code[lc].order, pcode[f_]);
	Code[lc].f = f_;
	lc++;
}
void constdec()
{
	//printf("This is a constdeclaration statament.\n");
	obj = 0;	//const = 0 常量类型
	getsym();
	if (!strcmp(symbol, "intSY"))
	{
		strcpy(typ, "int");	//类型说明
		getsym();
		while (strcmp(symbol, "semiSY"))
		{
			if (!strcmp(symbol, "commaSY"))
				getsym();
			else if (!strcmp(symbol, "IDSY"))
			{
				strcpy(id, token);	//常量名说明
				if (loc())
				{
					error(6);
					while (strcmp(symbol, "semiSY") && strcmp(symbol, "commaSY"))
						getsym();
				}//常量名不能重复命名
				else
				{
					getsym();
					if (!strcmp(symbol, "equSY"))
					{
						getsym();
						if (!strcmp(symbol, "integer"))
						{
							AD = atoi(token);
							enter();
						}
						else if (!strcmp(symbol, "minusSY"))
						{
							getsym();
							AD = 0 - atoi(token);
							enter();
						}
						else if(!strcmp(symbol, "real"))
						{
							error(1);	//类型不一致
							AD = 0;
							enter();
						}
						getsym();
					}
					else
					{
						error(7);
						while (strcmp(symbol, "semiSY") && strcmp(symbol, "commaSY"))
							getsym();
					}//语法错误，少了等号
				}
			}
			else
			{
				error(8);
				while (strcmp(symbol, "semiSY") && strcmp(symbol, "commaSY"))
					getsym();
			}//常量名不合法
		}
	}
	else if (!strcmp(symbol, "charSY"))
	{
		strcpy(typ, "char");	//类型说明
		getsym();
		while (strcmp(symbol, "semiSY"))
		{
			if (!strcmp(symbol, "commaSY"))
				getsym();
			else if (!strcmp(symbol, "IDSY"))
			{
				strcpy(id, token);	//常量名说明
				if (loc())
				{
					error(6);	//常量名不能重复命名
					while (strcmp(symbol, "semiSY") && strcmp(symbol, "commaSY"))
						getsym();
				}
				else
				{
					getsym();
					if (!strcmp(symbol, "equSY"))
					{
						getsym();
						if (!strcmp(symbol, "charC"))
						{
							AD = token[0];
							enter();
						}
						else if(!strcmp(symbol, "real"))
						{
							AD = 0;
							error(1);	//类型不一致或者不合法
							enter();
						}
						getsym();
					}
					else
					{
						error(7);	//语法错误，少了等号
						while (strcmp(symbol, "semiSY") && strcmp(symbol, "commaSY"))
							getsym();
					}
				}
			}
			else
			{
				error(8);	//常量名不合法
				while (strcmp(symbol, "semiSY") && strcmp(symbol, "commaSY"))
					getsym();
			}
		}
	}
	else if (!strcmp(symbol, "floatSY"))
	{
		strcpy(typ, "float");	//类型说明
		getsym();
		while (strcmp(symbol, "semiSY"))
		{
			if (!strcmp(symbol, "commaSY"))
				getsym();
			else if (!strcmp(symbol, "IDSY"))
			{
				strcpy(id, token);	//常量名说明
				if (loc())
				{
					error(6);	//常量名不能重复命名
					while (strcmp(symbol, "semiSY") && strcmp(symbol, "commaSY"))
						getsym();
				}
				else
				{
					getsym();
					if (!strcmp(symbol, "equSY"))
					{
						getsym();
						if (!strcmp(symbol, "real") || !strcmp(symbol, "integer"))
						{
							AD = rnum;
							enter();
							rconst[rnum++] = atof(token);
						}
						else if (!strcmp(symbol, "minusSY"))
						{
							getsym();
							if (!strcmp(symbol, "real") || !strcmp(symbol, "integer"))
							{
								AD = rnum;
								enter();
								rconst[rnum++] = 0 - atof(token);
							}
							else
							{
								error(1);	//类型不一致或者不合法
								AD = rnum;
								enter();
								rconst[rnum++] = atof(token);
							}
						}
						else if (!strcmp(symbol, "plusSY"))
						{
							getsym();
							if (!strcmp(symbol, "real") || !strcmp(symbol, "integer"))
							{
								AD = rnum;
								enter();
								rconst[rnum++] = atof(token);
							}
							else
							{
								error(1);	//类型不一致或者不合法
								AD = rnum;
								enter();
								rconst[rnum++] = atof(token);
							}
						}
						else
						{
							error(1);	//类型不一致或者不合法
							AD = rnum;
							enter();
							rconst[rnum++] = atof(token);
						}
						getsym();
					}
					else
					{
						error(7);	//语法错误，少了等号
						while (strcmp(symbol, "semiSY") && strcmp(symbol, "commaSY"))
							getsym();
					}
				}
			}
			else
			{
				if (strcmp(symbol, "constSY") && strcmp(symbol, "intSY") && strcmp(symbol, "charSY") && strcmp(symbol, "floatSY") && strcmp(symbol, "voidSY"))
					error(20);
				else {
					error(8);	//常量名不合法
					while (strcmp(symbol, "semiSY") && strcmp(symbol, "commaSY"))
						getsym();
				}
			}
		}
	}
	else
		error(19);
	while (strcmp(symbol, "semiSY"))
		getsym();
}
void vardeclaration(char y[], char x[])
{
	//printf("This is a vardeclarationstatement.\n");
	obj = 1;
	strcpy(typ, y);
	strcpy(id, x);
	if (!strcmp(symbol, "LbSY"))
	{
		getsym();
		if (!strcmp(symbol, "integer"))
		{
			strcpy(typ, "array");
			tab[tnum].el_top = atoi(token);
			strcpy(tab[tnum].el_type, y);
			tab[FLAG].func_size = tab[FLAG].func_size + atoi(token);
			AD = ad;
			ad = ad + tab[tnum].el_top;	//之后变量的偏转量需要加上数组元素个数
			enter();
			getsym();
		}
		else
		{
			error(22);
			getsym();
		}
		if (!strcmp(symbol, "RbSY"))
			getsym();
		else
		{
			error(21);
			skip();
		}
	}
	else
	{
		tab[FLAG].func_size++;
		AD = ad++;
		enter();
	}
	while (strcmp(symbol, "semiSY"))
	{
		if (!strcmp(symbol, "commaSY"))
			getsym();
		else if (!strcmp(symbol, "equSY"))
		{
			error(23);
			while (strcmp(symbol, "semiSY") && strcmp(symbol, "commaSY"))
				getsym();
		}
		else if (!strcmp(symbol, "IDSY"))
		{
			strcpy(id, token);
			strcpy(typ, y);
			if (loc())
			{
				error(16);		//函数名不能重复命名
				while (strcmp(symbol, "semiSY") && strcmp(symbol, "commaSY"))
					getsym();
			}
			else
			{
				getsym();
				if (!strcmp(symbol, "LbSY"))
				{
					getsym();
					if (!strcmp(symbol, "integer"))
					{
						strcpy(typ, "array");
						tab[tnum].el_top = atoi(token);
						tab[FLAG].func_size = tab[FLAG].func_size + atoi(token);
						strcpy(tab[tnum].el_type, y);
						AD = ad;
						ad = ad + tab[tnum].el_top;	//之后变量的偏转量需要加上数组元素个数
						enter();
						getsym();
					}
					else
					{
						error(22);
						while (strcmp(symbol, "RbSY"))
							getsym();
					}
					if (!strcmp(symbol, "RbSY"))
						getsym();
					else
						error(21);
				}
				else
				{
					tab[FLAG].func_size++;
					AD = ad++;
					enter();
				}
			}
		}
		else
		{
			error(8);
			while (strcmp(symbol, "semiSY") && strcmp(symbol, "commaSY"))
				getsym();
		}
	}
}
void parameters(int x)
{
	int Temp = 1, i = x;
	//printf("This is 值参数表.\n");
	expression();
	//if (strcmp(tab[++i].type, Expression.expression_Type))
		//error(26);
	i++;
	if (!strcmp(tab[i].type, "float") && !strcmp(Expression.expression_Type, "char")
		|| !strcmp(tab[i].type, "char") && !strcmp(Expression.expression_Type, "float")
		|| !strcmp(tab[i].type, "int") && !strcmp(Expression.expression_Type, "float"))
		error(26);
	while (!strcmp(symbol, "commaSY"))
	{
		Temp++;
		getsym();
		expression();
		i++;
		if (!strcmp(tab[i].type, "float") && !strcmp(Expression.expression_Type, "char")
			|| !strcmp(tab[i].type, "char") && !strcmp(Expression.expression_Type, "float")
			|| !strcmp(tab[i].type, "int") && !strcmp(Expression.expression_Type, "float"))
			error(26);
	}
	if (tab[x].Para != Temp)
		error(25);
}
void factor()
{
	int temp_;
	int Type_flag;
	Factor = set;
	//printf("This is a factor.\n");
	if (!strcmp(symbol, "IDSY"))
	{
		if (search())
		{
			if (!strcmp(tab[location].type, "char"))
			{
				Factor.expression_type = 0;
				strcpy(Factor.expression_Type, "char");
			}
			else if (!strcmp(tab[location].type, "int"))
			{
				Factor.expression_type = 1;
				strcpy(Factor.expression_Type, "int");
			}
			else if (!strcmp(tab[location].type, "float"))
			{
				Factor.expression_type = 2;
				strcpy(Factor.expression_Type, "float");
			}
			else if (!strcmp(tab[location].type, "array"))
			{
				if (!strcmp(tab[location].el_type, "char"))
				{
					Factor.expression_type = 0;
					strcpy(Factor.expression_Type, "char");
				}
				else if (!strcmp(tab[location].el_type, "int"))
				{
					Factor.expression_type = 1;
					strcpy(Factor.expression_Type, "int");
				}
				else if (!strcmp(tab[location].el_type, "float"))
				{
					Factor.expression_type = 2;
					strcpy(Factor.expression_Type, "float");
				}
			}
			Type_flag = Factor.expression_type;
			//printf("This is a ID symbol:%s.\n", token);
			getsym();
			if (!strcmp(symbol, "LPSY"))
			{
				temp_ = location;
				//printf("This is a 有返回值函数调用语句.\n");
				emit1(18);
				getsym();
				if (!strcmp(symbol, "RPSY"))
				{
					//printf("This is 值参数表.\n");
					if (Para_ == 0)
					{
						emit3(19, tab[temp_].func_size, tab[temp_].func_code_line);	//存入函数大小和下一代码行数
						getsym();
					}
					else
					{
						error(25);
						getsym();
					}
				}
				else {
					parameters(temp_);
					strcpy(Factor.expression_Type, tab[temp_].type);
					Factor.expression_type = Type_flag;
					emit3(19, tab[temp_].func_size, tab[temp_].func_code_line);	//存入函数大小和下一代码行数
					if (!strcmp(symbol, "RPSY"))
						getsym();
				}
			}
			else if (!strcmp(symbol, "LbSY")) //*************************
			{
				temp_ = location;
				getsym();
				emit3(0, tab[location].position, tab[location].adr);
				expression();
				strcpy(Factor.expression_Type, tab[temp_].el_type);
				Factor.expression_type = Type_flag;
				emit1(52);
				emit1(34);
				if (!strcmp(symbol, "RbSY"))
					getsym();
				else
					error(21);
			}
			else
				emit3(1, tab[location].position, tab[location].adr);
		}
		else if (search_())
		{
			if (!strcmp(tab[location].type, "char"))
			{
				Factor.expression_type = 0;
				strcpy(Factor.expression_Type, "char");
				emit2(24, tab[location].adr);
			}
			else if (!strcmp(tab[location].type, "int"))
			{
				Factor.expression_type = 1;
				strcpy(Factor.expression_Type, "int");
				emit2(24, tab[location].adr);
			}
			else if (!strcmp(tab[location].type, "float"))
			{
				Factor.expression_type = 2;
				strcpy(Factor.expression_Type, "float");
				emit2(25, tab[location].adr);	//装入实数是实数表的下标
			}
			//printf("This is a ID symbol:%s.\n", token);
			getsym();
		}
		else {
			error(24);
			skip();
			return;
		}
	}
	else if (!strcmp(symbol, "minusSY"))
	{
		getsym();
		if (!strcmp(symbol, "integer"))
		{
			Factor.expression_type = 1;
			strcpy(Factor.expression_Type, "int");
			//printf("This is a interger:%d.\n", atoi(token));
			emit2(24, 0 - atoi(token));	//装入字面常量
			getsym();
		}
		else if (!strcmp(symbol, "real"))
		{
			Factor.expression_type = 2;
			strcpy(Factor.expression_Type, "float");
			//printf("This is a float:%f.\n", atof(token));
			rconst[rnum++] = 0 - atof(token);
			emit2(25, rnum - 1);	//装入实数
			getsym();
		}
		else
		{
			error(0);
			while (strcmp(symbol, "semiSY"))
				getsym();
			return;
		}
	}
	else if (!strcmp(symbol, "integer"))
	{
		Factor.expression_type = 1;
		strcpy(Factor.expression_Type, "int");
		//printf("This is a interger:%d.\n", atoi(token));
		emit2(24, atoi(token));	//装入字面常量
		getsym();
	}
	else if (!strcmp(symbol, "real"))
	{
		Factor.expression_type = 2;
		strcpy(Factor.expression_Type, "float");
		//printf("This is a float:%f.\n", atof(token));
		rconst[rnum++] = atof(token);
		emit2(25, rnum - 1);	//装入实数
		getsym();
	}
	else if (!strcmp(symbol, "charC"))
	{
		Factor.expression_type = 0;
		strcpy(Factor.expression_Type, "char");
		emit2(24, token[0]);	//装入字面常量
		getsym();
	}
	else if (!strcmp(symbol, "LPSY"))
	{
		getsym();
		expression();
		if (!strcmp(symbol, "RPSY"))
			getsym();
	}
	else
	{
		error(0);
		skip();
	}
}
void term()
{
	//printf("This is a term.\n");
	factor();
	Term = Factor;
	while (!strcmp(symbol, "starSY") || !strcmp(symbol, "divSY"))
	{
		if (!strcmp(symbol, "starSY"))
		{
			//printf("This is a calculate symbol:%s.\n", token);
			getsym();
			factor();
			if (Factor.expression_type >= Term.expression_type)
			{
				Term.expression_type = Factor.expression_type;
				strcpy(Term.expression_Type, Factor.expression_Type);
			}
			emit1(60);
		}
		if (!strcmp(symbol, "divSY"))
		{
			//printf("This is a calculate symbol:%s.\n", token);
			getsym();
			factor();
			if (Factor.expression_type >= Term.expression_type)
			{
				Term.expression_type = Factor.expression_type;
				strcpy(Term.expression_Type, Factor.expression_Type);
			}
			emit1(61);
		}
	}
}
void expression()
{
	char CC[10];
	Expression = set;
	//printf("This is an expression.\n");
	if (!strcmp(symbol, "plusSY") || !strcmp(symbol, "minusSY"))
	{
		strcpy(CC, symbol);
		//printf("This is a calculate symbol:%s.\n", token);
		getsym();
	}
	term();
	if (!strcmp(CC, "minusSY"))
	{
		emit1(36);
	}
	Expression = Term;
	while (!strcmp(symbol, "plusSY") || !strcmp(symbol, "minusSY"))
	{
		if (!strcmp(symbol, "plusSY"))
		{
			//printf("This is a calculate symbol:%s.\n", token);
			getsym();
			term();
			if (Term.expression_type >= Expression.expression_type)
			{
				Expression = Term;
			}
			emit1(54);
		}
		else if (!strcmp(symbol, "minusSY"))
		{
			//printf("This is a calculate symbol:%s.\n", token);
			getsym();
			term();
			if (Term.expression_type >= Expression.expression_type)
			{
				Expression = Term;
			}
			emit1(55);
		}
	}
}

int search()
{
	int flag = 0;
	for (location = tnum-1;location>=0;location--)
	{
		if (!strcmp(token, tab[location].name) && tab[location].object == 2)
		{
			flag = 1;
			Para_ = tab[location].Para;
			break;
		}
		if (!strcmp(token, tab[location].name) && tab[location].position == pos || !strcmp(token, tab[location].name) && tab[location].position == 0 && tab[location].object == 1)
			//查找到相同函数名*************在同一函数作用域内找到了相同变量名 pos为当前函数作用域************全局变量同名变量
		{
			flag = 1;
			break;
		}
	}
	return flag;
}
int search_()
{
	int flag = 0;
	for (location = 0;location < tnum;location++)
	{
		if (tab[location].object == 0 && !strcmp(token, tab[location].name))
		{
			flag = 1;
			break;
		}
	}
	return flag;
}
void statement()
{
	int temp_;
	int jumpline;
	int jump_line;
	int temp_1, temp_2, temp_3;
	int i;
	if (!strcmp(symbol, "semiSY"))
	{
		//printf("This is a null statement\n");
		getsym();
	}
	else if (!strcmp(symbol, "ifSY"))
	{
		getsym();
		//printf("This is a ifstatement\n");
		if (!strcmp(symbol, "LPSY"))
		{
			getsym();
			expression();
			if (!strcmp(symbol, "equ_SY"))
			{
				//printf("This is a compare symbol:%s.\n", token);
				getsym();
				expression();
				emit1(39);
			}
			else if (!strcmp(symbol, "neqSY"))
			{
				//printf("This is a compare symbol:%s.\n", token);
				getsym();
				expression();
				emit1(40);
			}
			else if (!strcmp(symbol, "lssSY"))
			{
				//printf("This is a compare symbol:%s.\n", token);
				getsym();
				expression();
				emit1(41);
			}
			else if (!strcmp(symbol, "leqSY"))
			{
				//printf("This is a compare symbol:%s.\n", token);
				getsym();
				expression();
				emit1(42);
			}
			else if (!strcmp(symbol, "gtrSY"))
			{
				//printf("This is a compare symbol:%s.\n", token);
				getsym();
				expression();
				emit1(43);
			}
			else if (!strcmp(symbol, "geqSY"))
			{
				//printf("This is a compare symbol:%s.\n", token);
				getsym();
				expression();
				emit1(44);
			}
			else
			{
				emit2(24, 0);
				emit1(40);
			}
			if (!strcmp(symbol, "RPSY"))
			{
				getsym();
				emit1(11);
				jumpline = lc - 1;
				statement();
			}
			else
			{
				error(13);
				skip();
				return;
			}
			emit1(10);
			jump_line = lc - 1;
			Code[jumpline].y = lc;
			if (!strcmp(symbol, "elseSY"))
			{
				//printf("This is a elsestatement\n");
				getsym();
				statement();
				Code[jump_line].y = lc;
			}
			else
				Code[jump_line].y = lc;
		}
		else
		{
			error(12);
			skip();
			return;
		}
	}
	else if (!strcmp(symbol, "forSY"))
	{
		getsym();
		//printf("This is a forstatement\n");
		if (!strcmp(symbol, "LPSY"))
		{
			getsym();
			if (!strcmp(symbol, "IDSY"))
			{
				if (search())
				{
					emit3(0, tab[location].position, tab[location].adr);
					getsym();
					if (!strcmp(symbol, "equSY"))
					{
						getsym();
						expression();
						emit1(38);
						if (!strcmp(symbol, "semiSY"))
						{
							getsym();
							//printf("This is a condition statement\n");
							jumpline = lc;
							expression();
							if (!strcmp(symbol, "equ_SY"))
							{
								//printf("This is a compare symbol:%s.\n", token);
								getsym();
								expression();
								emit1(39);
							}
							else if (!strcmp(symbol, "neqSY"))
							{
								//printf("This is a compare symbol:%s.\n", token);
								getsym();
								expression();
								emit1(40);
							}
							else if (!strcmp(symbol, "lssSY"))
							{
								//printf("This is a compare symbol:%s.\n", token);
								getsym();
								expression();
								emit1(41);
							}
							else if (!strcmp(symbol, "leqSY"))
							{
								//printf("This is a compare symbol:%s.\n", token);
								getsym();
								expression();
								emit1(42);
							}
							else if (!strcmp(symbol, "gtrSY"))
							{
								//printf("This is a compare symbol:%s.\n", token);
								getsym();
								expression();
								emit1(43);
							}
							else if (!strcmp(symbol, "geqSY"))
							{
								//printf("This is a compare symbol:%s.\n", token);
								getsym();
								expression();
								emit1(44);
							}
							else
							{
								emit2(24, 0);
								emit1(40);
							}
							emit1(11);
							jump_line = lc - 1;
							if (!strcmp(symbol, "semiSY"))
							{
								getsym();
								if (!strcmp(symbol, "IDSY"))
								{
									if (search())
									{
										getsym();
										temp_1 = location;
										if (!strcmp(symbol, "equSY"))
										{
											getsym();
											if (!strcmp(symbol, "IDSY"))
											{
												if (search())
												{
													getsym();
													temp_2 = location;
													emit3(1, tab[location].position, tab[location].adr);
													if (!strcmp(symbol, "plusSY"))
													{
														getsym();
														if (!strcmp(symbol, "integer"))
														{
															//printf("This is 步长：%d.\n", atoi(token));
															temp_3 = atoi(token);
															getsym();
															if (!strcmp(symbol, "RPSY"))
															{
																getsym();
																statement();
																emit3(0, tab[temp_1].position, tab[temp_1].adr);
																emit3(1, tab[temp_2].position, tab[temp_2].adr);
																emit2(24, temp_3);
																emit1(52);
																emit1(38);
																emit2(10, jumpline);
																Code[jump_line].y = lc;
															}
															else
															{
																error(13);
																skip();
																return;
															}
														}
														else {
															error(29);
															skip();
															return;
														}
													}
													else if (!strcmp(symbol, "minusSY"))
													{
														getsym();
														if (!strcmp(symbol, "integer"))
														{
															//printf("This is 步长：%d.\n", atoi(token));
															temp_3 = atoi(token);
															getsym();
															if (!strcmp(symbol, "RPSY"))
															{
																getsym();
																statement();
																emit3(0, tab[temp_1].position, tab[temp_1].adr);
																emit3(1, tab[temp_2].position, tab[temp_2].adr);
																emit2(24, temp_3);
																emit1(53);
																emit1(38);
																emit2(10, jumpline);
																Code[jump_line].y = lc;
															}
															else {
																error(13);
																skip();
																return;
															}
														}
														else {
															error(29);
															skip();
															return;
														}
													}
													else {
														error(0);
														skip();
														return;
													}
												}
												else {
													error(24);
													skip();
													return;
												}
											}
											else {
												error(8);
												skip();
												return;
											}
										}
										else {
											error(7);
											skip();
											return;
										}
									}
									else {
										error(24);
										skip();
										return;
									}
								}
								else {
									error(8);
									skip();
									return;
								}
							}
							else {
								error(11);
								skip();
								return;
							}
						}
						else {
							error(11);
							skip();
							return;
						}
					}
					else {
						error(7);
						skip();
						return;
					}
				}
				else {
					error(24);
					skip();
					return;
				}
			}
			else {
				error(8);
				skip();
				return;
			}
		}
		else {
			error(12);
			skip();
			return;
		}
	}
	else if (!strcmp(symbol, "whileSY"))
	{
		getsym();
		//printf("This is a whilestatement\n");
		if (!strcmp(symbol, "LPSY"))
		{
			getsym();
			//printf("This is a condition statement\n");
			jumpline = lc;
			expression();
			if (!strcmp(symbol, "equ_SY"))
			{
				//printf("This is a compare symbol:%s.\n", token);
				getsym();
				expression();
				emit1(39);
			}
			else if (!strcmp(symbol, "neqSY"))
			{
				//printf("This is a compare symbol:%s.\n", token);
				getsym();
				expression();
				emit1(40);
			}
			else if (!strcmp(symbol, "lssSY"))
			{
				//printf("This is a compare symbol:%s.\n", token);
				getsym();
				expression();
				emit1(41);
			}
			else if (!strcmp(symbol, "leqSY"))
			{
				//printf("This is a compare symbol:%s.\n", token);
				getsym();
				expression();
				emit1(42);
			}
			else if (!strcmp(symbol, "gtrSY"))
			{
				//printf("This is a compare symbol:%s.\n", token);
				getsym();
				expression();
				emit1(43);
			}
			else if (!strcmp(symbol, "geqSY"))
			{
				//printf("This is a compare symbol:%s.\n", token);
				getsym();
				expression();
				emit1(44);
			}
			else
			{
				emit2(24, 0);
				emit1(40);
			}
			emit1(11);
			jump_line = lc - 1;
			if (!strcmp(symbol, "RPSY"))
			{
				getsym();
				statement();
				emit2(10, jumpline);
				Code[jump_line].y = lc;
			}
			else
			{
				error(13);
				skip();
				return;
			}
		}
		else {
			error(12);
			skip();
			return;
		}
	}
	else if (!strcmp(symbol, "scanfSY"))
	{
		getsym();
		//printf("This is a scanfstatement.\n");
		if (!strcmp(symbol, "LPSY"))
		{
			getsym();
			while (strcmp(symbol, "RPSY"))
			{
				if (!strcmp(symbol, "IDSY"))
				{
					if (search())
					{
						if (tab[location].object == 2)
						{
							error(29);
							skip();
							return;
						}
						else
						{
							emit3(0, tab[location].position, tab[location].adr);
							if (!strcmp(tab[location].type, "int"))
								emit2(27, 1);
							else if (!strcmp(tab[location].type, "float"))
								emit2(27, 2);
							else if (!strcmp(tab[location].type, "char"))
								emit2(27, 3);	//读（y表示类型：1整型，2实行，3字符型）
												//printf("This is scanf ID:%s.\n", token);
							getsym();
						}
					}
					else {
						error(24);
						skip();
						return;
					}
				}
				else if (!strcmp(symbol, "commaSY"))
					getsym();
				else
				{
					error(10);
					skip();
					return;
				}
			}
		}
		else {
			error(12);
			skip();
			return;
		}
		if (!strcmp(symbol, "RPSY"))
			getsym();
		else
		{
			error(13);
			skip();
			return;
		}
		if (strcmp(symbol, "semiSY"))
		{
			error(11);
			skip();
			return;
		}
		else
			getsym();
	}
	else if (!strcmp(symbol, "printfSY"))
	{
		//printf("This is a printfstatement.\n");
		getsym();
		if (!strcmp(symbol, "LPSY"))
		{
			getsym();
			if (!strcmp(symbol, "stringC"))
			{
				strcpy(stab[snum].string_, token);
				stab[snum].snum_ = strlen(token);
				//printf("This is 字符串:%s.\n", token);
				for (i = 0;i < stab[snum].snum_;i++)
				{
					emit2(24, stab[snum].string_[i]);
					emit2(28, 1);
				}
				emit2(24, 10);
				emit1(28);
				snum++;
				getsym();
				if (!strcmp(symbol, "commaSY"))
				{
					getsym();
					expression();
					if (!strcmp(Expression.expression_Type, "int"))
						emit2(29, 1);
					else if (!strcmp(Expression.expression_Type, "float"))
						emit1(37);
					else if (!strcmp(Expression.expression_Type, "char"))
						emit2(28, 1);
					if (!strcmp(symbol, "RPSY"))
						getsym();
				}
				else if (!strcmp(symbol, "RPSY"))
					getsym();
				else {
					error(11);
					skip();
					return;
				}
			}
			else
			{
				expression();
				if (!strcmp(Expression.expression_Type, "int"))
					emit2(29, 1);
				else if (!strcmp(Expression.expression_Type, "float"))
					emit1(37);
				else if (!strcmp(Expression.expression_Type, "char"))
					emit2(28, 1);
			}
			if (!strcmp(symbol, "RPSY"))
				getsym();
			if (strcmp(symbol, "semiSY"))
			{
				error(11);
				skip();
				return;
			}
			else
				getsym();
		}
		else {
			error(12);
			skip();
			return;
		}
	}
	else if (!strcmp(symbol, "returnSY"))
	{
		getsym();
		//printf("This is a return statement.\n");
		if (main_flag == 0)
		{
			if (!strcmp(symbol, "LPSY"))
			{
				if (return_ == 1)
					error(30);
				getsym();
				expression();
				emit1(22);	//判断这个函数是否有返回值的手段
				emit1(33);
				if (!strcmp(symbol, "RPSY"))
					getsym();
				else
				{
					error(13);
					skip();
					return;
				}
				if (strcmp(symbol, "semiSY"))
				{
					error(11);
					skip();
					return;
				}
				else
					getsym();
			}
			else if (!strcmp(symbol, "semiSY"))
			{	//判断这个函数是否有返回值的手段
				if(return_==2)
					error(31);
				emit1(33);
				getsym();
			}
			else
			{
				error(12);
				skip();
				return;
			}
		}
		else if (main_flag == 1)
		{
			if (!strcmp(symbol, "LPSY"))
			{
				error(30);
				skip();
				return;
			}
			else if (!strcmp(symbol, "semiSY"))
			{	//判断这个函数是否有返回值的手段
				emit1(32);
				getsym();
			}
			else
			{
				error(12);
				skip();
			}
		}
	}
	else if (!strcmp(symbol, "IDSY"))
	{
		if (search())
		{
			//printf("This is a ID symbol:%s.\n", token);
			temp_ = location;
			getsym();
			if (!strcmp(symbol, "LPSY"))
			{
				//printf("This is a 函数调用语句.\n");
				emit1(18);
				getsym();
				if (!strcmp(symbol, "RPSY"))
				{
					//printf("This is 值参数表.\n");
					if (Para_ == 0)
						getsym();
					else
					{
						error(25);
						getsym();
						return;
					}
				}
				else
				{
					parameters(temp_);
					if (!strcmp(symbol, "RPSY"))
						getsym();
					else
					{
						error(12);
						skip();
						return;
					}
				}
				emit3(19, tab[temp_].func_size, tab[temp_].func_code_line);	//存入函数大小和下一代码行数
				if (strcmp(symbol, "semiSY"))
				{
					error(11);
					return;
				}
				else
					getsym();
			}
			else if (!strcmp(symbol, "LbSY"))
			{
				getsym();
				emit3(0, tab[temp_].position, tab[temp_].adr);
				expression();
				emit1(52);
				if (Expression.expression_type != 1)
					error(27);
				if (!strcmp(symbol, "RbSY"))
					getsym();
				else
					error(21);
				if (!strcmp(symbol, "equSY"))
				{
					//printf("This is a assignment statement\n");
					getsym();
					expression();
					emit1(38);
					if (!strcmp(tab[temp_].el_type,"float")&&!strcmp(Expression.expression_Type,"char")
						|| !strcmp(tab[temp_].el_type, "char") && !strcmp(Expression.expression_Type, "float")
						|| !strcmp(tab[temp_].el_type, "int") && !strcmp(Expression.expression_Type, "float"))
						error(28);
					if (strcmp(symbol, "semiSY"))
						error(11);
					else
						getsym();
				}
			}
			else
			{
				emit3(0, tab[temp_].position, tab[temp_].adr);
				if (!strcmp(symbol, "equSY"))
				{
					//printf("This is a assignment statement\n");
					getsym();
					expression();
					emit1(38);
					if (!strcmp(tab[temp_].type, "float")&&!strcmp(Expression.expression_Type, "char")
						|| !strcmp(tab[temp_].type, "char") && !strcmp(Expression.expression_Type, "float")
						|| !strcmp(tab[temp_].type, "int") && !strcmp(Expression.expression_Type, "float"))
						error(28);
					if (strcmp(symbol, "semiSY"))
					{
						error(11);
						skip();
						return;
					}
					else
						getsym();
				}
				else {
					error(0);
					skip();
					return;
				}
			}
		}
		else
		{
			error(24);
			skip();
			return;
		}
	}
	else if (!strcmp(symbol, "LBSY"))
	{
		getsym();
		compoundstatement();
		if (!strcmp(symbol, "RBSY"))
		{
			//printf("This is 语句列结束.\n");
			getsym();
		}
		else
		{
			error(15);
			skip();
			return;
		}
	}
	else
	{
		error(2);
		skip();
		return;
	}
}
void compoundstatement()
{
	char type[10];
	char name[20];
	//if (!strcmp(symbol, "constSY") || !strcmp(symbol, "intSY") || !strcmp(symbol, "floatSY") || !strcmp(symbol, "charSY"))
	//printf("These are 复合语句.\n");
	//else
	//printf("These are 语句列.\n");
	while (strcmp(symbol, "RBSY"))
	{
		if (!strcmp(symbol, "constSY"))
			constdec();
		else if (!strcmp(symbol, "intSY") || !strcmp(symbol, "floatSY") || !strcmp(symbol, "charSY"))
		{
			strcpy(typ, token);
			strcpy(type, token);
			getsym();
			if (!strcmp(symbol, "IDSY"))
			{
				strcpy(id, token);
				strcpy(name, token);
				getsym();
				if (loc())
				{
					error(16);		//函数名不能重复命名
					skip();
					return;
				}
				else
				{
					vardeclaration(type, name);
				}
			}
			else
			{
				error(0);
				skip();
				return;
			}
		}
		else if (!strcmp(symbol, "semiSY"))
			getsym();
		else
			statement();
	}
}

void funcdeclaration()
{
	char Type[20];
	char Name[20];
	int temp_line;
	int Temp;
	int i;
	strcpy(typ, token);
	strcpy(Type, token);
	getsym();
	if (!strcmp(symbol, "mainSY"))
	{
		//printf("This is main function.\n");
		return_ = 1;
		main_lc = lc;
		main_flag = 1;
		strcpy(id, token);
		obj = 2;
		pos++;
		if (pos == 1)
			Var_num = ad - 3;
		Temp = tnum;
		FLAG = tnum;
		ad = 0;
		AD = ad;
		ad = ad + 3;
		enter();
		emit1(18);
		emit2(19, lc + 1);
		temp_line = lc - 1;
		getsym();
		if (!strcmp(symbol, "LPSY"))
			getsym();
		else
		{
			error(12);
			skip();
		}
		if (!strcmp(symbol, "RPSY"))
			getsym();
		else
		{
			error(13);
			skip();
		}
		if (!strcmp(symbol, "LBSY"))
		{
			getsym();
			compoundstatement();
			Code[temp_line].x = tab[Temp].func_size;
		}
		else
		{
			error(14);
			skip();
		}
		if (!strcmp(symbol, "RBSY"))
		{
			//printf("These are 复合语句 finished\n");
			if (tnum - 1 == Temp)
				tab[Temp].func_size = 0;
			else
				tab[Temp].func_size = tab[tnum - 1].adr - tab[Temp].adr - 2;
			Code[temp_line].x = tab[Temp].func_size;
		}
		else
		{
			error(15);
			skip();
		}
	}
	else if (!strcmp(symbol, "IDSY"))
	{
		strcpy(id, token);
		strcpy(Name, token);
		if (loc())
		{
			error(16);		//函数名不能重复命名
			while (strcmp(symbol, "semiSY") && strcmp(symbol, "LBSY"))
				getsym();
			return;
		}
		else
		{
			getsym();
			if (strcmp(symbol, "LPSY"))
				vardeclaration(Type, Name);
			else
			{
				if (!strcmp(Type, "void"))
					return_ = 1;
				else
					return_ = 2;
				for (i = 0;i < tnum;i++)
				{
					if (!strcmp(tab[i].name, Name) && tab[i].object != 2 && tab[i].position == 0 || !strcmp(tab[i].name, Name) && tab[i].object == 2)
					{
						error(16);
						while (strcmp(symbol, "LBSY"))
							getsym();
						return;
					}
				}
				pos++;
				if (pos == 1)
					Var_num = ad - 3;
				obj = 2;
				ad = 0;
				AD = ad;
				ad = ad + 3;
				Temp = tnum;
				FLAG = tnum;
				tab[Temp].func_code_line = lc;
				enter();
				//printf("This is 参数表.\n");
				getsym();
				while (strcmp(symbol, "RPSY"))
				{
					//形参存储
					if (!strcmp(symbol, "intSY") || !strcmp(symbol, "charSY") || !strcmp(symbol, "floatSY"))
					{
						//printf("%s型形参处理\n", symbol);
						obj = 3;	//形参标志
						strcpy(typ, token);
						getsym();
						if (!strcmp(symbol, "IDSY"))
						{
							//printf("名为：%s形参处理\n", token);
							//printf("This is 参数.\n");
							strcpy(id, token);
							if (loc())
							{
								error(6);	//形参名之间不能重复
								while (strcmp(symbol, "RPSY") && strcmp(symbol, "commaSY"))
									getsym();
							}
							else
							{
								tab[Temp].Para++;
								AD = ad++;
								enter();
								getsym();
							}
						}
						else
						{
							error(8);
							//printf("形参名不能是保留字");//error();	//形参名不能是保留字
							while (strcmp(symbol, "semiSY") && strcmp(symbol, "commaSY"))
								getsym();
						}
					}
					else if (!strcmp(symbol, "commaSY"))
					{
						//printf("读到逗号，作出处理\n");
						getsym();
					}
					else
					{
						error(17);
						while (strcmp(symbol, "semiSY") && strcmp(symbol, "commaSY"))
							getsym();
					}
				}
				getsym();
				if (!strcmp(symbol, "LBSY"))
				{
					getsym();
					compoundstatement();
				}
				else
				{
					error(14);
					skip();
				}
				if (!strcmp(symbol, "RBSY"))
				{
					//printf("These are 复合语句 finished\n");
					emit1(33);	//	退出函数
				}
				else
				{
					error(15);
					skip();
				}
			}
		}
	}
	else
	{
		error(8);
		skip();
		return;
	}
}

void syntax()
{
	getsym();
	if (flag == 0)
		return;
	if (!strcmp(symbol, "constSY"))
		constdec();
	else if (!strcmp(symbol, "intSY") || !strcmp(symbol, "floatSY") || !strcmp(symbol, "charSY") || !strcmp(symbol, "voidSY"))
		funcdeclaration();
	else
		error(2);
}

void interpret()
{
	int i;	//存储指针，索引变量
	int I;
	char ch;
	float j;	//存储读入时的数据
	if (!strcmp(Code[code_lc].order, "INT"))
	{
		etp = etp + Code[code_lc].y;
		code_lc++;
	}
	else if (!strcmp(Code[code_lc].order, "JMP"))
	{
		code_lc = Code[code_lc].y;	//除了跳转语句，其他所有代码执行完之后需要加一去进行下一行的执行
	}
	else if (!strcmp(Code[code_lc].order, "LDA"))
	{
		if (Code[code_lc].x == 0)
		{
			S[etp] = Code[code_lc].y;
			etp++;
			code_lc++;
		}
		else
		{
			S[etp] = abp + Code[code_lc].y;
			etp++;
			code_lc++;
		}
	}
	else if (!strcmp(Code[code_lc].order, "LDC"))
	{
		S[etp] = Code[code_lc].y;
		etp++;
		code_lc++;
	}
	else if (!strcmp(Code[code_lc].order, "STO"))
	{
		i = S[etp - 2];
		S[i] = S[etp - 1];
		etp = etp - 2;
		code_lc++;
	}
	else if (!strcmp(Code[code_lc].order, "LOD"))
	{
		if (Code[code_lc].x == 0)
		{
			S[etp] = S[Code[code_lc].y];
			etp++;
			code_lc++;
		}
		else
		{
			S[etp] = S[abp + Code[code_lc].y];
			etp++;
			code_lc++;
		}
	}
	else if (!strcmp(Code[code_lc].order, "ADD"))
	{
		S[etp - 2] = S[etp - 1] + S[etp - 2];
		etp--;
		code_lc++;
	}	 //所有的运算操作，此时栈顶指针指的是一个未赋值的栈元素，需要对栈顶指针减一进行操作。而给栈顶元素赋值的操作，直接进行就好，最后要给栈顶指针加一，保证栈顶元素不会被占用
	else if (!strcmp(Code[code_lc].order, "ADR"))
	{
		S[etp - 2] = S[etp - 1] + S[etp - 2];
		etp--;
		code_lc++;
	}
	else if (!strcmp(Code[code_lc].order, "SUB"))
	{
		S[etp - 2] = S[etp - 2] - S[etp - 1];
		etp--;
		code_lc++;
	}
	else if (!strcmp(Code[code_lc].order, "SUR"))
	{
		S[etp - 2] = S[etp - 2] - S[etp - 1];
		etp--;
		code_lc++;
	}
	else if (!strcmp(Code[code_lc].order, "MUS"))
	{
		S[etp - 1] = -S[etp - 1];
		code_lc++;
	}
	else if (!strcmp(Code[code_lc].order, "MUR"))
	{
		S[etp - 2] = S[etp - 2] * S[etp - 1];
		etp--;
		code_lc++;
	}
	else if (!strcmp(Code[code_lc].order, "MUL"))
	{
		S[etp - 2] = S[etp - 2] * S[etp - 1];
		etp--;
		code_lc++;
	}
	else if (!strcmp(Code[code_lc].order, "DIV"))
	{
		S[etp - 2] = S[etp - 2] / S[etp - 1];
		etp--;
		code_lc++;
	}
	else if (!strcmp(Code[code_lc].order, "DIR"))
	{
		S[etp - 2] = S[etp - 2] / S[etp - 1];
		etp--;
		code_lc++;
	}
	else if (!strcmp(Code[code_lc].order, "LDR"))
	{
		S[etp] = rconst[Code[code_lc].y];	//装入实数从实数表取值
		etp++;
		code_lc++;
	}
	else if (!strcmp(Code[code_lc].order, "LDT"))
	{
		i = S[etp - 1];
		S[etp - 1] = S[i];
		code_lc++;
	}
	else if (!strcmp(Code[code_lc].order, "WRS"))
	{
		i = S[etp - 1];
		printf("%c", i);
		etp--;
		code_lc++;
	}
	else if (!strcmp(Code[code_lc].order, "WRR"))
	{
		printf("%f\n", S[etp - 1]);
		etp--;
		code_lc++;
	}
	else if (!strcmp(Code[code_lc].order, "WRW"))
	{
		i = S[etp - 1];
		printf("%d\n", i);
		etp--;
		code_lc++;
	}
	else if (!strcmp(Code[code_lc].order, "RED"))
	{
		if (Code[code_lc].y == 1)
		{
			printf("输入整数:");
			scanf("%d", &I);
			getchar();
			i = S[etp - 1];
			S[i] = I;
			etp--;
			code_lc++;
		}
		else if (Code[code_lc].y == 2)
		{
			printf("输入实数:");
			scanf("%f", &j);
			getchar();
			i = S[etp - 1];
			S[i] = j;
			etp--;
			code_lc++;
		}
		else if (Code[code_lc].y == 3)
		{
			printf("输入字符:");
			scanf("%c", &ch);
			getchar();
			i = S[etp - 1];
			S[i] = ch;
			etp--;
			code_lc++;
		}
	}
	else if (!strcmp(Code[code_lc].order, "MKS"))
	{
		etp = etp + 3;
		etp_flag[++etp_num] = etp;
		code_lc++;
	}
	else if (!strcmp(Code[code_lc].order, "CAL"))
	{
		S[etp_flag[etp_num] - 1] = code_lc + 1;	//存储返回地址
		S[etp_flag[etp_num] - 2] = abp;		//存储上一层基地址
		abp = etp_flag[etp_num] - 3;		//将现在的基地址更新
		etp_num--;
		etp = etp + Code[code_lc].x;	//根据函数大小增加栈顶指针
		code_lc = Code[code_lc].y;		//跳转代码到分函数开始行数
	}
	else if (!strcmp(Code[code_lc].order, "LDB"))
	{
		return_flag[return_num] = 1;
		return_num++;
		code_lc++;
	}
	else if (!strcmp(Code[code_lc].order, "EXP"))
	{
		code_lc = lc;
	}
	else if (!strcmp(Code[code_lc].order, "EXF"))
	{
		if (return_flag[return_num-1])	//有返回值
		{
			S[abp] = S[etp - 1];	//写返回值
			code_lc = S[abp + 2];	//跳转至之前代码的目标地址
			etp = abp + 1;			//之前的元素退栈
			abp = S[abp + 1];		//回到上一层的基地址
			return_num--;
		}
		else
		{
			code_lc = S[abp + 2];	//跳转至之前代码的目标地址
			etp = abp;			//之前的元素退栈
			abp = S[abp + 1];		//回到上一层的基地址
		}
	}
	else if (!strcmp(Code[code_lc].order, "EQR"))
	{
		if (S[etp - 1] == S[etp - 2])
		{
			S[etp - 2] = 1;
			code_lc++;
			etp--;
		}
		else
		{
			S[etp - 2] = 0;
			code_lc++;
			etp--;
		}
	}
	else if (!strcmp(Code[code_lc].order, "NER"))
	{
		if (S[etp - 1] != S[etp - 2])
		{
			S[etp - 2] = 1;
			code_lc++;
			etp--;
		}
		else
		{
			S[etp - 2] = 0;
			code_lc++;
			etp--;
		}
	}
	else if (!strcmp(Code[code_lc].order, "LSR"))
	{
		if (S[etp - 2] < S[etp - 1])
		{
			S[etp - 2] = 1;
			code_lc++;
			etp--;
		}
		else
		{
			S[etp - 2] = 0;
			code_lc++;
			etp--;
		}
	}
	else if (!strcmp(Code[code_lc].order, "LER"))
	{
		if (S[etp - 2] <= S[etp - 1])
		{
			S[etp - 2] = 1;
			code_lc++;
			etp--;
		}
		else
		{
			S[etp - 2] = 0;
			code_lc++;
			etp--;
		}
	}
	else if (!strcmp(Code[code_lc].order, "GTR"))
	{
		if (S[etp - 2] > S[etp - 1])
		{
			S[etp - 2] = 1;
			code_lc++;
			etp--;
		}
		else
		{
			S[etp - 2] = 0;
			code_lc++;
			etp--;
		}
	}
	else if (!strcmp(Code[code_lc].order, "GER"))
	{
		if (S[etp - 2] >= S[etp - 1])
		{
			S[etp - 2] = 1;
			code_lc++;
			etp--;
		}
		else
		{
			S[etp - 2] = 0;
			code_lc++;
			etp--;
		}
	}
	else if (!strcmp(Code[code_lc].order, "JPC"))
	{
		if (S[etp - 1] == 0)
		{
			code_lc = Code[code_lc].y;
			etp--;
		}
		else
		{
			code_lc++;
			etp--;
		}
	}
}
int main()
{
	int i = 1;
	char filename[100];
	c = ' ';
	scanf("%s", &filename);
	fp1 = fopen(filename, "r");
	//fp2 = fopen("D:/14061098_result.txt", "w");
	while (flag)
		syntax();
	printf("Program is completed\n");
	/*for (i = 0;i < tnum;i++)
	{
		fprintf(fp2, "%d----名字域：%s 性质：%d 类型：%s 声明行：%d 函数区域：%d 相对地址或值：%d \n形参数:%d 数组元素类型:%s 数组大小:%d 函数大小:%d 代码开始行:%d\n", i, tab[i].name, tab[i].object, tab[i].type, tab[i].decline, tab[i].position, tab[i].adr, tab[i].Para, tab[i].el_type, tab[i].el_top, tab[i].func_size, tab[i].func_code_line);
		fprintf(fp2, "----------------------------------------------------\n");
	}*/
	//for (i = 0;i < rnum;i++)
	//printf("实数表的值：%f\n", rconst[i]);
	strcpy(Code[0].order, "INT");
	Code[0].f = 9;
	Code[0].y = Var_num;
	strcpy(Code[1].order, "JMP");
	Code[1].f = 10;
	Code[1].y = main_lc;
	for (i = 0;i < lc;i++)
	{
		if (Code[i].f >= 0 && Code[i].f <= 3 || Code[i].f == 19)
			fprintf(fp2, "%d-%s %d %d\n", i, Code[i].order, Code[i].x, Code[i].y);
		else if (Code[i].f >= 8 && Code[i].f <= 30 && Code[i].f != 18 && Code[i].f != 19 && Code[i].f != 22)
			fprintf(fp2, "%d-%s   %d\n", i, Code[i].order, Code[i].y);
		else if (Code[i].f >= 31 && Code[i].f <= 63 || Code[i].f == 18 || Code[i].f == 22)
			fprintf(fp2, "%d-%s \n", i, Code[i].order);
	}
	while (code_lc != lc)	//解释执行
		interpret();
	return 0;
}
