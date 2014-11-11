//headfile
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
char *Mem;//主存起始地址指针
int *Reg;//通用寄存器文件起始地址指针
long pc = 0;//模拟pc寄存器
unsigned int ins_pc[100];//保存内存指令编码与对应的pc值，用于指令跳转时对比pc

//定义32个通用寄存器地址，寄存器reg[1] = "r1"，所以r1的标号就是1，即所有寄存器的标号就是数组下标。第0号地址不放数据。
//另外两个寄存器分别是：EPC,Cause_Reg
char *reg[35] = {"None", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", "r16", 
				"r17", "r18", "r19", "r20", "r21", "r22", "r23", "r24", "rd1", "rd2", "rs1", "rs2", "rt1", "rt2", "lo", "hi",
				"EPC", "Cause_Reg"};

//指令格式类型分类，分别是R,J,I型指令格式，比如ins[0][0]就是指add地址，ins[2][1]就是指jal地址，第0号地址不放数据。数组第二个大小设为31，对于R指令还有一个没有存储内容，系统会设置为NULL，为下面判断埋下伏笔。
#define R 0
#define I 1
#define J 2
char *ins[3][31] = {{"None", "add", "addu", "sub", "subu", "mul", "mulu", "div", "divu", "slt", "sltu", "and", "or", "nor", "xor", "jr", "jalr", "nop", "mfhi", "mflo", "mfepc", "mfco", "add.s", "add.d", "sub.s", "sub.d", "mul.s", "mul.d", "div.s", "div.d"},
					{"None", "addi", "addiu", "slti", "sltiu", "andi", "ori", "xori", "lw", "sw", "lbu", "lb", "sb", "lui", "beq", "bne", "blez", "bgtz", "bltz", "lwcl", "swcl"},
					{"None", "j", "jal"}};


/*
	模拟的64KB内存	
*/
char * Memory()
{
	const int Memory_size = 0x10000;//申请一个64KB大小内存
	Mem = (char*)malloc(sizeof(char) * Memory_size);
	if(Mem == NULL)
	{
		printf("Memory allocation error!\n\n");
		exit(-1);
	}
	else
	{
		printf("内存初始化完成!\n\n");
	}
	return Mem;
}

/*
	模拟32个32位寄存器+EPC寄存器+Cause_Reg寄存器
*/
int *Register()
{
	int Register_size = 35;//通用寄存器数目,与上面寄存器编号对应，所以第一个不用
	Reg = (int*)malloc(sizeof(int) * Register_size);
	if(Reg == NULL)
	{
		printf("Register allocation error!\n\n");
		exit(-1);
	}
	else
	{
		printf("寄存器初始化完成!\n\n");
	}

	//初始化所有寄存器为0
	for(; --Register_size != 0; Register_size)
	{
		*((int *)(Reg + Register_size)) = 0;
	}

	return Reg;
}

/**********************************************************************************************************/
/*
	无符号字节读取
	addr:访存地址
*/
unsigned char read_mem_ubyte(long addr)
{
	return *((unsigned char *)(Mem + addr));
}
/*
	无符号字节写入
	addr:写入地址
	data:写入数据
*/
void write_mem_ubyte(long addr, unsigned char data)
{
	*((unsigned char *)(Mem + addr)) = data;
}
/**********************************************************************************************************/
/*
	有符号字节读取
	addr:访存地址
*/
signed char read_mem_byte(long addr)
{
	return *((signed char *)(Mem + addr));
}
/*
	有符号字节写入
	addr:写入地址
	data:写入数据
*/
void write_mem_byte(long addr, signed char data)
{
	*((signed char *)(Mem + addr)) = data;
}
/**********************************************************************************************************/
/*
	无符号半字读取
	addr:访存地址
*/
unsigned short int read_mem_uhalfword(long addr)
{
	return *((unsigned short int *)(Mem + addr));
}
/*
	无符号半字写入
	addr:写入地址
	data:写入数据
*/
void write_mem_uhalfword(long addr, unsigned short int data)
{
	*((unsigned short int *)(Mem + addr)) = data;
}
/**********************************************************************************************************/
/*
	有符号半字读取
	addr:访存地址
*/
short int read_mem_halfword(long addr)
{
	return *((short int *)(Mem + addr));
}
/*
	有符号半字写入
	addr:写入地址
	data:写入数据
*/
void write_mem_halfword(long addr, short int data)
{
	*((short int *)(Mem + addr)) = data;
}
/**********************************************************************************************************/
/*
	无符号字读取
	addr:访存地址
*/
unsigned int read_mem_uword(long addr)
{
	return *((unsigned int *)(Mem + addr));
}
/*
	无符号字写入
	addr:写入地址
	data:写入数据
*/
void write_mem_uword(long addr, unsigned int data)
{
	*((unsigned int *)(Mem + addr)) = data;
}
/**********************************************************************************************************/
/*
	有符号字读取
	addr:访存地址
*/
int read_mem_word(long addr)
{
	return *((int *)(Mem + addr));
}
/*
	有符号字写入
	addr:写入地址
	data:写入数据
*/
void write_mem_word(long addr, int data)
{
	*((int *)(Mem + addr)) = data;
}
/**********************************************************************************************************/
/*
	单精度浮点数读取
	addr:访存地址
*/
float read_mem_float(long addr)
{
	return *((float *)(Mem + addr));
}
/*
	单精度浮点数写入
	addr:写入地址
	data:写入数据
*/
void write_mem_float(long addr, float data)
{
	*((float *)(Mem + addr)) = data;
}
/**********************************************************************************************************/
/*
	双精度浮点数读取
	addr:访存地址
*/
double read_mem_double(long addr)
{
	return *((double *)(Mem + addr));
}
/*
	双精度浮点数写入
	addr:写入地址
	data:写入数据
*/
void write_mem_double(long addr, double data)
{
	*((double *)(Mem + addr)) = data;
}
/**********************************************************************************************************/
/*
	无符号字节读取
	addr:寄存器地址
*/
unsigned char get_ubyte(long addr)
{
	return *((unsigned char *)(Reg + addr));
}
/*
	无符号字节写入
	addr:写入地址
	data:写入数据
*/
void put_ubyte(long addr, unsigned char data)
{
	*((unsigned char *)(Reg + addr)) = data;
}
/**********************************************************************************************************/
/*
	有符号字节读取
	addr:寄存器地址
*/
signed char get_byte(long addr)
{
	return *((signed char *)(Reg + addr));
}
/*
	有符号字节写入
	addr:写入地址
	data:写入数据
*/
void put_byte(long addr, signed char data)
{
	*((signed char *)(Reg + addr)) = data;
}
/**********************************************************************************************************/
/*
	无符号半字读取
	addr:寄存器地址
*/
unsigned short int get_uhalfword(long addr)
{
	return *((unsigned short int *)(Reg + addr));
}
/*
	无符号半字写入
	addr:写入地址
	data:写入数据
*/
void put_uhalfword(long addr, unsigned short int data)
{
	*((unsigned short int *)(Reg + addr)) = data;
}
/**********************************************************************************************************/
/*
	有符号半字读取
	addr:寄存器地址
*/
short int get_halfword(long addr)
{
	return *((short int *)(Reg + addr));
}
/*
	有符号半字写入
	addr:写入地址
	data:写入数据
*/
void put_halfword(long addr, short int data)
{
	*((short int *)(Reg + addr)) = data;
}
/**********************************************************************************************************/
/*
	无符号字读取
	addr:寄存器地址
*/
unsigned int get_uword(long addr)
{
	return *((unsigned int *)(Reg + addr));
}
/*
	无符号字写入
	addr:写入地址
	data:写入数据
*/
void put_uword(long addr, unsigned int data)
{
	*((unsigned int *)(Reg + addr)) = data;
}
/**********************************************************************************************************/
/*
	有符号字读取
	addr:寄存器地址
*/
int get_word(long addr)
{
	return *((int *)(Reg + addr));
}
/*
	有符号字写入
	addr:写入地址
	data:写入数据
*/
void put_word(long addr, int data)
{
	*((int *)(Reg + addr)) = data;
}
/**********************************************************************************************************/
/*
	单精度浮点数读取
	addr:寄存器地址
*/
float get_float(long addr)
{
	return *((float *)(Reg + addr));
}
/*
	单精度浮点数写入
	addr:写入地址
	data:写入数据
*/
void put_float(long addr, float data)
{
	*((float *)(Reg + addr)) = data;
}
/**********************************************************************************************************/
/*
	双精度浮点数读取
	addr:寄存器地址
*/
double get_double(long addr)
{
	return *((double *)(Reg + addr));
}
/*
	双精度浮点数写入
	addr:写入地址
	data:写入数据
*/
void put_double(long addr, double data)
{
	*((double *)(Reg + addr)) = data;
}
/**********************************************************************************************************/
/*
	将立即数的16进制的字符串转成unsigned int输出，比如"#1004"--->1004
	要将第一个字符过滤掉
*/
unsigned int string_to_int(char * str)
{
	unsigned int temp = 0;
	int len = strlen(str);
	int i = 1;//用于每次取数时的运算
	while(str[len - 1] != '#')//过滤第一个'#'
	{
		if(str[len - 1] >= '0' && str[len - 1] <= '9')//16进制可能会出现字母，比如0x10a1
		{
			temp += (str[len - 1] - '0') * i;
			--len;
			i *= 16;
		}
		else
		{
			temp += (str[len - 1] - 'a' + 10) * i;
			--len;
			i *= 16;
		}

	}
	printf("16进制立即数：%s 转成unsigned int型为：0x%x\n\n",str, temp);
	return temp;
}


/*
	判断mips指令的指令类型，是R型指令格式，I型指令格式还是J型指令格式。
	对mips指令进行编码，存储到内存中
*/
unsigned int mips_instruction_code(char *line)
{
	char *token = NULL;//记录每行的分割单词
	int num = 1;//记录是第几个token，只要是用于下面的if
	int type = -1;  //type用来记录类型，0表示R型，1表示I型，2表示J型，-1表示无效
	int i;//i记录的数据就是该指令操作码的编码
	int temp;//在去左括号中用到的临时变量
	int flag = 0;//用来标记是否出现过“#”和“（”等符号
	unsigned int code;
	printf("********************************************************\n");
	printf("汇编指令 : %s\n",line);
	token = strtok(line, " ,");

	//生成指令的二进制编码
	while(token != NULL)
	{
		if(num == 1 && token != "")//只在第一个token判断指令类型
		{
			token[strlen(token) - 1] == '\n' ? token[strlen(token) - 1] = 0 : 0;//去除换行符

			for(i = 0; ins[R][i] != NULL && type == -1; ++i)//在R型指令中查找，R已经在宏定义中定义
			{
				if(strcmp(ins[R][i], token) == 0)
				{
					type = 0;
					//printf("%s %d\n",ins[R][i], i);
					//printf("%s\n",token);
					code = i;//指令编码，操作数编码
					break;
				}
			
			}

			for(i = 0; ins[I][i] != NULL && type == -1; ++i)//在I型指令中查找，I已经在宏定义中定义
			{
				if(strcmp(ins[I][i], token) == 0)
				{
					type = 1;
					//printf("%s %d\n",ins[I][i], i);
					//printf("%s\n",token);
					code = (unsigned int)(i * pow(2, 26));//指令编码，操作数编码
					break;
				}
			}
			for(i = 0; ins[J][i] != NULL && type == -1; ++i)//在J型指令中查找，J已经在宏定义中定义
			{
				if(strcmp(ins[J][i], token) == 0)
				{
					type = 2;
					//printf("%s %d\n",ins[J][i], i);
					//printf("%s\n",token);
					code = (unsigned int)(i * pow(2, 26));//指令编码，操作数编码
					break;
				}
			}
		}
		else if(num == 2 && token != "")//对第二个操作数进行编码
		{
			token[strlen(token) - 1] == '\n' ? token[strlen(token) - 1] = 0 : 0;//去除换行符

			for(i = 0; reg[i] != NULL; ++i)//在寄存器数组中查找该寄存器位置，i就是它们最终的标号
			{
				if(strcmp(reg[i], token) == 0)
				{
					//printf("%s 标号：%d\n",reg[i], i);
					//printf("%s\n",token);
					break;
				}
			}

			switch(type)
			{
				case R:
				case I:
				{
					code = code + (unsigned int)(i * pow(2, 21));
					break;
				}
				case J:
				{
					code = code + i;
					break;
				}
			}
		}
		else if(num == 3 && token != "")//对第三个操作数进行编码
		{
			token[strlen(token) - 1] == '\n' ? token[strlen(token) - 1] = 0 : 0;//去除换行符
			if(token[0] != '#' && token[0] != '(' )//判断是否为立即数，或寄存器间接选址(rs1)
			{
				for(i = 0; reg[i] != NULL; ++i)//在寄存器数组中查找该寄存器位置，i就是它们最终的标号
				{
					if(strcmp(reg[i], token) == 0)
					{
						//printf("%s 标号：%d\n",reg[i], i);
						//printf("%s\n",token);
						break;
					}
				}
			}
			else
			{
				flag = 1;//更改标记
				if(token[0] == '#')//是立即数,用16进制存储的
				{
					//将字符串"#1000"转成unsigned int 类型的1000
					code = code + string_to_int(token);
				}
				else//寄存器间接选址(rs1)，读取寄存器内容存到rs1的位置
				{
					//先去括号
					token[strlen(token) - 1] = 0;//去右括号,这种赋值的方式只对右边有用，所以要想消去左边的括号必须移位
					temp = 0;
					while(temp < strlen(token) - 1)//去左括号
					{
						token[temp] = token[temp + 1];
						++temp;
					}
					token[strlen(token) - 1] = 0;//去掉右边多余的字符


					for(i = 0; reg[i] != NULL; ++i)//在寄存器数组中查找该寄存器位置，i就是它们最终的标号
					{
						if(strcmp(reg[i], token) == 0)
						{
							//printf("%s 标号：%d\n",reg[i], i);
							//printf("%s\n",token);
							break;
						}
					}
					//printf("************%x %d %s\n",code, (unsigned int)(i * pow(2, 16)), token);
					code = code + (unsigned int)(i * pow(2, 16));

				}

			}

			switch(type)
			{
				case R:
				{
					code = code + (unsigned int)(i * pow(2, 16));
					break;
				}
				case I:
				{
					if(flag == 1)//出现过"#"和"("
					{
						code = code;
						flag = 0;
					}
					else
					{
						code = code + (unsigned int)(i * pow(2, 16));	
					}

					break;
				}
			}
		}
		else if(num == 4 && token != "")//对第四个操作数进行编码
		{
			token[strlen(token) - 1] == '\n' ? token[strlen(token) - 1] = 0 : 0;//去除换行符
			if(token[0] != '#')//判断是否为立即数,已经不会有上面(r1)这种寄存器访问内存的方式了
			{
				for(i = 0; reg[i] != NULL; ++i)//在寄存器数组中查找该寄存器位置，i就是它们最终的标号
				{
					if(strcmp(reg[i], token) == 0)
					{
						//printf("%s 标号：%d\n",reg[i], i);
						//printf("%s\n",token);
						break;
					}
				}
			}
			else
			{
					//将字符串"#1000"转成unsigned int 类型的1000
				code = code + string_to_int(token);
			}


			switch(type)
			{
				case R:
				{
					code = code + (unsigned int)(i * pow(2, 11));
					break;
				}
				case I://
				{
					code = code;
					break;
				}
			}
		}
		else
		{
			
		}
		token = strtok(NULL, " ,");
		++num;
	}
	printf("原指令编码为：0x%x\n\n",code);
	return code;
}

/*
	将小端转成大端,或小端转成大端，传入大端会自动转成小端，传入的是小端会自动转成大端，原理是一样的。
*/
unsigned int little_exch_big(unsigned int data)
{
	unsigned int temp = 0;
	int i;
	for(i = 0; i <4; i++)//要操作4个字节
	{
		temp = temp * 0x100 + data % 0x100;
		data = data / 0x100;
	}
	printf("大小端转换后指令编码为：0x%x\n\n",temp);
	return temp;
}

/*
	存储指令
*/
void ins_store(char *line)
{
	//对指令编码
	unsigned int code = mips_instruction_code(line);
	ins_pc[pc/4] = code;
	//转成大端格式
	code = little_exch_big(code);
	*((unsigned int *)(Mem + pc)) = code;
	printf("PC=%d, 内存存储指令成功!\n\n", pc);
	pc = pc + 4;
	return;
}

/*
	加载指令
*/
unsigned int ins_load()
{
	unsigned int code = *((unsigned int*)(Mem + pc));
	//转成大端格式
	code = little_exch_big(code);
	printf("内存取指 0x%x 成功!\n\n", code);
	return code;
}


void INSN_ADDU(unsigned int ins)
{
	int rd = ins / 0x800 % 0x20;  //先截去后面的11位，再截取5位
	unsigned int rs1 = get_uword(ins / 0x200000 % 0x20);//先截去后面的21位，再截取5位
	unsigned int rs2 = get_uword(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	put_uword(rd, rs1 + rs2);
	pc = pc + 4;
	printf("执行ADDU指令：rs1=%ud, rs2=%ud, 目的寄存器r%d=%ud\n\n",rs1, rs2, rd, get_uword(rd));
}

void INSN_ADD(unsigned int ins)
{
	int rd = ins / 0x800 % 0x20;  //先截去后面的11位，再截取5位
	int rs1 = get_word(ins / 0x200000 % 0x20);//先截去后面的21位，再截取5位
	int rs2 = get_word(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	put_word(rd, rs1 + rs2);
	pc = pc + 4;
	printf("执行ADD指令：rs1=%d, rs2=%d, 目的寄存器r%d=%d\n\n",rs1, rs2, rd, get_word(rd));
}

void INSN_SUB(unsigned int ins)
{
	int rd = ins / 0x800 % 0x20;  //先截去后面的11位，再截取5位
	int rs1 = get_word(ins / 0x200000 % 0x20);//先截去后面的21位，再截取5位
	int rs2 = get_word(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	put_word(rd, rs1 - rs2);
	pc = pc + 4;
	printf("执行SUB指令：rs1=%d, rs2=%d, 目的寄存器r%d=%d\n\n",rs1, rs2, rd, get_word(rd));
}

void INSN_SUBU(unsigned int ins)
{
	int rd = ins / 0x800 % 0x20;  //先截去后面的11位，再截取5位
	unsigned int rs1 = get_uword(ins / 0x200000 % 0x20);//先截去后面的21位，再截取5位
	unsigned int rs2 = get_uword(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	put_uword(rd, rs1 - rs2);
	pc = pc + 4;
	printf("执行SUBU指令：rs1=%ud, rs2=%ud, 目的寄存器r%d=%ud\n\n",rs1, rs2, rd, get_uword(rd));
}

void INSN_MUL(unsigned int ins)
{
	int rs1 = get_word(ins / 0x200000 % 0x20);//先截去后面的21位，再截取5位
	int rs2 = get_word(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	double a = rs1;//转成double才能计算出结果
	double b = rs2;
	double result = a * b;
	a = result / 0x100000000;//截取高位保存到32号寄存器hi
	b = result - (int)a * 0x100000000;//截取低位保存到31号寄存器lo,浮点数没有取余操作
	put_uword(31, (int)b);//低位用无符号来存
	put_word(32, (int)a);//result=a*0x100000000+b;
	pc = pc + 4;
	printf("执行MUL指令：rs1=%d, rs2=%d, 结算结果为:%.0f, 目的寄存器hi=%d, lo=%ud\n\n",rs1, rs2, result, get_word(32), get_uword(31));
}

void INSN_MULU(unsigned int ins)
{
	unsigned int rs1 = get_word(ins / 0x200000 % 0x20);//先截去后面的21位，再截取5位
	unsigned int rs2 = get_word(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	double a = rs1;//转成double才能计算出结果
	double b = rs2;
	double result = a * b;
	a = result / 0x100000000;//截取高位保存到32号寄存器hi
	b = result - (int)a * 0x100000000;//截取低位保存到31号寄存器lo,浮点数没有取余操作
	put_uword(31, (int)b);//低位用无符号来存
	put_uword(32, (int)a);//result=a*0x100000000+b;
	pc = pc + 4;
	printf("执行MULU指令：rs1=%d, rs2=%d, 结算结果为:%.0f, 目的寄存器hi=%d, lo=%ud\n\n",rs1, rs2, result, get_uword(32), get_uword(31));
}

void INSN_DIV(unsigned int ins)
{
	int rs1 = get_word(ins / 0x200000 % 0x20);//先截去后面的21位，再截取5位
	int rs2 = get_word(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	put_word(31, rs1/rs2);//低位用无符号来存
	put_word(32, rs1%rs2);//result=a*0x100000000+b;
	pc = pc + 4;
	printf("执行DIV指令：rs1=%d, rs2=%d, 目的寄存器hi=%d, lo=%d\n\n",rs1, rs2, get_word(32), get_word(31));
}

void INSN_DIVU(unsigned int ins)
{
	unsigned int rs1 = get_word(ins / 0x200000 % 0x20);//先截去后面的21位，再截取5位
	unsigned int rs2 = get_word(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	put_uword(31, rs1/rs2);//低位用无符号来存
	put_uword(32, rs1%rs2);//result=a*0x100000000+b;
	pc = pc + 4;
	printf("执行DIVU指令：rs1=%ud, rs2=%ud, 目的寄存器hi=%ud, lo=%ud\n\n",rs1, rs2, get_uword(32), get_uword(31));
}

void INSN_SLT(unsigned int ins)
{
	int rt = get_word(ins / 0x800 % 0x20);  //先截去后面的11位，再截取5位
	int rd = ins / 0x200000 % 0x20;//先截去后面的21位，再截取5位
	int rs = get_word(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	if(rs < rt)
	{
		put_word(rd, 1);
	}
	else
	{
		put_word(rd, 0);
	}
	pc = pc + 4;
	printf("执行SLT指令：rs=%d, rt=%d, 目的寄存器rd=%d\n\n",rs, rt, get_word(rd));
}

void INSN_SLTU(unsigned int ins)
{
	unsigned int rt = get_word(ins / 0x800 % 0x20);  //先截去后面的11位，再截取5位
	int rd = ins / 0x200000 % 0x20;//先截去后面的21位，再截取5位
	unsigned int rs = get_word(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	if(rs < rt)
	{
		put_word(rd, 1);
	}
	else
	{
		put_word(rd, 0);
	}
	pc = pc + 4;
	printf("执行SLTU指令：rs=%ud, rt=%ud, 目的寄存器rd=%d\n\n",rs, rt, get_word(rd));
}

void INSN_AND(unsigned int ins)
{
	int rt = get_word(ins / 0x800 % 0x20);  //先截去后面的11位，再截取5位
	int rd = ins / 0x200000 % 0x20;//先截去后面的21位，再截取5位
	int rs = get_word(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	put_word(rd, rs & rt); 
	pc = pc + 4;
	printf("执行AND指令：rs=0x%x, rt=0x%x, 目的寄存器rd=0x%x\n\n",rs, rt, get_word(rd));
}

void INSN_OR(unsigned int ins)
{
	int rt = get_word(ins / 0x800 % 0x20);  //先截去后面的11位，再截取5位
	int rd = ins / 0x200000 % 0x20;//先截去后面的21位，再截取5位
	int rs = get_word(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	put_word(rd, rs | rt); 
	pc = pc + 4;
	printf("执行OR指令：rs=0x%x, rt=0x%x, 目的寄存器rd=0x%x\n\n",rs, rt, get_word(rd));
}

void INSN_NOR(unsigned int ins)
{
	int rt = get_word(ins / 0x800 % 0x20);  //先截去后面的11位，再截取5位
	int rd = ins / 0x200000 % 0x20;//先截去后面的21位，再截取5位
	int rs = get_word(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	put_word(rd, !(rs | rt)); 
	pc = pc + 4;
	printf("执行NOR指令：rs=0x%x, rt=0x%x, 目的寄存器rd=0x%x\n\n",rs, rt, get_word(rd));
}

void INSN_XOR(unsigned int ins)
{
	int rt = get_word(ins / 0x800 % 0x20);  //先截去后面的11位，再截取5位
	int rd = ins / 0x200000 % 0x20;//先截去后面的21位，再截取5位
	int rs = get_word(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	put_word(rd, rs ^ rt); 
	pc = pc + 4;
	printf("执行XOR指令：rs=0x%x, rt=0x%x, 目的寄存器rd=0x%x\n\n",rs, rt, get_word(rd));
}

void INSN_ADDI(unsigned int ins) 
{
	int rs = get_word(ins / 0x10000 % 0x20);  //先截去后面的16位，再截取5位
	int rt = ins / 0x200000 % 0x20;//先截去后面的21位，再截取5位,需要存储的数据
	int imm = (ins % 0x8000);//得到最后15立即数
	int sign = (ins % 0x10000) / 0x8000;//得到这个数的符号
	sign == 1 ? put_word(rt, rs - imm) : put_word(rt, rs + imm);//根据符号好存数
	sign == 1 ? printf("执行ADDI指令：rs=%d, 立即数=-%d, rt=%d \n\n",rs , imm, get_word(rt))
		: printf("执行ADDI指令：rs=%d, 立即数=%d, rt=%d \n\n",rs , imm, get_word(rt));
	pc = pc + 4;
}

void INSN_ADDIU(unsigned int ins) 
{
	unsigned int rs = get_uword(ins / 0x10000 % 0x20);  //先截去后面的16位，再截取5位
	int rt = ins / 0x200000 % 0x20;//先截去后面的21位，再截取5位,需要存储的数据
	int imm = (ins % 0x10000);//得到最后16立即数
	put_uword(rt, rs + imm);
	printf("执行ADDIU指令：rs=%ud, 立即数=%ud, rt=%ud \n\n",rs , imm, get_uword(rt));
	pc = pc + 4;
}

void INSN_SLTI(unsigned int ins) 
{
	int rs = get_word(ins / 0x10000 % 0x20);  //先截去后面的16位，再截取5位
	int rt = ins / 0x200000 % 0x20;//先截去后面的21位，再截取5位,需要存储的数据
	int imm = (ins % 0x8000);//得到最后15立即数
	int sign = (ins % 0x10000) / 0x8000;//得到这个数的符号
	sign == 1 ? imm = -imm : imm;//根据判断立即数符号
	rs < imm ? put_word(rt, 1) : put_word(rt, 0);
	printf("执行SLTI指令：rs=%d, 立即数=%d, rt=%d \n\n",rs , imm, get_word(rt));
	pc = pc + 4;
}

void INSN_SLTIU(unsigned int ins) 
{
	unsigned int rs = get_uword(ins / 0x10000 % 0x20);  //先截去后面的16位，再截取5位
	int rt = ins / 0x200000 % 0x20;//先截去后面的21位，再截取5位,需要存储的数据
	int imm = (ins % 0x10000);//得到最后16立即数
	rs < imm ? put_uword(rt, 1) : put_uword(rt, 0);
	printf("执行SLTIU指令：rs=%ud, 立即数=%ud, rt=%ud \n\n",rs , imm, get_uword(rt));
	pc = pc + 4;
}

void INSN_ANDI(unsigned int ins) 
{
	unsigned int rs = get_uword(ins / 0x10000 % 0x20);  //先截去后面的16位，再截取5位
	int rt = ins / 0x200000 % 0x20;//先截去后面的21位，再截取5位,需要存储的数据
	int imm = (ins % 0x8000);//得到最后15立即数
	 put_uword(rt, imm & rs);
	printf("执行ANDI指令：rs=0x%x, 立即数=0x%x, rt=0x%x \n\n",rs , imm, get_uword(rt));
	pc = pc + 4;
}

void INSN_ORI(unsigned int ins) 
{
	unsigned int rs = get_uword(ins / 0x10000 % 0x20);  //先截去后面的16位，再截取5位
	int rt = ins / 0x200000 % 0x20;//先截去后面的21位，再截取5位,需要存储的数据
	int imm = (ins % 0x8000);//得到最后15立即数
	 put_uword(rt, imm | rs);
	printf("执行ORI指令：rs=0x%x, 立即数=0x%x, rt=0x%x \n\n",rs , imm, get_uword(rt));
	pc = pc + 4;
}

void INSN_XORI(unsigned int ins) 
{
	unsigned int rs = get_uword(ins / 0x10000 % 0x20);  //先截去后面的16位，再截取5位
	int rt = ins / 0x200000 % 0x20;//先截去后面的21位，再截取5位,需要存储的数据
	int imm = (ins % 0x8000);//得到最后15立即数
	 put_uword(rt, imm ^ rs);
	printf("执行XORI指令：rs=0x%x, 立即数=0x%x, rt=0x%x \n\n",rs , imm, get_uword(rt));
	pc = pc + 4;
}

void INSN_LBU(unsigned int ins)
{
	int rt = ins / 0x200000 % 0x20;  //先截去后面的21位，再截取5位
	int rs = get_word(ins / 0x10000 % 0x20);
	int imm = (ins % 0x8000);//得到最后15立即数
	int sign = (ins % 0x10000) / 0x8000;//得到这个数的符号
	sign == 1 ? imm = -imm : imm;//根据判断立即数符号
	put_ubyte(rt, read_mem_ubyte(rs + imm));
	printf("执行LBU指令：rs=%d, 内存地址=0x%x, 内存数据=%d, 目的寄存器rt=%d\n\n", rs, rs + imm, read_mem_ubyte(rs + imm), get_ubyte(rt));
	pc = pc + 4;
}

void INSN_LB(unsigned int ins)
{
	int rt = ins / 0x200000 % 0x20;  //先截去后面的21位，再截取5位
	int rs = get_word(ins / 0x10000 % 0x20);
	int imm = (ins % 0x8000);//得到最后15立即数
	int sign = (ins % 0x10000) / 0x8000;//得到这个数的符号
	sign == 1 ? imm = -imm : imm;//根据判断立即数符号
	put_byte(rt, read_mem_byte(rs + imm));
	printf("执行LB指令：rs=%d, 内存地址=0x%x, 内存数据=%d, 目的寄存器rt=%d\n\n", rs, rs + imm, read_mem_byte(rs + imm), get_byte(rt));
	pc = pc + 4;
}

void INSN_SB(unsigned int ins)
{
	int rt = get_byte(ins / 0x200000 % 0x20);  //先截去后面的21位，再截取5位
	int rs = get_word(ins / 0x10000 % 0x20);
	int imm = (ins % 0x8000);//得到最后15立即数
	int sign = (ins % 0x10000) / 0x8000;//得到这个数的符号
	sign == 1 ? imm = -imm : imm;//根据判断立即数符号
	write_mem_byte(rs + imm, rt);
	printf("执行SB指令：rs=%d, 内存地址=0x%x, 内存数据=%d, 目的寄存器rt=%d\n\n", rs, rs + imm, read_mem_byte(rs + imm), rt);
	pc = pc + 4;
}

void INSN_LUI(unsigned int ins)
{
	int rt = ins / 0x200000 % 0x20;  //先截去后面的21位，再截取5位
	int imm = (ins % 0x10000);//得到最后15立即数
	put_word(rt, imm * 0x10000);
	printf("执行LUI指令: 目的寄存器rt=0x%x, 立即数=0x%x\n\n", get_word(rt), imm);
	pc = pc + 4;
}

void INSN_BEQ(unsigned int ins)
{
	int rt = get_word(ins / 0x200000 % 0x20);  //先截去后面的21位，再截取5位
	int rs = get_word(ins / 0x10000 % 0x20);
	int imm = (ins % 0x8000);//得到最后15立即数
	int sign = (ins % 0x10000) / 0x8000;//得到这个数的符号
	sign == 1 ? imm = -imm : imm;//根据判断立即数符号
	rt == rs ? (pc = pc + imm * 4) : (pc = pc + 4);
	printf("执行BEQ指令：rs=%d, 目的寄存器rt=%d, pc地址=%d\n\n", rs, rt, pc);

}

void INSN_BNE(unsigned int ins)
{
	int rt = get_word(ins / 0x200000 % 0x20);  //先截去后面的21位，再截取5位
	int rs = get_word(ins / 0x10000 % 0x20);
	int imm = (ins % 0x8000);//得到最后15立即数
	int sign = (ins % 0x10000) / 0x8000;//得到这个数的符号
	sign == 1 ? imm = -imm : imm;//根据判断立即数符号
	rt != rs ? (pc = pc + imm * 4) : (pc = pc + 4);
	printf("执行BNE指令：rs=%d, 目的寄存器rt=%d, pc地址=%d\n\n", rs, rt, pc);
}

void INSN_BLEZ(unsigned int ins)
{
	int rt = get_word(ins / 0x200000 % 0x20);  //先截去后面的21位，再截取5位
	int imm = (ins % 0x8000);//得到最后15立即数
	int sign = (ins % 0x10000) / 0x8000;//得到这个数的符号
	sign == 1 ? imm = -imm : imm;//根据判断立即数符号
	rt <= 0 ? (pc = pc + imm * 4) : (pc = pc + 4);
	printf("执行BLEZ指令：目的寄存器rt=%d, pc地址=%d\n\n", rt, pc);
}

void INSN_BGTZ(unsigned int ins)
{
	int rt = get_word(ins / 0x200000 % 0x20);  //先截去后面的21位，再截取5位
	int imm = (ins % 0x8000);//得到最后15立即数
	int sign = (ins % 0x10000) / 0x8000;//得到这个数的符号
	sign == 1 ? imm = -imm : imm;//根据判断立即数符号
	rt > 0 ? (pc = pc + imm * 4) : (pc = pc + 4);
	printf("执行BGTZ指令：目的寄存器rt=%d, pc地址=%d\n\n", rt, pc);
}

void INSN_BLTZ(unsigned int ins)
{
	int rt = get_word(ins / 0x200000 % 0x20);  //先截去后面的21位，再截取5位
	int imm = (ins % 0x8000);//得到最后15立即数
	int sign = (ins % 0x10000) / 0x8000;//得到这个数的符号
	sign == 1 ? imm = -imm : imm;//根据判断立即数符号
	rt < 0 ? (pc = pc + imm * 4) : (pc = pc + 4);
	printf("执行BLTZ指令：目的寄存器rt=%d, pc地址=%d\n\n", rt, pc);
}

void INSN_JR(unsigned int ins)
{
	int rt = get_word(ins / 0x200000 % 0x20);  //先截去后面的21位，再截取5位
	pc = rt;
	printf("执行JR指令：pc地址=%d\n\n", pc);
}

void INSN_JALR(unsigned int ins)
{
	int rt = ins / 0x200000 % 0x20;  //先截去后面的21位，再截取5位
	int rs = get_word(ins / 0x10000 % 0x20);
	put_word(rt, pc + 4);
	pc = rs;
	printf("执行JALR指令：寄存器pc=%d, pc地址=%d\n\n", get_word(rt), pc);
}

void INSN_NOP(unsigned int ins)
{
	pc = pc + 4;
	printf("执行NOP指令：pc地址=%d\n\n", pc);
}

void INSN_MFHI(unsigned int ins)
{
	int rt = ins / 0x200000 % 0x20;  //先截去后面的21位，再截取5位
	put_word(rt, get_word(32));
	pc = pc + 4;
	printf("执行MFHI指令：rt=%d, pc地址=%d\n\n", get_word(rt), pc);
}

void INSN_MFLO(unsigned int ins)
{
	int rt = ins / 0x200000 % 0x20;  //先截去后面的21位，再截取5位
	put_word(rt, get_word(31));
	pc = pc + 4;
	printf("执行MFLO指令：rt=%d, pc地址=%d\n\n", get_word(rt), pc);
}

void INSN_LWCL(unsigned int ins)
{
	int rt = ins / 0x200000 % 0x20;  //先截去后面的21位，再截取5位
	int rs = get_word(ins / 0x10000 % 0x20);
	int imm = (ins % 0x8000);//得到最后15立即数
	int sign = (ins % 0x10000) / 0x8000;//得到这个数的符号
	sign == 1 ? imm = -imm : imm;//根据判断立即数符号
	put_float(rt, read_mem_float(rs + imm));
	printf("执行LWCL指令：rs=%d, 内存地址=0x%x, 内存数据=%f, 目的寄存器rt=%f\n\n", rs, rs + imm, read_mem_float(rs + imm), get_float(rt));
	pc = pc + 4;
}

void INSN_SWCL(unsigned int ins)
{
	float rt = get_float(ins / 0x200000 % 0x20);  //先截去后面的21位，再截取5位
	int rs = get_word(ins / 0x10000 % 0x20);
	int imm = (ins % 0x8000);//得到最后15立即数
	int sign = (ins % 0x10000) / 0x8000;//得到这个数的符号
	sign == 1 ? imm = -imm : imm;//根据判断立即数符号
	write_mem_float(rs + imm, rt);
	printf("执行SWCL指令：rs=%d, 内存地址=0x%x, 内存数据=%f, 目的寄存器rt=%f\n\n", rs, rs + imm, read_mem_float(rs + imm), rt);
	pc = pc + 4;
}

void INSN_ADDS(unsigned int ins)
{
	int rd = ins / 0x200000 % 0x20;  //先截去后面的21位，再截取5位
	float rs = get_float(ins / 0x800 % 0x20);//先截去后面的11位，再截取5位
	float rt = get_float(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	put_float(rd, rs + rt);
	pc = pc + 4;
	printf("执行ADD.S指令：rs=%f, rt=%f, 目的寄存器rd=%f\n\n",rs, rt, get_float(rd));
}

double binary_to_double(int r1, int r2)//将IEEE754中尾数M部分计算出来
{
	double temp = 0;
	unsigned int index = 0x80000;
	int i;
	for(i = 1; i <= 20; i++)//r2的20位数据
	{
		
		if((r2 & index) != 0)//表示该位不为0
		{
			temp += pow(2, -i);
		}
		index >>= 1;//将index循环右移1位，再赋值给index
		
	}
	index = 0x80000000;
	for(i = 21; i <= 52; i++)//r1的32位数据
	{
		if((r1 & index) != 0)//表示该位不为0
		{
			temp += pow(2, -i);
		}
		index >>= 1;//将index循环右移1位，再赋值给index
	}
	return temp;
}

double two_int_to_double(int r1, int r2)//r2是double前部数据，r1是double后部分数据
{
    int sign = r2 / 0x80000000;     //提取符号位
    int expoent = r2 / 0x100000 % 0x800;    //阶码11表示，阶码用移码表示的
    int temp = r2 % 0x100000;   //把剩下r2中尾数部分提取出来
    double mantissa = binary_to_double(r1, temp);//将r1和r2剩余部分拼接成尾数，尾数是用原码表示的
    double result = (1 + mantissa) * pow(2, expoent - 1023);
	sign == 1 ? (sign = -1) : (sign = 1);
    return result * sign;
}

void INSN_ADDD(unsigned int ins)//这里必须使用我们自己定义的浮点寄存器
{
	int rd1 = ins / 0x200000 % 0x20;  //先截去后面的21位，再截取5位
	int rd2 = ins / 0x200000 % 0x20 + 1;//紧跟在rd1后面的一个寄存器
	int rt1 = get_word(ins / 0x800 % 0x20);//先截去后面的11位，再截取5位
	int rt2 = get_word(ins / 0x800 % 0x20 + 1);
	int rs1 = get_word(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	int rs2 = get_word(ins / 0x10000 % 0x20 + 1);
	double rt = two_int_to_double(rt1, rt2);//根据IEEE754指令格式得到两个寄存器拼凑得到的实际double值
	double rs = two_int_to_double(rs1, rs2);
	double result = rt + rs;
	put_double(rd1, result);//这条指令将result存到rd1及其后续一个寄存器rd2中
	pc = pc + 4;
	printf("执行ADD.D指令：rs=%.10f, rt=%.10f, 目的寄存器rd=%f\n\n",rs, rt, result);
}

void INSN_SUBS(unsigned int ins)
{
	int rd = ins / 0x200000 % 0x20;  //先截去后面的21位，再截取5位
	float rs = get_float(ins / 0x800 % 0x20);//先截去后面的11位，再截取5位
	float rt = get_float(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	put_float(rd, rs - rt);
	pc = pc + 4;
	printf("执行SUB.S指令：rs=%f, rt=%f, 目的寄存器rd=%f\n\n",rs, rt, get_float(rd));
}

void INSN_SUBD(unsigned int ins)//这里必须使用我们自己定义的浮点寄存器
{
	int rd1 = ins / 0x200000 % 0x20;  //先截去后面的21位，再截取5位
	int rd2 = ins / 0x200000 % 0x20 + 1;//紧跟在rd1后面的一个寄存器
	int rt1 = get_word(ins / 0x800 % 0x20);//先截去后面的11位，再截取5位
	int rt2 = get_word(ins / 0x800 % 0x20 + 1);
	int rs1 = get_word(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	int rs2 = get_word(ins / 0x10000 % 0x20 + 1);
	double rt = two_int_to_double(rt1, rt2);//根据IEEE754指令格式得到两个寄存器拼凑得到的实际double值
	double rs = two_int_to_double(rs1, rs2);
	double result = rs - rt;
	put_double(rd1, result);//这条指令将result存到rd1及其后续一个寄存器rd2中
	pc = pc + 4;
	printf("执行SUB.D指令：rs=%.10f, rt=%.10f, 目的寄存器rd=%.20f\n\n",rs, rt, result);
}

void INSN_MULS(unsigned int ins)
{
	int rd = ins / 0x200000 % 0x20;  //先截去后面的21位，再截取5位
	float rs = get_float(ins / 0x800 % 0x20);//先截去后面的11位，再截取5位
	float rt = get_float(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	put_float(rd, rs * rt);
	pc = pc + 4;
	printf("执行MUL.S指令：rs=%f, rt=%f, 目的寄存器rd=%f\n\n",rs, rt, get_float(rd));
}

void INSN_MULD(unsigned int ins)//这里必须使用我们自己定义的浮点寄存器
{
	int rd1 = ins / 0x200000 % 0x20;  //先截去后面的21位，再截取5位
	int rd2 = ins / 0x200000 % 0x20 + 1;//紧跟在rd1后面的一个寄存器
	int rt1 = get_word(ins / 0x800 % 0x20);//先截去后面的11位，再截取5位
	int rt2 = get_word(ins / 0x800 % 0x20 + 1);
	int rs1 = get_word(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	int rs2 = get_word(ins / 0x10000 % 0x20 + 1);
	double rt = two_int_to_double(rt1, rt2);//根据IEEE754指令格式得到两个寄存器拼凑得到的实际double值
	double rs = two_int_to_double(rs1, rs2);
	double result = rs * rt;
	put_double(rd1, result);//这条指令将result存到rd1及其后续一个寄存器rd2中
	pc = pc + 4;
	printf("执行MUL.D指令：rs=%.10f, rt=%.10f, 目的寄存器rd=%.20f\n\n",rs, rt, result);
}

void INSN_DIVS(unsigned int ins)
{
	int rd = ins / 0x200000 % 0x20;  //先截去后面的21位，再截取5位
	float rs = get_float(ins / 0x800 % 0x20);//先截去后面的11位，再截取5位
	float rt = get_float(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	put_float(rd, rs / rt);
	pc = pc + 4;
	printf("执行DIV.S指令：rs=%f, rt=%f, 目的寄存器rd=%f\n\n",rs, rt, get_float(rd));
}

void INSN_DIVD(unsigned int ins)//这里必须使用我们自己定义的浮点寄存器
{
	int rd1 = ins / 0x200000 % 0x20;  //先截去后面的21位，再截取5位
	int rd2 = ins / 0x200000 % 0x20 + 1;//紧跟在rd1后面的一个寄存器
	int rt1 = get_word(ins / 0x800 % 0x20);//先截去后面的11位，再截取5位
	int rt2 = get_word(ins / 0x800 % 0x20 + 1);
	int rs1 = get_word(ins / 0x10000 % 0x20);//先截去后面的16位，再截取5位
	int rs2 = get_word(ins / 0x10000 % 0x20 + 1);
	double rt = two_int_to_double(rt1, rt2);//根据IEEE754指令格式得到两个寄存器拼凑得到的实际double值
	double rs = two_int_to_double(rs1, rs2);
	double result = rs / rt;
	put_double(rd1, result);//这条指令将result存到rd1及其后续一个寄存器rd2中
	pc = pc + 4;
	printf("执行DIV.D指令：rs=%.10f, rt=%.10f, 目的寄存器rd=%.20f\n\n",rs, rt, result);
}

void INSN_MFEPC(unsigned int ins)
{
	int rt = ins / 0x200000 % 0x20;  //先截去后面的21位，再截取5位
	put_word(rt, get_word(33));//EPC是33号寄存器
	pc = pc + 4;
	printf("执行MFEPC指令：rt=%d, pc地址=%d\n\n", get_word(rt), pc);
}

void INSN_MFCO(unsigned int ins)
{
	int rt = ins / 0x200000 % 0x20;  //先截去后面的21位，再截取5位
	put_word(rt, get_word(34));//Cause_Reg是34号寄存器
	pc = pc + 4;
	printf("执行MFCO指令：rt=%d, pc地址=%d\n\n", get_word(rt), pc);
}

void INSN_LW(unsigned int ins)
{
	int rd = ins / 0x200000 % 0x20;  //先截去后面的21位，再截取5位
	int rs1;
	int imm;
	if(ins / 0x10000 % 0x20 == 0)//先截去后面的16位，再截取5位
	{
		rs1 = 0;//没有用rs1寄存器
	}
	else
	{
		rs1 = get_word(ins / 0x10000 % 0x20);
	}
	
	imm = read_mem_word(ins % 0x10000 + rs1);//得到最后16立即数
	
	put_word(rd, imm);

	printf("执行LW指令：rs1=%d, 内存地址=0x%x, 内存数据=%d, 目的寄存器r%d=%d\n\n",rs1, (ins % 0x10000 + rs1), imm, rd, get_word(rd));
	pc = pc + 4;
}


void INSN_SW(unsigned int ins)//这里的rs1与rd和前面定义的不一样，需要写一张自己的表来定义
{
	int rd = ins / 0x10000 % 0x20;  //先截去后面的16位，再截取5位
	int rs1 = ins / 0x200000 % 0x20;//先截去后面的21位，再截取5位,需要存储的数据
	int imm = (ins % 0x10000);//得到最后16立即数
	if(rd != 0)//先截去后面的21位，再截取5位
	{
		rd = get_word(ins / 0x10000 % 0x20);
	}
	printf("执行SW指令：rs1=%d, 内存地址=0x%x, 内存数据=%d \n\n",get_word(rs1), rd + imm, get_word(rs1));
	write_mem_word(rd + imm, get_word(rs1));
	pc = pc + 4;
}
/*
	指令执行
	先通过OPCODE区分这三种指令格式，然后直接就能操作相应数据 ,返回1表示执行成功
*/
int ins_exe(unsigned int ins)
{
	if(ins / 0x4000000 == 0)//全为零是R型指令格式
	{
		switch(ins % 0x800)//截取FUNC位
		{
			case 1:INSN_ADD(ins);return 1;
			case 2:INSN_ADDU(ins);return 1;
			case 3:INSN_SUB(ins);return 1;
		    case 4:INSN_SUBU(ins);return 1;
			case 5:INSN_MUL(ins);return 1;
			case 6:INSN_MULU(ins);return 1;
			case 7:INSN_DIV(ins);return 1;
			case 8:INSN_DIVU(ins);return 1;
			case 9:INSN_SLT(ins);return 1;
			case 10:INSN_SLTU(ins);return 1;
			case 11:INSN_AND(ins);return 1;
			case 12:INSN_OR(ins);return 1;
			case 13:INSN_NOR(ins);return 1;
			case 14:INSN_XOR(ins);return 1;
			case 15:INSN_JR(ins);return 1;
			case 16:INSN_JALR(ins);return 1;
			case 17:INSN_NOP(ins);return 1;
			case 18:INSN_MFHI(ins);return 1;
			case 19:INSN_MFLO(ins);return 1;
			case 20:INSN_MFEPC(ins);return 1;
			case 21:INSN_MFCO(ins);return 1;
			case 22:INSN_ADDS(ins);return 1;
			case 23:INSN_ADDD(ins);return 1;
			case 24:INSN_SUBS(ins);return 1;
			case 25:INSN_SUBD(ins);return 1;
			case 26:INSN_MULS(ins);return 1;
			case 27:INSN_MULD(ins);return 1;
	 		case 28:INSN_DIVS(ins);return 1;
			case 29:INSN_DIVD(ins);return 1;
			default:return 0;
		}
	}
	else if(ins / 0x80000000 == 0)//最高位为零是I型指令
	{
		switch(ins / 0x4000000)//截取OPCODE位
		{
			case 1:INSN_ADDI(ins);return 1;
			case 2:INSN_ADDIU(ins);return 1;
			case 3:INSN_SLTI(ins);return 1;
			case 4:INSN_SLTIU(ins);return 1;
			case 5:INSN_ANDI(ins);return 1;
			case 6:INSN_ORI(ins);return 1;
			case 7:INSN_XORI(ins);return 1;
			case 8:INSN_LW(ins);return 1;
			case 9:INSN_SW(ins);return 1;
			case 10:INSN_LBU(ins);return 1;
			case 11:INSN_LB(ins);return 1;
			case 12:INSN_SB(ins);return 1;
			case 13:INSN_LUI(ins);return 1;
			case 14:INSN_BEQ(ins);return 1;
			case 15:INSN_BNE(ins);return 1;
			case 16:INSN_BLEZ(ins);return 1;
			case 17:INSN_BGTZ(ins);return 1;
			case 18:INSN_BLTZ(ins);return 1;
			case 19:INSN_LWCL(ins);return 1;
	 		case 20:INSN_SWCL(ins);return 1;
			default:return 0;
		}
	}
	else if(ins / 0x80000000 == 1)//最高位为1是J型指令
	{/*
		case 1:INSN_J(ins);return 1;
		case 2:INSN_JAL(ins);return 1;
	*/}
	return 0;
}

int main()
{	
	FILE *f;
	char file_line[100];
	unsigned int ins;
	int ins_num = 0;//定义指令条数
	freopen("test.log","w",stdout);//数据重定向到log文件
	Memory();//创建内存空间
	Register();//创建32个通用寄存器
	
	//定义数据段地址和存入内存的数据
	write_mem_word(0x1000, 0x1100);
	write_mem_word(0x1004, 0x1104);
	write_mem_word(0x1008, 0x1108);
	write_mem_word(0x1100, 308);
	write_mem_word(0x1104, 292);
	write_mem_word(0x11fc, 0x7ffffff0);

	write_mem_uword(0x1010,7854543);
	write_mem_uword(0x1014,11);

	write_mem_uword(0x1030,63);
	write_mem_uword(0x1034,18);

	write_mem_word(0x1020,-43);
	write_mem_word(0x1024,-11);
	write_mem_word(0x1028,0);
	write_mem_ubyte(0x1040,50);
	write_mem_byte(0x1041,55);
	write_mem_float(0x1060,13.4598);
	write_mem_float(0x1068,32.92013);
	write_mem_double(0x1070,78.45965443548);
	write_mem_double(0x1080,32.924543013);
	printf("0x%x内存地址,int型数据为：%d\n\n",0x1000, read_mem_word(0x1000));
	printf("0x%x内存地址,int型数据为：%d\n\n",0x1004, read_mem_word(0x1004));	
	printf("0x%x内存地址,int型数据为：%d\n\n",0x1008, read_mem_word(0x1008));
	printf("0x%x内存地址,int型数据为：%d\n\n",0x1100, read_mem_word(0x1100));	
	printf("0x%x内存地址,int型数据为：%d\n\n",0x1104, read_mem_word(0x1104));
	printf("0x%x内存地址,int型数据为：%d\n\n",0x11fc, read_mem_word(0x11fc));
	printf("0x%x内存地址,unsigned int型数据为：%ud\n\n",0x1010, read_mem_uword(0x1010));
	printf("0x%x内存地址,unsigned int型数据为：%ud\n\n",0x1014, read_mem_uword(0x1014));
	printf("0x%x内存地址,unsigned int型数据为：%ud\n\n",0x1030, read_mem_uword(0x1030));
	printf("0x%x内存地址,unsigned int型数据为：%ud\n\n",0x1034, read_mem_uword(0x1034));
	printf("0x%x内存地址,int型数据为：%d\n\n",0x1020, read_mem_word(0x1020));
	printf("0x%x内存地址,int型数据为：%d\n\n",0x1024, read_mem_word(0x1024));
	printf("0x%x内存地址,int型数据为：%d\n\n",0x1028, read_mem_word(0x1028));
	printf("0x%x内存地址,unsigned byte型数据为：%d\n\n",0x1040, read_mem_ubyte(0x1040));
	printf("0x%x内存地址,byte型数据为：%d\n\n",0x1041, read_mem_byte(0x1041));
	printf("0x%x内存地址,float型数据为：%f\n\n",0x1060, read_mem_float(0x1060));
	printf("0x%x内存地址,float型数据为：%f\n\n",0x1064, read_mem_float(0x1064));
	printf("0x%x内存地址,double型数据为：%f\n\n",0x1070, read_mem_double(0x1070));
	printf("0x%x内存地址,double型数据为：%f\n\n",0x1080, read_mem_double(0x1080));
	/***************** end *********************/


	f = fopen("test.txt", "r");
	if(f == NULL)
	{
		perror("File open error! ");
	}
	else
	{
		while(fgets(file_line, 100, f) != NULL)
		{
		 	if(*file_line  != '\n' && *file_line != '/')//有换行就跳过，跳过注释
			{
				++ins_num;
				ins_store(file_line);//将指令存到内存
			} 
			
		}
	}
	fclose(f);
	

	pc = 0;//指定代码段起始地址
	while(ins_num > 0)//此处判断条件还需修改
	{
		printf("********************************************************\n");
		printf("取指令! PC = %d\n\n",pc);
		ins = ins_load();  //加载指令
		if(ins_exe(ins) == 1)//指令执行成功
		{
			--ins_num;
		}
		else
		{
			printf("指令取完!\n");
			break;
		}
	}
	printf("********************************************************\n");
	printf("指令执行结束!\n\n");
	free(Mem);
	free(Reg);
	return 0;
}
